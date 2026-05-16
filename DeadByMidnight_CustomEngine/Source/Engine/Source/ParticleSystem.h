/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       ParticleSystem.h
@author     SIA Xin Tian (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Update particles
*//*______________________________________________________________________*/


#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "ComponentBase.h"
#include "Vector3.h"

int const MAX_PARTICLES = 4;

struct Particle
{
	float mLife = 0; //life of particle
	Vec3 mPos{};
	Vec3 mVel{};
	Vec3 mScale{};
};


class ParticleSystem : public Singleton<ParticleSystem>, public Manager
{
public:
	Vec3 mPosition;
	Vec3 mVelocity;
	Vec3 mParticleSize;
	bool mIsCharacterSmall{false};

private:
	std::vector<Particle> mParticles; //array of particles
	std::vector<EntityID> mParticleEntityIDs; //array storing particles id
	float const mStartAlphaValue{ 0.04f };
	float const mDecreaseAlphaValue{ 0.00005f };
	Vec3 const mChangeSize{ 0.0005f, 0.0005f, 1 };
	Vec3 mParticleBigSize{ 6.5f, 6.5f, 1 };
	Vec3 mParticleSmallSize{ 3.5f, 3.5f, 1 };

public:
	/*!
	 * @brief
	 * Init function for Particle System
	*/
	void Init();

	/*!
	 * @brief
	 * Load function for Particle System
	*/
	void Load();

	/*!
	 * @brief
	 * Update function for Particle System
	*/
	void Update();

	/*!
	 * @brief
	 * Unload function for Particle System
	*/
	void Unload();

	/*!
	 * @brief Spawn in particles at indicated position
	 * @param _pos 
	 * Position to spawn particles at
	 * @param _offset
	 * Offset the spawn place of particles
	*/
	void SpawnParticles(Vec3 _pos, Vec3 _offset);

	/*!
	 * @brief Get particle index from array mParticleEntityIDs
	 * @param id Get particle index from array
	 * @return 
	 * Index of particle
	*/
	int GetParticleIndex(EntityID id);

	/*!
	 * @brief
	 * Clear particles in array
	*/
	void ClearParticles();

	/*!
	 *@brief
	 * Checking if character is small
	 */
	bool IsSmall();

private:
	friend Singleton<ParticleSystem>;
};


#endif