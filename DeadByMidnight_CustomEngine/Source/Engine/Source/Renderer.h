/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Renderer.h
@author     TEO Wen Xi (95%)
@co-author	CAI Wenjin (5%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Renderer Component class for rendering purposes
*//*______________________________________________________________________*/

#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include <string>

#include "JSONManager.h"
#include "ComponentBase.h"

class Renderer : public ComponentBase
{
public:
	enum class MODEL_TYPE
	{
		NO_MODEL = 0,
		POINT,
		LINE,
		CIRCLE,
		QUAD,
		BOX,
		TRIANGLE,
		SPRITE,
		SPRITE_ANIMATION,
		COLLIDABLE,
		PARALLAX,
		AI_IRIS,
		LASER,
		NUM_MODELTYPE_COUNT
	};

	enum class DEBUG_MODEL_TYPE
	{
		NO_MODEL = 0,
		POINT,
		LINE,
		CIRCLE,
		BOX,
		NUM_DEBUGMODELTYPE_COUNT
	};

	enum class RENDER_LAYER 
	{
		BACKGROUND = 0,
		MIDGROUND,
		FOREGROUND,
		ABOVETILES,
		INTERFACE,
		NUM_RENDER_LAYER
	};

	MODEL_TYPE mModelType = MODEL_TYPE::SPRITE;					// model type
	DEBUG_MODEL_TYPE mDebugModelType = DEBUG_MODEL_TYPE::BOX;	// debug model type
	DEBUG_MODEL_TYPE mScaleRotateModelType = DEBUG_MODEL_TYPE::BOX;	// scale rotate model type
	RENDER_LAYER mRenderLayer = RENDER_LAYER::FOREGROUND; // layer to render on
	glm::mat3 mMdl_to_ndc_xform{};									// model to NDC transform matrix
	bool mGfxDebugCollided{false};
	bool mEnabled{true};
	std::string mShaderName{ "BasicShader" };						// shader to use for rendering
	std::string mTextureName{ "placeholder" };						// texture to use for sprite & sprite animation
	bool mCustomAlphaEnabled{false};
	float mAlphaValue{0.5f};

public:

	/*!
	 * @brief 
	 * Init function for the renderer component.
	*/
	void Init();

	/*!
	 * @brief 
	 * Update function for the renderer component.
	*/
	void Update() {}

	/*!
	 * @brief 
	 * Exit function for the renderer component.
	*/
	void Exit() {}

	/*!
	 * @brief 
	 * Save function for the renderer component.
	*/
	void Save();

	/*!
	 * @brief 
	 * Load function for the renderer component.
	*/
	void Load();

	/*!
	 * @brief 
	 * Display renderer component properties for ImGui.
	*/
	void DisplayProperties();

	/*!
	 * @brief 
	 * Output stream operator overload to print out information about the renderer component.
	 * @param os 
	 * The output stream.
	 * @param _renderer 
	 * The renderer component object.
	 * @return 
	 * The printed output stream.
	*/
	friend std::ostream& operator<< (std::ostream& os, const Renderer& _renderer);

	/*!
	 * @brief 
	 * Converts the specified render layer into a string for ImGui.
	 * @param _layer 
	 * The render layer to convert to string.
	 * @return 
	 * The string value of the render layer.
	*/
	static const char* ToString(RENDER_LAYER _layer);

	private:
		std::vector<std::string> mRenderLayerName;
};

#endif