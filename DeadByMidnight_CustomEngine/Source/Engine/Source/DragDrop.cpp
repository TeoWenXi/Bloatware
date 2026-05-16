/*!
All content © 2024 DigiPen Institute of Technology Singapore, all rights reserved.
@file       DragDrop.cpp
@author		HEAR Li Heng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2451F24
@section    CSD2451F24-A
@project    CSD2451F24 Software Engineering Project 4

@brief      The code for the drag drop using windows api

*//*______________________________________________________________________*/

#include "WindowsManager.h"
#include "DragDrop.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#undef GLFW_EXPOSE_NATIVE_WIN32

#include <imgui.h>
#include "AssetManager.h"

void DragDrop::Init()
{
	RegisterDragDrop(glfwGetWin32Window(WindowsManager::GetInstance()->mPtrWindow), &*this);
}

void DragDrop::Unload()
{
	RevokeDragDrop(glfwGetWin32Window(WindowsManager::GetInstance()->mPtrWindow));
	OleUninitialize();
}

bool DragDrop::IsDragDrop()
{
	return mIsDragDrop;
}

ULONG DragDrop::AddRef()
{
	return 1;
}

ULONG DragDrop::Release()
{
	return 0;
}

HRESULT DragDrop::QueryInterface(REFIID riid, void** ppvObject)
{
	if (riid == IID_IDropTarget)
	{
		*ppvObject = this;	
		return S_OK;
	}

	*ppvObject = NULL;
	return E_NOINTERFACE;
}
HRESULT DragDrop::DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	UNREFERENCED_PARAMETER(pDataObj);
	UNREFERENCED_PARAMETER(grfKeyState);
	UNREFERENCED_PARAMETER(pt);

	mIsDragDrop = true;//Set to spawn drag object

	//Need to manually set ImGui mouse down since glfw not updating when not in focus
	ImGui::SetMouseDown(0);
	*pdwEffect &= DROPEFFECT_COPY;//sets so that it copies over
	return S_OK;
}

HRESULT DragDrop::DragLeave()
{
	mIsDragDrop = false;
	return S_OK;
}
HRESULT DragDrop::DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	UNREFERENCED_PARAMETER(grfKeyState);

	//Need to manually set ImGui mouse pos since glfw not updating when not in focus
	POINT point = POINT(pt.x,pt.y);
	ScreenToClient(glfwGetWin32Window(WindowsManager::GetInstance()->mPtrWindow), &point);
	ImGui::SetMousePos(ImVec2((float)point.x, (float)point.y));

	*pdwEffect &= DROPEFFECT_COPY;
	return S_OK;
}

HRESULT DragDrop::Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	UNREFERENCED_PARAMETER(pt);
	UNREFERENCED_PARAMETER(grfKeyState);
	UNREFERENCED_PARAMETER(pdwEffect);


	FORMATETC format = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };//formating for get data
	STGMEDIUM data;

	if (SUCCEEDED(pDataObj->GetData(&format, &data)))
	{
		HDROP hdrop = (HDROP)data.hGlobal;
		UINT fileCount = DragQueryFile(hdrop, 0xFFFFFFFF, NULL, 0);//Gets the names of the files

		//loop through all files
		for (UINT i = 0; i < fileCount; i++)
		{
			TCHAR szFile[MAX_PATH];
			DragQueryFile(hdrop, i, szFile, MAX_PATH);
			//Save to assetManager first
			AssetManager::GetInstance()->SaveExternalFiles(szFile);
		}

		//cleanup
		ReleaseStgMedium(&data);

	}


	mIsDragDrop = false;//Set to stop spawning drag object
	ImGui::SetMouseReleased(0);//Update ImGui

	*pdwEffect &= DROPEFFECT_COPY;
	return S_OK;
}

