#include "BaseScene.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Timer.h"
#include "Fade.h"
#include "Actor.h"
#include "InputSystem.h"
#include "Parameter.h"
#include "PauseScreen.h"
#include "ParticleManager.h"

BaseScene::BaseScene(SceneManager* sm, const Parameter& parameter)
	:
	mParticleManager(nullptr),
	mSceneManager(sm),
	mTimer(new Timer()),
	mUpdatingActors(false),
	mGameTime(0.0f),
	mDeltaTime(0.016f),
	mSceneState(SceneState::EPlay),
	mNextScene(SceneType::ETitle),
	mFade(new Fade(this))
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
	while (!mPauseUIStack.empty())
	{
		delete mPauseUIStack.back();
		mPauseUIStack.pop_back();
	}

	if (mTimer)
	{
		delete mTimer;
		mTimer = nullptr;
	}
	if (mFade)
	{
		delete mFade;
		mFade = nullptr;
	}
	if (mParticleManager)
	{
		delete mParticleManager;
		mParticleManager = nullptr;
	}

	GetRenderer()->UnloadData();
}

void BaseScene::ProcessInput()
{
	GetInputSystem()->Update();

	if (mSceneState == SceneState::EPlay)
	{
		mUpdatingActors = true;
		for (auto actor : mActors)
		{
			actor->ProcessInput();
		}
		mUpdatingActors = false;
	}
	else if (mSceneState == SceneState::EPaused)
	{
		for (auto ui : mPauseUIStack)
		{
			ui->ProcessInput();
		}
	}
}

void BaseScene::Update()
{
	while (mTimer->Peek() <= 1.0f / 60.0f);
	mDeltaTime = mTimer->Mark();
	if (mDeltaTime > 0.05f)
	{
		mDeltaTime = 0.05f;
	}

	if (mSceneState == SceneState::EPlay)
	{
		mGameTime += mDeltaTime;

		mUpdatingActors = true;
		for (auto actor : mActors)
		{
			actor->Update(mDeltaTime);
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
	else if (mSceneState == SceneState::EPaused)
	{
		for (auto ui : mPauseUIStack)
		{
			ui->Update(mDeltaTime);
		}
	}

	mFade->Update(mDeltaTime);
}

void BaseScene::GenerateOutput()
{
	GetRenderer()->Draw();
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

void BaseScene::PushUI(PauseScreen* screen)
{
	mPauseUIStack.emplace_back(screen);
}

Renderer* BaseScene::GetRenderer()
{
	return mSceneManager->GetRenderer();
}

InputSystem* BaseScene::GetInputSystem()
{
	return mSceneManager->GetInputSystem();
}

AudioSystem* BaseScene::GetAudioSystem()
{
	return mSceneManager->GetAudioSystem();
}

Window* BaseScene::GetWindow()
{
	return mSceneManager->GetWindow();
}
