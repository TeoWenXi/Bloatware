/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       VFXEntity.h
@author     Jovan Zhuo Wen LOW (100%)
@co-author  

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      VFXEntity class component, used for spawning Visual Effects and deleting them once their animation finishes
*//*______________________________________________________________________*/

#ifndef VFXENTITY_H
#define VFXENTITY_H

#include "ComponentBase.h"

#include "Vector3.h"
#include "EntityManager.h"

class VFXEntity : public ComponentBase
{
public:
	bool mSetToBeDestroy{ false };
	static std::vector<EntityID> mVFXIDs;

private:



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
	void Update();

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
	 * Operator << overload to print to logs on creation
	 * @param _os 
	 * Output stream to print to
	 * @param _vfxentity 
	 * Reference to the VFX entity
	 * @return 
	 * The output stream to continue printing to
	*/
	friend std::ostream& operator<< (std::ostream& _os, const VFXEntity& _vfxentity);

	EntityID mVFXID;
};

#endif