/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       GameUIObjectHandler.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Component for game UI text
*//*______________________________________________________________________*/
#ifndef GAMEUI_TEXT_HANDLER_H
#define GAMEUI_TEXT_HANDLER_H

#include "ComponentBase.h"
#include "Renderer.h"
#include "TextManager.h"

class GameUITextHandler : public ComponentBase
{
public:
	/*!
	 * @brief
	 * Init virtual function for components
	*/
	void Init();

	/*!
	 * @brief
	 * Update virtual function for components
	*/
	void Update() {};

	/*!
	 * @brief 
	 * Update virtual function for components
	*/
	void Exit();

	/*!
	 * @brief
	 * Load virtual function for components
	*/
	void Load();

	/*!
	 * @brief 
	 * Save virtual function for components
	*/
	void Save();

	/*!
	 * @brief 
	 * DisplayProperties virtual function for components
	*/
	void DisplayProperties();

	/*!
	 * @brief
	 *	Operator << overload to print to logs on creation
	 * @param os
	 *	Output stream to print to
	 * @param _handler
	 *	Reference to the GameUITextHandler
	 * @return
	 *	The output stream to continue printing to
	*/
	friend std::ostream& operator<< (std::ostream& os, const GameUITextHandler& _handler);

	std::string mText;
	std::string mFont;
	int mFontSize{10};
	Vector3 mPos{};
	Vector3 mColor;
	float mAlpha{ 1.0f };
	Renderer::RENDER_LAYER mRenderLayer{ Renderer::RENDER_LAYER::INTERFACE };
};

#endif