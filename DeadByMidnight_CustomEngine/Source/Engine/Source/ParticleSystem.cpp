/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       ParticleSystem.cpp
@author     SIA Xin Tian (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Update particles
*//*______________________________________________________________________*/


#include <random>
#include "ParticleSystem.h"

#include "Transform.h"
#include "InputManager.h"
#include "Renderer.h"
#include "WindowsManager.h"
#include "EntityManager.h"
#include "CoreEngine.h"


void ParticleSystem::SpawnParticles(Vec3 _pos, Vec3 _offset)
{
	for (int i = 0; i < MAX_PARTICLES; ++i) {
		Entity* newEnt = EntityManager::GetInstance()->CreateEntity();
		newEnt->SetName("Particle");

		if (newEnt == nullptr)
			continue;

		//Random number generator
		std::default_random_engine randomGen(std::random_device{}());
		std::uniform_real_distribution<float> distributionLife(.3f, .8f);
		std::uniform_real_distribution<float> distributionpos(-2.f, 2.f);

		Vec3 offset = Vec3(distributionpos(randomGen), distributionpos(randomGen), 0);

		Transform newXform{};
		if (IsSmall())
		{
			std::uniform_real_distribution<float> distributionoffset(-1.5f, 1.5f);
			Vec3 smalloffset = Vec3(0, distributionoffset(randomGen), 0);
			newXform.SetPosition(_pos + smalloffset);
		}
		else newXform.SetPosition(_pos + offset);		

		newXform.mScale = mParticleSize;
		newEnt->AddComponent(newXform);

		Renderer* renderer = newEnt->AddComponent<Renderer>();
		renderer->mModelType = Renderer::MODEL_TYPE::SPRITE;
		renderer->mRenderLayer = Renderer::RENDER_LAYER::MIDGROUND;
		renderer->mCustomAlphaEnabled = true;
		if (IsSmall()) renderer->mAlphaValue = mStartAlphaValue * 1.2f;
		else
			renderer->mAlphaValue = mStartAlphaValue;


		mParticleEntityIDs.push_back(newEnt->GetID());

		//create and initialize the particle
		Particle particle;
		particle.mPos = newXform.GetPosition();
		particle.mLife = distributionLife(randomGen);
		mParticles.push_back(particle);
	}
	
}

int ParticleSystem::GetParticleIndex(EntityID _id) 
{
	for (int i = 0; i < mParticleEntityIDs.size(); ++i) 
	{
		if (mParticleEntityIDs[i] == _id) 
		{
			return i;
		}
	}
	return -1;
}

bool ParticleSystem::IsSmall()
{
	if (InputManager::GetInstance()->GetAction("PlayerShrink"))
	{
		return true;
	}
	else return false;
}


void ParticleSystem::Init()
{
	mPosition = Vec3(0.f, 0.f, 0.f);
	mVelocity = Vec3(0.f, 0.f, 0.f);
}

void ParticleSystem::Update()
{

	if (CoreEngine::GetInstance()->mGameState != CoreEngine::GameState::GS_PLAY)
		return;

	//loop through vector that stores the particle's entities
	for (size_t i = 0; i < mParticleEntityIDs.size(); ++i) 
	{
		EntityID particleID = mParticleEntityIDs[i];
		Entity* particleEntity = EntityManager::GetInstance()->GetEntity(particleID);
		Transform* xform = particleEntity->GetComponent<Transform>();
		Renderer* renderer = particleEntity->GetComponent<Renderer>();

		if (particleEntity == nullptr || xform == nullptr || renderer == nullptr)
			continue;

		int particleIndex = GetParticleIndex(particleID);
		if (particleIndex != -1) //particle exists
		{ 
			//update position of particle
			mParticles[particleIndex].mPos += mParticles[particleIndex].mVel * (float)WindowsManager::GetInstance()->mDeltaTime;
			mParticles[particleIndex].mLife -= (float)WindowsManager::GetInstance()->mDeltaTime; //decrease mLife

			if (IsSmall())
			{
				if (mParticleSize.X() > mParticleSmallSize.X() && mParticleSize.Y() > mParticleSmallSize.Y())
				{
					mParticleSize -= mChangeSize;
				}
				else mParticleSize = mParticleSmallSize;
			}
			else 
			{
				if (mParticleSize.X() < mParticleBigSize.X() && mParticleSize.Y() < mParticleBigSize.Y()) 
				{
					mParticleSize += mChangeSize;
				}
				else mParticleSize = mParticleBigSize;
			}
			xform->mScale = mParticleSize;


			xform->SetPosition(mParticles[particleIndex].mPos);
			renderer->mAlphaValue -= mDecreaseAlphaValue;

			//check if particle still alive
			if (mParticles[particleIndex].mLife <= 0.f) 
			{
				EntityManager::GetInstance()->DestroyEntity(particleID);

				mParticleEntityIDs.erase(mParticleEntityIDs.begin() + i);
				mParticles.erase(mParticles.begin() + particleIndex);

				--i;
				continue;
			}
		}
	}
}


void ParticleSystem::Load()
{
	mParticleEntityIDs.clear();
}


void ParticleSystem::Unload()
{
	mParticleEntityIDs.clear();
	mParticles.clear();

	// Destroy instance
	Destroy();
}

void ParticleSystem::ClearParticles()
{
	mParticleEntityIDs.clear();
	mParticles.clear();
}