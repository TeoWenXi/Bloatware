/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       CoreEngine.cpp
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Core Engine of the program. Contains the main game loop and assists
			in managing all registered systems/managers.
*//*______________________________________________________________________*/

#include "CoreEngine.h"
#include "WindowsManager.h"
#include "SceneManager.h"
#include "GraphicsSystem.h"
#include "LibraryManager.h"
#include "TextManager.h"

CoreEngine::CoreEngine()
	: mLastRenderTime(0)
	, mRenderFPS(0)
	, mRenderCount(0)
	, mLastFpsUpdateTime(0)
	, mLastUpdateTime(0)
{
}

CoreEngine::~CoreEngine()
{
}

void CoreEngine::Init()
{
	//Init all Systems
	for (size_t i = 0; i < mSystemInitSeq.size(); ++i)
		mSystemInitSeq[i]->Init();

	//Clear vector as not required anymore
	mSystemInitSeq.clear();
}

void CoreEngine::Load()
{
	//Load all Systems
	for (size_t i = 0; i < mSystemsList.size(); ++i)
		mSystemsList[i]->Load();
}

void CoreEngine::MainGameLoop()
{
	while (!glfwWindowShouldClose(WindowsManager::GetInstance()->mPtrWindow))
	{
		//FPS stuff
		double currTime = glfwGetTime();

		//Update all systems
		if (mEnableUpdateLoopTarget)
		{
			double elapsedUpdateTime = currTime - mLastUpdateTime;
			if (elapsedUpdateTime >= 1.0 / mTargetUpdateFrequency)
			{
				for (size_t i = 0; i < mSystemsList.size(); ++i)
				{
					double _startTime = glfwGetTime();
					mSystemsList[i]->Update();
					double _endTime = glfwGetTime();
					mSystemsDeltaTime[i].second = (_endTime - _startTime) * 1000;
				}
				mTotalSystemsTime = (glfwGetTime() - currTime) * 1000;
				mLastUpdateTime = currTime;
			}
		}
		else
		{
			for (size_t i = 0; i < mSystemsList.size(); ++i)
			{
				double _startTime = glfwGetTime();
				mSystemsList[i]->Update();
				double _endTime = glfwGetTime();
				mSystemsDeltaTime[i].second = (_endTime - _startTime) * 1000;
			}
			mTotalSystemsTime = (glfwGetTime() - currTime) * 1000;
		}


		//FPS Display Update
		double elapsedTime = currTime - mLastFpsUpdateTime;
		if (elapsedTime >= mRenderFpsUpdateInterval)
		{
			mRenderFPS = mRenderCount / elapsedTime;
			mLastFpsUpdateTime = currTime;
			mRenderCount = 0;
		}

		//FPS Capping
		if (mEnableFPSTarget)
		{
			elapsedTime = currTime - mLastRenderTime;
			if (elapsedTime >= 1.0 / mTargetRenderFPS)
			{
				//Render
				GraphicsSystem::GetInstance()->Render();
				TextManager::GetInstance()->Render();
#if _ENGINE
				LibraryManager::GetInstance()->ImGUIRender();
#endif // _ENGINE

				glfwSwapBuffers(WindowsManager::GetInstance()->mPtrWindow);

				//Update fps variables
				mLastRenderTime = currTime;
				mRenderCount++;
			}
			else
			{
#if _ENGINE
				LibraryManager::GetInstance()->ImGUIEndFrame();
#endif // _ENGINE
			}
		}
		else
		{
			//Render
			GraphicsSystem::GetInstance()->Render();
			TextManager::GetInstance()->Render();
#if _ENGINE
			LibraryManager::GetInstance()->ImGUIRender();
#elif _BLOATWARE
			GraphicsSystem::GetInstance()->PostRender();
#endif // _ENGINE

			glfwSwapBuffers(WindowsManager::GetInstance()->mPtrWindow);

			//Update fps variables
			mRenderCount++;
#if _ENGINE
			LibraryManager::GetInstance()->ImGUIEndFrame();
#endif // _ENGINE
		}
	}
}

void CoreEngine::Update()
{
}

void CoreEngine::Unload()
{
	//Unload & Destroys all systems and their instances from the back
	for (size_t i = 0; i < mSystemsList.size(); ++i)
		mSystemsList[i]->Unload();

	//Unload GLFW
	glfwTerminate();

	//Destroy instance
	Destroy();
}

void CoreEngine::Exit()
{
	glfwSetWindowShouldClose(WindowsManager::GetInstance()->mPtrWindow, true);
}