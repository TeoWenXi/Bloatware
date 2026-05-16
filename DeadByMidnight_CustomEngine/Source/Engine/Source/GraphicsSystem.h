/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       GraphicsSystem.h
@author     CAI Wenjin (95%)
@co-author	TEO Wen Xi (5%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      GraphicsSystem class declarations.
*//*______________________________________________________________________*/

#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

//External libraries
#include <glad/glad.h>
#include <glm/glm.hpp>

//Standard Libraries
#include <memory>
#include <map>
#include <string>
#include <utility>
#include <algorithm>

//Utility
#include "Singleton.h"
#include "Manager.h"

//Graphics Stuff
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

//Models
#include "PointModel.h"
#include "LineModel.h"
#include "QuadModel.h"
#include "TriangleModel.h"
#include "CircleModel.h"
#include "BoxModel.h"
#include "GridModel.h"

//Components
#include "ComponentsHeader.h"
#include <variant>

#include <stb_image.h>

class GraphicsSystem : public Singleton<GraphicsSystem>, public Manager
{
	/*!
	 * @brief
	 * Viewport struct.
	*/
	struct Viewport
	{
		GLuint mWidth{};
		GLuint mHeight{};
	} ;

	std::string mCurrentBoundShader;

	std::unordered_map<EntityID, std::deque<std::pair<std::string, std::variant<float,glm::vec4>>>> mUniformBuffer;

	friend Singleton<GraphicsSystem>;
public:
	using EntityRdr = std::pair<std::vector<Entity>::iterator, Renderer*>;
	using EntityCol = std::pair<std::vector<Entity>::iterator, Collider_AABB*>;

	bool		mDebugGfxMode{ false };
	bool		mScaleRotateGfxMode{ false };
	bool		mSetColliderColor{ false };
	GLuint  mFrameBufferObject{};
	GLuint  mFrameBufferTextureID{};
	GLuint  mRenderBufferID{};
	Shader  mShader{};
	Texture mTexture{};
	Camera  mCamera{};  //Scene camera
	Model* mModel = nullptr;
	float mBackgroundColor[4]{ 0.f, 0.f, 0.f, 1.f };
	Viewport mVp;

	//World Size
	//From left side of screen to right side of screen
	//From top of screen to bottom of screen
	//Could change it to a vector2 next time if need separate values for width & height
	glm::vec2 mWorldSize{ 200, 200 };

	//Grid Stuff
	GridModel mScreenGrid;
	GridModel mCollisionGrid;

	std::map<std::string, Model*> mModelList;
	std::vector<EntityRdr> mRendererList;
	std::vector<EntityCol> mCollidersList;
	Entity* mSelectedScaleRotate;

	//Framebuffers for Post Processing
	GLuint mPPFrameBufferID{};
	GLuint mPPFrameBufferTextureID{};
	GLuint mPPRenderBufferID{};

	//Bloom shader values
	GLfloat mParticleAlpha = 0.f;
	GLfloat mParticleSpeed = 0.f;
	GLfloat mParticleBloom = 0.f;

	/*!
	 * @brief
	 * Initialization for the graphics system.
	*/
	void Init();
	/*!
	 * @brief
	 * Load function for the graphics system.
	 */
	void Load();

	/*!
	 * @brief
	 * Update function for the graphics system.
	*/
	void Update();
	/*!
	 * @brief
	 * Render function for the graphics system.
	*/
	void Render();
	void PostRender();
	/*!
	 * @brief
	 * Unload function for the graphics system.
	*/
	void Unload();

	/*!
	 * @brief
	 * Returns the frame buffer status.
	 * @return
	 * GL_TRUE if frame buffer status is complete, GL_FALSE otherwise with the printed error message.
	*/
	GLboolean GetFrameBufferStatus();
	/*!
	 * @brief
	 * Updates the transformation matrix after building the scaling, rotation
	 * and translation matrices.
	 * @param _xform
	 * Pointer to the transform component.
	 * @param _rdr
	 * Pointer to the renderer component.
	*/
	void UpdateTransformMatrix(Transform* _xform, Renderer* _rdr);
	/*!
	 * @brief
	 * Updates the colliders transform matrix after building the scaling, rotation
	 * and translation matrices.
	 * @param _col
	 * Pointer to the collider component.
	*/
	void UpdateCollidersTransformMatrix(Collider_AABB* _col, Transform* _xform);

	/*!
	 * @brief
	 * Creates the frame buffer for rendering onto the ImGUI viewport
	 * as a texture.
	*/
	void CreateFrameBuffer();
	/*!
	 * @brief
	 * Rescales the frame buffer with the new width and height.
	 * @param _width
	 * The new width of the framebuffer.
	 * @param _height
	 * The new height of the framebuffer.
	*/
	void RescaleFrameBuffer(GLint _width, GLint _height);

	/*!
	 * @brief
	 * Renders the graphical debugging stuff.
	 * @param _entity
	 * The entity to render the graphical debugging stuff on.
	 * @param _debugShader
	 * The graphics debugging shader, usually "GfxDebug".
	*/
	void RenderGfxDebug(Entity& _entity, Shader* _debugShader);

	/*!
	 * @brief
	 * Renders the graphical scale and rotating.
	 * @param _entity
	 * The entity to render the graphical scale and rotate stuff on.
	 * @param _scaleRotateShader
	 * The graphics debugging shader, usually "GfxScaleRotate".
	*/
	void RenderGfxScaleRotate(Entity& _entity, Shader* _scaleRotateShader);

	/*!
	 * @brief
	 * Renders the animation for entities with sprite animations.
	 * @param _entity
	 * The entity to render the animation onto.
	 * @param _animShader
	 * The animation shader, usually "Animation".
	*/
	void RenderAnimation(Entity& _entity, Shader* _animShader);

	/*!
	 * @brief
	 * Renders the parallax for entities.
	 * @param _entity
	 * The entity to render the parallax onto.
	 * @param _parallaxShader
	 * The parallax shader, usually "Parallax".
	*/
	void RenderParallax(Entity& _entity, Shader* _parallaxShader);

	/*!
	 * @brief 
	 * Adds the specified shader uniform variable and value to the
	 * entity that requires it.
	 * @param _entID 
	 * The ID of the entity that requires the shader uniform uniform to be set.
	 * @param _uniName 
	 * The name of the uniform variable to set.
	 * @param _val 
	 * The value of the uniform variable to set.
	*/
	void AddUniformToSet(EntityID _entID, std::string _uniName, float _val);

	/*!
	 * @brief
	 * Adds the specified shader uniform variable and value to the
	 * entity that requires it.
	 * @param _entID
	 * The ID of the entity that requires the shader uniform uniform to be set.
	 * @param _uniName
	 * The name of the uniform variable to set.
	 * @param _val
	 * The value of the uniform variable to set.
	*/
	void AddUniformToSet(EntityID _entID, std::string _uniName, glm::vec4 _val);

};

// aliases
using GfxSys = GraphicsSystem;
#endif // !GRAPHICS_SYSTEM_H
