/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Camera.h
@author     CAI Wenjin (80%)
@co-author	TEO Wen Xi (10%)
@co-author	Jovan Zhuo Wen LOW (10%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Camera class declarations.
*//*______________________________________________________________________*/

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "WindowsManager.h"
#include "ECS_Types.h"
#include "CamWaypoint.h"

class Camera
{
public:
	/*!
	 * @brief
	 * Init function for the camera class.
	 * @param _pos
	 * Position of the camera.
	 * @param _up
	 * Up vector for the camera.
	 * @param _tgt
	 * Target vector for the camera.
	 * @param _top
	 * Top clipping plane value for the orthographic camera.
	 * @param _btm
	 * Bottom clipping plane value for the orthographic camera.
	 * @param _near
	 * Near clipping plane value for the orthographic camera.
	 * @param _far
	 * Far clipping value for the orthographic camera.
	 */
	void Init(glm::vec3 _pos, glm::vec3 _up, glm::vec3 _tgt, GLfloat _top, GLfloat _btm, GLfloat _near, GLfloat _far);
	/*!
	 * @brief 
	 * Updates the camera.
	*/
	void Update();

	/*!
	 * @brief 
	 * Updates the transform matrices of the camera.
	*/
	void UpdateXformMatrices();
	
	/*!
	 * @brief
	 * Update camera's orthogonal clipping plane values with the new top & bottom plane values
	*/
	void UpdateOrtho();

	/*!
	 * @brief 
	 * Toggles the follow player flag.
	 * Requires the player object to be in the scene.
	*/
	void ToggleFollowPlayer();
	/*!
	 * @brief 
	 * Resets the camera position back to the original.
	*/
	void ResetCameraPos();
	/*!
	 * @brief 
	 * Searches for the player object in the scene.
	 * @return 
	 * True if the player object is found, false otherwise.
	*/
	bool SearchPlayer();
	/*!
	 * @brief
	 * Searches for the textbox object in the scene.
	 * @return
	 * True if the textbox object is found, false otherwise.
	*/
	bool SearchTextbox();
	/*!
	 * @brief
	 * Searches for the 3 Parallax BG objects in the scene.
	 * @return
	 * True if the 3 Parallax BG objects are found, false otherwise.
	*/
	bool SearchParallaxBG();
	/*!
	 * @brief
	 * Updates Parallax Background Movement, depending on Camera Position and Time Elapsed since initialization.
	*/
	//void UpdateParallaxBG();

	/*!
	 * @brief
	 * Updates transform matrices and orthogonal clipping plane values
	*/
	void UpdateCameraInfo();

	/*!
	 * @brief 
	 * Screen Shake
	*/
	void DoScreenshake();

	/*!
	 * @brief
	 * Screen Shake until game ends
	*/
	void DoPermanentScreenshake();

	/*!
	 * @brief
	 * Ends Permanent Screen Shake
	*/
	void EndPermanentScreenshake();

	/*!
	 * @brief
	 * Checks if the player is within a certain distance from the left-border of the current camera waypoint or not
	 * @param _range 
	 * The range within the X-Axis to check the player's position for
	 * @return 
	 * True if player is within this range from the left-border, false otherwise.
	*/
	bool IsPlayerInLeftSideRange(float _range);

	/*!
	 * @brief
	 * Interpolate the cmaera movement using smoothstep.
	 * @param _leftEdge 
	 * The value of the left edge.
	 * @param _rightEdge 
	 * The value of right edge.
	 * @param _x 
	 * The value of x.
	 * @return 
	 * The value after computation for the smooth step.
	*/
	float CameraSmoothstep(float _leftEdge, float _rightEdge, float _x);

	//Variables
	glm::vec3 mOriginalPosition{}, mUp{}, mTarget{}, mOriginalTarget{};
	glm::vec3 mOldPosition{}, mOldTarget{};
	//Aspect Ratio
	GLfloat mAr{};

	//Six clipping plane values
	GLfloat mTop{},  mBottom{};	//Top & Bottom
	GLfloat mLeft{}, mRight{};	//Left & Right
	GLfloat mNear{}, mFar{};	//Near & Far

	//Transformation Matrices
	glm::mat4 mXformMat{};
	glm::mat4 mViewXform{};
	glm::mat4 mProjectionXform{};

	//Player ID
	EntityID mPrevPlayerID{}, mPlayerID{};
	//Textbox ID
	EntityID mTextboxID{};

	//Camera movement waypoint system (Zoom + Positions + Clamping + Tracking)
	CamWaypoint mCurrCamInfo;
	GLfloat mCamMoveSpeed = 50.f;
	glm::vec2 mHorizontalThreshold{ 5.5f, 5.5f }; //Left, Right horizontal threshold
	glm::vec2 mVerticalThreshold{ 5.5f, 5.5f }; //Left, Right horizontal threshold



	float mScreenShakeTimeLeft = 0.0f;
	float mPermanentScreenShakeTicker = 0.0f;
	bool mPermanentScreenShakeEnabled = false;
	float mCameraFollowSpeed = 2.0f * 8.f;
	float mCameraFollowMinDistanceThreshold = 1.5f * 8.f;
	float mCameraFollowMaxDistanceThreshold = 5.0f * 8.f;

private:
};

#endif // !CAMERA_H

