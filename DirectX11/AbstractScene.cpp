#include "AbstractScene.h"
#include "SceneManager.h"
#include "GameApp.h"
#include "Renderer.h"
#include "Timer.h"
#include "Actor.h"

AbstractScene::AbstractScene(SceneManager* sm, const Parameter& parameter)
	:
	mSceneManager(sm),
	mTimer(new Timer()),
	mUpdatingActors(false)
{
}

AbstractScene::~AbstractScene()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	while (!mPendingActors.empty())
	{
		delete mPendingActors.back();
	}

	mSceneManager->GetApp()->GetRenderer()->UnloadData();

	if (mTimer)
	{
		delete mTimer;
		mTimer = nullptr;
	}
}

void AbstractScene::ProcessInput()
{
}

void AbstractScene::Update()
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

void AbstractScene::GenerateOutput()
{
	mSceneManager->GetApp()->GetRenderer()->Draw();
}

void AbstractScene::AddActor(Actor* actor)
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

void AbstractScene::RemoveActor(Actor* actor)
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
