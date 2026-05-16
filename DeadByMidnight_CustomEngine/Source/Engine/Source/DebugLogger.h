/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       DebugLogger.h
@author     SIM Mong Chuan (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      The DebugLogger class provides function Log to print debug information 
			into the debug log tab.
*//*______________________________________________________________________*/

#if _ENGINE
#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#include "Singleton.h"
#include <string>
#include <vector>
#include <stdarg.h>

class DebugLogger : public Singleton < DebugLogger> 
{
private:
	std::vector<std::string> mLog;
	bool mFirst = true;
public:
	/*!
	 * @brief
	 * Function to print debug information similar to printf() function
	 * @param _printlog
	 * C string
	 * @param ...
	 * Variadic arguments
	*/
	void Log(const char* _printlog,...);

};


#endif
#endif // _ENGINE