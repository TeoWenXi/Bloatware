/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       DebugLogger.cpp
@author     SIM Mong Chuan (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		The DebugLogger class provides function Log to print debug information 
			into the debug log tab.
*//*______________________________________________________________________*/

#if _ENGINE
#include "DebugLogger.h"

#pragma warning(push, 0)
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#pragma warning(pop)

/*!
 * @brief
 * Function to print debug information similar to printf() function
 * @param _printlog
 * C string
 * @param ...
 * Variadic arguments
*/
void DebugLogger::Log(const char* _printlog,...)
{
	if (!ImGui::GetCurrentContext())
	{
		va_list args;
		va_start(args, _printlog);
		char mbuffer[1024];

		int written = vsnprintf(mbuffer, sizeof(mbuffer), _printlog, args);

		std::string error_message(mbuffer, written);
		mLog.emplace_back(error_message);
		va_end(args);mFirst = true;
	}
	else
	{
		if (mFirst == true)
		{
			for (int i = 0; i < mLog.size(); ++i)
			{
				ImGui::DebugLog(mLog[i].c_str());
				mLog[i].erase();
			}
			mFirst = false;
			mLog.clear();
		}

		va_list args;
		va_start(args, _printlog);
		char mbuffer[1024];

		int written = vsnprintf(mbuffer, sizeof(mbuffer), _printlog, args);

		std::string error_message(mbuffer, written);
		ImGui::DebugLog(error_message.c_str());
		va_end(args);
	}
}
#endif // _ENGINE
