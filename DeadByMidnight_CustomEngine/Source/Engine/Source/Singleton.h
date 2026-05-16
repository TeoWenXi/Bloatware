/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Singleton.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Singleton template
*//*______________________________________________________________________*/

#ifndef SINGLETON_TEMPLATE_H
#define SINGLETON_TEMPLATE_H

template <typename T>
class Singleton
{
public:	
	/*!
	 * @brief 
	 *	Returns a pointer to the instance if it exist. If instance does not exist, a new one is created.
	 * @return 
	 *	Returns a pointer to the instance object
	*/
	static T* GetInstance()
	{
		if (instance == nullptr)
			instance = new T();

		return instance;
	}

	/*!
	 * @brief 
	 *	Deletes and nulls the instance pointer
	*/
	void Destroy()
	{
		if (instance)
		{
			delete instance;
			instance = nullptr;
		}
	}

protected:
	/*!
	 * @brief 
	 *	Default constructor
	*/
	Singleton() { };

	/*!
	 * @brief 
	 * Virtual destructor function for the singleton class
	*/
	virtual ~Singleton() { };

private:
	static T* instance;
};

// Created to allow static GetInstance to call private member instance
template <typename T>
T* Singleton<T>::instance = nullptr;

#endif