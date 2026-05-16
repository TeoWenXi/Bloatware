/*!
All content © 2024 DigiPen Institute of Technology Singapore, all rights reserved.
@file       DragDrop.h
@author		HEAR Li Heng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2451F24
@section    CSD2451F24-A
@project    CSD2451F24 Software Engineering Project 4

@brief      This is to allow us to drag and drop even when window is not focused
			Because glfw doest update mouse presses when not in window
			This way we can use DragEnter to see if user is dragging in by
			updating ImGUI ourselves
*//*______________________________________________________________________*/
#ifndef DRAGDROP_H
#define DRAGDROP_H


#include <oleidl.h> //To inherit IDropTarget https://learn.microsoft.com/en-us/windows/win32/api/oleidl/nn-oleidl-idroptarget

class DragDrop : public IDropTarget
{
public:
	void Init();

	void Unload();

	bool IsDragDrop();//Check if there is currently something being dragged

private:
	//From IUnknown

	ULONG AddRef();	//Not used
	ULONG Release();//Not used
	HRESULT QueryInterface(REFIID riid, void** ppvObject);//Not used


	//From IDropTarget

	//When user drags items into the app
	HRESULT DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);

	//When user drags out without releasing
	HRESULT DragLeave();

	//Gets called every frame when user is dragging something
	//Use this to update IMGUI like the highlights
	HRESULT DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);

	//When user releases
	HRESULT Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);

	bool mIsDragDrop = false;
};

#endif