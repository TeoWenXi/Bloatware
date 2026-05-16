/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       GraphicsSystem.cpp
@author     CAI Wenjin (95%)
@co-author	TEO Wen Xi (5%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      GraphicsSystem class definitions.
*/
/*______________________________________________________________________*/

#if _ENGINE
#include <imgui.h>
#endif // _ENGINE

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include "GraphicsSystem.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "WindowsManager.h"
#include "EventManager.h"

GLboolean g_colorToggled{GL_FALSE};
GLboolean g_wireframeMode{GL_FALSE};
bool g_postProcessingEnabled{true};

void GraphicsSystem::Init()
{
	//Setup world size
	mWorldSize.x = mWorldSize.y * ((GLfloat)WindowsManager::GetInstance()->mWidth / WindowsManager::GetInstance()->mHeight);

	// setup viewport
	mVp.mWidth = WindowsManager::GetInstance()->mWidth;
	mVp.mHeight = WindowsManager::GetInstance()->mHeight;

	// Initialise camera variables
	glm::vec3 camPos{0, 0, 50};
	glm::vec3 worldUp{0, 1, 0};
	glm::vec3 camTgt{0, 0, 0};
	mCamera.Init(camPos, worldUp, camTgt, (GLfloat)mVp.mHeight / 2, (GLfloat)mVp.mHeight / -2, camPos.z, -camPos.z);
	mCamera.UpdateXformMatrices();

	// Set stb_image to flip vertically
	stbi_set_flip_vertically_on_load(true);

	// set the initial color for the color buffer
	glClearColor(mBackgroundColor[0], mBackgroundColor[1], mBackgroundColor[2], mBackgroundColor[3]);

	// create the framebuffer object for rendering onto the ImGUI viewport
	CreateFrameBuffer();

	if (!GetFrameBufferStatus())
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Screen grid
	mScreenGrid.GenerateGridModel(false);
}

void GraphicsSystem::Load()
{
	// set initial shader
	mCurrentBoundShader = "BasicShader";

	// load the models
	mModel = new PointModel(5.f, glm::vec2{0.f, 0.f}, glm::vec4{1.f, 0.f, 0.f, 1.f});
	mModel->Init();
	mModelList.emplace("Point", mModel);

	mModel = new LineModel(10.f, glm::vec2{0.5f, 0.f}, glm::vec2{-0.5f, 0.5f}, glm::vec4{0.f, 1.f, 1.f, 1.f});
	mModel->Init();
	mModelList.emplace("Line", mModel);

	mModel = new CircleModel(360, glm::vec4{1.f, 0.f, 0.f, 1.f});
	mModel->Init();
	mModelList.emplace("Circle", mModel);

	mModel = new QuadModel();
	mModel->Init();
	mModelList.emplace("Quad", mModel);

	mModel = new QuadModel();
	mModel->Init();
	mModelList.emplace("Scene", mModel);

	mModel = new BoxModel();
	mModel->Init();
	mModelList.emplace("Box", mModel);

	mModelList["Line"]->mActive = GL_FALSE;
	// mModelList["Circle"]->mActive = GL_FALSE;
	mModelList["Quad"]->mActive = GL_FALSE;
	mModelList["Scene"]->mActive = GL_FALSE;
}

void GraphicsSystem::Update()
{
	// update the camera first
	mCamera.Update();
	// toggle the gfx debugging mode
	if (InputManager::GetInstance()->GetKeyTriggered(KeyCode::V))
		mDebugGfxMode = !mDebugGfxMode;

	if (InputManager::GetInstance()->GetActionTriggered("textureToggle"))
		mModelList["Quad"]->mActive = ~mModelList["Quad"]->mActive;

	// toggle the color mode
	if (InputManager::GetInstance()->GetActionTriggered("colorToggle"))
		g_colorToggled = ~g_colorToggled;

	if (InputManager::GetInstance()->GetKeyTriggered(KeyCode::P))
		g_postProcessingEnabled = !g_postProcessingEnabled;

	// toggle wireframe mode
	//if (InputManager::GetInstance()->GetActionTriggered("wireframeToggle"))
	//	g_wireframeMode = ~g_wireframeMode;

	// calculate all matrices of the entities
	for (auto &entity : EntityManager::GetInstance()->mEntityList)
	{
		Transform *xform = entity.GetComponent<Transform>();
		Renderer *rdr = entity.GetComponent<Renderer>();

		// entity does not have these components
		if (rdr == nullptr || xform == nullptr)
			continue;

		//rdr->mEnabled ? entity.SetActive(true) : entity.SetActive(false);

		if (!entity.GetActive())
			continue;

		// update transformation matrix of the entity
		UpdateTransformMatrix(xform, rdr);

		// update the animation component if it is a sprite animation
		if (rdr->mModelType == Renderer::MODEL_TYPE::SPRITE_ANIMATION)
		{
			AnimationComponent *anim = entity.GetComponent<AnimationComponent>();
			if (anim == nullptr)
				// add the animation component to the entity
				anim = entity.AddComponent<AnimationComponent>();

			// update the animation
			anim->Update();
		}
	}

	for (auto& entity : EntityManager::GetInstance()->mEntityList)
	{
		if (!entity.GetActive())
			continue;

		Transform* xform = entity.GetComponent<Transform>();
		// check if there is a collider present on the entity
		Collider_AABB* col = entity.GetComponent<Collider_AABB>();

		// entity does not have these components
		if (col == nullptr || xform == nullptr)
			continue;

		UpdateCollidersTransformMatrix(col, xform);
	}
}

void GraphicsSystem::Render()
{
	mRendererList.clear();
	mCollidersList.clear();
	// wireframe mode toggle
	if (g_wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// bind frame buffer
	if (g_postProcessingEnabled)
		glBindFramebuffer(GL_FRAMEBUFFER, mPPFrameBufferID);
	else
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);

	// clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(mBackgroundColor[0], mBackgroundColor[1], mBackgroundColor[2], mBackgroundColor[3]);

	// for the tiles
	if (TileComponent::mEnableScreenGrid)
		mScreenGrid.Render();
	if (TileComponent::mEnableCollisionGrid)
		mCollisionGrid.Render();

	// Default shader
	Shader *basicShader{nullptr};
	AssetManager::GetInstance()->GetAsset("BasicShader", basicShader);
	
	for (auto it = EntityManager::GetInstance()->mEntityList.begin(); it != EntityManager::GetInstance()->mEntityList.end(); ++it)
	{
		//Skip inactive entities
		if (!it->GetActive())
			continue;

		Renderer* rdr = it->GetComponent<Renderer>();
		if (rdr)
			mRendererList.push_back(std::make_pair(it, rdr));

		Collider_AABB* col = it->GetComponent<Collider_AABB>();
		if (col)
			mCollidersList.push_back(std::make_pair(it, col));
	}

	auto sortByRenderLayer = [](EntityRdr a, EntityRdr b)
	{
		return a.second->mRenderLayer < b.second->mRenderLayer;
	};
	std::stable_sort(mRendererList.begin(), mRendererList.end(), sortByRenderLayer);


	for (auto& [it, rdr] : mRendererList)
	{
		Entity& entity = *it;
		// use the currently bound shader
		Shader* currShader{nullptr};
		if (false == AssetManager::GetInstance()->GetAsset(rdr->mShaderName, currShader))
			currShader = basicShader;

		//Skip inactive objects
		if (rdr->mEnabled == false)
			continue;

		TextManager::GetInstance()->RenderText(static_cast<int>(rdr->mRenderLayer));

		//Skip objects that are not in camera
		Transform* entXform = entity.GetComponent<Transform>();
		if (abs(entXform->GetPosition().mX - GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition.x) > 
				(GraphicsSystem::GetInstance()->mWorldSize.x / 2) * (1 / GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom) + entXform->mScale.mX / 2
			|| abs(entXform->GetPosition().mY - GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition.y) > 
				(GraphicsSystem::GetInstance()->mWorldSize.y / 2) * (1 / GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom) + entXform->mScale.mY / 2)
			continue;

		currShader->Use();

		// set variables
		AssetManager::ImageAsset* texture{nullptr};
		AssetManager::GetInstance()->GetAsset(rdr->mTextureName, texture);
		currShader->SetUniform("uModel_to_NDC", rdr->mMdl_to_ndc_xform);
		currShader->SetUniform("uAlpha", rdr->mAlphaValue);
		currShader->SetUniform("uCustomAlphaEnabled", rdr->mCustomAlphaEnabled);

		// texture enabling
		if (rdr->mModelType == Renderer::MODEL_TYPE::SPRITE)
		{
			if (!texture || rdr->mTextureName == "") // asset doesnt exist
				continue;

			//currShader->SetUniform("uTex2d", (GLint)texture->mTexID);
			currShader->SetUniform("uTextureEnabled", true);
			// Bind texture unit
			glBindTexture(GL_TEXTURE_2D, texture->mTexID);
			//glBindTextureUnit(texture->mTexID, texture->mTexID);
		}

		switch (rdr->mModelType)
		{
			case Renderer::MODEL_TYPE::POINT:
				mModelList["Point"]->Render();
				break;
			case Renderer::MODEL_TYPE::LINE:
				mModelList["Line"]->Render();
				break;
			case Renderer::MODEL_TYPE::CIRCLE:
				mModelList["Circle"]->Render();
				break;
			case Renderer::MODEL_TYPE::QUAD:
				mModelList["Quad"]->Render();
				break;
			case Renderer::MODEL_TYPE::BOX:
				mModelList["Box"]->Render();
				break;
			case Renderer::MODEL_TYPE::SPRITE:
				mModelList["Quad"]->Render();
				break;
			//case Renderer::MODEL_TYPE::SPRITE_ANIMATION:
			//	mModelList["Quad"]->Render();
			//	break;
			case Renderer::MODEL_TYPE::PARALLAX:
				mModelList["Quad"]->Render();
				break;
			case Renderer::MODEL_TYPE::AI_IRIS:
				mModelList["Quad"]->Render();
				break;
			case Renderer::MODEL_TYPE::LASER:
				mModelList["Quad"]->Render();
				break;
		}

		//Sprite animation
		if (rdr->mModelType == Renderer::MODEL_TYPE::SPRITE_ANIMATION)
		{
			Shader* animShader{nullptr};
			AssetManager::GetInstance()->GetAsset("Animation", animShader);

			RenderAnimation(entity, animShader);
		}

		if (rdr->mModelType == Renderer::MODEL_TYPE::PARALLAX)
		{
			Shader* parallaxShader{nullptr};
			AssetManager::GetInstance()->GetAsset(rdr->mShaderName, parallaxShader);
			RenderParallax(entity, parallaxShader);
		}

		currShader->UnUse();
	}

#if !_BLOATWARE
	for (auto& [it, col] : mCollidersList)
	{
		Entity& entity = *it;
		// for gfx debugging
		Shader* debugShader{nullptr};
		AssetManager::GetInstance()->GetAsset("GfxDebug", debugShader);

		if (mDebugGfxMode)
			RenderGfxDebug(entity, debugShader);
	}
	if (mSelectedScaleRotate != nullptr)
	{
		Entity& entity = *mSelectedScaleRotate;
		Shader* scaleRotateShader{ nullptr };
		AssetManager::GetInstance()->GetAsset("GfxScaleRotate", scaleRotateShader);
		RenderGfxScaleRotate(entity, scaleRotateShader);
	}

#endif // _BLOATWARE

	// unbind frame buffer & texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Post Processing
	//__________________________________________________________________________________________*/

	if (g_postProcessingEnabled)
	{
		//Bind default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);

			//Use Bloom Shader
			Shader* bloomShader{ nullptr };
			AssetManager::GetInstance()->GetAsset("BloomShader", bloomShader);
			bloomShader->Use();
			if (SceneManager::GetInstance()->mActiveSceneName == "CreditsScene")
			{
				bloomShader->SetUniform("_BloomAmount", 0.0f);
			}
			else
			{
				bloomShader->SetUniform("_BloomAmount", 1.0f);
			}
			bloomShader->SetUniform("uWidth", static_cast<int>(mVp.mWidth));
			bloomShader->SetUniform("uHeight", static_cast<int>(mVp.mHeight));
			bloomShader->SetUniform("mOpacity", WindowsManager::GetInstance()->mSceneTrueOpacity); //will be used for scene fade outs, only doesn't affect text
			bloomShader->SetUniform("mRedIntensity", WindowsManager::GetInstance()->mSceneRedIntensity);
			bloomShader->SetUniform("_TimeUniform", (float)glfwGetTime());
			bloomShader->SetUniform("_DeathUniform", WindowsManager::GetInstance()->mDeathVignette);
			AssetManager::ImageAsset* texture{ nullptr };
			AssetManager::GetInstance()->GetAsset("Misc_DisruptionNoiseTexture", texture);
			bloomShader->SetUniform("_NoiseTexture", (GLint)texture->mTexID);
			glBindTextureUnit(texture->mTexID, texture->mTexID);

			//Resetting shader values
			if (EntityManager::GetInstance()->GetEntity(Player::mPlayerID) != nullptr
				&& EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>() != nullptr
				&& EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>()->mCutsceneTimer <= 0.f)
				mParticleAlpha = mParticleSpeed = mParticleBloom = 0.f;

			bloomShader->SetUniform("_DisruptProgress", mParticleAlpha);
			bloomShader->SetUniform("_DisruptSpeedProgress", mParticleSpeed);
			bloomShader->SetUniform("_GlowProgress", mParticleBloom);
			glBindTexture(GL_TEXTURE_2D, mPPFrameBufferTextureID);	//Bind scene texture
			mModelList["Quad"]->Render();	//Render scene onto quad object into framebuffer
			bloomShader->UnUse();	//Close shader


		//Unbind frame buffer & texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void GraphicsSystem::Unload()
{
	for (auto &[name, mdl] : mModelList)
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Deleting model: %s\n", name.c_str());
#endif // _ENGINE
		glDeleteVertexArrays(1, &mdl->mVaoid);
		glDeleteBuffers(1, &mdl->mVboid);
		delete mdl;
	}

	glDeleteFramebuffers(1, &mFrameBufferObject);
	glDeleteTextures(1, &mFrameBufferTextureID);
	glDeleteRenderbuffers(1, &mRenderBufferID);
	glDeleteFramebuffers(1, &mPPFrameBufferID);
	glDeleteTextures(1, &mPPFrameBufferTextureID);
	glDeleteRenderbuffers(1, &mPPRenderBufferID);

	// destroy instance
	Destroy();
}

GLboolean GraphicsSystem::GetFrameBufferStatus()
{
	GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (framebufferStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::string errorMessage = "Framebuffer is not complete. Error code: ";
		switch (framebufferStatus)
		{
		case GL_FRAMEBUFFER_UNDEFINED:
			errorMessage += "GL_FRAMEBUFFER_UNDEFINED - The default framebuffer does not exist.";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			errorMessage += "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT - At least one attachment is incomplete or not attached.";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			errorMessage += "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT - No image attachments were specified.";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			errorMessage += "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER - Draw buffer is incomplete or undefined.";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			errorMessage += "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER - Read buffer is incomplete or undefined.";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			errorMessage += "GL_FRAMEBUFFER_UNSUPPORTED - The combination of internal formats is not supported.";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			errorMessage += "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE - Multisampling is incomplete or inconsistent.";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			errorMessage += "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS - Layered rendering is incomplete.";
			break;
		default:
			errorMessage += "Unknown error.";
			break;
		}

#if _ENGINE
		DebugLogger::GetInstance()->Log("%s\n", errorMessage.c_str());
#endif // _ENGINE
		return GL_FALSE;
	}
	return GL_TRUE;
}

void GraphicsSystem::UpdateTransformMatrix(Transform *_xform, Renderer *_rdr)
{
	// rotation Matrix
	float angleRad = glm::radians(_xform->mRotation);
	glm::vec2 scale = _xform->mScale;
	glm::vec2 translate = _xform->GetPosition();

	glm::mat3 T(1.f), R(1.f), S(1.f);

	T = glm::translate(T, translate);
	R = glm::rotate(R, angleRad);
	S = glm::scale(S, scale * 0.5f);

	// MDL to NDC matrix
	glm::mat4 mdlXform = T * R * S;
	_rdr->mMdl_to_ndc_xform = mCamera.mXformMat * mdlXform;
	_xform->mRdr.mMdl_to_ndc_xform = _rdr->mMdl_to_ndc_xform;
}

void GraphicsSystem::UpdateCollidersTransformMatrix(Collider_AABB *_col, Transform *_xform)
{
	glm::mat3 T(1.f), R(1.f), S(1.f);

	glm::vec2 translation = _col->mFinalpos;
	glm::vec2 scaling = _xform->mScale;
	float rot = glm::radians(_xform->mRotation);;

	T = glm::translate(T, translation);
	R = glm::rotate(R, rot);
	S = glm::scale(S, scaling * 0.5f);
	glm::mat4 mdlXform = T * R * S;
	_col->mRdr.mMdl_to_ndc_xform = mCamera.mXformMat * mdlXform;
}

void GraphicsSystem::CreateFrameBuffer()
{
	//DEFAULT FRAME BUFFER
	// create frame buffer object
	glGenFramebuffers(1, &mFrameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);

	// apply the rendered scene to this texture
	glGenTextures(1, &mFrameBufferTextureID);
	glBindTexture(GL_TEXTURE_2D, mFrameBufferTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mVp.mWidth, mVp.mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFrameBufferTextureID, 0);

	// create render buffer
	glGenRenderbuffers(1, &mRenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mVp.mWidth, mVp.mHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderBufferID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
#endif // _ENGINE
	}

	// unbind all buffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//SCENE FRAME BUFFER (TO BE USED FOR POST PROCESSING)
	// create frame buffer object
	glGenFramebuffers(1, &mPPFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mPPFrameBufferID);

	// apply the rendered scene to this texture
	glGenTextures(1, &mPPFrameBufferTextureID);
	glBindTexture(GL_TEXTURE_2D, mPPFrameBufferTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mVp.mWidth, mVp.mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPPFrameBufferTextureID, 0);


	// create render buffer
	glGenRenderbuffers(1, &mPPRenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, mPPRenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mVp.mWidth, mVp.mHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mPPRenderBufferID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
#endif // _ENGINE
	}

	// unbind all buffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

}

void GraphicsSystem::RescaleFrameBuffer(GLint _width, GLint _height)
{
	glBindTexture(GL_TEXTURE_2D, mFrameBufferTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFrameBufferTextureID, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderBufferID);

	glBindTexture(GL_TEXTURE_2D, mPPFrameBufferTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPPFrameBufferTextureID, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, mPPRenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mPPRenderBufferID);

	// update viewport size
	mVp.mWidth = _width;
	mVp.mHeight = _height;

	mCamera.UpdateCameraInfo();
}

void GraphicsSystem::RenderGfxDebug(Entity &_entity, Shader *_debugShader)
{
	_debugShader->Use();
	Collider_AABB *col = _entity.GetComponent<Collider_AABB>();
	if (col != nullptr)
	{
		static double elapsedTime{};

		switch (col->mColliderType)
		{
		case ColliderType::AABB:
			if (col->GetIsCollided())
			{
				_debugShader->SetUniform("uIsColliding", true);
				elapsedTime += WindowsManager::GetInstance()->mDeltaTime;

				if (elapsedTime > 1)
				{
					_debugShader->SetUniform("uIsColliding", false);
					col->SetIsCollided(false);
					elapsedTime = 0;
				}
			}
			break;
		case ColliderType::OOBB:
			_debugShader->SetUniform("uIsColliding", col->GetIsCollided());
			break;
		default:
			break;
		}
		_debugShader->SetUniform("uModel_to_NDC", col->mRdr.mMdl_to_ndc_xform);

		mModelList["Point"]->Render();

		switch (col->mRdr.mDebugModelType)
		{
		case Renderer::DEBUG_MODEL_TYPE::POINT:
			mModelList["Point"]->Render();
			break;
		case Renderer::DEBUG_MODEL_TYPE::LINE:
			mModelList["Line"]->Render();
			break;
		case Renderer::DEBUG_MODEL_TYPE::CIRCLE:
			mModelList["Circle"]->Render();
			break;
		case Renderer::DEBUG_MODEL_TYPE::BOX:
			mModelList["Box"]->Render();
			break;
		default:
			break;
		}
		if (col->mColliderType == ColliderType::OOBB)
			col->SetIsCollided(false);
	}

	_debugShader->UnUse();
}

void GraphicsSystem::RenderGfxScaleRotate(Entity& _entity, Shader* _scaleRotateShader)
{
	_scaleRotateShader->Use();
	Transform* col = _entity.GetComponent<Transform>();
	if (col != nullptr)
	{
		_scaleRotateShader->SetUniform("uModel_to_NDC", col->mRdr.mMdl_to_ndc_xform);

		//mModelList["Point"]->Render();

		switch (col->mRdr.mScaleRotateModelType)
		{
		case Renderer::DEBUG_MODEL_TYPE::POINT:
			mModelList["Point"]->Render();
			break;
		case Renderer::DEBUG_MODEL_TYPE::LINE:
			mModelList["Line"]->Render();
			break;
		case Renderer::DEBUG_MODEL_TYPE::CIRCLE:
			mModelList["Circle"]->Render();
			break;
		case Renderer::DEBUG_MODEL_TYPE::BOX:
			mModelList["Box"]->Render();
			break;
		default:
			break;
		}
	}

	_scaleRotateShader->UnUse();
}

void GraphicsSystem::RenderAnimation(Entity &_entity, Shader *_animShader)
{
	_animShader->Use();
	AnimationComponent *animComponent = _entity.GetComponent<AnimationComponent>();
	Renderer *rdr = _entity.GetComponent<Renderer>();
	if (animComponent != nullptr)
	{
		_animShader->SetUniform("uModel_to_NDC", rdr->mMdl_to_ndc_xform);
		AssetManager::ImageAsset *texture{nullptr};
		if (AssetManager::GetInstance()->GetAsset(rdr->mTextureName, texture))
		{
			_animShader->SetUniform("uTex2d", (GLint)texture->mTexID);
			_animShader->SetUniform("uCurrFrame", animComponent->mAnimation.mCurrFrameIndex);
			_animShader->SetUniform("uMaxFrames", animComponent->mAnimation.mTotalFrames);
			// bind texture unit
			glBindTextureUnit(texture->mTexID, texture->mTexID);
			mModelList["Quad"]->Render();
		}
	}
	_animShader->UnUse();
}

void GraphicsSystem::RenderParallax(Entity& _entity, Shader* _parallaxShader)
{
	Renderer* rdr = _entity.GetComponent<Renderer>();
	if (rdr != nullptr)
	{
		EntityManager::GetInstance()->GetEntity(_entity.GetID())->GetComponent<Transform>()->SetPosition(Vector3{ GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition.x, GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition.y, 9.f });
		_parallaxShader->SetUniform("uModel_to_NDC", rdr->mMdl_to_ndc_xform);
		_parallaxShader->Use();
		AssetManager::ImageAsset* texture{ nullptr };
		if (AssetManager::GetInstance()->GetAsset(rdr->mTextureName, texture))
		{
			_parallaxShader->SetUniform("uTex2d", (GLint)texture->mTexID);
			_parallaxShader->SetUniform("uTextureEnabled", true);
			// bind texture unit
			glBindTextureUnit(texture->mTexID, texture->mTexID);
			while (mUniformBuffer[_entity.GetID()].size() > 0)
			{
				if (std::holds_alternative<float>(mUniformBuffer[_entity.GetID()].front().second))
					_parallaxShader->SetUniform(mUniformBuffer[_entity.GetID()].front().first, std::get<float>(mUniformBuffer[_entity.GetID()].front().second));
				else
					_parallaxShader->SetUniform(mUniformBuffer[_entity.GetID()].front().first, std::get<glm::vec4>(mUniformBuffer[_entity.GetID()].front().second));
				mUniformBuffer[_entity.GetID()].pop_front();
			}
		}
		_parallaxShader->UnUse();
	}
}

void GraphicsSystem::AddUniformToSet(EntityID _entID, std::string _uniName, float _val)
{
	mUniformBuffer[_entID].emplace_back(std::make_pair(_uniName, _val));
}

void GraphicsSystem::AddUniformToSet(EntityID _entID, std::string _uniName, glm::vec4 _val)
{
	mUniformBuffer[_entID].emplace_back(std::make_pair(_uniName, _val));
}

void GraphicsSystem::PostRender()
{
	Shader* notBasicShader{ nullptr };
	AssetManager::GetInstance()->GetAsset("NotBasicShader", notBasicShader);
	notBasicShader->Use();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mFrameBufferTextureID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, WindowsManager::GetInstance()->mWidth, WindowsManager::GetInstance()->mHeight,
		0, WindowsManager::GetInstance()->mHeight, WindowsManager::GetInstance()->mWidth, 0,
		GL_COLOR_BUFFER_BIT,
		GL_LINEAR);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	notBasicShader->UnUse();
}