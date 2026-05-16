/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Player.cpp
@author     TEO Wen Xi (50%)
@co-author	Jovan Zhuo Wen LOW (50%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Definitions of the player class functions
*//*______________________________________________________________________*/

#include "Player.h"
#if _ENGINE
#include "MyImGui.h"
#include <imgui.h>
#include <imgui_internal.h>
#endif // _ENGINE

#include "Transform.h"
#include "TileComponent.h"
#include "ParticleSystem.h"
#include "Renderer.h"
#include "Key.h"
#include "KeyWall.h"

#include "InputManager.h"
#include "WindowsManager.h"
#include "TextManager.h"
#include "LevelManager.h"

#include <limits>
#include "AnimationComponent.h"
#include "VFXEntity.h"
#include "MathDefines.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CamWaypoint.h"

EntityID Player::mPlayerID = 0;

void Player::Init()
{
	mPhy = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<PhysicsComponent>();
	mMap = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<TileComponent>();
	mPlayerID = mEntID;
	mInitialPosition = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition();

	EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Renderer>()->mEnabled = false;

	if (mLoaded == true)
		return;

	mPhy->mMass = 5.0f;

	//Create player bodies
	mEntBodySprite = EntityManager::GetInstance()->CreateEntity();
	mEntBodySprite->SetName("PlayerBodySprite");
	Renderer* entBody_rend = mEntBodySprite->AddComponent<Renderer>();
	entBody_rend->mTextureName = "Character_PixelSlimeBody";
	entBody_rend->mShaderName = "PlayerBodyShader";
	entBody_rend->mModelType = Renderer::MODEL_TYPE::SPRITE; //IMPORTANT
	EntityManager::GetInstance()->GetEntity(mEntID)->AddChild(mEntBodySprite->GetID());
	Transform newXform{};
	newXform.SetPosition(Vector3{ 0, 0, -0.4f });
	newXform.mScale = mInitialBodyScale;
	mEntBodySprite->AddComponent(newXform);

	mEntEyeHolder = EntityManager::GetInstance()->CreateEntity();
	mEntEyeHolder->SetName("PlayerEyeHolder");
	EntityManager::GetInstance()->GetEntity(mEntID)->AddChild(mEntEyeHolder->GetID());
	newXform.SetPosition(Vector3{ 0, -0.14f, -0.8f });
	newXform.mScale = Vector3{ 1, 1, 1 };
	mEntEyeHolder->AddComponent(newXform);

	mEntEyeLeft = EntityManager::GetInstance()->CreateEntity();
	mEntEyeLeft->SetName("PlayerLeftEyeSprite");
	Renderer* entEyeLeft_rend = mEntEyeLeft->AddComponent<Renderer>();
	entEyeLeft_rend->mTextureName = "Character_PixelSlimeEye";
	entEyeLeft_rend->mModelType = Renderer::MODEL_TYPE::SPRITE; //IMPORTANT
	EntityManager::GetInstance()->GetEntity(mEntID)->AddChild(mEntEyeLeft->GetID());
	newXform.SetPosition(Vector3{ -2.69f * 0.5f, -1.88f * 0.5f, 0 });
	newXform.mScale = mInitialEyeLeftScale;
	mEntEyeLeft->AddComponent(newXform);

	mEntEyeRight = EntityManager::GetInstance()->CreateEntity();
	mEntEyeRight->SetName("PlayerRightEyeSprite");
	Renderer* entEyeRight_rend = mEntEyeRight->AddComponent<Renderer>();
	entEyeRight_rend->mTextureName = "Character_PixelSlimeEye";
	entEyeRight_rend->mModelType = Renderer::MODEL_TYPE::SPRITE; //IMPORTANT
	EntityManager::GetInstance()->GetEntity(mEntID)->AddChild(mEntEyeRight->GetID());
	newXform.SetPosition(Vector3{ 7.72f * 0.5f, -1.88f * 0.5f, 0 });
	newXform.mScale = mInitialEyeRightScale;
	mEntEyeRight->AddComponent(newXform);

}
bool Player::GetJumpKey_Hold()
{
	if (mCutsceneTimer > 0)
		return false;

	return InputManager::GetInstance()->GetAction("PlayerJump");
}
bool Player::GetJumpKey_OnPress()
{
	if (mCutsceneTimer > 0)
		return false;

	return InputManager::GetInstance()->GetActionTriggered("PlayerJump");
}

bool Player::IsWindAffected()
{
	return mWindAttachedProgress < FLT_EPSILON;
}

void Player::SetSpriteFlip(bool _right, bool _immediate)
{
	if (_right)
	{
		mEyeLInternal = mEyeLLocalRight;
		mEyeRInternal = mEyeRLocalRight;
		mBodyFacingInternal = 1;
	}
	else
	{
		mEyeLInternal = mEyeLLocalLeft;
		mEyeRInternal = mEyeRLocalLeft;
		mBodyFacingInternal = 0;
	}

	if (_immediate)
	{
		mEyeLCurrent = mEyeLInternal;
		mEyeRCurrent = mEyeRInternal;
		mBodyFacingCurrent = mBodyFacingInternal;
	}
}

float Player::SmoothStep(float _edge0, float _edge1, float _x) {
	_x = std::clamp((_x - _edge0) / (_edge1 - _edge0), 0.f, 1.f);
	return _x * _x * (3.0f - 2.0f * _x);
}

bool Player::GroundLineRaycast()
{
	if (mDirectionalVector.mY > FLT_EPSILON) return false;

	Transform* xform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();
	float bottomx1, bottomx2, bottomy;
	float scaleValue = mZoomScale * mPhy->mObjBigScale.mY;

	if (ShouldExtendBottom())
		bottomx1 = (float)((int)(xform->GetPosition().mX - scaleValue / 2));
	else
		bottomx1 = (float)((int)(xform->GetPosition().mX - scaleValue / 4));
	if (ShouldExtendBottom())
		bottomx2 = (float)((int)(xform->GetPosition().mX + scaleValue / 2));
	else
		bottomx2 = (float)((int)(xform->GetPosition().mX + scaleValue / 4));
	bottomy = (xform->GetPosition().mY - scaleValue / 2);

	//mDebugDisplayValue = ((float)bottomy - TileComponent::mTileSize - TileComponent::mMinBound.mY) / TileComponent::mTileSize;
	int estTileHeight_Binary = (int)(((float)bottomy - TileComponent::mTileSize - TileComponent::mMinBound.mY) / TileComponent::mTileSize);
	float estTileHeight = estTileHeight_Binary * TileComponent::mTileSize + TileComponent::mMinBound.mY + TileComponent::mTileSize;

	//ONE: TILE MUST SHARE THEIR TOP HEIGHT WITH THE PLAYER'S BOTTOM-Y
	if (fabsf(bottomy - estTileHeight) <= FLT_EPSILON && estTileHeight_Binary >= 0 && estTileHeight_Binary < TileComponent::mMapHeight)
	{
		//TWO: TILE MUST EITHER BE AT THE HIGHEST HEIGHT OR NOT HAVE A TILE ABOVE IT
		int leftEnd = (int)(((float)bottomx1 - TileComponent::mTileSize - TileComponent::mMinBound.mX) / TileComponent::mTileSize) + 1;
		int rightEnd = (int)(((float)bottomx2 - TileComponent::mTileSize - TileComponent::mMinBound.mX) / TileComponent::mTileSize) + 1;
		if (leftEnd < 0) leftEnd = 0;
		if (leftEnd >= TileComponent::mMapWidth) leftEnd = TileComponent::mMapWidth - 1;
		if (rightEnd < 0) rightEnd = 0;
		if (rightEnd >= TileComponent::mMapWidth) rightEnd = TileComponent::mMapWidth - 1;
		
		for (int i = leftEnd; i <= rightEnd; ++i)
		{
			if (TileComponent::mBinaryMap[estTileHeight_Binary][i] == 1
				&&
				(
					estTileHeight_Binary == TileComponent::mMapHeight - 1
				||
					TileComponent::mBinaryMap[estTileHeight_Binary + 1][i] == 0
				))
			{
				return true;
			}
		}
	}


//ref
	//std::pair<int, int> YourCurrentTile = TileComponent::GetCellValue_YX(xform->GetPosition().mX, xform->GetPosition().mY).second;

	//float FakeRaycastedTile = YourCurrentTile.second - 1;
	//if (FakeRaycastedTile < TileComponent::mMapHeight
	//	&& TileComponent::mBinaryMap[FakeRaycastedTile][YourCurrentTile.first] == 1)
	//{
	//	float theNewYWithRadius = traj + xform->GetPosition().mY
	//		- mZoomScale * mPhy->mObjBigScale.mY / 2.f;
	//	float theYBoundary = FakeRaycastedTile * TileComponent::mTileSize + TileComponent::mMinBound.mY
	//		+ TileComponent::mTileSize / 2.f + TileComponent::mTileSize / 2.f;

	//	if (theNewYWithRadius <= theYBoundary)
	//	{
	//		traj -= theNewYWithRadius - theYBoundary;
	//		mDebugDisplayValue = rand();
	//		mIsOnGround = true;
	//		SoundManager::GetInstance()->PlaySound("SFX_Landing", SoundManager::GetInstance()->GetChannelVolume(), SoundManager::GetInstance()->GetChannelPitch());
	//	}
	//}





	//RaycastHit2D hit = GetGroundHitLineLEFT;
	//if (hit.collider == null) return false;
	//RaycastHit2D easygroundhit = Physics2D.Raycast(transform.position, -Vector2.up, GetComponent<Collider2D>().bounds.size.y / 2 + 0.02f, LayerMask.GetMask("Platform"));
	//if (easygroundhit == true) return true;

	//float theheight = GetComponent<CircleCollider2D>().bounds.max.y - GetComponent<CircleCollider2D>().bounds.min.y;
	//float teenyoffset = 0.01f;
	//if (hit.point.x < transform.position.x) teenyoffset = -teenyoffset;
	//RaycastHit2D emptySpaceChecker =
	//	Physics2D.Raycast(
	//		new Vector3(hit.point.x + teenyoffset, 0.05f * theheight + GetComponent<CircleCollider2D>().bounds.min.y, 0),
	//		Vector2.up,
	//		theheight * 0.95f,
	//		LayerMask.GetMask("Platform")
	//	);

	//return emptySpaceChecker.collider == null;

	return false;
}

void Player::FallingFunction()
{
	Transform* xform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();

	if (!mIsOnGround)
	//if (!(mIsJumping && mTotalJumpDurationTimer >= FLT_EPSILON))
	{
		if (mDisableGravityTimer > 0)
		{
			mDisableGravityTimer -= (float)WindowsManager::GetInstance()->mDeltaTime;
			return;
		}

		mMinimumJumpDurationTimer -= (float)WindowsManager::GetInstance()->mDeltaTime;
		if (mMinimumJumpDurationTimer <= 0.0f && !mIsJumping)
		{
			float lowestbidder = 1;
			//for (int i = 0; i < AttachedWindList.Count; ++i)
			//{
			//	AttachedWind wind = AttachedWindList[i];
			//	float progval = 1 - wind.progress * SmallBigZoomWeightMultiplier; //1 - wind.progress * (1 - Mathf.Clamp((mZoomScale - ZoomLvlStorage[0]) / (ZoomLvlStorage[1] - ZoomLvlStorage[0]), 0, 1));
			//	if (progval < lowestbidder)
			//	{
			//		lowestbidder = progval;
			//	}
			//}
			lowestbidder = 1 - mWindAttachedProgress;

			bool isGroundPoundEnabled = false;
			float isGroundPoundEnabledMultiplier = 0.0f;
			if (isGroundPoundEnabled) isGroundPoundEnabledMultiplier = 1.0f;

			//additional weight
			float test2 = mGravitationalforce;
			if (isGroundPoundEnabled)
			{
				test2 *= lowestbidder;
			}
			float thecap = (test2 + 2 * mGravitationalforce * isGroundPoundEnabledMultiplier * lowestbidder) * (0.3f + 0.7f * SmoothStep(mZoomLvlStorageSmall, mZoomLvlStorageNormal, mZoomScale));
			if (mRisingGravity < thecap) mRisingGravity += mGravityRiseRate * (float)WindowsManager::GetInstance()->mDeltaTime;
			else mRisingGravity = thecap;
			
			float risingGravityToUse = mRisingGravity;

			float zoomhi = powf(mZoomScale, 1.3f);

			//bool prevGrounded = mIsOnGround;
			float traj = -1.0f * lowestbidder * zoomhi * risingGravityToUse * (float)WindowsManager::GetInstance()->mDeltaTime;

			float bottomx1, bottomx2, bottomy;
			float scaleValue = mZoomScale * mPhy->mObjBigScale.mY;
			if (ShouldExtendBottom())
				bottomx1 = (float)((int)(xform->GetPosition().mX - scaleValue / 2));
			else
				bottomx1 = (float)((int)(xform->GetPosition().mX - scaleValue / 4));
			if (ShouldExtendBottom())
				bottomx2 = (float)((int)(xform->GetPosition().mX + scaleValue / 2));
			else
				bottomx2 = (float)((int)(xform->GetPosition().mX + scaleValue / 4));
			bottomy = (xform->GetPosition().mY - scaleValue / 2);
			int estTileHeight_Binary = (int)(((float)bottomy - TileComponent::mTileSize - TileComponent::mMinBound.mY) / TileComponent::mTileSize);
			//float estTileHeight = estTileHeight_Binary * TileComponent::mTileSize + TileComponent::mMinBound.mY + TileComponent::mTileSize;
			int leftEnd = (int)(((float)bottomx1 - TileComponent::mTileSize - TileComponent::mMinBound.mX) / TileComponent::mTileSize) + 1;
			int rightEnd = (int)(((float)bottomx2 - TileComponent::mTileSize - TileComponent::mMinBound.mX) / TileComponent::mTileSize) + 1;
			if (leftEnd < 0) leftEnd = 0;
			if (leftEnd >= TileComponent::mMapWidth) leftEnd = TileComponent::mMapWidth - 1;
			if (rightEnd < 0) rightEnd = 0;
			if (rightEnd >= TileComponent::mMapWidth) rightEnd = TileComponent::mMapWidth - 1;

			for (int i = leftEnd; i <= rightEnd; ++i)
			{
				if (estTileHeight_Binary >= 0 && estTileHeight_Binary < TileComponent::mMapHeight
					&& TileComponent::mBinaryMap[estTileHeight_Binary][i] == 1
					&& (
						estTileHeight_Binary == TileComponent::mMapHeight - 1
						||
						TileComponent::mBinaryMap[estTileHeight_Binary + 1][i] == 0
						))
				{
					float theNewYWithRadius = traj + xform->GetPosition().mY
						- mZoomScale * mPhy->mObjBigScale.mY / 2.f;
					float theYBoundary = estTileHeight_Binary * TileComponent::mTileSize + TileComponent::mMinBound.mY
						+ TileComponent::mTileSize / 2.f + TileComponent::mTileSize / 2.f;

					if (theNewYWithRadius <= theYBoundary)
					{
						traj -= theNewYWithRadius - theYBoundary;
						mDebugDisplayValue = (float)rand();
						mIsOnGround = true;
						if (mWasJumping)
						{
							mWasJumping = false;
							SoundManager::GetInstance()->PlaySound("SFX_Landing", SoundManager::GetInstance()->GetSFXVolume(), SoundManager::GetInstance()->GetChannelPitch());
						}
						break;
					}
				}
			}


			//std::pair<int, int> YourCurrentTile = TileComponent::GetCellValue_YX(xform->GetPosition().mX, xform->GetPosition().mY).second;

			//float FakeRaycastedTile = YourCurrentTile.second - 1;
			//if (FakeRaycastedTile < TileComponent::mMapHeight
			//	&& TileComponent::mBinaryMap[FakeRaycastedTile][YourCurrentTile.first] == 1)
			//{
			//	float theNewYWithRadius = traj + xform->GetPosition().mY
			//		- mZoomScale * mPhy->mObjBigScale.mY / 2.f;
			//	float theYBoundary = FakeRaycastedTile * TileComponent::mTileSize + TileComponent::mMinBound.mY
			//		+ TileComponent::mTileSize / 2.f + TileComponent::mTileSize / 2.f;

			//	if (theNewYWithRadius <= theYBoundary)
			//	{
			//		traj -= theNewYWithRadius - theYBoundary;
			//		mDebugDisplayValue = rand();
			//		mIsOnGround = true;
			//		SoundManager::GetInstance()->PlaySound("SFX_Landing", SoundManager::GetInstance()->GetChannelVolume(), SoundManager::GetInstance()->GetChannelPitch());
			//	}
			//}

			xform->SetPosition(Vector3(xform->GetPosition().mX,
				traj + xform->GetPosition().mY,
				xform->GetPosition().mZ));
			mLastFallingValue = traj;



			//if (CheckIfGroundedBoolean())
			//{
			//	if (mRisingGravity > 14.0f)
			//	{
			//		CreateJumpParticle_Land();
			//	}
			//	SFX_Landing.Play();
			//	mRisingGravity = 0.0f;
			//	TimerToGroundParticle = TimeToGroundParticle / 2;
			//	IsGrounded = true;
			//	currentsurfacenormal = new Vector2(0, 1); //hit.normal;
			//}

			//if (prevGrounded == false && IsGrounded == true)
			//{
			//	RaycastHit2D temphit = GetGroundHitLineLEFT;
			//	if (temphit.collider != null) transform.position = new Vector3(transform.position.x, temphit.point.y + groundhitOffset + GetComponent<Collider2D>().bounds.size.y / 2, transform.position.z);
			//}
		}
	}
}

void Player::Update()
{
	Transform* xform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();
	mPhy = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<PhysicsComponent>();
	mMap = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<TileComponent>();

	Vector3 prevpos = xform->GetPosition();

	//Cutscene timer
	if (mCutsceneTimer > 0)
		mCutsceneTimer -= (float)WindowsManager::GetInstance()->mDeltaTime;

	//LevelManager::GetInstance()->UpdateCameraInfo(xform->GetPosition());

	if (mIsDead)
	{
		//mPhy->mEnabled = false;
		if (mDeathDurationTimer > FLT_EPSILON)
		{
			mEntBodySprite->GetComponent<Transform>()->mScale = Vector3{ 0, 0, 0 };
			mEntEyeLeft->GetComponent<Transform>()->mScale = Vector3{ 0, 0, 0 };
			mEntEyeRight->GetComponent<Transform>()->mScale = Vector3{ 0, 0, 0 };

			if (mDelayRespawnTimer > 0)
			{
				mDelayRespawnTimer -= (float)WindowsManager::GetInstance()->mDeltaTime;
				return;
			}

			mDeathDurationTimer = std::clamp(mDeathDurationTimer - (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, mDeathDurationTimer);

			if (mDeathDurationTimer <= FLT_EPSILON)
			{
				Respawn();
				mIsReviving = true;
			}
			else
			{
				return;
			}
		}
		else if (mReviveDurationTimer > FLT_EPSILON)
		{
			mReviveDurationTimer = std::clamp(mReviveDurationTimer - (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, mReviveDurationTimer);

			Shader* currBodyShader{ nullptr };
			AssetManager::GetInstance()->GetAsset("PlayerBodyShader", currBodyShader);
			currBodyShader->Use();
			float visualRespawnProgress = std::clamp(1.2f * (1.f - mReviveDurationTimer / mReviveDurationTime), 0.f, 1.f);
			currBodyShader->SetUniform("_RespawnProgress", visualRespawnProgress);
			currBodyShader->UnUse();

			mEntBodySprite->GetComponent<Transform>()->mScale = mZoomScale * mInitialBodyScale;
			if (visualRespawnProgress >= 1.0f - FLT_EPSILON)
			{
				mEntEyeLeft->GetComponent<Transform>()->mScale = mZoomScale * mInitialEyeLeftScale;
				mEntEyeRight->GetComponent<Transform>()->mScale = mZoomScale * mInitialEyeRightScale;
			}
			else
			{
				mEntEyeLeft->GetComponent<Transform>()->mScale = Vector3{ 0, 0, 0 };
				mEntEyeRight->GetComponent<Transform>()->mScale = Vector3{ 0, 0, 0 };
			}

			if (mReviveDurationTimer <= FLT_EPSILON)
			{
				//mPhy->mEnabled = true;
				mIsDead = false;
				mIsReviving = false;

				mEntBodySprite->GetComponent<Transform>()->mScale = mZoomScale * mInitialBodyScale;
				mEntEyeLeft->GetComponent<Transform>()->mScale = mZoomScale * mInitialEyeLeftScale;
				mEntEyeRight->GetComponent<Transform>()->mScale = mZoomScale * mInitialEyeRightScale;
			}
			else
			{
				//Do not play death sound if cutscene is going on
				if (mCutsceneTimer > 0 || mDeathPlayed == true)
					return;

				TextManager::GetInstance()->SetVisualConsoleText(mPlayerDeathText, mPlayerDeathFont, mPlayerDeathTextTime);
				DialogueTrigger* dialogueTrigger = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<DialogueTrigger>();
				if (dialogueTrigger != nullptr)
					dialogueTrigger->PlayTrigger();
				mCurrentLevelDeaths++;
				mDeathPlayed = true;
				return;
			}

		}
	}

	mMinimumJumpDurationTimer = std::clamp(mMinimumJumpDurationTimer - (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, mMinimumJumpDurationTime);
	mTotalJumpDurationTimer = std::clamp(mTotalJumpDurationTimer - (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, mTotalJumpDurationTime);

	//WIND TUNNEL STUFF
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	if (mPhy->mIsCharacterWindTunnelUp) {
		mPhy->mWindTunnelForce.mY = TileComponent::mWindTunnelForceY * mPhy->mCharacterWindFactor;

	}
	else if (mPhy->mIsCharacterWindTunnelRight) {
		mPhy->mWindTunnelForce.mX = TileComponent::mWindTunnelForceX * mPhy->mCharacterWindFactor;

	}
	else if (mPhy->mIsCharacterWindTunnelLeft) {
		mPhy->mWindTunnelForce.mX = -TileComponent::mWindTunnelForceX * mPhy->mCharacterWindFactor;

	}
	else {
		mPhy->mWindTunnelForce.mX = 0;
		mPhy->mWindTunnelForce.mY = 0;
	}

	//Reset
	mPhy->mIsCharacterWindTunnelLeft = false;
	mPhy->mIsCharacterWindTunnelRight = false;
	mPhy->mIsCharacterWindTunnelUp = false;

	//Wind tunnel update
	if (mPhy->mWindTunnelForce.Length() > FLT_EPSILON)
	{
		mLastSavedExistentWindForce = mPhy->mWindTunnelForce;
		mWindAttachedProgress = std::clamp(mWindAttachedProgress + (1.f / mWindTransitionTime) * (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, 1.f);
	}
	else
	{
		mWindAttachedProgress = std::clamp(mWindAttachedProgress - (1.f / mWindExitTime) * (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, 1.f);
	}
	if (mWindAttachedProgress > FLT_EPSILON)
	{
		float WindHorizontal = (0.2f + 0.8f * mWindAttachedProgress) * mLastSavedExistentWindForce.mX * (float)WindowsManager::GetInstance()->mDeltaTime;
		float WindVertical = (0.2f + 0.8f * mWindAttachedProgress) * mLastSavedExistentWindForce.mY * (float)WindowsManager::GetInstance()->mDeltaTime;

		std::pair<int, int> YourCurrentTile = TileComponent::GetCellValue_YX((int)xform->GetPosition().mX, (int)xform->GetPosition().mY).second;

		//Horizontal
		if (WindHorizontal < -FLT_EPSILON) //Move Left
		{
			float FakeRaycastedTile = YourCurrentTile.first - 1.f;
			if (FakeRaycastedTile >= 0
				&& TileComponent::mBinaryMap[YourCurrentTile.second][(int)FakeRaycastedTile] == 1)
			{
				float theNewXWithRadius = WindHorizontal + xform->GetPosition().mX - mZoomScale * mPhy->mObjBigScale.mX / 2.f;
				float theXBoundary = FakeRaycastedTile * TileComponent::mTileSize + TileComponent::mMinBound.mX + TileComponent::mTileSize;

				if (theNewXWithRadius < theXBoundary)
				{
					WindHorizontal -= theNewXWithRadius - theXBoundary;
				}
			}
		}
		else if (WindHorizontal > FLT_EPSILON) //Move Right
		{
			float FakeRaycastedTile = YourCurrentTile.first + 1.f;
			if (FakeRaycastedTile < TileComponent::mMapWidth
				&& TileComponent::mBinaryMap[YourCurrentTile.second][(int)FakeRaycastedTile] == 1)
			{
				float theNewXWithRadius = WindHorizontal + xform->GetPosition().mX + mZoomScale * mPhy->mObjBigScale.mX / 2.f;
				float theXBoundary = FakeRaycastedTile * TileComponent::mTileSize + TileComponent::mMinBound.mX;

				if (theNewXWithRadius > theXBoundary)
				{
					WindHorizontal -= theNewXWithRadius - theXBoundary;
				}
			}
		}

		//Vertically Upwards
		float FakeRaycastedTile = (float)(YourCurrentTile.second + 1);
		if (FakeRaycastedTile < TileComponent::mMapHeight
			&& TileComponent::mBinaryMap[(int)FakeRaycastedTile][YourCurrentTile.first] == 1)
		{
			float theNewYWithRadius = WindVertical + xform->GetPosition().mY + mZoomScale * mPhy->mObjBigScale.mY / 2.f;
			float theYBoundary = FakeRaycastedTile * TileComponent::mTileSize + TileComponent::mMinBound.mY;

			if (theNewYWithRadius > theYBoundary)
			{
				WindVertical -= theNewYWithRadius - theYBoundary;
			}
		}

		xform->SetPosition(Vector3(WindHorizontal + xform->GetPosition().mX,
			WindVertical + xform->GetPosition().mY,
			xform->GetPosition().mZ));
	}

	//END OF WIND TUNNEL STUFF
	//----------------------------------------------------------------------------------------------------------------------------------------------------

	if (mIsOnGround)
	{
		mCoyoteTimer = 0.0f;
}
	else
	{
		//if (CurrentPreviousPosition.y > transform.position.y)
		if (!mIsJumping || mWindAttachedProgress < FLT_EPSILON)
		{
			mCoyoteTimer = std::clamp(mCoyoteTimer + (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, mCoyoteTimeMax);
		}
		else
		{
			mCoyoteTimer = mCoyoteTimeMax;
		}
	}

	if (mCutsceneTimer <= 0 && InputManager::GetInstance()->GetAction("PlayerShrink"))
	{
		if (mPhy->mIsCharacterSmall == false)
		{
			SoundManager::GetInstance()->PlaySound("SFX_ChangeSizeForward", SoundManager::GetInstance()->GetSFXVolume(), SoundManager::GetInstance()->GetChannelPitch());
		}
		mPhy->mIsCharacterSmall = true;
		mZoomScaleTarget = mZoomLvlStorageSmall;
	}
	else
	{
		if (mPhy->mIsCharacterSmall == true)
		{
			SoundManager::GetInstance()->PlaySound("SFX_ChangeSizeBackward", SoundManager::GetInstance()->GetSFXVolume(), SoundManager::GetInstance()->GetChannelPitch());
		}
		mPhy->mIsCharacterSmall = false;
		mZoomScaleTarget = mZoomLvlStorageNormal;
	}

	if (fabsf(mZoomScale - mZoomScaleTarget) > FLT_EPSILON)
	{
		float changeVal = ((mZoomScaleTarget - mZoomScale) / fabsf(mZoomScaleTarget - mZoomScale))
			* std::clamp(fabsf(mZoomScaleTarget - mZoomScale), 0.1f, 1.0f) * mZoomSpeed * (float)WindowsManager::GetInstance()->mDeltaTime;

		if (fabsf(changeVal) > fabsf(mZoomScaleTarget - mZoomScale))
		{
			mZoomScale = mZoomScaleTarget;
		}
		else
			mZoomScale += changeVal;

		if (changeVal > FLT_EPSILON)
		{
			std::pair<int, int> YourCurrentTile = TileComponent::GetCellValue_YX((int)xform->GetPosition().mX, (int)xform->GetPosition().mY).second;

			float FakeRaycastedTile;

			//LEFT
			FakeRaycastedTile = (float)(YourCurrentTile.first - 1);
			if (FakeRaycastedTile >= 0
				&& TileComponent::mBinaryMap[YourCurrentTile.second][(int)FakeRaycastedTile] == 1)
			{
				float theNewXWithRadius = xform->GetPosition().mX
					- mZoomScale * mPhy->mObjBigScale.mX / 2.f;
				float theXBoundary = FakeRaycastedTile * TileComponent::mTileSize + TileComponent::mMinBound.mX
					+ TileComponent::mTileSize / 2.f + TileComponent::mTileSize / 2.f;

				if (theNewXWithRadius < theXBoundary)
				{
					xform->SetPosition(Vector3{ xform->GetPosition().mX - (theNewXWithRadius - theXBoundary),
						xform->GetPosition().mY,
						xform->GetPosition().mZ });
				}
			}

			//RIGHT
			FakeRaycastedTile = (float)(YourCurrentTile.first + 1);
			if (FakeRaycastedTile < TileComponent::mMapWidth
				&& TileComponent::mBinaryMap[YourCurrentTile.second][(int)FakeRaycastedTile] == 1)
			{
				float theNewXWithRadius = xform->GetPosition().mX
					+ mZoomScale * mPhy->mObjBigScale.mX / 2.f;
				float theXBoundary = FakeRaycastedTile * TileComponent::mTileSize + TileComponent::mMinBound.mX
					+ TileComponent::mTileSize / 2.f - TileComponent::mTileSize / 2.f;

				if (theNewXWithRadius > theXBoundary)
				{
					xform->SetPosition(Vector3{ xform->GetPosition().mX - (theNewXWithRadius - theXBoundary),
						xform->GetPosition().mY,
						xform->GetPosition().mZ });
				}
			}

			//DOWN
			FakeRaycastedTile = (float)(YourCurrentTile.second - 1);
			if (FakeRaycastedTile >= 0
				&& TileComponent::mBinaryMap[(int)FakeRaycastedTile][YourCurrentTile.first] == 1)
			{
				float theNewYWithRadius = xform->GetPosition().mY
					- mZoomScale * mPhy->mObjBigScale.mY / 2.f;
				float theYBoundary = FakeRaycastedTile * TileComponent::mTileSize + TileComponent::mMinBound.mY
					+ TileComponent::mTileSize / 2.f + TileComponent::mTileSize / 2.f;

				if (theNewYWithRadius <= theYBoundary)
				{
					xform->SetPosition(Vector3{ xform->GetPosition().mX,
						xform->GetPosition().mY - (theNewYWithRadius - theYBoundary),
						xform->GetPosition().mZ });
				}
			}

			////UP
			//FakeRaycastedTile = YourCurrentTile.second + 1;
			//if (FakeRaycastedTile < TileComponent::mMapHeight
			//	&& TileComponent::mBinaryMap[FakeRaycastedTile][YourCurrentTile.first] == 1)
			//{
			//	float theNewYWithRadius = xform->GetPosition().mY
			//		+ mZoomScale * mPhy->mObjBigScale.mY / 2.f;
			//	float theYBoundary = FakeRaycastedTile * TileComponent::mTileSize + TileComponent::mMinBound.mY
			//		+ TileComponent::mTileSize / 2.f - TileComponent::mTileSize / 2.f;

			//	if (theNewYWithRadius <= theYBoundary)
			//	{
			//		xform->SetPosition(Vector3{ xform->GetPosition().mX,
			//			xform->GetPosition().mY - (theNewYWithRadius - theYBoundary),
			//			xform->GetPosition().mZ });
			//	}
			//}
		}
	}

	if (mIsOnGround)
	{
		mRisingGravity = 0;
	}
	
	//UPDATE COLLISION HITBOX HERE
	//xform->mScale = Vector3(mZoomScale, mZoomScale, 1.f); //PLAYER SCALING POINT
	mPhy->mCharacterSizeFactor = mZoomScale;
	mPhy->mCharacterWindFactor = 1.0f - powf(1.0f - (1.f - mZoomScale) / (mZoomLvlStorageNormal - mZoomLvlStorageSmall), 2.f);


	//mDebugDisplayValue = TileComponent::GetCoord(xform->GetPosition().mX, xform, TileComponent::COLLIDED_DIRECTION::RIGHT);

	//PLAYER MOVEMENT CURRENTLY
	//if (mIsJumping)
	//{
	//	if ((mMinimumJumpDurationTimer <= FLT_EPSILON &&
	//		!GetJumpKey_Hold()) //Did the player want to stop jumping?
	//		|| mPhy->mCurrentVelocity.mY <= FLT_EPSILON //Is jumping velocity zero?
	//		|| mTotalJumpDurationTimer <= FLT_EPSILON //Can't seem to use this properly due to Force system
	//		|| mMap->mGridCollisionFlag & TileComponent::COLLISION_TOP) //Hit a Ceiling and bonked, = will fall
	//	{
	//		mIsJumping = false;
	//		mPhy->mCurrentVelocity.mY = 0;
	//		mPhy->mJumpForce.mY = 0;
	//	}
	//}
	//else
	//{
	//	//if X and W is pressed at the same time
	//	//if (!mIsXTriggered && (mPhy->mIsCharacterSmall) && (GetJumpKey_OnPress()))
	//	//{
	//	//	JumpBig();
	//	//	mIsXTriggered = true;
	//	//}
	//	//mIsXTriggered = (InputManager::GetInstance()->GetKey(KeyCode::X));

	//	if (GetJumpKey_OnPress())
	//	{
	//		if (true == mIsOnGround)
	//		{
	//			Jump();
	//			//if (mPhy->mIsCharacterSmall)
	//			//	Jump();
	//			//else
	//			//	JumpBig();
	//		}
	//	}
	//}

	FallingFunction();

	if (mIsJumping && !GetJumpKey_Hold() && mMinimumJumpDurationTimer <= 0.0f)
	{
		mIsJumping = false;
		mTotalJumpDurationTimer = 0.0f;
	}
	if (GetJumpKey_OnPress() && (mIsOnGround || mCoyoteTimer < mCoyoteTimeMax - (float)WindowsManager::GetInstance()->mDeltaTime))
	{
		//CreateJumpParticle_String();
		//CreateJumpParticle_Jump();
		mIsOnGround = false;
		mIsJumping = true;
		mWasJumping = true;

		mMinimumJumpDurationTimer = mMinimumJumpDurationTime;
		mTotalJumpDurationTimer = mTotalJumpDurationTime;
		SoundManager::GetInstance()->PlaySound("SFX_Jump", SoundManager::GetInstance()->GetSFXVolume(), SoundManager::GetInstance()->GetChannelPitch());

		mRisingGravity = 0;
		mInitiatedJumpForce = mZoomScale;
		mCoyoteTimer = mCoyoteTimeMax;

		float jumpOverall = (float)(mTotalJumpDurationTimer / mTotalJumpDurationTime) * mJumpStrength * mInitiatedJumpForce * (float)WindowsManager::GetInstance()->mDeltaTime;

		std::pair<int, int> YourCurrentTile = TileComponent::GetCellValue_YX((int)xform->GetPosition().mX, (int)xform->GetPosition().mY).second;

		float FakeRaycastedTile = YourCurrentTile.second + 1.f;
		if (FakeRaycastedTile < TileComponent::mMapHeight
			&& TileComponent::mBinaryMap[(int)FakeRaycastedTile][YourCurrentTile.first] == 1)
		{
			float theNewYWithRadius = jumpOverall + xform->GetPosition().mY
				+ mZoomScale * mPhy->mObjBigScale.mY / 2.f;
			float theYBoundary = FakeRaycastedTile * TileComponent::mTileSize + TileComponent::mMinBound.mY
				- TileComponent::mTileSize / 2.f + TileComponent::mTileSize / 2.f;

			if (theNewYWithRadius > theYBoundary)
			{
				jumpOverall -= theNewYWithRadius - theYBoundary;
			}
		}


		xform->SetPosition(Vector3(xform->GetPosition().mX,
			jumpOverall + xform->GetPosition().mY,
			xform->GetPosition().mZ));
	}
	else if ((GetJumpKey_Hold() || mMinimumJumpDurationTimer > 0.0f) && mIsJumping)
	{
		if (mTotalJumpDurationTimer > 0.0f)
		{
			float testval = mZoomScaleTarget;
			if (mZoomScaleTarget < 1.0f)
			{
				testval *= 0.3f;
			}
			else
			{
				testval *= 1.5f;
			}

			mTotalJumpDurationTimer -= (float)WindowsManager::GetInstance()->mDeltaTime * testval;

			float jumpOverall = (float)(mTotalJumpDurationTimer / mTotalJumpDurationTime) * mJumpStrength * mInitiatedJumpForce * (float)WindowsManager::GetInstance()->mDeltaTime;

			std::pair<int, int> YourCurrentTile = TileComponent::GetCellValue_YX((int)xform->GetPosition().mX, (int)xform->GetPosition().mY).second;

			float FakeRaycastedTile = YourCurrentTile.second + 1.f;
			if (FakeRaycastedTile < TileComponent::mMapHeight
				&& TileComponent::mBinaryMap[(int)FakeRaycastedTile][YourCurrentTile.first] == 1)
			{
				float theNewYWithRadius = jumpOverall + xform->GetPosition().mY
					+ mZoomScale * mPhy->mObjBigScale.mY / 2.f;
				float theYBoundary = FakeRaycastedTile * TileComponent::mTileSize + TileComponent::mMinBound.mY
					- TileComponent::mTileSize / 2.f + TileComponent::mTileSize / 2.f;

				if (theNewYWithRadius > theYBoundary)
				{
					jumpOverall -= theNewYWithRadius - theYBoundary;
				}
			}

			xform->SetPosition(Vector3(xform->GetPosition().mX,
				jumpOverall + xform->GetPosition().mY,
				xform->GetPosition().mZ));
		}
		if (mTotalJumpDurationTimer <= 0.0f)
		{
			mIsJumping = false;
		}
	}

	//if (mIsOnGround && mPreviousYVelocity <= -FLT_EPSILON && !(mMap->mGridCollisionFlag & TileComponent::COLLISION_BOTTOM))
	//{
	//	mIsOnGround = false;
	//}
	mIsOnGround = GroundLineRaycast();

	if (mPreviousYVelocity <= -15.0f && (mMap->mGridCollisionFlag & TileComponent::COLLISION_BOTTOM))
	{
		//mIsOnGround = true;
		SoundManager::GetInstance()->PlaySound("SFX_Landing", SoundManager::GetInstance()->GetSFXVolume(), SoundManager::GetInstance()->GetChannelPitch());
	}
	//else mIsOnGround = false;

	if (mIsOnGround || (!IsWindAffected() && !mIsJumping && mDirectionalVector.mY < -FLT_EPSILON))
	{
		mExtendBottomTimer = mExtendBottomTime;
	}
	else
	{
		mExtendBottomTimer = std::clamp(mExtendBottomTimer - (1.0f / mExtendBottomTime) * (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, mExtendBottomTime);
		//mExtendBottomTimer = 0;
	}

	mPreviousYVelocity = mPhy->mCurrentVelocity.mY;

	float totalvelocity = mMoveSpeed * (float)WindowsManager::GetInstance()->mDeltaTime;
	if (mCutsceneTimer <= 0)
	{
		if (InputManager::GetInstance()->GetAction("PlayerLeft"))
		{
			MoveLeft();
			//Shader* currShader{ nullptr };
			//AssetManager::GetInstance()->GetAsset("Animation", currShader);
			//currShader->Use();
			//currShader->SetUniform("FlipX", 0.f);
			//currShader->UnUse();
		}
		else if (InputManager::GetInstance()->GetAction("PlayerRight"))
		{
			MoveRight();
			//Shader* currShader{ nullptr };
			//AssetManager::GetInstance()->GetAsset("Animation", currShader);
			//currShader->Use();
			//currShader->SetUniform("FlipX", 1.f);
			//currShader->UnUse();
		}
		else
			ResetHorizontalVel();
	}

	float multsign = 0;
	if (fabsf(mInternalLeftRightVelocity) > FLT_EPSILON)
	{
		multsign = (mInternalLeftRightVelocity / fabsf(mInternalLeftRightVelocity));
	}
	totalvelocity *= multsign * (0.2f + 0.8f * powf(fabsf(mInternalLeftRightVelocity), mInternalLeftRightPower));

	std::pair<int, int> YourCurrentTile = TileComponent::GetCellValue_YX((int)xform->GetPosition().mX, (int)xform->GetPosition().mY).second;

	totalvelocity = (0.2f + 0.8f * mZoomScale) * totalvelocity;

	if (totalvelocity < -FLT_EPSILON) //Move Left
	{
		SetSpriteFlip(false);

		float FakeRaycastedTile = YourCurrentTile.first - 1.f;
		if (FakeRaycastedTile >= 0
			&& TileComponent::mBinaryMap[YourCurrentTile.second][(int)FakeRaycastedTile] == 1)
		{
			float theNewXWithRadius = totalvelocity + xform->GetPosition().mX
				- mZoomScale * mPhy->mObjBigScale.mX / 2.f;
			float theXBoundary = FakeRaycastedTile * TileComponent::mTileSize + TileComponent::mMinBound.mX
				+ TileComponent::mTileSize / 2.f + TileComponent::mTileSize / 2.f;

			if (theNewXWithRadius < theXBoundary)
			{
				totalvelocity -= theNewXWithRadius - theXBoundary;
			}
		}
	}
	else if (totalvelocity > FLT_EPSILON) //Move Right
	{
		SetSpriteFlip(true);

		float FakeRaycastedTile = YourCurrentTile.first + 1.f;
		if (FakeRaycastedTile < TileComponent::mMapWidth
			&& TileComponent::mBinaryMap[YourCurrentTile.second][(int)FakeRaycastedTile] == 1)
		{
			float theNewXWithRadius = totalvelocity + xform->GetPosition().mX
				+ mZoomScale * mPhy->mObjBigScale.mX / 2.f;
			float theXBoundary = FakeRaycastedTile * TileComponent::mTileSize + TileComponent::mMinBound.mX
				+ TileComponent::mTileSize / 2.f - TileComponent::mTileSize / 2.f;

			if (theNewXWithRadius > theXBoundary)
			{
				totalvelocity -= theNewXWithRadius - theXBoundary;
			}
		}
	}
	else totalvelocity = 0;

	xform->SetPosition(Vector3(totalvelocity * mMoveSpeedMultiplier + xform->GetPosition().mX,
		xform->GetPosition().mY,
		xform->GetPosition().mZ));


	// dynamic scaling of player
	if (mCutsceneTimer <= 0)
	{
		//if (InputManager::GetInstance()->GetKey(KeyCode::M))
		//{
		//	// scale x value up
		//	xform->mScale.mX += 1;
		//}
		//if (InputManager::GetInstance()->GetKey(KeyCode::N))
		//{
		//	// scale x value down
		//	xform->mScale.mX -= 1;
		//}
		//if (InputManager::GetInstance()->GetKey(KeyCode::L))
		//{
		//	// scale y value up
		//	xform->mScale.mY += 1;
		//}
		//if (InputManager::GetInstance()->GetKey(KeyCode::K))
		//{
		//	// scale y value down
		//	xform->mScale.mY -= 1;
		//}
		//if (InputManager::GetInstance()->GetKey(KeyCode::COMMA))
		//{
		//	// scale y value up
		//	xform->mRotation += (10.f * (float)WindowsManager::GetInstance()->mDeltaTime);
		//}
		//if (InputManager::GetInstance()->GetKey(KeyCode::PERIOD))
		//{
		//	// scale y value down
		//	xform->mRotation -= (10.f * (float)WindowsManager::GetInstance()->mDeltaTime);
		//}

		if (InputManager::GetInstance()->GetActionTriggered("CheatReset"))
		{
			Kill();
		}
	}

	if (abs(mPhy->mCurrentVelocity.mX) > DBL_EPSILON)
	{
		mMorphPlanXProgressInternal = 1;
	}
	else
	{
		mMorphPlanXProgressInternal = 0;
	}

	if (fabsf(mMorphPlanXProgressCurrent - mMorphPlanXProgressInternal) > (1.0f / mMorphPlanXTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
		mMorphPlanXProgressCurrent += ((mMorphPlanXProgressInternal - mMorphPlanXProgressCurrent) /fabsf((mMorphPlanXProgressInternal - mMorphPlanXProgressCurrent))) * (1.0f / mMorphPlanXTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
	else
		mMorphPlanXProgressCurrent = mMorphPlanXProgressInternal;

	if (mMorphPlanXProgressCurrent > FLT_EPSILON)
	{
		mMorphPlanXRunningTimer += (float)WindowsManager::GetInstance()->mDeltaTime;
	}
	else
	{
		mMorphPlanXRunningTimer = 0;
	}

	//NOTE: Implement Wind Variant AFFECTOR LATER
	if (!mIsOnGround && (mIsJumping /*|| (windY > Mathf.Epsilon && windY + mLastFallingValue > Mathf.Epsilon)*/))
	{
		mEyeHeightInternal = mEyeHeightTop;
		mMorphBodyProgressInternal = 1;
		mMorphBodyVerticalSInternal = 1;
	}
	else if (!mIsOnGround)
	{
		mEyeHeightInternal = mEyeHeightBottom;
		mMorphBodyProgressInternal = 1;
		mMorphBodyVerticalSInternal = 0;
	}
	else
	{
		mEyeHeightInternal = mEyeHeightDefault;
		mMorphBodyProgressInternal = 0;
		mMorphBodyVerticalSInternal = 0;
	}

	if (fabsf(mEyeHeightCurrent - mEyeHeightInternal) > (1.0f / mTurnEyeHeightTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
		mEyeHeightCurrent += ((mEyeHeightInternal - mEyeHeightCurrent) / fabsf((mEyeHeightInternal - mEyeHeightCurrent))) * (1.0f / mTurnEyeHeightTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
	else
		mEyeHeightCurrent = mEyeHeightInternal;

	if (mMorphBodyProgressInternal < 0.5f)
	{
		if (fabsf(mMorphBodyProgressCurrent - mMorphBodyProgressInternal) > (2.0f / mMorphBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
			mMorphBodyProgressCurrent += ((mMorphBodyProgressInternal - mMorphBodyProgressCurrent) / fabsf((mMorphBodyProgressInternal - mMorphBodyProgressCurrent))) * (2.0f / mMorphBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
		else
			mMorphBodyProgressCurrent = mMorphBodyProgressInternal;
	}
	else
	{
		if (fabsf(mMorphBodyProgressCurrent - mMorphBodyProgressInternal) > (1.0f / mMorphBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
			mMorphBodyProgressCurrent += ((mMorphBodyProgressInternal - mMorphBodyProgressCurrent) / fabsf((mMorphBodyProgressInternal - mMorphBodyProgressCurrent))) * (1.0f / mMorphBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
		else
			mMorphBodyProgressCurrent = mMorphBodyProgressInternal;
	}

	if (fabsf(mMorphBodyVerticalSCurrent - mMorphBodyVerticalSInternal) > (1.0f / mMorphBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
		mMorphBodyVerticalSCurrent += ((mMorphBodyVerticalSInternal - mMorphBodyVerticalSCurrent) / fabsf((mMorphBodyVerticalSInternal - mMorphBodyVerticalSCurrent))) * (1.0f / mMorphBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
	else
		mMorphBodyVerticalSCurrent = mMorphBodyVerticalSInternal;

	if (fabsf(mEyeLCurrent - mEyeLInternal) > (1.0f / mTurnBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
		mEyeLCurrent += ((mEyeLInternal - mEyeLCurrent) / fabsf((mEyeLInternal - mEyeLCurrent))) * (1.0f / mTurnBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
	else
		mEyeLCurrent = mEyeLInternal;
	if (fabsf(mEyeRCurrent - mEyeRInternal) > (1.0f / mTurnBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
		mEyeRCurrent += ((mEyeRInternal - mEyeRCurrent) / fabsf((mEyeRInternal - mEyeRCurrent))) * (1.0f / mTurnBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
	else
		mEyeRCurrent = mEyeRInternal;
	if (fabsf(mBodyFacingCurrent - mBodyFacingInternal) > (1.0f / mTurnBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
		mBodyFacingCurrent += ((mBodyFacingInternal - mBodyFacingCurrent) / fabsf((mBodyFacingInternal - mBodyFacingCurrent))) * (1.0f / mTurnBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
	else
		mBodyFacingCurrent = mBodyFacingInternal;

	if (abs(mPhy->mCurrentVelocity.mX) <= DBL_EPSILON && mIsOnGround)
	{
		mIdleBodyProgressInternal = 1;
		mIdleBodyRunningTimer += (float)WindowsManager::GetInstance()->mDeltaTime;
	}
	else
	{
		mIdleBodyProgressInternal = 0;
		mIdleBodyRunningTimer = 0;
	}

	if (fabsf(mIdleBodyProgressCurrent - mIdleBodyProgressInternal) > (1.0f / mIdleBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
		mIdleBodyProgressCurrent += ((mIdleBodyProgressInternal - mIdleBodyProgressCurrent) / fabsf((mIdleBodyProgressInternal - mIdleBodyProgressCurrent))) * (1.0f / mIdleBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
	else
		mIdleBodyProgressCurrent = mIdleBodyProgressInternal;

	float addEyeIdle = 0.08f * mIdleBodyProgressCurrent * (cosf(mIdleBodyRunningTimer * 0.65f * PI * 2 + PI) * 0.5f + 0.5f);

	mEntBodySprite->GetComponent<Transform>()->SetPosition(Vector3(0, 0, -0.4f) + xform->GetPosition());
	mEntEyeHolder->GetComponent<Transform>()->SetPosition(Vector3( 0, -0.14f * mZoomScale, -0.8f ) + xform->GetPosition());
	mEntEyeLeft->GetComponent<Transform>()->SetPosition(Vector3(mRatioAnimMultiplier * mEyeLCurrent * mZoomScale, addEyeIdle + mRatioAnimMultiplier * mEyeHeightCurrent * mZoomScale, 0) + mEntEyeHolder->GetComponent<Transform>()->GetPosition());
	mEntEyeRight->GetComponent<Transform>()->SetPosition(Vector3(mRatioAnimMultiplier * mEyeRCurrent * mZoomScale, addEyeIdle + mRatioAnimMultiplier * mEyeHeightCurrent * mZoomScale, 0) + mEntEyeHolder->GetComponent<Transform>()->GetPosition());

	mEntBodySprite->GetComponent<Transform>()->mScale = mZoomScale * mInitialBodyScale;
	mEntEyeLeft->GetComponent<Transform>()->mScale = mZoomScale * mInitialEyeLeftScale;
	mEntEyeRight->GetComponent<Transform>()->mScale = mZoomScale * mInitialEyeRightScale;

	Shader* currBodyShader{ nullptr };
	AssetManager::GetInstance()->GetAsset("PlayerBodyShader", currBodyShader);
	currBodyShader->Use();

	AssetManager::ImageAsset* texture{ nullptr };
	AssetManager::GetInstance()->GetAsset("Character_PixelSlimeBody", texture);
	currBodyShader->SetUniform("uTex2d", (GLint)texture->mTexID);
	glBindTextureUnit(texture->mTexID, texture->mTexID);


	currBodyShader->SetUniform("_FlipRight", mBodyFacingCurrent);
	currBodyShader->SetUniform("_Progress", mMorphBodyProgressCurrent);
	currBodyShader->SetUniform("_ProgressX", mMorphPlanXProgressCurrent * (0.5f + 0.25f *
		cosf(powf(fmodf(mMorphPlanXRunningTimer * mMorphPlanXRate, 1.0f), 2.f) * PI * 2.f)
		));
	currBodyShader->SetUniform("_VerticalStretch", mMorphBodyVerticalSCurrent);
	currBodyShader->SetUniform("_IdleProgress", mIdleBodyProgressCurrent);
	currBodyShader->SetUniform("_RunningTimer", mIdleBodyRunningTimer);
	currBodyShader->SetUniform("_RespawnProgress", 1.0f);
	currBodyShader->UnUse();

	mDirectionalVector = xform->GetPosition() - prevpos;

	//Clamp player movement so they cant move out of the camera's clamp zone
	{
		Vec3 playerPos = xform->GetPosition();
		Vec3 playerScaleValue = { mZoomScale * mPhy->mObjBigScale.mX, mZoomScale * mPhy->mObjBigScale.mY, 1 };

		//Keeping player in level
		Vec3 horiClamp = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mClampHoriRange;
		Vec3 vertClamp = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mClampVertRange;
		Vec3 camZoomOffset = GraphicsSystem::GetInstance()->mWorldSize - GraphicsSystem::GetInstance()->mWorldSize * (1 / GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom);
		camZoomOffset /= 2;

		//Keeping player within camera
		Vec3 cameraPos = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition;
		Vec3 worldSize = GraphicsSystem::GetInstance()->mWorldSize * (1 / GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom);

		//Player must be within camera view or in stage

		//Left
		if (playerPos.mX - playerScaleValue.mX / 2 < horiClamp.mX + camZoomOffset.mX
			&& playerPos.mX - playerScaleValue.mX / 2 < cameraPos.mX - worldSize.mX / 2)
			playerPos.mX = horiClamp.mX + camZoomOffset.mX + playerScaleValue.mX / 2;
		//Right
		else if (playerPos.mX + playerScaleValue.mX / 2 > horiClamp.mY - camZoomOffset.mX
			&& playerPos.mX + playerScaleValue.mX / 2 > cameraPos.mX + worldSize.mX / 2)
			playerPos.mX = horiClamp.mY - camZoomOffset.mX - playerScaleValue.mX / 2;
		//Bot
		if (playerPos.mY - playerScaleValue.mY / 2 < vertClamp.mX + camZoomOffset.mY
			&& playerPos.mY - playerScaleValue.mY / 2 < cameraPos.mY - worldSize.mY / 2)
			playerPos.mY = vertClamp.mX + camZoomOffset.mY + playerScaleValue.mY / 2;
		//Top
		else if (playerPos.mY + playerScaleValue.mY / 2 > vertClamp.mY - camZoomOffset.mY
			&& playerPos.mY + playerScaleValue.mY / 2 > cameraPos.mY + worldSize.mY / 2)
			playerPos.mY = vertClamp.mY - camZoomOffset.mY - playerScaleValue.mY / 2;

		xform->SetPosition(playerPos);
	}
}

void Player::Exit()
{
}

void Player::MoveLeft()
{
	//mPhy->mCurrentVelocity.mX = -mMoveSpeed;
	if (mInternalLeftRightVelocity > FLT_EPSILON) mInternalLeftRightVelocity = 0;

	mInternalLeftRightVelocity = std::clamp(mInternalLeftRightVelocity - mInternalLeftRightBuildupRate * (float)WindowsManager::GetInstance()->mDeltaTime, -1.f, 1.f);

	if (mCutsceneTimer <= 0.f && mMoveSpeedMultiplier < 1.f)
		mMoveSpeedMultiplier = 1.f;

	float xScale = mZoomScale * mPhy->mObjBigScale.mX;
	float yScale = mZoomScale * mPhy->mObjBigScale.mY;

	Transform* xform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();
	ParticleSystem::GetInstance()->SpawnParticles(xform->GetPosition(), Vec3(xScale / 2 - xScale * 0.1f, -yScale / 2 + yScale * 0.15f, 0));

}

void Player::MoveRight()
{
	//mPhy->mCurrentVelocity.mX = mMoveSpeed;
	if (mInternalLeftRightVelocity < -FLT_EPSILON) mInternalLeftRightVelocity = 0;

	mInternalLeftRightVelocity = std::clamp(mInternalLeftRightVelocity + mInternalLeftRightBuildupRate * (float)WindowsManager::GetInstance()->mDeltaTime, -1.f, 1.f);

	if(mCutsceneTimer <= 0.f && mMoveSpeedMultiplier < 1.f)
		mMoveSpeedMultiplier = 1.f;

	float xScale = mZoomScale * mPhy->mObjBigScale.mX;
	float yScale = mZoomScale * mPhy->mObjBigScale.mY;

	Transform* xform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();
	ParticleSystem::GetInstance()->SpawnParticles(xform->GetPosition(), Vec3(-xScale / 2 + xScale * 0.1f, -yScale / 2 + yScale * 0.15f, 0));

}

bool Player::ShouldExtendBottom()
{
	return mExtendBottomTimer > FLT_EPSILON;
}

void Player::Jump()
{
	if (mIsOnGround)
	{
		mPhy->mJumpForce.mY = mJumpStrength * mZoomScale;

		mIsOnGround = false;
		mIsJumping = true;
		mWasJumping = true;
		mMinimumJumpDurationTimer = mMinimumJumpDurationTime;
		mTotalJumpDurationTimer = mTotalJumpDurationTime;
		SoundManager::GetInstance()->PlaySound("SFX_Jump", SoundManager::GetInstance()->GetSFXVolume(), SoundManager::GetInstance()->GetChannelPitch());
	}
	else
	{
		mPhy->mJumpForce.mY = 0;
	}
	
}

void Player::JumpBig() //Jovan: Doesn't work like this, probably just leave this obsolete
{
	if (mIsOnGround) {
		mPhy->mJumpForce.mY = mJumpStrength * mPhy->mJumpMultiplier;
		mIsOnGround = false;
		mIsJumping = true;
		mMinimumJumpDurationTimer = mMinimumJumpDurationTime;
		SoundManager::GetInstance()->PlaySound("SFX_Jump", SoundManager::GetInstance()->GetSFXVolume(), SoundManager::GetInstance()->GetChannelPitch());
	}
	else
	{
		mPhy->mJumpForce.mY = 0;
	}
}

void Player::ResetHorizontalVel()
{
	//mPhy->mCurrentVelocity.mX = 0;

	mInternalLeftRightVelocity = 0;
}

void Player::SpawnDeathVFX_Normal()
{
	Entity* newEnt = EntityManager::GetInstance()->CreateEntity();
	newEnt->SetName("DeathVFX_Normal");

	if (newEnt == nullptr)
		return;

	if (EntityManager::GetInstance()->GetEntity(mEntID) == nullptr || EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>() == nullptr)
		return;

	Transform newXform{};
	newXform.SetPosition(EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() + Vector3(0, 0, -0.1f));
	newXform.mScale = Vector3{ 40, 40, 1 };
	newEnt->AddComponent(newXform);

	Renderer* _renderer = newEnt->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE_ANIMATION;
	_renderer->mShaderName = "Animation";
	_renderer->mTextureName = "SpriteSheet_DeathAnimation";
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;

	AnimationComponent* anim = newEnt->AddComponent<AnimationComponent>();
	anim->mAnimation.mTotalFrames = 16;
	anim->mAnimation.mTimeDelay = 0.0125f;
	anim->mAnimation.mIsLooping = false;

	newEnt->AddComponent<VFXEntity>();

}

void Player::Kill()
{
	if (mIsDead) return;

	SceneManager::GetInstance()->totalDeathCount++;
	LevelManager::GetInstance()->DoScreenshake();
	SoundManager::GetInstance()->PlaySound("SFX_DeathNormal_Raw", SoundManager::GetInstance()->GetSFXVolume(), SoundManager::GetInstance()->GetChannelPitch());
	ResetHorizontalVel();
	mIsOnGround = true;
	mIsJumping = false;
	//mPhy->mCurrentVelocity.mY = 0;
	mWindAttachedProgress = 0;
	mRisingGravity = 0;
	mMinimumJumpDurationTimer = 0.f;
	mTotalJumpDurationTimer = 0.f;

	SpawnDeathVFX_Normal();

	WindowsManager::GetInstance()->mDeathVignette = 1.0f;

	mIsDead = true;
	mDeathPlayed = false;

	mDeathDurationTimer = mDeathDurationTime;
	mReviveDurationTimer = mReviveDurationTime;

	//Reset keys
	for (size_t i = 0; i < Key::mKeyIDs.size(); ++i)
	{
		if (EntityManager::GetInstance()->GetEntity(Key::mKeyIDs[i]) == nullptr ||
			EntityManager::GetInstance()->GetEntity(Key::mKeyIDs[i])->GetComponent<Key>() == nullptr ||
			EntityManager::GetInstance()->GetEntity(Key::mKeyIDs[i])->GetActive() == false)
			continue;

		EntityManager::GetInstance()->GetEntity(mEntID)->RemoveChild(Key::mKeyIDs[i]);
		EntityManager::GetInstance()->GetEntity(Key::mKeyIDs[i])->GetComponent<Key>()->Reset();
	}
	if (mHasKey)
	{
		mHasKey = false;
		mAttachedKeyID = 0;
	}
	
	//Reset walls
	for (size_t i = 0; i < KeyWall::mKeyWallIDs.size(); ++i)
	{
		if (EntityManager::GetInstance()->GetEntity(KeyWall::mKeyWallIDs[i]) == nullptr || 
			EntityManager::GetInstance()->GetEntity(KeyWall::mKeyWallIDs[i])->GetComponent<KeyWall>() == nullptr ||
			EntityManager::GetInstance()->GetEntity(KeyWall::mKeyWallIDs[i])->GetActive() == false)
			continue;

		EntityManager::GetInstance()->GetEntity(KeyWall::mKeyWallIDs[i])->GetComponent<KeyWall>()->Reset();
	}

	Shader* currBodyShader{ nullptr };
	AssetManager::GetInstance()->GetAsset("PlayerBodyShader", currBodyShader);
	currBodyShader->Use();
	currBodyShader->SetUniform("_RespawnProgress", 0.0f);
	currBodyShader->UnUse();

	mDirectionalVector = Vector3{ 0.f, 0.f, 0.f };
}

void Player::Respawn()
{
	SoundManager::GetInstance()->PlaySound("SFX_ReviveNormal", SoundManager::GetInstance()->GetSFXVolume(), SoundManager::GetInstance()->GetChannelPitch());
	//EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->GetPosition() = mInitialPosition;
	CamWaypoint playerCurrWaypoint = SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints[SceneManager::GetInstance()->GetActiveScene()->mCamWaypointIndex];
	EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->SetPosition(playerCurrWaypoint.mPlayerSpawnPos);
	ResetHorizontalVel();
	mIsOnGround = true;
	mIsJumping = false;
	//mPhy->mCurrentVelocity.mY = 0;
	mRisingGravity = 0;
	mMinimumJumpDurationTimer = 0.f;
	mTotalJumpDurationTimer = 0.f;
	mDeathDurationTimer = 0.f;
	mZoomScaleTarget = 1.0f;
	mZoomScale = 1.0f;
	mCoyoteTimer = 0.0f;
}

void Player::Load()
{
	mJumpStrength = JSONManager::GetInstance()->GetFloat("JumpStrength");
	mMoveSpeed = JSONManager::GetInstance()->GetFloat("MoveSpeed");
	mIsOnGround = JSONManager::GetInstance()->GetBool("IsOnGround");
	mPlayerDeathText = JSONManager::GetInstance()->GetString("DeathText");
	mPlayerDeathFont = JSONManager::GetInstance()->GetString("DeathFont");
	mPlayerDeathTextTime = JSONManager::GetInstance()->GetFloat("DeathTextTime");
	mLoaded = true;
}

void Player::Save()
{
	JSONManager::GetInstance()->Save("Component", "Player");
	JSONManager::GetInstance()->Save("JumpStrength", mJumpStrength);
	JSONManager::GetInstance()->Save("MoveSpeed", mMoveSpeed);
	JSONManager::GetInstance()->Save("IsOnGround", mIsOnGround);
	JSONManager::GetInstance()->Save("DeathText", mPlayerDeathText);
	JSONManager::GetInstance()->Save("DeathFont", mPlayerDeathFont);
	JSONManager::GetInstance()->Save("DeathTextTime", mPlayerDeathTextTime);
}

void Player::DisplayProperties()
{
#if _ENGINE
	int entityID = static_cast<int>(mPlayerID);
	MyImGui::MyImGuiFloatInput(mDebugDisplayValue, "DebugTest");
	MyImGui::MyImGuiIntInput(entityID, "PlayerID");
	MyImGui::MyImGuiStringInput(mPlayerDeathText, "Player Death Text");
	ImGui::Text("Player Death Font");
	ImGui::SameLine();
	{//Box to be able to drag and drop
		char buff[1024];
		strcpy(buff, mPlayerDeathFont.c_str());
		ImDrawList* draw_list = ImGui::GetForegroundDrawList();
		//Draw uneditable input box
		ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 1);
		ImGui::BeginDisabled();
		ImGui::SameLine(ImGui::GetWindowWidth() - 300); ImGui::SetNextItemWidth(250);
		ImGui::InputText("##fontName", buff, 1024, ImGuiInputTextFlags_ReadOnly);
		ImGui::EndDisabled();
		ImGui::PopStyleVar();
		ImRect inner_rect = ImGui::GetCurrentContext()->LastItemData.Rect;
		if (ImGui::BeginDragDropTargetCustom(inner_rect, ImGui::GetID("##fontName")))//drag drop for sprites
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FontDrag", ImGuiDragDropFlags_AcceptBeforeDelivery);//need to check before delivery so we can draw the yellow box
			if (payload)
			{
				if (payload->IsPreview())
				{
					draw_list->AddRectFilled(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget, 0.05f));//draw inner darker overlay
					draw_list->AddRect(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget), 0.0f, 0, 2.0f);//draw the yellow outer box
				}
				if (payload->IsDelivery())
				{
					mPlayerDeathFont = std::string(static_cast<const char*>(payload->Data), payload->DataSize);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	MyImGui::MyImGuiFloatInput(mPlayerDeathTextTime, "Death Text Time");
#endif // _ENGINE
}

std::ostream& operator<<(std::ostream& os, const Player& _player)
{
	os << "Player MoveSpeed: " << _player.mMoveSpeed << " Player JumpStrength: " << _player.mJumpStrength;
	return os;
}

void Player::SetChildObjects()
{
	std::vector<EntityID> childVecList = EntityManager::GetInstance()->GetEntity(mEntID)->GetChildEntList();
	for (size_t i = 0; i < childVecList.size(); ++i)
	{
		Entity* currEnt = EntityManager::GetInstance()->GetEntity(childVecList[i]);
		if (currEnt->GetName() == "PlayerBodySprite")
			mEntBodySprite = currEnt;
		else if (currEnt->GetName() == "PlayerEyeHolder")
			mEntEyeHolder = currEnt;
		else if (currEnt->GetName() == "PlayerLeftEyeSprite")
			mEntEyeLeft = currEnt;
		else if (currEnt->GetName() == "PlayerRightEyeSprite")
			mEntEyeRight = currEnt;
	}
}

void Player::ResetCurrentLevelDeaths()
{
	mCurrentLevelDeaths = 0;
}

unsigned int Player::GetCurrentLevelDeaths()
{
	return mCurrentLevelDeaths;
}

void Player::ResetCutsceneTimers()
{
	mCutsceneTimer = 0;
	mDisableGravityTimer = 0;
	mDelayRespawnTimer = 0;
}

void Player::UpdatePlayerShaders()
{
	if (abs(mPhy->mCurrentVelocity.mX) > DBL_EPSILON)
	{
		mMorphPlanXProgressInternal = 1;
	}
	else
	{
		mMorphPlanXProgressInternal = 0;
	}

	if (fabsf(mMorphPlanXProgressCurrent - mMorphPlanXProgressInternal) > (1.0f / mMorphPlanXTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
		mMorphPlanXProgressCurrent += ((mMorphPlanXProgressInternal - mMorphPlanXProgressCurrent) / fabsf((mMorphPlanXProgressInternal - mMorphPlanXProgressCurrent))) * (1.0f / mMorphPlanXTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
	else
		mMorphPlanXProgressCurrent = mMorphPlanXProgressInternal;

	if (mMorphPlanXProgressCurrent > FLT_EPSILON)
	{
		mMorphPlanXRunningTimer += (float)WindowsManager::GetInstance()->mDeltaTime;
	}
	else
	{
		mMorphPlanXRunningTimer = 0;
	}

	//NOTE: Implement Wind Variant AFFECTOR LATER
	if (!mIsOnGround && (mIsJumping /*|| (windY > Mathf.Epsilon && windY + mLastFallingValue > Mathf.Epsilon)*/))
	{
		mEyeHeightInternal = mEyeHeightTop;
		mMorphBodyProgressInternal = 1;
		mMorphBodyVerticalSInternal = 1;
	}
	else if (!mIsOnGround)
	{
		mEyeHeightInternal = mEyeHeightBottom;
		mMorphBodyProgressInternal = 1;
		mMorphBodyVerticalSInternal = 0;
	}
	else
	{
		mEyeHeightInternal = mEyeHeightDefault;
		mMorphBodyProgressInternal = 0;
		mMorphBodyVerticalSInternal = 0;
	}

	if (fabsf(mEyeHeightCurrent - mEyeHeightInternal) > (1.0f / mTurnEyeHeightTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
		mEyeHeightCurrent += ((mEyeHeightInternal - mEyeHeightCurrent) / fabsf((mEyeHeightInternal - mEyeHeightCurrent))) * (1.0f / mTurnEyeHeightTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
	else
		mEyeHeightCurrent = mEyeHeightInternal;

	if (mMorphBodyProgressInternal < 0.5f)
	{
		if (fabsf(mMorphBodyProgressCurrent - mMorphBodyProgressInternal) > (2.0f / mMorphBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
			mMorphBodyProgressCurrent += ((mMorphBodyProgressInternal - mMorphBodyProgressCurrent) / fabsf((mMorphBodyProgressInternal - mMorphBodyProgressCurrent))) * (2.0f / mMorphBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
		else
			mMorphBodyProgressCurrent = mMorphBodyProgressInternal;
	}
	else
	{
		if (fabsf(mMorphBodyProgressCurrent - mMorphBodyProgressInternal) > (1.0f / mMorphBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
			mMorphBodyProgressCurrent += ((mMorphBodyProgressInternal - mMorphBodyProgressCurrent) / fabsf((mMorphBodyProgressInternal - mMorphBodyProgressCurrent))) * (1.0f / mMorphBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
		else
			mMorphBodyProgressCurrent = mMorphBodyProgressInternal;
	}

	if (fabsf(mMorphBodyVerticalSCurrent - mMorphBodyVerticalSInternal) > (1.0f / mMorphBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
		mMorphBodyVerticalSCurrent += ((mMorphBodyVerticalSInternal - mMorphBodyVerticalSCurrent) / fabsf((mMorphBodyVerticalSInternal - mMorphBodyVerticalSCurrent))) * (1.0f / mMorphBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
	else
		mMorphBodyVerticalSCurrent = mMorphBodyVerticalSInternal;

	if (fabsf(mEyeLCurrent - mEyeLInternal) > (1.0f / mTurnBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
		mEyeLCurrent += ((mEyeLInternal - mEyeLCurrent) / fabsf((mEyeLInternal - mEyeLCurrent))) * (1.0f / mTurnBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
	else
		mEyeLCurrent = mEyeLInternal;
	if (fabsf(mEyeRCurrent - mEyeRInternal) > (1.0f / mTurnBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
		mEyeRCurrent += ((mEyeRInternal - mEyeRCurrent) / fabsf((mEyeRInternal - mEyeRCurrent))) * (1.0f / mTurnBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
	else
		mEyeRCurrent = mEyeRInternal;
	if (fabsf(mBodyFacingCurrent - mBodyFacingInternal) > (1.0f / mTurnBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
		mBodyFacingCurrent += ((mBodyFacingInternal - mBodyFacingCurrent) / fabsf((mBodyFacingInternal - mBodyFacingCurrent))) * (1.0f / mTurnBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
	else
		mBodyFacingCurrent = mBodyFacingInternal;

	if (abs(mPhy->mCurrentVelocity.mX) <= DBL_EPSILON && mIsOnGround)
	{
		mIdleBodyProgressInternal = 1;
		mIdleBodyRunningTimer += (float)WindowsManager::GetInstance()->mDeltaTime;
	}
	else
	{
		mIdleBodyProgressInternal = 0;
		mIdleBodyRunningTimer = 0;
	}

	if (fabsf(mIdleBodyProgressCurrent - mIdleBodyProgressInternal) > (1.0f / mIdleBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime)
		mIdleBodyProgressCurrent += ((mIdleBodyProgressInternal - mIdleBodyProgressCurrent) / fabsf((mIdleBodyProgressInternal - mIdleBodyProgressCurrent))) * (1.0f / mIdleBodyTime) * (float)WindowsManager::GetInstance()->mDeltaTime;
	else
		mIdleBodyProgressCurrent = mIdleBodyProgressInternal;

	float addEyeIdle = 0.08f * mIdleBodyProgressCurrent * (cosf(mIdleBodyRunningTimer * 0.65f * PI * 2 + PI) * 0.5f + 0.5f);
	Transform* xform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();

	mEntBodySprite->GetComponent<Transform>()->SetPosition(Vector3(0, 0, -0.4f) + xform->GetPosition());
	mEntEyeHolder->GetComponent<Transform>()->SetPosition(Vector3(0, -0.14f * mZoomScale, -0.8f) + xform->GetPosition());
	mEntEyeLeft->GetComponent<Transform>()->SetPosition(Vector3(mRatioAnimMultiplier * mEyeLCurrent * mZoomScale, addEyeIdle + mRatioAnimMultiplier * mEyeHeightCurrent * mZoomScale, 0) + mEntEyeHolder->GetComponent<Transform>()->GetPosition());
	mEntEyeRight->GetComponent<Transform>()->SetPosition(Vector3(mRatioAnimMultiplier * mEyeRCurrent * mZoomScale, addEyeIdle + mRatioAnimMultiplier * mEyeHeightCurrent * mZoomScale, 0) + mEntEyeHolder->GetComponent<Transform>()->GetPosition());

	mEntBodySprite->GetComponent<Transform>()->mScale = mZoomScale * mInitialBodyScale;
	mEntEyeLeft->GetComponent<Transform>()->mScale = mZoomScale * mInitialEyeLeftScale;
	mEntEyeRight->GetComponent<Transform>()->mScale = mZoomScale * mInitialEyeRightScale;

	Shader* currBodyShader{ nullptr };
	AssetManager::GetInstance()->GetAsset("PlayerBodyShader", currBodyShader);
	currBodyShader->Use();

	AssetManager::ImageAsset* texture{ nullptr };
	AssetManager::GetInstance()->GetAsset("Character_PixelSlimeBody", texture);
	currBodyShader->SetUniform("uTex2d", (GLint)texture->mTexID);
	glBindTextureUnit(texture->mTexID, texture->mTexID);


	currBodyShader->SetUniform("_FlipRight", mBodyFacingCurrent);
	currBodyShader->SetUniform("_Progress", mMorphBodyProgressCurrent);
	currBodyShader->SetUniform("_ProgressX", mMorphPlanXProgressCurrent * (0.5f + 0.25f *
		cosf(powf(fmodf(mMorphPlanXRunningTimer * mMorphPlanXRate, 1.0f), 2.f) * PI * 2.f)
		));
	currBodyShader->SetUniform("_VerticalStretch", mMorphBodyVerticalSCurrent);
	currBodyShader->SetUniform("_IdleProgress", mIdleBodyProgressCurrent);
	currBodyShader->SetUniform("_RunningTimer", mIdleBodyRunningTimer);
	currBodyShader->SetUniform("_RespawnProgress", 1.0f);
	currBodyShader->UnUse();
}