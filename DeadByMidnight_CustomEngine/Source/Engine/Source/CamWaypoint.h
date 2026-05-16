/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       CamWaypoint.h
@author     TEO Wen Xi (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Camera waypoint class declarations.
*//*______________________________________________________________________*/

#ifndef CAMERA_WAYPOINT_H
#define CAMERA_WAYPOINT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class CamWaypoint
{
public:
	//Position & FOV
	glm::vec3 mPosition = { 0, 0, 0 };	//Starting position of the waypoint
	GLfloat mZoom = 1.f;

	//Tracking toggle
	bool mTrackPlayer = false;

	//Player spawn position for the checkpoint
	glm::vec3 mPlayerSpawnPos = { 0, 0, 0 };

	//Horizontal/Vertical movement only
	bool mHoldVertical = false;
	bool mHoldHorizontal = false;

	//Clamping
	bool mClampEnabled = false;
	glm::vec2 mClampVertRange;	//x = bottom, y = top
	glm::vec2 mClampHoriRange;	//x = left, y = right

public:
	//Reseting

	/*!
	 * @brief
	 * Resets the camera waypoint to default values
	*/
	void Reset();

	//Saving & Loading

	/*!
	 * @brief 
	 * Load function for the camera waypoints
	*/
	void Load();
	/*!
	 * @brief 
	 * Save function for the camera waypoints
	*/
	void Save();

	//ImGUI display

	/*!
	 * @brief 
	 * Imgui display for the camera waypoints
	 * @param _waypointIndex 
	 * The index of the current camera waypoint
	*/
	void DisplayProperties(int _waypointIndex);
};


#endif
