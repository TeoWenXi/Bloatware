/*!
All content � 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       AIComponent.cpp
@author     TAN Kang Zheng (50%)
@co-author	Jovan Zhuo Wen LOW (50%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Handles the AI logic
*//*______________________________________________________________________*/

#include "AIComponent.h"
#include "InputManager.h"

#if _ENGINE
#include "DebugLogger.h"
#endif // _ENGINE

Player* g_PlayerAI;
Transform* g_PlayerAITransform;
bool HasFoundPlayer = false;

void AIComponent::Init()
{
	mCurrentQuadrant = UP_RIGHT;
	mCurrentAIState = AI_MOVE;
	mFinalPos = Vector3{ 0, 0, 0 };
	mHeightOffset = 0;
	mIdleElapsedTime = 0;

	//TODO:: Load these value from file
	mMoveSpeed = 75.f;
	mHeightOffsetRange = 10.f;

	AIManager::GetInstance()->AddAIComponent(mEntID);
	HasFoundPlayer = SearchPlayer();


	ent_HB1_FocusRing = EntityManager::GetInstance()->CreateEntity();
	ent_HB1_FocusRing->SetName("HB1_FocusRing");
	Transform newXform{};
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 40, 80, 1 };
	ent_HB1_FocusRing->AddComponent(newXform);
	Renderer* _renderer = ent_HB1_FocusRing->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE;
	_renderer->mShaderName = "HB1_FocusRingShader";
	_renderer->mTextureName = "whitecircle";
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB1_FocusRing->SetSaveInScene(false);

	ent_HB1_Sparks = EntityManager::GetInstance()->CreateEntity();
	ent_HB1_Sparks->SetName("HB1_Sparks");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 80, 80, 1 };
	ent_HB1_Sparks->AddComponent(newXform);
	_renderer = ent_HB1_Sparks->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE_ANIMATION;
	_renderer->mShaderName = "Animation";
	_renderer->mTextureName = "SpriteSheet_HB1_Sparks";
	_renderer->mEnabled = false;
	AnimationComponent* anim = ent_HB1_Sparks->AddComponent<AnimationComponent>();
	anim->mAnimation.mTotalFrames = 9;
	anim->mAnimation.mTimeDelay = 0.06f;
	anim->mAnimation.mIsLooping = false;
	anim->mAnimationLoopLimit_Max = 3;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB1_Sparks->SetSaveInScene(false);

	ent_HB1_Orb = EntityManager::GetInstance()->CreateEntity();
	ent_HB1_Orb->SetName("HB1_Orb");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 50, 50, 1 };
	ent_HB1_Orb->AddComponent(newXform);
	_renderer = ent_HB1_Orb->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE;
	_renderer->mShaderName = "BasicShader";
	_renderer->mTextureName = "HB1_Orb2";
	_renderer->mEnabled = false;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB1_Orb->SetSaveInScene(false);

	ent_HB2_Sparks = EntityManager::GetInstance()->CreateEntity();
	ent_HB2_Sparks->SetName("HB2_Sparks");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 80, 80, 1 };
	ent_HB2_Sparks->AddComponent(newXform);
	_renderer = ent_HB2_Sparks->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE_ANIMATION;
	_renderer->mShaderName = "Animation";
	_renderer->mTextureName = "SpriteSheet_HB2_Sparks";
	_renderer->mEnabled = false;
	anim = ent_HB2_Sparks->AddComponent<AnimationComponent>();
	anim->mAnimation.mTotalFrames = 5;
	anim->mAnimation.mTimeDelay = 0.05f;
	anim->mAnimation.mIsLooping = false;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB2_Sparks->SetSaveInScene(false);

	ent_HB3_Sparks = EntityManager::GetInstance()->CreateEntity();
	ent_HB3_Sparks->SetName("HB3_Sparks");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 80, 80, 1 };
	ent_HB3_Sparks->AddComponent(newXform);
	_renderer = ent_HB3_Sparks->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE_ANIMATION;
	_renderer->mShaderName = "Animation";
	_renderer->mTextureName = "SpriteSheet_HB3_Sparks";
	_renderer->mEnabled = false;
	 anim = ent_HB3_Sparks->AddComponent<AnimationComponent>();
	anim->mAnimation.mTotalFrames = 9;
	anim->mAnimation.mTimeDelay = 0.06f;
	anim->mAnimation.mIsLooping = false;
	anim->mAnimationLoopLimit_Max = 3;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB3_Sparks->SetSaveInScene(false);

	ent_HB2_RingA = EntityManager::GetInstance()->CreateEntity();
	ent_HB2_RingA->SetName("HB2_RingA");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 200, 200, 1 };
	ent_HB2_RingA->AddComponent(newXform);
	_renderer = ent_HB2_RingA->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE;
	_renderer->mShaderName = "BasicShader";
	_renderer->mTextureName = "HB2_Ring";
	_renderer->mEnabled = false;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB2_RingA->SetSaveInScene(false);

	ent_HB2_RingB = EntityManager::GetInstance()->CreateEntity();
	ent_HB2_RingB->SetName("HB2_RingB");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 200, 200, 1 };
	ent_HB2_RingB->AddComponent(newXform);
	_renderer = ent_HB2_RingB->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE;
	_renderer->mShaderName = "BasicShader";
	_renderer->mTextureName = "HB2_Ring";
	_renderer->mEnabled = false;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB2_RingB->SetSaveInScene(false);

	ent_HB2_HyperGlow = EntityManager::GetInstance()->CreateEntity();
	ent_HB2_HyperGlow->SetName("HB2_HyperGlow");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 400, 300 * mLaserHeightMagnification, 1 };
	ent_HB2_HyperGlow->AddComponent(newXform);
	_renderer = ent_HB2_HyperGlow->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE;
	_renderer->mShaderName = "HB2_HyperGlowShader";
	_renderer->mTextureName = "whitesquare";
	_renderer->mEnabled = false;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB2_HyperGlow->SetSaveInScene(false);

	ent_HB3_BackBeamSparksA = EntityManager::GetInstance()->CreateEntity();
	ent_HB3_BackBeamSparksA->SetName("HB3_BackBeamSparksA");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 100, 100, 1 };
	ent_HB3_BackBeamSparksA->AddComponent(newXform);
	_renderer = ent_HB3_BackBeamSparksA->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE_ANIMATION;
	_renderer->mShaderName = "Animation";
	_renderer->mTextureName = "SpriteSheet_HB3_BackBeamSparksA";
	_renderer->mEnabled = false;
	anim = ent_HB3_BackBeamSparksA->AddComponent<AnimationComponent>();
	anim->mAnimation.mTotalFrames = 2;
	anim->mAnimation.mTimeDelay = 0.12f;
	anim->mAnimation.mIsLooping = true;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB3_BackBeamSparksA->SetSaveInScene(false);

	ent_HB3_BackBeamSparksB = EntityManager::GetInstance()->CreateEntity();
	ent_HB3_BackBeamSparksB->SetName("HB3_BackBeamSparksB");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 100, 100, 1 };
	ent_HB3_BackBeamSparksB->AddComponent(newXform);
	_renderer = ent_HB3_BackBeamSparksB->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE_ANIMATION;
	_renderer->mShaderName = "Animation";
	_renderer->mTextureName = "SpriteSheet_HB3_BackBeamSparksB";
	_renderer->mEnabled = false;
	anim = ent_HB3_BackBeamSparksB->AddComponent<AnimationComponent>();
	anim->mAnimation.mTotalFrames = 2;
	anim->mAnimation.mTimeDelay = 0.12f;
	anim->mAnimation.mIsLooping = true;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB3_BackBeamSparksB->SetSaveInScene(false);

	ent_HB3_BackBeamSparksC = EntityManager::GetInstance()->CreateEntity();
	ent_HB3_BackBeamSparksC->SetName("HB3_BackBeamSparksC");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 100, 100, 1 };
	ent_HB3_BackBeamSparksC->AddComponent(newXform);
	_renderer = ent_HB3_BackBeamSparksC->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE_ANIMATION;
	_renderer->mShaderName = "Animation";
	_renderer->mTextureName = "SpriteSheet_HB3_BackBeamSparksC";
	_renderer->mEnabled = false;
	anim = ent_HB3_BackBeamSparksC->AddComponent<AnimationComponent>();
	anim->mAnimation.mTotalFrames = 2;
	anim->mAnimation.mTimeDelay = 0.12f;
	anim->mAnimation.mIsLooping = true;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB3_BackBeamSparksC->SetSaveInScene(false);

	ent_HB4_SparksAftermathA = EntityManager::GetInstance()->CreateEntity();
	ent_HB4_SparksAftermathA->SetName("HB4_SparksAftermathA");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 50, 50, 1 };
	ent_HB4_SparksAftermathA->AddComponent(newXform);
	_renderer = ent_HB4_SparksAftermathA->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE_ANIMATION;
	_renderer->mShaderName = "Animation";
	_renderer->mTextureName = "HB4_Spark0";
	_renderer->mEnabled = false;
	anim = ent_HB4_SparksAftermathA->AddComponent<AnimationComponent>();
	anim->mAnimation.mTotalFrames = 4;
	anim->mAnimation.mTimeDelay = 0.09f;
	anim->mAnimation.mIsLooping = true;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB4_SparksAftermathA->SetSaveInScene(false);

	ent_HB4_SparksAftermathB = EntityManager::GetInstance()->CreateEntity();
	ent_HB4_SparksAftermathB->SetName("HB4_SparksAftermathB");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 50, 50, 1 };
	ent_HB4_SparksAftermathB->AddComponent(newXform);
	_renderer = ent_HB4_SparksAftermathB->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE_ANIMATION;
	_renderer->mShaderName = "Animation";
	_renderer->mTextureName = "HB4_Spark1";
	_renderer->mEnabled = false;
	anim = ent_HB4_SparksAftermathB->AddComponent<AnimationComponent>();
	anim->mAnimation.mTotalFrames = 4;
	anim->mAnimation.mTimeDelay = 0.09f;
	anim->mAnimation.mIsLooping = true;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB4_SparksAftermathB->SetSaveInScene(false);

	ent_HB4_SparksAftermathC = EntityManager::GetInstance()->CreateEntity();
	ent_HB4_SparksAftermathC->SetName("HB4_SparksAftermathC");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 50, 50, 1 };
	ent_HB4_SparksAftermathC->AddComponent(newXform);
	_renderer = ent_HB4_SparksAftermathC->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE_ANIMATION;
	_renderer->mShaderName = "Animation";
	_renderer->mTextureName = "HB4_Spark2";
	_renderer->mEnabled = false;
	anim = ent_HB4_SparksAftermathC->AddComponent<AnimationComponent>();
	anim->mAnimation.mTotalFrames = 4;
	anim->mAnimation.mTimeDelay = 0.09f;
	anim->mAnimation.mIsLooping = true;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB4_SparksAftermathC->SetSaveInScene(false);

	ent_HB2_HyperBeam = EntityManager::GetInstance()->CreateEntity();
	ent_HB2_HyperBeam->SetName("HB2_HyperBeam");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 400, 300 * mLaserHeightMagnification, 1 };
	ent_HB2_HyperBeam->AddComponent(newXform);
	_renderer = ent_HB2_HyperBeam->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE;
	_renderer->mShaderName = "HB2_HyperBeamShader";
	_renderer->mTextureName = "whitesquare";
	_renderer->mEnabled = false;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB2_HyperBeam->SetSaveInScene(false);

	ent_HB3_FrontBeamSparksA = EntityManager::GetInstance()->CreateEntity();
	ent_HB3_FrontBeamSparksA->SetName("HB3_FrontBeamSparksA");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 100, 100, 1 };
	ent_HB3_FrontBeamSparksA->AddComponent(newXform);
	_renderer = ent_HB3_FrontBeamSparksA->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE_ANIMATION;
	_renderer->mShaderName = "Animation";
	_renderer->mTextureName = "SpriteSheet_HB3_FrontBeamSparksA";
	_renderer->mEnabled = false;
	anim = ent_HB3_FrontBeamSparksA->AddComponent<AnimationComponent>();
	anim->mAnimation.mTotalFrames = 2;
	anim->mAnimation.mTimeDelay = 0.12f;
	anim->mAnimation.mIsLooping = true;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB3_FrontBeamSparksA->SetSaveInScene(false);

	ent_HB3_FrontBeamSparksB = EntityManager::GetInstance()->CreateEntity();
	ent_HB3_FrontBeamSparksB->SetName("HB3_FrontBeamSparksB");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 100, 100, 1 };
	ent_HB3_FrontBeamSparksB->AddComponent(newXform);
	_renderer = ent_HB3_FrontBeamSparksB->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE_ANIMATION;
	_renderer->mShaderName = "Animation";
	_renderer->mTextureName = "SpriteSheet_HB3_FrontBeamSparksB";
	_renderer->mEnabled = false;
	anim = ent_HB3_FrontBeamSparksB->AddComponent<AnimationComponent>();
	anim->mAnimation.mTotalFrames = 2;
	anim->mAnimation.mTimeDelay = 0.12f;
	anim->mAnimation.mIsLooping = true;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB3_FrontBeamSparksB->SetSaveInScene(false);

	ent_HB1_Sparkle = EntityManager::GetInstance()->CreateEntity();
	ent_HB1_Sparkle->SetName("HB1_Sparkle");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 12.0f, 24.0f, 1.0f };
	ent_HB1_Sparkle->AddComponent(newXform);
	_renderer = ent_HB1_Sparkle->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE;
	_renderer->mShaderName = "BasicShader";
	_renderer->mTextureName = "HB1_Sparkle";
	_renderer->mEnabled = false;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB1_Sparkle->SetSaveInScene(false);

	ent_HB1_SparkleLineVertical = EntityManager::GetInstance()->CreateEntity();
	ent_HB1_SparkleLineVertical->SetName("HB1_SparkleLineVertical");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 3, 100, 1 };
	ent_HB1_SparkleLineVertical->AddComponent(newXform);
	_renderer = ent_HB1_SparkleLineVertical->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE;
	_renderer->mShaderName = "BasicShader";
	_renderer->mTextureName = "HB1_Capsule";
	_renderer->mEnabled = false;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB1_SparkleLineVertical->SetSaveInScene(false);

	ent_HB1_SparkleLineHorizontal = EntityManager::GetInstance()->CreateEntity();
	ent_HB1_SparkleLineHorizontal->SetName("HB1_SparkleLineHorizontal");
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 60, 3, 1 };
	newXform.mRotation = 180.0f;
	ent_HB1_SparkleLineHorizontal->AddComponent(newXform);
	newXform.mRotation = 0.0f;
	_renderer = ent_HB1_SparkleLineHorizontal->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE;
	_renderer->mShaderName = "BasicShader";
	_renderer->mTextureName = "HB1_Capsule";
	_renderer->mEnabled = false;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	ent_HB1_SparkleLineHorizontal->SetSaveInScene(false);

}

void AIComponent::DelayMovement()
{
	if (mIsHostile)
	{
		mHostileDelayedMovement = true;
		mHostileDelayedMovementTimer = mHostileDelayedMovementTime;

		mHostileDelayedLaser = false;
		//mHostileDelayedLaserTimer = 0.0f;
		mPlayerInLeftDelay = true;
	}
}

void AIComponent::DelayNextLaser()
{
	if (mIsHostile)
	{
		mHostileDelayedLaser = true;
		mHostileDelayedLaserTimer = mHostileDelayedLaserTime;
	}
}

void AIComponent::ResetLaser()
{
	if (mIsHostile)
	{
		mDetectPlayerCooldown_Timer = mDetectPlayerCooldown_Max;
		mIsHostileLockedOn = false;
		mSwitchToSmallSparks = false;
		mCanDamagePlayer = false;

		hostileLockedOn_Phase = 1;
		ent_HB1_Orb->GetComponent<Renderer>()->mEnabled = false;
		ent_HB1_Sparks->GetComponent<Renderer>()->mEnabled = false;
		ent_HB1_Sparkle->GetComponent<Renderer>()->mEnabled = false;
		ent_HB1_SparkleLineVertical->GetComponent<Renderer>()->mEnabled = false;
		ent_HB1_SparkleLineHorizontal->GetComponent<Renderer>()->mEnabled = false;
		ent_HB2_Sparks->GetComponent<Renderer>()->mEnabled = false;
		ent_HB2_RingA->GetComponent<Renderer>()->mEnabled = false;
		ent_HB2_RingB->GetComponent<Renderer>()->mEnabled = false;
		ent_HB2_HyperBeam->GetComponent<Renderer>()->mEnabled = false;
		ent_HB2_HyperGlow->GetComponent<Renderer>()->mEnabled = false;
		ent_HB3_FrontBeamSparksA->GetComponent<Renderer>()->mEnabled = false;
		ent_HB3_FrontBeamSparksB->GetComponent<Renderer>()->mEnabled = false;
		ent_HB3_BackBeamSparksA->GetComponent<Renderer>()->mEnabled = false;
		ent_HB3_BackBeamSparksB->GetComponent<Renderer>()->mEnabled = false;
		ent_HB3_BackBeamSparksC->GetComponent<Renderer>()->mEnabled = false;
		ent_HB3_Sparks->GetComponent<Renderer>()->mEnabled = false;
		ent_HB4_SparksAftermathA->GetComponent<Renderer>()->mEnabled = false;
		ent_HB4_SparksAftermathB->GetComponent<Renderer>()->mEnabled = false;
		ent_HB4_SparksAftermathC->GetComponent<Renderer>()->mEnabled = false;
		mHyperBeamProgress = 0.0f;

		SoundComponent* _soundComp = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<SoundComponent>();
		if (_soundComp != nullptr)
		{
			_soundComp->Stop();
		}
	}
}

bool AIComponent::IsPlayerInKillRange()
{
	glm::vec3 campos = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition;
	return fabsf(g_PlayerAITransform->GetPosition().mY - (campos.y + mTargetMovePos.mY * mMovePosExtents.mY)) <= mKillRange * mLaserHeightMagnification;
}

void AIComponent::Load()
{
	mCurrentQuadrant = static_cast<QUADRANT>(JSONManager::GetInstance()->GetInt("CurrentQuadrant"));
	mCurrentAIState = static_cast<AI_STATE>(JSONManager::GetInstance()->GetInt("CurrentAIState"));
	mMoveSpeed = JSONManager::GetInstance()->GetFloat("MoveSpeed");
	mFinalPos = JSONManager::GetInstance()->GetVec3("FinalPos");
	mHeightOffset = JSONManager::GetInstance()->GetFloat("HeightOffset");
	mHeightOffsetRange = JSONManager::GetInstance()->GetFloat("HightOffset_range");
	mIdleElapsedTime = JSONManager::GetInstance()->GetDouble("IdleElapsedTime");
	mBGMReductionVal = JSONManager::GetInstance()->GetFloat("BGMReductionVal");
}

void AIComponent::Save()
{
	JSONManager::GetInstance()->Save("Component", "AIComponent");
	JSONManager::GetInstance()->Save("CurrentQuadrant", static_cast<int>(mCurrentQuadrant));
	JSONManager::GetInstance()->Save("CurrentAIState", static_cast<int>(mCurrentAIState));
	JSONManager::GetInstance()->Save("MoveSpeed", mMoveSpeed);
	JSONManager::GetInstance()->Save("FinalPos", mFinalPos);
	JSONManager::GetInstance()->Save("HeightOffset", mHeightOffset);
	JSONManager::GetInstance()->Save("HightOffset_range", mHeightOffsetRange);
	JSONManager::GetInstance()->Save("IdleElapsedTime", mIdleElapsedTime);
	JSONManager::GetInstance()->Save("BGMReductionVal", mBGMReductionVal);
}

void AIComponent::InitiateTalking(std::string _dialogueText, std::string _audioFileName)
{
	if (!mIsVisible) mIsVisible = true;

	mTalkingFullText = _dialogueText;
	for (int i = 0; i < mAIAudioSourceHandler.size(); i++)
	{
		mAIAudioSourceHandler.at(i).second = false;
	}
	mAIAudioSourceHandler.push_back(
		std::pair<FMOD::Channel*, bool>(
			SoundManager::GetInstance()->PlaySound(_audioFileName, SoundManager::GetInstance()->GetSFXVolume(), SoundManager::GetInstance()->GetChannelPitch())
			, true
		)
	);

	mDialogueVoiceDuration = SoundManager::GetInstance()->GetAudioLength(_audioFileName);
	mDialogueVoiceElapsed = 0.0f;
	mIsTalking = true;
	mCurrentTalkingTime = mDialogueTimeMultiplier * mBaseTalkingTime + (float)(mTalkingFullText.length()) * mTimePerChar;
	mCurrentTalkingTime = ceilf(mCurrentTalkingTime / (1.0f / mTalkSpeed)) / mTalkSpeed;
	mCurrentTalkingTimer = 0.f;
	mPostTalkingTimer = 0.f;
	mStateOfTyping = 0;
	mStringCharPointer = 0;
	mTimerToNextChar = 0.0f;
	mTalkingProgress = 0.f;
	mCurrentText = "";
	mCurrentText2 = "";
	mTimePerCharExtra = mCurrentTalkingTime / (float)(mTalkingFullText.length());
	mBlankSpaces = "";
	for (int j = 0; j < mTalkingFullText.length(); ++j)
	{
		mBlankSpaces += " ";
	}

	if(mBGMRestored)
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Changing the BGM Volume for dialogue!\n");
#endif // _ENGINE

		//Lower BGM for the audio
		SoundManager::GetInstance()->SetBGMDucking(mBGMReductionVal);
		mBGMRestored = false;
	}
}

void AIComponent::Update()
{
	if (!mHostileDelayedMovement)
	{
		EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->SetPosition(GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition + mScreenPos);
	}

	UpdateLasers();
	UpdateWinds();

	for (int i = (int)mAIAudioSourceHandler.size() - 1; i >= 0; i--)
	{
		if (mAIAudioSourceHandler.at(i).second == false)
		{
			float currentAudioVolume = 0;
			mAIAudioSourceHandler.at(i).first->getVolume(&currentAudioVolume);
			mAIAudioSourceHandler.at(i).first->setVolume(std::clamp(currentAudioVolume - 10.f * (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, 1.f));
			mAIAudioSourceHandler.at(i).first->getVolume(&currentAudioVolume);
			if (currentAudioVolume <= FLT_EPSILON)
			{
				mAIAudioSourceHandler.erase(mAIAudioSourceHandler.begin() + i);
			}
		}
	}

	if (mIsHostile)
	{
		if (!mPlayerInLeftDelay)
		{
			if (g_PlayerAI != nullptr && g_PlayerAI->mIsReviving)
			{
				if (GraphicsSystem::GetInstance()->mCamera.IsPlayerInLeftSideRange(85.0f))
				{
					mPlayerInLeftDelay = true;
				}
			}
		}
		else
		{
			if (
				(!(GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPlayerSpawnPos.x >= 1500.0f) && !GraphicsSystem::GetInstance()->mCamera.IsPlayerInLeftSideRange(85.0f))
				||
				((GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPlayerSpawnPos.x >= 1500.0f) && !GraphicsSystem::GetInstance()->mCamera.IsPlayerInLeftSideRange(63.0f))
				)
			{
				mPlayerInLeftDelay = false;
			}
		}
	}

	if (mIsTalking)
	{
		mCurrentTalkingTimer = std::clamp(mCurrentTalkingTimer + (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, mCurrentTalkingTime);
		mDialogueVoiceElapsed += (float)WindowsManager::GetInstance()->mDeltaTime;
		mTalkingProgress = fmodf(mTalkSpeed * mCurrentTalkingTimer, 1.0f);
		if (!mPlayerInLeftDelay)
		{
			mTrueAlpha = 0.8f;
		}

		if (mStateOfTyping == 2)
		{
			mPostTalkingTimer += (float)WindowsManager::GetInstance()->mDeltaTime;
			if (mPostTalkingTimer >= mPostTalkingTime)
			{
				mIsTalking = false;
				TextManager::GetInstance()->SetDialogueText("");
				mDialogueVoiceElapsed = mDialogueVoiceDuration;
			}
			else
			{
				TextManager::GetInstance()->SetDialogueText(mTalkingFullText);
			}
		}
		else if (mStateOfTyping != 2)
		{
			mTimerToNextChar += (float)WindowsManager::GetInstance()->mDeltaTime;
			if (mTimerToNextChar >= mTimePerCharExtra / 3.f)
			{
				mTimerToNextChar = 0;
				if (mStateOfTyping == 0)
				{
					if (mTalkingFullText[mStringCharPointer] == ' ')
					{
						mCurrentText += ' ';
					}
					else
					{
						char randomChar;
						int luckydraw = rand() % 100;
						if (luckydraw > 90) randomChar = '!';
						else if (luckydraw > 80) randomChar = '@';
						else if (luckydraw > 70) randomChar = '#';
						else if (luckydraw > 60) randomChar = '$';
						else if (luckydraw > 50) randomChar = '%';
						else if (luckydraw > 30) randomChar = '&';
						else if (luckydraw > 10) randomChar = '?';
						else randomChar = ';';
						mCurrentText += randomChar;
					}
				}
				else if (mStateOfTyping == 1)
				{
					mCurrentText2 += mTalkingFullText[mStringCharPointer];
				}
				mStringCharPointer++;
				if (mStringCharPointer >= mTalkingFullText.length())
				{
					mStateOfTyping++;
					mStringCharPointer = 0;
				}
			}

			std::string tempstring = "";

			if (mStateOfTyping == 0)
			{
				tempstring = mCurrentText;
				for (int j = 0; j < mBlankSpaces.substr(mStringCharPointer).length(); ++j)
				{
					tempstring += " ";
				}
			}
			else if (mStateOfTyping == 1)
			{
				for (int j = 0; j < mCurrentText2.length(); ++j)
				{
					tempstring += mCurrentText2[j];
				}
				tempstring += mCurrentText.substr(mStringCharPointer);
			}
			else
			{
				for (int j = 0; j < mCurrentText2.length(); ++j)
				{
					tempstring += mCurrentText2[j];
				}
			}
			TextManager::GetInstance()->SetDialogueText(tempstring);
		}
	}
	else
	{
		if (mIsHostile && !mPlayerInLeftDelay)
			mTrueAlpha = 0.8f;
		else
			mTrueAlpha = 0.4f;
		TextManager::GetInstance()->SetDialogueText("");
	}

	if (mDialogueVoiceElapsed >= mDialogueVoiceDuration)
	{
		//Restore BGM
		//if (!mBGMRestored)
		//{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Restoring the BGM volume for dialogue!\n");
#endif // _ENGINE

		SoundManager::GetInstance()->SetBGMDucking(1.0f);
		mBGMRestored = true;
		//}
	}
	if (mPlayerInLeftDelay)
	{
		mTrueAlpha = 0.4f;
	}

	if (!HasFoundPlayer || g_PlayerAI == nullptr)
	{
		HasFoundPlayer = SearchPlayer();
	}

	//if (InputManager::GetInstance()->GetKeyTriggered(KeyCode::H))
	//{
	//	ResetLaser();
	//	mIsHostile = !mIsHostile;
	//	mDetectPlayerCooldown_Timer = mDetectPlayerCooldown_Max * 0.4f;

	//	mHostileDelayedMovement = false;
	//	mHostileDelayedMovementTimer = 0.0f;

	//	mHostileDelayedLaser = false;
	//	//mHostileDelayedLaserTimer = 0.0f;
	//	mPlayerInLeftDelay = false;

	//	mHostileInitialNoContactTimer = mHostileInitialNoContactTime;
	//}
	mHostileInitialNoContactTimer = std::clamp(mHostileInitialNoContactTimer - (float)WindowsManager::GetInstance()->mDeltaTime, 0.0f, mHostileInitialNoContactTime);

	glm::vec3 campos = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition;

	if (mIsHostile && (!mPlayerInLeftDelay || mTargetAlpha >= 0.55f))
	{
		EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Renderer>()->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	}
	else
	{
		EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Renderer>()->mRenderLayer = Renderer::RENDER_LAYER::MIDGROUND;
	}

	if (HasFoundPlayer && mIsVisible)
	{
		Vector3 ScreenMax = Vector3(campos.x + (GraphicsSystem::GetInstance()->mWorldSize.x / 2), campos.y + (GraphicsSystem::GetInstance()->mWorldSize.y / 2), 0.f);
		Vector3 ScreenMin = Vector3(campos.x - (GraphicsSystem::GetInstance()->mWorldSize.x / 2), campos.y - (GraphicsSystem::GetInstance()->mWorldSize.y / 2), 0.f);
		Vector3 playerScreenRelativeCoords = Vector3((g_PlayerAITransform->GetPosition().mX - ScreenMin.mX) / (ScreenMax.mX - ScreenMin.mX), (g_PlayerAITransform->GetPosition().mY - ScreenMin.mY) / (ScreenMax.mY - ScreenMin.mY), 0.f);

		if (mHostileDelayedMovement)
		{
			if (mIsHostile)
			{
				mHostileDelayedMovementTimer -= (float)WindowsManager::GetInstance()->mDeltaTime;
				if (mHostileDelayedMovementTimer <= FLT_EPSILON)
				{
					mHostileDelayedMovement = false;
					mTargetMovePos.mX = -1.85f - 0.8f;
				}
			}
			else
			{
				mHostileDelayedMovement = false;
				mHostileDelayedMovementTimer = 0.0f;
			}
		}

		if (!mIsHostileLockedOn || hostileLockedOn_Phase == 4)
		{
			float tempspeed = 0.5f;
			if (!mIsHostile) tempspeed = 3.0f;
			mHostileDelayedLaserTimer = std::clamp((float)mHostileDelayedLaserTimer - tempspeed * (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, (float)mHostileDelayedLaserTime);
			if (mHostileDelayedLaserTimer <= FLT_EPSILON && mHostileDelayedLaser)
			{
				mHostileDelayedLaser = false;
			}
		}

		if (mIsHostile /*&& !(!mIsHostileLockedOn && mHostileDelayedLaser)*/)
		{
			if (!mPlayerInLeftDelay)
			{
				mTrueAlpha = 0.8f;
			}
			if (!mIsHostileLockedOn && !mPlayerInLeftDelay && ((mDetectPlayerCooldown_Timer <= FLT_EPSILON && IsPlayerInKillRange()) /*|| InputManager::GetInstance()->GetKeyTriggered(KeyCode::B)*/))
			{
				mDetectPlayerCooldown_Timer = mDetectPlayerCooldown_Max;
				mBeamLastSavedCamY = campos.y;
				mBeamLastSavedY = mTargetMovePos.mY;

				mHostileDelayedMovement = false;
				mHostileDelayedMovementTimer = 0.0f;

				mHostileDelayedLaser = false;
				mHostileDelayedLaserTimer = 0.0f;

				mIsHostileLockedOn = true;
				hostileLockedOn_Phase = 1;
				hostileLockedOn_Timer = hostileLockedOn_Phase1Time;

				ent_HB1_Sparks->GetComponent<AnimationComponent>()->ResetAnimation();
				ent_HB1_Sparks->GetComponent<Renderer>()->mEnabled = true;

				ent_HB1_Orb->GetComponent<Renderer>()->mEnabled = true;
				ent_HB1_Orb->GetComponent<Transform>()->mScale = Vector3(0.1f * 30.f, 0.1f * 30.f, 0.f);

				ent_HB1_Sparkle->GetComponent<Renderer>()->mEnabled = true;
				ent_HB1_SparkleLineVertical->GetComponent<Renderer>()->mEnabled = true;
				ent_HB1_SparkleLineHorizontal->GetComponent<Renderer>()->mEnabled = true;
				ent_HB1_Sparkle->GetComponent<Transform>()->mScale = 0.0f * Vector3(20.0f, 40.0f, 1.0f);
				ent_HB1_SparkleLineVertical->GetComponent<Transform>()->mScale = Vector3(3.0f, 0.0f * 100.0f, 1.0f);
				ent_HB1_SparkleLineHorizontal->GetComponent<Transform>()->mScale = Vector3(0.0f * 60.0f, 3.0f, 1.0f);

				SoundComponent* _soundComp = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<SoundComponent>();
				if (_soundComp != nullptr)
				{
					_soundComp->Play();
				}
			}
			//else if (mIsHostileLockedOn && InputManager::GetInstance()->GetKeyTriggered(KeyCode::B))
			//{
			//	ResetLaser();
			//}

			if (mHostileDelayedMovement)
			{

			}
			else if (!mIsHostileLockedOn)
			{
				if (mHostileDelayedLaserTimer <= FLT_EPSILON && !mPlayerInLeftDelay)
				{
					mDetectPlayerCooldown_Timer -= (float)WindowsManager::GetInstance()->mDeltaTime;
				}

				if (!mHostileDelayedLaser)
				{
					mTrueMovePos.mX = -1.85f;
					mTrueMovePos.mY = (g_PlayerAITransform->GetPosition().mY - campos.y) / mMovePosExtents.mY;

					if ((mTargetMovePos - mTrueMovePos).Length() > mMovePosSpeed * (float)WindowsManager::GetInstance()->mDeltaTime)
					{
						mTargetMovePos += (mTrueMovePos - mTargetMovePos) * mMovePosSpeed * (float)WindowsManager::GetInstance()->mDeltaTime;
					}
					else
					{
						mTargetMovePos = mTrueMovePos;
					}
				}
			}
			else
			{
				mTrueMovePos.mX = -1.85f;
				float camYDiff = campos.y - mBeamLastSavedCamY;
				mTrueMovePos.mY = mBeamLastSavedY - camYDiff / mMovePosExtents.mY;
				mTargetMovePos.mY = mTrueMovePos.mY;

				if ((mTargetMovePos - mTrueMovePos).Length() > mMovePosSpeed * (float)WindowsManager::GetInstance()->mDeltaTime)
				{
					mTargetMovePos += (mTrueMovePos - mTargetMovePos) * mMovePosSpeed * (float)WindowsManager::GetInstance()->mDeltaTime;
				}
				else
				{
					mTargetMovePos = mTrueMovePos;
				}

				hostileLockedOn_Timer = std::clamp(hostileLockedOn_Timer - (float)WindowsManager::GetInstance()->mDeltaTime, 0.0f, 999.f);
				float progVal, ballprog;
				switch (hostileLockedOn_Phase)
				{
				case 1:
					ent_HB1_FocusRing->GetComponent<Transform>()->mScale = (1.0f - powf(1.0f - hostileLockedOn_Timer / hostileLockedOn_Phase1Time, 2.5f)) * Vector3(40.0f, 80.0f, 1.0f);
					mEyeRadiusAdditional = std::clamp(mEyeRadiusAdditional + 2.0f * (float)WindowsManager::GetInstance()->mDeltaTime, 0.0f, 1.0f);

					progVal = 1.0f - std::clamp(hostileLockedOn_Timer / (0.03f * hostileLockedOn_Phase1Time), 0.0f, 1.0f);
					ent_HB1_Sparkle->GetComponent<Transform>()->mScale = progVal * Vector3(12.0f, 24.0f, 1.0f);
					ent_HB1_SparkleLineVertical->GetComponent<Transform>()->mScale = Vector3(3.0f - progVal, powf(progVal, 3) * 100.0f, 1.0f);
					ent_HB1_SparkleLineHorizontal->GetComponent<Transform>()->mScale = Vector3(powf(progVal, 3) * 60.0f, 3.0f - progVal, 1.0f);
					
					//if (hostileLockedOn_Timer >= )

					ballprog = 0.0f;
					if (hostileLockedOn_Phase1Time >= hostileLockedOn_Timer && hostileLockedOn_Timer >= hostileLockedOn_Phase1Time * 0.95f)
					{
						ballprog = 0.5f * (1.0f - (hostileLockedOn_Timer - hostileLockedOn_Phase1Time * 0.95f) / (hostileLockedOn_Timer * 0.05f));
					}
					else if (hostileLockedOn_Timer >= hostileLockedOn_Phase1Time * 0.66f)
					{
						ballprog = 0.5f;
					}
					else if (hostileLockedOn_Phase1Time * 0.66f >= hostileLockedOn_Timer && hostileLockedOn_Timer >= hostileLockedOn_Phase1Time * 0.61f)
					{
						ballprog = 0.5f + 0.25f * (1.0f - (hostileLockedOn_Timer - hostileLockedOn_Phase1Time * 0.61f) / (hostileLockedOn_Timer * 0.05f));
					}
					else if (hostileLockedOn_Timer >= hostileLockedOn_Phase1Time * 0.33f)
					{
						ballprog = 0.75f;
					}
					else if (hostileLockedOn_Phase1Time * 0.33f >= hostileLockedOn_Timer && hostileLockedOn_Timer >= hostileLockedOn_Phase1Time * 0.28f)
					{
						ballprog = 0.75f + 0.25f * (1.0f - (hostileLockedOn_Timer - hostileLockedOn_Phase1Time * 0.28f) / (hostileLockedOn_Timer * 0.05f));
					}
					else
					{
						ballprog = 1.0f;
					}


					ent_HB1_Orb->GetComponent<Transform>()->mScale = Vector3((0.1f + 0.80f * ballprog + 0.05f * (sinf((float)glfwGetTime() * PI * 16.0f) + 1.0f)) * 30.f, (0.1f + 0.80f * ballprog + 0.05f * (sinf((float)glfwGetTime() * PI * 16.0f) + 1.0f) ) * 30.f, 0.f);

					if (hostileLockedOn_Timer <= FLT_EPSILON)
					{
						ent_HB1_Orb->GetComponent<Transform>()->mScale = Vector3(30.f, 30.f, 0.f);

						ent_HB1_Sparks->GetComponent<Renderer>()->mEnabled = false;
						hostileLockedOn_Phase = 2;
						hostileLockedOn_Timer = hostileLockedOn_Phase2Time;

						ent_HB2_Sparks->GetComponent<AnimationComponent>()->ResetAnimation();
						ent_HB2_Sparks->GetComponent<Renderer>()->mEnabled = true;

						ent_HB2_RingA->GetComponent<Renderer>()->mEnabled = true;
						ent_HB2_RingA->GetComponent<Transform>()->mScale = 0.5f * Vector3(200, 200, 1);
						ent_HB2_RingB->GetComponent<Renderer>()->mEnabled = true;
						ent_HB2_RingB->GetComponent<Transform>()->mScale = 0.5f * Vector3(200, 200, 1);

						mHyperBeamProgress = 0.0f;
						ent_HB2_HyperBeam->GetComponent<Renderer>()->mEnabled = true;
						ent_HB2_HyperGlow->GetComponent<Renderer>()->mEnabled = true;

					}
					break;
				case 2:
					progVal = (1.0f - hostileLockedOn_Timer / hostileLockedOn_Phase2Time);
					progVal = 1.0f - powf(1.0f - progVal, 2);
					ent_HB2_RingA->GetComponent<Transform>()->mScale = (0.5f + 0.5f * progVal) * Vector3(200, 200, 1);
					ent_HB2_RingB->GetComponent<Transform>()->mScale = (0.5f + 0.5f * progVal) * Vector3(200, 200, 1);

					mHyperBeamProgress = progVal * 0.5f;

					ent_HB1_Sparkle->GetComponent<Transform>()->mScale = (0.1f + 0.9f * (1.0f - progVal)) * Vector3(12.0f, 24.0f, 1.0f);
					ent_HB1_SparkleLineVertical->GetComponent<Transform>()->mScale = Vector3(2.0f, (1.0f - progVal) * 100.0f, 1.0f);
					ent_HB1_SparkleLineHorizontal->GetComponent<Transform>()->mScale = Vector3((1.0f - progVal) * 60.0f, 2.0f, 1.0f);

					ent_HB1_Orb->GetComponent<Transform>()->mScale = Vector3((1.0f - progVal) * 30.f, (1.0f - progVal) * 30.f, 0.f);

					if (hostileLockedOn_Timer <= 0.5f * hostileLockedOn_Phase2Time && hostileLockedOn_Timer + (float)WindowsManager::GetInstance()->mDeltaTime > 0.5f * hostileLockedOn_Phase2Time)
					{
						ent_HB3_FrontBeamSparksA->GetComponent<Renderer>()->mEnabled = true;
						ent_HB3_FrontBeamSparksB->GetComponent<Renderer>()->mEnabled = true;
						ent_HB3_BackBeamSparksA->GetComponent<Renderer>()->mEnabled = true;
						ent_HB3_BackBeamSparksB->GetComponent<Renderer>()->mEnabled = true;
						ent_HB3_BackBeamSparksC->GetComponent<Renderer>()->mEnabled = true;
					}

					if (hostileLockedOn_Timer <= FLT_EPSILON)
					{
						ent_HB1_Orb->GetComponent<Renderer>()->mEnabled = false;
						ent_HB2_Sparks->GetComponent<Renderer>()->mEnabled = false;
						hostileLockedOn_Phase = 3;
						hostileLockedOn_Timer = hostileLockedOn_Phase3Time;

						ent_HB2_RingA->GetComponent<Renderer>()->mEnabled = false;
						ent_HB2_RingB->GetComponent<Renderer>()->mEnabled = false;

						ent_HB3_Sparks->GetComponent<AnimationComponent>()->ResetAnimation();
						ent_HB3_Sparks->GetComponent<Renderer>()->mEnabled = true;

						ent_HB3_FrontBeamSparksA->GetComponent<Renderer>()->mEnabled = true;
						ent_HB3_FrontBeamSparksB->GetComponent<Renderer>()->mEnabled = true;
						ent_HB3_BackBeamSparksA->GetComponent<Renderer>()->mEnabled = true;
						ent_HB3_BackBeamSparksB->GetComponent<Renderer>()->mEnabled = true;
						ent_HB3_BackBeamSparksC->GetComponent<Renderer>()->mEnabled = true;

						ent_HB1_Sparkle->GetComponent<Renderer>()->mEnabled = false;
						ent_HB1_SparkleLineVertical->GetComponent<Renderer>()->mEnabled = false;
						ent_HB1_SparkleLineHorizontal->GetComponent<Renderer>()->mEnabled = false;

						mHyperBeamProgress = 0.5f;
						if (!g_PlayerAI->mIsDead && !g_PlayerAI->mIsReviving && !mPlayerInLeftDelay)
						{
							mCanDamagePlayer = true;
						}
					}
					break;
				case 3: //This is the only phase where the beam can damage the player

					if (mPlayerInLeftDelay)
					{
						mCanDamagePlayer = false;
					}

					if (mCanDamagePlayer && IsPlayerInKillRange())
					{
						g_PlayerAI->Kill();
						mCanDamagePlayer = false;
						DelayNextLaser();
					}

					if (hostileLockedOn_Timer <= FLT_EPSILON)
					{
						hostileLockedOn_Phase = 4;
						hostileLockedOn_Timer = hostileLockedOn_Phase4Time;
						mSwitchToSmallSparks = false;
						mCanDamagePlayer = false;
					}
					break;
				case 4:

					if (hostileLockedOn_Timer <= 0.9f * hostileLockedOn_Phase4Time)
					{
						progVal = (1.0f - hostileLockedOn_Timer / (0.9f * hostileLockedOn_Phase4Time));


						mHyperBeamProgress = 0.74f + 0.015f * progVal;
					}
					else
					{
						progVal = (1.0f - (hostileLockedOn_Timer - 0.9f * hostileLockedOn_Phase4Time) / (0.1f * hostileLockedOn_Phase4Time));
						progVal = 1.0f - powf(1.0f - progVal, 2.5f);

						mHyperBeamProgress = 0.5f + 0.24f * progVal;
					}

					mEyeRadiusAdditional = std::clamp(mEyeRadiusAdditional - (float)WindowsManager::GetInstance()->mDeltaTime, 0.0f, 1.0f);
					
					if (hostileLockedOn_Timer <= 0.9f * hostileLockedOn_Phase4Time && !mSwitchToSmallSparks)
					{
						ent_HB3_FrontBeamSparksA->GetComponent<Renderer>()->mEnabled = false;
						ent_HB3_FrontBeamSparksB->GetComponent<Renderer>()->mEnabled = false;
						ent_HB3_BackBeamSparksA->GetComponent<Renderer>()->mEnabled = false;
						ent_HB3_BackBeamSparksB->GetComponent<Renderer>()->mEnabled = false;
						ent_HB3_BackBeamSparksC->GetComponent<Renderer>()->mEnabled = false;

						ent_HB4_SparksAftermathA->GetComponent<Renderer>()->mEnabled = true;
						ent_HB4_SparksAftermathB->GetComponent<Renderer>()->mEnabled = true;
						ent_HB4_SparksAftermathC->GetComponent<Renderer>()->mEnabled = true;

						mSwitchToSmallSparks = true;
					}
					
					if (hostileLockedOn_Timer <= FLT_EPSILON)
					{
						ent_HB3_FrontBeamSparksA->GetComponent<Renderer>()->mEnabled = false;
						ent_HB3_FrontBeamSparksB->GetComponent<Renderer>()->mEnabled = false;
						ent_HB3_BackBeamSparksA->GetComponent<Renderer>()->mEnabled = false;
						ent_HB3_BackBeamSparksB->GetComponent<Renderer>()->mEnabled = false;
						ent_HB3_BackBeamSparksC->GetComponent<Renderer>()->mEnabled = false;
						ent_HB4_SparksAftermathA->GetComponent<Renderer>()->mEnabled = false;
						ent_HB4_SparksAftermathB->GetComponent<Renderer>()->mEnabled = false;
						ent_HB4_SparksAftermathC->GetComponent<Renderer>()->mEnabled = false;
						ent_HB3_Sparks->GetComponent<Renderer>()->mEnabled = false;
						ent_HB2_HyperBeam->GetComponent<Renderer>()->mEnabled = false;
						ent_HB2_HyperGlow->GetComponent<Renderer>()->mEnabled = false;
						mIsHostileLockedOn = false;
						mSwitchToSmallSparks = false;
						mHyperBeamProgress = 0.0f;
					}
					break;
				default:
					break;
				}
			}
		}
		else
		{
			mEyeRadiusAdditional = std::clamp(mEyeRadiusAdditional - (float)WindowsManager::GetInstance()->mDeltaTime, 0.0f, 1.0f);

			if (mAnchoredFloatIsRight && playerScreenRelativeCoords.mX >= mMovePosPercentThresholdX)
			{
				mAnchoredFloatIsRight = false;
			}
			else if (!mAnchoredFloatIsRight && playerScreenRelativeCoords.mX < 1 - mMovePosPercentThresholdX)
			{
				mAnchoredFloatIsRight = true;
			}
			if (mAnchoredFloatIsTop && playerScreenRelativeCoords.mY >= MovePosPercentThresholdY)
			{
				mAnchoredFloatIsTop = false;
			}
			else if (!mAnchoredFloatIsTop && playerScreenRelativeCoords.mY < 1 - MovePosPercentThresholdY)
			{
				mAnchoredFloatIsTop = true;
			}

			float targetRightAnchor;
			if (mAnchoredFloatIsRight) targetRightAnchor = 1;
			else targetRightAnchor = -1;
			if (fabsf(mRightAnchorSlow - targetRightAnchor) > (1.0f / 0.75f) * (float)WindowsManager::GetInstance()->mDeltaTime)
			{
				mRightAnchorSlow += ((targetRightAnchor - mRightAnchorSlow) / fabsf(targetRightAnchor - mRightAnchorSlow)) * (1.0f / 0.75f) * (float)WindowsManager::GetInstance()->mDeltaTime;
			}
			else
			{
				mRightAnchorSlow = targetRightAnchor;
			}

			float goCloser = 1.0f;
			if (mIsTalking)
			{
				goCloser *= mTalkingDistMag;
			}
			mTrueMovePos.mX = (g_PlayerAITransform->GetPosition().mX + mRightAnchorSlow * mDistMagTestX * goCloser - campos.x) / mMovePosExtents.mX;
			if (mAnchoredFloatIsTop)
			{
				mTrueMovePos.mY = (g_PlayerAITransform->GetPosition().mY + mDistMagTestY * goCloser - campos.y) / mMovePosExtents.mY
					+ 2 * sinf(PI * (0.5f + 0.5f * mRightAnchorSlow))
					;
			}
			else
			{
				mTrueMovePos.mY = (g_PlayerAITransform->GetPosition().mY - mDistMagTestY * goCloser - campos.y) / mMovePosExtents.mY
					- 2 * sinf(PI * (0.5f + 0.5f * mRightAnchorSlow))
					;
			}

			//mTrueMovePos.mX = std::clamp(mTrueMovePos.mX, -20.f, 20.f);
			//mTrueMovePos.mY = std::clamp(mTrueMovePos.mY, -20.f, 20.f);

			if ((mTargetMovePos - mTrueMovePos).Length() > mMovePosSpeed * (float)WindowsManager::GetInstance()->mDeltaTime)
			{
				mTargetMovePos += (mTrueMovePos - mTargetMovePos) * mMovePosSpeed * (float)WindowsManager::GetInstance()->mDeltaTime;
			}
			else
			{
				mTargetMovePos = mTrueMovePos;
			}

		}
	}

	if (!mHostileDelayedMovement)
	{
		mActualLocalPositionNoHover = Vector3(mTargetMovePos.mX * mMovePosExtents.mX, mTargetMovePos.mY * mMovePosExtents.mY, 0.f);
		EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->SetPosition(campos + mActualLocalPositionNoHover + Vector3(0.f, 0.8f * sinf((float)glfwGetTime() * PI), 0.f));
	}

	if (HasFoundPlayer)
	{
		if (mIsHostile && mIsHostileLockedOn && hostileLockedOn_Phase < 4 /*!((!mIsHostileLockedOn || hostileLockedOn_Phase == 4) && mHostileDelayedLaser)*/)
		{
			if ((mTargetCoordinate - Vector3(0.75f, 0.48f, 0.f)).Length() > 1.2f * (float)WindowsManager::GetInstance()->mDeltaTime)
			{
				Vector3 normalizeddir = Vector3(0.75f, 0.48f, 0.f) - mTargetCoordinate;
				normalizeddir.Normalize();
				mTargetCoordinate += normalizeddir * 1.2f * (float)WindowsManager::GetInstance()->mDeltaTime;
			}
			else
			{
				mTargetCoordinate = Vector3(0.75f, 0.48f, 0.f);
			}
		}
		else
		{
			Vector3 directionalVector = Vector3(g_PlayerAITransform->GetPosition().mX - EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition().mX, g_PlayerAITransform->GetPosition().mY - EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition().mY, 0.f);
			if (directionalVector.Length() <= FLT_EPSILON) mTargetCoordinate = Vector3(0.5f, 0.5f, 0.f);
			else
			{
				if (directionalVector.Length() >= 5.f * 10.f)
				{
					Vector3 normalizeddir = directionalVector;
					normalizeddir.Normalize();
					directionalVector = normalizeddir * 5 * 10.f;
				}
				mTargetCoordinate = Vector3(0.5f, 0.5f, 0.f) + mClampRadius * directionalVector / mClampPlayerDistance;
				if (mIsHostile)
					mTargetCoordinate.mX = 0.75f;
			}
		}
	}

	float eyeSpeed;
	if (mIsHostile/*mHostileDelayedLaser*/)
	{
		eyeSpeed = 5.f;
	}
	else
	{
		eyeSpeed = 20.f;
	}

	if ((mCurrentCoordinate - mTargetCoordinate).Length() > eyeSpeed * (float)WindowsManager::GetInstance()->mDeltaTime)
	{
		Vector3 normalizeddir = mTargetCoordinate - mCurrentCoordinate;
		normalizeddir.Normalize();
		mCurrentCoordinate += normalizeddir * eyeSpeed * (float)WindowsManager::GetInstance()->mDeltaTime;
	}
	else
	{
		mCurrentCoordinate = mTargetCoordinate;
	}

	if (fabsf(mTargetAlpha - mTrueAlpha) > 1.1f * (float)(WindowsManager::GetInstance()->mDeltaTime))
	{
		mTargetAlpha += ((mTrueAlpha - mTargetAlpha) / fabsf(mTrueAlpha - mTargetAlpha)) * 1.1f * (float)(WindowsManager::GetInstance()->mDeltaTime);
	}
	else
	{
		mTargetAlpha = mTrueAlpha;
	}

	if (mIsHostile)
	{
		mTileIntensity = 1.0f;
	}
	else
	{
		mTileIntensity = 0.0f;
	}
	if (fabsf(mTileTrueIntensity - mTileIntensity) > (1.0f / 0.6f) * (float)(WindowsManager::GetInstance()->mDeltaTime))
	{
		mTileTrueIntensity += ((mTileIntensity - mTileTrueIntensity) / fabsf(mTileIntensity - mTileTrueIntensity)) * (1.0f / 0.6f) * (float)(WindowsManager::GetInstance()->mDeltaTime);
	}
	else
	{
		mTileTrueIntensity = mTileIntensity;
	}


	Shader* currShader{ nullptr };
	AssetManager::GetInstance()->GetAsset("AIShader", currShader);
	currShader->Use();
	AssetManager::ImageAsset* texture{ nullptr };
	AssetManager::GetInstance()->GetAsset("Misc_CodeJibberishTexture", texture);
	AssetManager::ImageAsset* texture2{ nullptr };
	AssetManager::GetInstance()->GetAsset("Misc_Gam200VillainIris", texture2);
	currShader->SetUniform("uTex2d", (GLint)texture->mTexID);
	glBindTextureUnit(texture->mTexID, texture->mTexID);
	currShader->SetUniform("uTex2d_Iris", (GLint)texture2->mTexID);
	glBindTextureUnit(texture2->mTexID, texture2->mTexID);
	currShader->SetUniform("_Time", (float)glfwGetTime());
	currShader->SetUniform("_TalkProgress", mTalkingProgress);
	currShader->SetUniform("_alphaMultiplier", mTargetAlpha * mOverallAlpha);
	currShader->SetUniform("_eyeRadiusUniform", 0.22f + 0.09f * mEyeRadiusAdditional);
	currShader->SetUniform("_demonVarUniform", mTileTrueIntensity);
	currShader->SetUniform("_eyelidAUniform", 2.0f + 0.75f * mTileTrueIntensity);

	if (HasFoundPlayer)
	{
		currShader->SetUniform("_eyeX", mCurrentCoordinate.mX);
		currShader->SetUniform("_eyeY", mCurrentCoordinate.mY);
	}
	else
	{
		currShader->SetUniform("_eyeX", 0.5f + 0.28f * sin((float)glfwGetTime() * 2.f));
		currShader->SetUniform("_eyeY", 0.34f);
	}
	currShader->UnUse();

	ent_HB1_Orb->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(40.0f, -1.0f, -0.1f));
	ent_HB1_Sparks->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(4.0f, 4.0f * mLaserHeightMagnification, -0.1f));
	ent_HB2_Sparks->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(6.0f, 0.0f, -0.1f));
	ent_HB3_Sparks->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(6.0f, 0.0f, -0.1f));
	ent_HB2_RingA->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(80.0f, 0.0f, -0.1f));
	ent_HB2_RingB->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(160.0f, 0.0f, -0.1f));

	ent_HB4_SparksAftermathA->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(120.0f, 20.0f * mLaserHeightMagnification, -0.1f));
	ent_HB4_SparksAftermathB->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(190.0f + 50.f, -30.0f * mLaserHeightMagnification, -0.1f));
	ent_HB4_SparksAftermathC->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(260.0f + 90.f, 25.0f * mLaserHeightMagnification, -0.1f));

	ent_HB3_FrontBeamSparksA->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(8.0f + 80.0f, 0.0f, -0.2f));
	ent_HB3_FrontBeamSparksB->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(8.0f + 200.0f + 50.f, 0.0f, -0.2f));
	ent_HB3_BackBeamSparksA->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(8.0f + 120.0f, 0.0f, -0.05f));
	ent_HB3_BackBeamSparksB->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(8.0f + 190.0f + 50.f, 0.0f, -0.05f));
	ent_HB3_BackBeamSparksC->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(8.0f + 260.0f + 100.f, 0.0f, -0.05f));

	ent_HB1_Sparkle->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(15.0f, 0, -0.1f));
	ent_HB1_SparkleLineVertical->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(15.0f, 0, -0.1f));
	ent_HB1_SparkleLineHorizontal->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(15.0f, 0, -0.1f));

	ent_HB1_FocusRing->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(15.0f, 0, -0.1f));
	AssetManager::GetInstance()->GetAsset("HB1_FocusRingShader", currShader);
	currShader->Use();
	if (mIsHostileLockedOn && hostileLockedOn_Phase == 1)
	{
		currShader->SetUniform("_Progress", 1.0f - hostileLockedOn_Timer / hostileLockedOn_Phase1Time);
	}
	else
	{
		currShader->SetUniform("_Progress", 0.0f);
	}
	currShader->UnUse();

	ent_HB2_HyperBeam->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(15.0f + 150.0f + 50.f, 0, -0.1f));
	AssetManager::GetInstance()->GetAsset("HB2_HyperBeamShader", currShader);
	currShader->Use();
	AssetManager::GetInstance()->GetAsset("HB3_turbulencenoise", texture);
	AssetManager::GetInstance()->GetAsset("HB3_distortionnoise", texture2);
	currShader->SetUniform("_TurbulenceTex", (GLint)texture->mTexID);
	glBindTextureUnit(texture->mTexID, texture->mTexID);
	currShader->SetUniform("_NoiseTex", (GLint)texture2->mTexID);
	glBindTextureUnit(texture2->mTexID, texture2->mTexID);
	currShader->SetUniform("_ProgressUniform", mHyperBeamProgress);
	currShader->SetUniform("_TimeUniform", (float)glfwGetTime());
	currShader->UnUse();

	ent_HB2_HyperGlow->GetComponent<Transform>()->SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(15.0f + 150.0f + 50.f, 0, -0.04f));
	AssetManager::GetInstance()->GetAsset("HB2_HyperGlowShader", currShader);
	currShader->Use();
	//AssetManager::GetInstance()->GetAsset("whitesquare", texture);
	AssetManager::GetInstance()->GetAsset("HB3_distortionnoise", texture2);
	//currShader->SetUniform("_TurbulenceTex", (GLint)texture->mTexID);
	//glBindTextureUnit(texture->mTexID, texture->mTexID);
	currShader->SetUniform("_NoiseTex", (GLint)texture2->mTexID);
	glBindTextureUnit(texture2->mTexID, texture2->mTexID);
	currShader->SetUniform("_ProgressUniform", mHyperBeamProgress);
	currShader->SetUniform("_TimeUniform", (float)glfwGetTime());
	currShader->UnUse();

	AssetManager::GetInstance()->GetAsset("TileShader", currShader);
	currShader->Use();
	currShader->SetUniform("uIntensity", mTileTrueIntensity);
	currShader->UnUse();
	WindowsManager::GetInstance()->mSceneRedIntensity = mTileTrueIntensity;

	//laser delay offset
	if (!mHostileDelayedMovement && !(GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPlayerSpawnPos.x >= 1500.0f))
	{
		float offsetVary = (float)(mHostileDelayedLaserTimer / mHostileDelayedLaserTime);
		if (offsetVary > 0.8f) //quickly 1 to 0.5 instead of slow 1 to 0.5
		{
			offsetVary = 0.5f + 0.5f * ((offsetVary - 0.8f) / 0.2f);
		}
		else //slowly 0.5 to 0 instead of normal 0.5 to 0
		{
			offsetVary = 0.5f * powf(offsetVary / 0.8f, 1.6f);
		}
		Vector3 currentPos = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition();
		EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->SetPosition(currentPos + Vector3(0.25f * mMovePosExtents.mX * sinf(PI * offsetVary), 0.f, 0.f));
	}

	if (mIsHostile && g_PlayerAI != nullptr && !g_PlayerAI->mIsDead && !g_PlayerAI->mIsReviving && !mPlayerInLeftDelay && mHostileInitialNoContactTimer <= FLT_EPSILON)
	{
		Vector3 distanceVector = Vector3(g_PlayerAITransform->GetPosition().mX, g_PlayerAITransform->GetPosition().mY, 0) - Vector3(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition().mX, EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition().mY, 0);
		if (
			distanceVector.Length() <= 0.5f * (g_PlayerAITransform->mScale.mX + EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->mScale.mX)
			)
		{
			g_PlayerAI->Kill();

			if (hostileLockedOn_Phase == 1 && mIsHostileLockedOn)
			{
				ResetLaser();
			}
		}
	}
}

void AIComponent::Exit()
{
#if _ENGINE
	DebugLogger::GetInstance()->Log("AI Component with entity %i removed\n", mEntID);
#endif // _ENGINE
	AIManager::GetInstance()->RemoveAIComponent(mEntID);
}

//void AIComponent::OnNotify(EventObject* _eventObj)
//{
//	//TODO: Get notified of all the player movements, and set the mCurrentQuadrant to the opposite one
//}

void AIComponent::DisplayProperties()
{
#if _ENGINE
	std::string quadrantText = "";
	switch (mCurrentQuadrant)
	{
	case UP_LEFT:
		quadrantText = "UP_LEFT";
		break;
	case UP_RIGHT:
		quadrantText = "UP_RIGHT";
		break;
	case DOWN_LEFT:
		quadrantText = "DOWN_LEFT";
		break;
	case DOWN_RIGHT:
		quadrantText = "DOWN_RIGHT";
		break;
	}

	MyImGui::MyImGuiStringInput(quadrantText, "Current Quadrant: ");

	MyImGui::MyImGuiVector3Input(mFinalPos, "FinalPos: ");
	MyImGui::MyImGuiFloatInput(mBGMReductionVal, "BGM Reduction Val: ");
#endif
}

void AIComponent::UpdateLasers()
{
	Shader* currShader{ nullptr };
	AssetManager::GetInstance()->GetAsset("LaserShader", currShader);
	currShader->Use();
	AssetManager::ImageAsset* texture{ nullptr };
	AssetManager::GetInstance()->GetAsset("Misc_NoiseTextureGeneral", texture);
	currShader->SetUniform("uTex2d_Noise", (GLint)texture->mTexID);
	glBindTextureUnit(texture->mTexID, texture->mTexID);
	currShader->SetUniform("_Time", (float)glfwGetTime());
	currShader->SetUniform("_Interval", 0.123f);
	currShader->SetUniform("_LaserSpeed", 1.3f);
	currShader->UnUse();
}

void AIComponent::UpdateWinds()
{
	Shader* currShader{ nullptr };
	AssetManager::GetInstance()->GetAsset("WindShader", currShader);
	currShader->Use();
	AssetManager::ImageAsset* texture{ nullptr };
	AssetManager::GetInstance()->GetAsset("Misc_NoiseTextureGeneral", texture);
	currShader->SetUniform("uTex2d_Base", (GLint)texture->mTexID);
	glBindTextureUnit(texture->mTexID, texture->mTexID);
	AssetManager::ImageAsset* texture2{ nullptr };
	AssetManager::GetInstance()->GetAsset("Misc_NoiseTextureFractal", texture2);
	currShader->SetUniform("uTex2d_Noise", (GLint)texture2->mTexID);
	glBindTextureUnit(texture2->mTexID, texture2->mTexID);
	currShader->SetUniform("_Time", (float)glfwGetTime());
	currShader->SetUniform("_WindThreshold", 0.35f);
	currShader->UnUse();

}

bool AIComponent::SearchPlayer()
{
	for (size_t i{}; i < EntityManager::GetInstance()->mEntityList.size(); ++i)
	{
		if ("Player" == EntityManager::GetInstance()->mEntityList[i].GetName())
		{
			g_PlayerAI = EntityManager::GetInstance()->mEntityList[i].GetComponent<Player>();
			g_PlayerAITransform = EntityManager::GetInstance()->mEntityList[i].GetComponent<Transform>();
			if (g_PlayerAI != nullptr)
			{
				return true;
			}
		}
	}
	return false;
}

std::ostream& operator<< (std::ostream& os, const AIComponent& _handler)
{
	os << "AI Component created and attached to Entity ID : " << _handler.mEntID << ". Speed: " << _handler.mMoveSpeed << "| mHeightOffset_range :" << _handler.mHeightOffsetRange;
	return os;
}

void AIComponent::EnableHostileAI()
{
	ResetLaser();
	mIsHostile = true;
	mDetectPlayerCooldown_Timer = mDetectPlayerCooldown_Max * 0.4f;

	mHostileDelayedMovement = false;
	mHostileDelayedMovementTimer = 0.0f;

	mHostileDelayedLaser = false;
	//mHostileDelayedLaserTimer = 0.0f;
	mPlayerInLeftDelay = false;

	mHostileInitialNoContactTimer = mHostileInitialNoContactTime;
}

void AIComponent::DisableHostileAI()
{
	ResetLaser();
	mIsHostile = false;
	mDetectPlayerCooldown_Timer = mDetectPlayerCooldown_Max * 0.4f;

	mHostileDelayedMovement = false;
	mHostileDelayedMovementTimer = 0.0f;

	mHostileDelayedLaser = false;
	//mHostileDelayedLaserTimer = 0.0f;
	mPlayerInLeftDelay = false;

	mHostileInitialNoContactTimer = mHostileInitialNoContactTime;
}

void AIComponent::SetAudioDucking(float _val)
{
	mBGMReductionVal = _val;
}
