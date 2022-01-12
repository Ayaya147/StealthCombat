#include "BaseScene.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Timer.h"
#include "Actor.h"
#include "InputSystem.h"
#include "Parameter.h"

BaseScene::BaseScene(SceneManager* sm, const Parameter& parameter)
	:
	mSceneManager(sm),
	mTimer(new Timer()),
	mUpdatingActors(false)
{
}

BaseScene::~BaseScene()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	while (!mPendingActors.empty())
	{
		delete mPendingActors.back();
	}

	mSceneManager->GetRenderer()->UnloadData();

	if (mTimer)
	{
		delete mTimer;
		mTimer = nullptr;
	}
}

void BaseScene::ProcessInput()
{
	GetSceneManager()->GetInputSystem()->Update();

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput();
	}
	mUpdatingActors = false;
}

void BaseScene::Update()
{
	while (mTimer->Peek() <= 1.0f / 60.0f);
	float deltaTime = mTimer->Mark();
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;
	
	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetActorState() == Actor::ActorState::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void BaseScene::GenerateOutput()
{
	mSceneManager->GetRenderer()->Draw();
}

void BaseScene::AddActor(Actor* actor)
{
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void BaseScene::RemoveActor(Actor* actor)
{
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}
