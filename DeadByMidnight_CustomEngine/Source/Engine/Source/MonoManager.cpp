/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       MonoManager.cpp
@author		HEAR Li Heng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Inits the Mono runtime and adds the internal calls to link
            EngineDll.h and EngineScript.cs so that the C# scripts can
            'call' our engine functions

            C# scripts will be compiled using the Roslyn C# compiler if
            AssetManager detects any changes to it
*//*______________________________________________________________________*/
#include "MonoManager.h"

#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>
#include <mono/metadata/debug-helpers.h>

#include <vector>
#include <cstdlib>
#if _ENGINE
#include "DebugLogger.h"
#endif // _ENGINE
#include "EngineDll.h"

#include <exception>
//for WinExec
#include <windows.h>

#include <fstream>//for log files


//Contains the name of the script and also the path to where the .dll is stored
struct ScriptInfo
{
    std::string name{};
    std::string dllPath{};
};

//The MonoObject to run with the relevant functions to run
struct ScriptObject
{
    uint64_t entityID{ 0 };
    std::string scriptName{ "" };
    bool updateThroughPause{ false };
    MonoAssembly* assembly{ nullptr };
    MonoImage* image{ nullptr };
    MonoClass* scriptClass{ nullptr };
    MonoObject* scriptObject{ nullptr };

    MonoMethod* scriptEngineUpdate{ nullptr };
    //MonoMethod* scriptEnginePreUpdate{ nullptr };

    MonoMethod* scriptAwake{ nullptr };
    MonoMethod* scriptStart{ nullptr };

    MonoMethod* scriptTriggerEnter{ nullptr };
    MonoMethod* scriptTriggerStay{ nullptr };
    MonoMethod* scriptTriggerExit{ nullptr };


    bool displayInProperties{ false };
    CSScriptComponent* component{ nullptr };
    std::vector<MonoClassField*> fields;
};
MonoDomain* g_rootDomain;
MonoDomain* g_appDomain;

//Contains all the valid scripts
std::vector<ScriptInfo> g_scriptList;
//Contains all the objects to run
std::vector<ScriptObject> g_objectsList;

enum MONO_STATE
{
    FIRST_RUN,
    RUNNING,
    PAUSED,
    STOPPED
}g_monoState;

bool IsAdmin()
{
    bool isAdmin = false;
    HANDLE handle = NULL;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &handle))
    {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);

        if (GetTokenInformation(handle, TokenElevation, &Elevation, sizeof(Elevation), &cbSize))
        {
            isAdmin = Elevation.TokenIsElevated;
        }

        // Cleanup
        CloseHandle(handle);
    }

    return isAdmin;
}

bool RunAsAdmin()
{
    char PathProg[MAX_PATH];

    if (GetModuleFileNameA(NULL, PathProg, MAX_PATH))//Get exe path
    {
        SHELLEXECUTEINFOA SEIA = { sizeof(SEIA) };
        SEIA.lpVerb = "runas";
        SEIA.lpFile = PathProg;
        SEIA.hwnd = NULL;
        SEIA.nShow = SW_NORMAL;

        return ShellExecuteExA(&SEIA);
    }

    return false;
}


//Change to in _BLOATWARE check for dll first if not then compile(Check last updated??)
//Compare with .cs & scene file??
//replace in Asset folder??
void MonoManager::Compile(std::string _scriptPath)
{

    // Define the C# script file name
    // Compile the C# script using mcs
    ScriptInfo sInfo;

    //get script name
    sInfo.name = _scriptPath;
    //remove the folders
    size_t offset = _scriptPath.find_last_of('\\');
    if (offset != std::string::npos)
        sInfo.name = _scriptPath.substr(offset + 1, _scriptPath.size() - offset);
    //remove the extension
    offset = sInfo.name.find_last_of('.');
    if (offset != std::string::npos)
        sInfo.name = sInfo.name.substr(0, offset);

    sInfo.dllPath = "Mono\\" + sInfo.name + ".dll";

#if _BLOATWARE
    g_scriptList.push_back(sInfo);
    return;
#endif

    std::string logName = "Mono\\compileLog" + sInfo.name + ".txt";
    std::string compileCommand = "/C Mono\\roslyn\\csc.exe -unsafe -target:library -out:" + sInfo.dllPath + " Mono\\scripts\\EngineScript.cs " + _scriptPath + " > "+ logName;// +" -r:Mono\\roslyn\\System.Runtime.CompilerServices.Unsafe.dll";


    HINSTANCE result = ShellExecuteA(NULL, "open", "cmd", compileCommand.c_str(), NULL, SW_HIDE);

    //check if file exist first if not cannot compile
    //Find all that are using and then close
    if (g_objectsList.size() > 0)
    {
        mono_assembly_close(g_objectsList[0].assembly);
    }


    if (reinterpret_cast<INT_PTR>(result) <= 32)//Error
    {
        // CMD has error
#if _ENGINE
        DebugLogger::GetInstance()->Log("Compile Failure <%s>\n", _scriptPath.c_str());
#endif // _ENGINE
        //Find alr compiled and load it
        if (AssetManager::GetInstance()->DoesFileExist(sInfo.dllPath))
            g_scriptList.push_back(sInfo);
    }
    else
    {
        // Read output from compile to ensure compile success
        std::ifstream logFile;
        do//wait until file is available for opening
        {
            logFile.open(logName);
        } while (!logFile.is_open());

        std::string input;
        bool error = false;
        while (getline(logFile, input))
        {
            if (input.find("error") != std::string::npos)//Compile failure
            {
                error = true;
                break;
            }
        }
        if (!error)// Compilation succeeded
        {
            g_scriptList.push_back(sInfo);
#if _ENGINE
            DebugLogger::GetInstance()->Log("Compile Success <%s>\n", _scriptPath.c_str());
#endif // _ENGINE
        }
        
        logFile.close();
    }

    //If failed because of admin
    //if (MessageBox(0, "Please allow admin for compilation of scripts.", "", MB_OKCANCEL | MB_ICONQUESTION | MB_TOPMOST) == IDOK)
    //{
    //    if (IsAdmin())
    //    {
    //        ExitProcess(0); // Close this non admin one
    //    }
    //}

    if (g_appDomain)
        Reload();
}

void MonoManager::Reload()
{
    //clear previous entries
    std::vector<std::pair<uint64_t, std::string>> entityIDs;

    for (ScriptObject& obj : g_objectsList)
    {
        entityIDs.push_back(std::make_pair(obj.entityID, obj.scriptName));
        //obj.assembly = nullptr;
        //obj.assembly = nullptr;
        //obj.image = nullptr;
        //obj.scriptObject = nullptr;
        //obj.scriptStart = nullptr;
        //obj.scriptUpdate = nullptr;
        //obj.scriptClass = nullptr;
    }
    g_objectsList.clear();

    //Reset runtime domain

    MonoDomain* oldDomain = g_appDomain;
    if (oldDomain && oldDomain != g_rootDomain)
    {
        mono_domain_set(g_rootDomain, 0);
        mono_domain_unload(oldDomain);
    }

    g_appDomain = mono_domain_create_appdomain(const_cast<char*>("RuntimeApp"), nullptr);
    mono_domain_set(g_appDomain, false);

    for (std::pair<uint64_t, std::string> ent : entityIDs)
    {
        RegisterScript(ent.first, ent.second, ComponentManager::GetInstance()->GetComponent<CSScriptComponent>(ent.first));
    }
}

void MonoManager::UpdateSerializedValues(std::vector<CSScriptComponent::SerializedInfo>& infoList, CSScriptComponent* component)
{
    for (ScriptObject& script : g_objectsList)
    {
        if (script.component == component)
        {
            int fieldCount = 0;
            for (CSScriptComponent::SerializedInfo& info : infoList)
            {
                void* val = NULL;
                if (!val) val = std::get_if<int>(&info.value);
                if (!val) val = std::get_if<float>(&info.value);
                if (!val) val = std::get_if<bool>(&info.value);
                if (!val) val = std::get_if<Vector3>(&info.value);
                if (!val) val = std::get_if<Vector4>(&info.value);
                if (!val) continue;//Unsupported type

                mono_field_set_value(script.scriptObject, script.fields[fieldCount++], val);
            }
            //C# - C++
            //single - float | int - int | bool - bool | Engine.Vector3 - Vector3
            return;
        }
    }
}

void MonoManager::SetFirstRun()
{
    g_monoState = FIRST_RUN;
}

void MonoManager::OnTriggerEnter(EntityID thisID, EntityID otherID)
{
    mTriggerFunctions[thisID].push_back(std::make_pair(otherID, TRIGGER_FUNC::ENTER));
    mTriggerFunctions[otherID].push_back(std::make_pair(thisID, TRIGGER_FUNC::ENTER));
}

void MonoManager::OnTriggerStay(EntityID thisID, EntityID otherID)
{
    mTriggerFunctions[thisID].push_back(std::make_pair(otherID, TRIGGER_FUNC::STAY));
    mTriggerFunctions[otherID].push_back(std::make_pair(thisID, TRIGGER_FUNC::STAY));
}

void MonoManager::OnTriggerExit(EntityID thisID, EntityID otherID)
{
    mTriggerFunctions[thisID].push_back(std::make_pair(otherID, TRIGGER_FUNC::EXIT));
    mTriggerFunctions[otherID].push_back(std::make_pair(thisID, TRIGGER_FUNC::EXIT));
}

void MonoManager::RunFunction(EntityID entityID, std::string funcName)
{
    for (ScriptObject& script : g_objectsList)
    {
        if (script.entityID == entityID)
        {
            MonoMethod* method = mono_class_get_method_from_name(script.scriptClass, funcName.c_str(), 0);
            if (!method) {
#if _ENGINE
                DebugLogger::GetInstance()->Log("Failed to find Function %s\n", funcName.c_str());
#endif // _ENGINE
                return;
            }
            mono_runtime_invoke(method, script.scriptObject, nullptr, nullptr);
        }
    }
}

void MonoManager::RegisterInternalCalls()
{
    mono_add_internal_call("Engine.Time::GetDeltaTime", Engine::Time_GetDeltaTime);
    mono_add_internal_call("Engine.Time::GetRunTime", Engine::Time_GetRunTime);

    mono_add_internal_call("Engine.Debug::ConsoleLog", Engine::Console_Log);

    mono_add_internal_call("Engine.Components::Engine_GetComponent", Engine::Engine_GetComponent);
    mono_add_internal_call("Engine.Components::Engine_GetTriggerEntered", Engine::Engine_GetTriggerEntered);

    mono_add_internal_call("Engine.ParticleSystem::SpawnParticlesExternal", Engine::ParticleSystem_SpawnParticles);

    mono_add_internal_call("Engine.GameObject::SpawnEntity", Engine::GameObject_SpawnEntity);
    mono_add_internal_call("Engine.GameObject::SpawnEntityPrefab", Engine::GameObject_SpawnEntityPrefab);
    mono_add_internal_call("Engine.GameObject::DeleteEntityExternal", Engine::GameObject_DeleteEntity);
    mono_add_internal_call("Engine.GameObject::GetName", Engine::GameObject_GetName);
    mono_add_internal_call("Engine.GameObject::SetName", Engine::GameObject_SetName);
    mono_add_internal_call("Engine.GameObject::RunFuncExternal", Engine::GameObject_RunFuncExternal);
    mono_add_internal_call("Engine.GameObject::FindEntity", Engine::GameObject_FindEntity);

    
    mono_add_internal_call("Engine.SoundManager::PlaySoundExternal", Engine::SoundManager_PlaySoundExternal);
    mono_add_internal_call("Engine.SoundManager::GetChannelVolume", Engine::SoundManager_GetChannelVolume);
    mono_add_internal_call("Engine.SoundManager::SetChannelVolume", Engine::SoundManager_SetChannelVolume);
    mono_add_internal_call("Engine.SoundManager::GetChannelPitch", Engine::SoundManager_GetChannelPitch);
    mono_add_internal_call("Engine.SoundManager::SetChannelPitch", Engine::SoundManager_SetChannelPitch);

    mono_add_internal_call("Engine.Camera::GetPosition", Engine::Camera_GetPosition);
    mono_add_internal_call("Engine.Camera::SetPosition", Engine::Camera_SetPosition);
    mono_add_internal_call("Engine.Camera::GetHorizontalClampRange", Engine::Camera_GetHorizontalClampRange);
    mono_add_internal_call("Engine.Camera::SetHorizontalClampRange", Engine::Camera_SetHorizontalClampRange);
    mono_add_internal_call("Engine.Camera::GetVerticalClampRange", Engine::Camera_GetVerticalClampRange);
    mono_add_internal_call("Engine.Camera::SetVerticalClampRange", Engine::Camera_SetVerticalClampRange);
    mono_add_internal_call("Engine.Camera::GetWorldSize", Engine::Camera_GetWorldSize);
    mono_add_internal_call("Engine.Camera::SetWorldSize", Engine::Camera_SetWorldSize);
    mono_add_internal_call("Engine.Camera::GetZoom", Engine::Camera_GetZoom);
    mono_add_internal_call("Engine.Camera::SetZoom", Engine::Camera_SetZoom);
    mono_add_internal_call("Engine.Camera::ScreenShakeExternal", Engine::Camera_ScreenShake);
    mono_add_internal_call("Engine.Camera::UpdateCameraTarget", Engine::UpdateCameraTarget);



    mono_add_internal_call("Engine.DialogueTrigger::PlayTriggerExternal", Engine::DialogueTrigger_PlayTrigger);

    mono_add_internal_call("Engine.CamWaypoint::GetCurrentCamWaypointExternal", Engine::CamWaypoint_GetCurrentCamWaypoint);
    mono_add_internal_call("Engine.CamWaypoint::NextCamWaypointExternal", Engine::CamWaypoint_NextCamWaypoint);

    


    mono_add_internal_call("Engine.Input::GetKey", Engine::InputManager_GetKey);
    mono_add_internal_call("Engine.Input::GetKeyDown", Engine::InputManager_GetKeyTriggered);
    mono_add_internal_call("Engine.Input::GetKeyUp", Engine::InputManager_GetKeyReleased);
    mono_add_internal_call("Engine.Input::GetAction", Engine::InputManager_GetAction);
    mono_add_internal_call("Engine.Input::GetActionDown", Engine::InputManager_GetActionTriggered);
    mono_add_internal_call("Engine.Input::GetActionUp", Engine::InputManager_GetActionReleased);

    mono_add_internal_call("Engine.Transform::GetPosition", Engine::Transform_GetPosition);
    mono_add_internal_call("Engine.Transform::SetPosition", Engine::Transform_SetPosition);
    mono_add_internal_call("Engine.Transform::GetLocalPosition", Engine::Transform_GetLocalPosition);
    mono_add_internal_call("Engine.Transform::GetScale", Engine::Transform_GetScale);
    mono_add_internal_call("Engine.Transform::SetScale", Engine::Transform_SetScale);
    mono_add_internal_call("Engine.Transform::GetRotation", Engine::Transform_GetRotation);
    mono_add_internal_call("Engine.Transform::SetRotation", Engine::Transform_SetRotation);
    mono_add_internal_call("Engine.Transform::GetChildExternal", Engine::Transform_GetChild);
    mono_add_internal_call("Engine.Transform::GetChildCountExternal", Engine::Transform_GetChildCount);
    mono_add_internal_call("Engine.Transform::GetParent", Engine::Transform_GetParent);
    mono_add_internal_call("Engine.Transform::SetParent", Engine::Transform_SetParent);

    mono_add_internal_call("Engine.PhysicsComponent::GetVelocity", Engine::Physics_GetVelocity);
    mono_add_internal_call("Engine.PhysicsComponent::SetVelocity", Engine::Physics_SetVelocity);
    mono_add_internal_call("Engine.PhysicsComponent::GetGravity", Engine::Physics_GetGravity);
    mono_add_internal_call("Engine.PhysicsComponent::SetGravity", Engine::Physics_SetGravity);
    mono_add_internal_call("Engine.PhysicsComponent::GetAcceleration", Engine::Physics_GetAcceleration);
    mono_add_internal_call("Engine.PhysicsComponent::SetAcceleration", Engine::Physics_SetAcceleration);
    mono_add_internal_call("Engine.PhysicsComponent::GetMass", Engine::Physics_GetMass);
    mono_add_internal_call("Engine.PhysicsComponent::SetMass", Engine::Physics_SetMass);
    mono_add_internal_call("Engine.PhysicsComponent::GetWindTunnelForce", Engine::Physics_GetWindTunnelForce);
    mono_add_internal_call("Engine.PhysicsComponent::SetWindTunnelForce", Engine::Physics_SetWindTunnelForce);
    mono_add_internal_call("Engine.PhysicsComponent::GetJumpForce", Engine::Physics_GetJumpForce);
    mono_add_internal_call("Engine.PhysicsComponent::SetJumpForce", Engine::Physics_SetJumpForce);
    mono_add_internal_call("Engine.PhysicsComponent::GetObjectBig", Engine::Physics_GetObjectBig);
    mono_add_internal_call("Engine.PhysicsComponent::SetObjectBig", Engine::Physics_SetObjectBig);
    mono_add_internal_call("Engine.PhysicsComponent::GetCharacterSmall", Engine::Physics_GetCharacterSmall);
    mono_add_internal_call("Engine.PhysicsComponent::SetCharacterSmall", Engine::Physics_SetCharacterSmall);
    mono_add_internal_call("Engine.PhysicsComponent::GetCharacterSize", Engine::Physics_GetCharacterSize);
    mono_add_internal_call("Engine.PhysicsComponent::SetCharacterSize", Engine::Physics_SetCharacterSize);
    mono_add_internal_call("Engine.PhysicsComponent::GetCharacterWind", Engine::Physics_GetCharacterWind);
    mono_add_internal_call("Engine.PhysicsComponent::SetCharacterWind", Engine::Physics_SetCharacterWind);
    mono_add_internal_call("Engine.PhysicsComponent::SetEnable", Engine::Physics_SetEnable);

    

    mono_add_internal_call("Engine.BoxCollider::GetOffset", Engine::BoxCollider_GetOffset);
    mono_add_internal_call("Engine.BoxCollider::SetOffset", Engine::BoxCollider_SetOffset);
    mono_add_internal_call("Engine.BoxCollider::GetScale", Engine::BoxCollider_GetScale);
    mono_add_internal_call("Engine.BoxCollider::SetScale", Engine::BoxCollider_SetScale);

    mono_add_internal_call("Engine.Renderer::GetShaderName", Engine::Renderer_GetShaderName);
    mono_add_internal_call("Engine.Renderer::SetShaderName", Engine::Renderer_SetShaderName);
    mono_add_internal_call("Engine.Renderer::GetTextureName", Engine::Renderer_GetTextureName);
    mono_add_internal_call("Engine.Renderer::SetTextureName", Engine::Renderer_SetTextureName);
    mono_add_internal_call("Engine.Renderer::SetUniformFloat", Engine::Renderer_SetUniformFloat);
    mono_add_internal_call("Engine.Renderer::SetUniformVec4", Engine::Renderer_SetUniformVec4);
    mono_add_internal_call("Engine.Renderer::GetTextureIDExternal", Engine::Renderer_GetTextureIDExternal);
    

    mono_add_internal_call("Engine.TileComponent::GetMinBound", Engine::TileComponent_GetMinBound);
    mono_add_internal_call("Engine.TileComponent::SetMinBound", Engine::TileComponent_SetMinBound);
    mono_add_internal_call("Engine.TileComponent::GetTileSize", Engine::TileComponent_GetTileSize);
    mono_add_internal_call("Engine.TileComponent::SetTileSize", Engine::TileComponent_SetTileSize);
    mono_add_internal_call("Engine.TileComponent::GetMapWidth", Engine::TileComponent_GetMapWidth);
    mono_add_internal_call("Engine.TileComponent::SetMapWidth", Engine::TileComponent_SetMapWidth);
    mono_add_internal_call("Engine.TileComponent::GetMapHeight", Engine::TileComponent_GetMapHeight);
    mono_add_internal_call("Engine.TileComponent::SetMapHeight", Engine::TileComponent_SetMapHeight);
    mono_add_internal_call("Engine.TileComponent::GetCollisionFlag", Engine::TileComponent_GetCollisionFlag);
    mono_add_internal_call("Engine.TileComponent::SetCollisionFlag", Engine::TileComponent_SetCollisionFlag);
    mono_add_internal_call("Engine.TileComponent::GetBinaryMapExternal", Engine::TileComponent_GetBinaryMap);
    mono_add_internal_call("Engine.TileComponent::SetBinaryMapExternal", Engine::TileComponent_SetBinaryMap);
    mono_add_internal_call("Engine.TileComponent::GetCellValue_YX_SecondExternal", Engine::GetCellValue_YX_Second);

    mono_add_internal_call("Engine.Player::SetPlayerID", Engine::Player_SetPlayerID);

    
}


void MonoManager::Init()
{
    EventManager::GetInstance()->AddObserver(EventPlayPause{}, this);
    g_monoState = MONO_STATE::STOPPED;

    // Initialize the Mono runtime
    mono_set_dirs("Mono", ".");
    g_rootDomain = mono_jit_init("GameProjectApp");
    g_appDomain = mono_domain_create_appdomain(const_cast<char*>("RuntimeApp"), nullptr);

    mono_domain_set(g_appDomain, false);
    //Load all interlinked functions
    //Engine(namespace).Input(class)::Function

    RegisterInternalCalls();

    return;
}

void MonoManager::Load()
{
    //call script start here?
}

void MonoManager::Update()
{
    if (g_monoState == MONO_STATE::FIRST_RUN || g_monoState == MONO_STATE::RUNNING)
    {
        if (g_monoState == MONO_STATE::FIRST_RUN)
        {
            for (ScriptObject& script : g_objectsList)
            {
                mono_runtime_invoke(script.scriptStart, script.scriptObject, nullptr, nullptr);
                UpdateSerializedValues(script.component->serializedValues, script.component);
            }
            g_monoState = MONO_STATE::RUNNING;
        }
        for (ScriptObject& script : g_objectsList)
        {
            //If have collisions
            {
                for (auto collision : mTriggerFunctions[script.entityID])
                {
                    void* args[1];
                    args[0] = &collision.first;
                    switch (collision.second)
                    {
                    case ENTER:
                        mono_runtime_invoke(script.scriptTriggerEnter, script.scriptObject, args, nullptr);
                        break;
                    case STAY:
                        mono_runtime_invoke(script.scriptTriggerStay, script.scriptObject, args, nullptr);
                        break;
                    case EXIT:
                        mono_runtime_invoke(script.scriptTriggerExit, script.scriptObject, args, nullptr);
                        break;
                    default:
                        break;
                    }
                }
            }
#if _ENGINE
            //Get all fields of the class
            {
                if (script.component->serializedValues.size() != script.fields.size())
                    break;
                int valueCounter = 0;
                for (MonoClassField* field : script.fields)
                {
                    void* val = NULL;
                    if (!val) val = std::get_if<int>(&script.component->serializedValues[valueCounter].value);
                    if (!val) val = std::get_if<float>(&script.component->serializedValues[valueCounter].value);
                    if (!val) val = std::get_if<bool>(&script.component->serializedValues[valueCounter].value);
                    if (!val) val = std::get_if<Vector3>(&script.component->serializedValues[valueCounter].value);
                    if (!val) val = std::get_if<Vector4>(&script.component->serializedValues[valueCounter].value);
                    if (!val) continue;//Not supported type
                    mono_field_get_value(script.scriptObject, field, val);
                    ++valueCounter;
                }
            }
#endif // _ENGINE

            MonoObject* exception{ nullptr };
            mono_runtime_invoke(script.scriptEngineUpdate, script.scriptObject, nullptr, &exception);
            if (exception)
            {
                mono_print_unhandled_exception(exception);
            }
        }

        mTriggerFunctions.clear();
    }
    else
    {
        for (ScriptObject& script : g_objectsList)
        {
            if (!script.updateThroughPause)
                continue;
            //If have collisions
            {
                for (auto collision : mTriggerFunctions[script.entityID])
                {
                    void* args[1];
                    args[0] = &collision.first;
                    switch (collision.second)
                    {
                    case ENTER:
                        mono_runtime_invoke(script.scriptTriggerEnter, script.scriptObject, args, nullptr);
                        break;
                    case STAY:
                        mono_runtime_invoke(script.scriptTriggerStay, script.scriptObject, args, nullptr);
                        break;
                    case EXIT:
                        mono_runtime_invoke(script.scriptTriggerExit, script.scriptObject, args, nullptr);
                        break;
                    default:
                        break;
                    }
                }
            }
#if _ENGINE
            //Get all fields of the class
            {
                if (script.component->serializedValues.size() != script.fields.size())
                    break;
                int valueCounter = 0;
                for (MonoClassField* field : script.fields)
                {
                    void* val = NULL;
                    if (!val) val = std::get_if<int>(&script.component->serializedValues[valueCounter].value);
                    if (!val) val = std::get_if<float>(&script.component->serializedValues[valueCounter].value);
                    if (!val) val = std::get_if<bool>(&script.component->serializedValues[valueCounter].value);
                    if (!val) val = std::get_if<Vector3>(&script.component->serializedValues[valueCounter].value);
                    if (!val) val = std::get_if<Vector4>(&script.component->serializedValues[valueCounter].value);
                    if (!val) continue;//Not supported type
                    mono_field_get_value(script.scriptObject, field, val);
                    ++valueCounter;
                }
            }
#endif // _ENGINE

            MonoObject* exception{ nullptr };
            mono_runtime_invoke(script.scriptEngineUpdate, script.scriptObject, nullptr, &exception);
            if (exception)
            {
                mono_print_unhandled_exception(exception);
            }
        }
    }
}

void MonoManager::Unload()
{
    EventManager::GetInstance()->RemoveObserver(EventPlayPause{}, this);

    mono_domain_set(mono_get_root_domain(), false);
    mono_domain_unload(g_appDomain);
    // Shutdown the Mono runtime
    mono_jit_cleanup(g_rootDomain);

    //Destroy instance
    Destroy();
}

void MonoManager::CreateScriptAsset(std::string _fullPath, void*& _dataPtr)
{
    _dataPtr = nullptr;//will be handled by mono runtime so nothing to new
    Compile(_fullPath);
}

void MonoManager::DeleteScriptAsset(void*& _dataPtr)
{
    std::string scriptName = *static_cast<std::string*>(_dataPtr);

    for (std::vector<ScriptInfo>::iterator it = g_scriptList.begin(); it != g_scriptList.end(); ++it)
    {
        if (it->name == scriptName)
        {
            g_scriptList.erase(it);
            break;
        }
    }
    delete static_cast<std::string*>(_dataPtr);
}

void MonoManager::RegisterScript(uint64_t _entID, std::string _scriptName, CSScriptComponent* _component)
{
#if _ENGINE
    DebugLogger::GetInstance()->Log("Registering Script\n");
#endif // _ENGINE
    ScriptObject newScriptObject;
    newScriptObject.entityID = _entID;
    newScriptObject.component = _component;
    // Load the C# assembly (Script.dll)

    for (ScriptInfo& script : g_scriptList)
    {
        if (script.name == _scriptName)
        {
            newScriptObject.scriptName = _scriptName;
            //Load the script dll
            newScriptObject.assembly = mono_domain_assembly_open(g_appDomain, script.dllPath.c_str());
            if (!newScriptObject.assembly)
            {
#if _ENGINE
                DebugLogger::GetInstance()->Log("Failed to load script assembly <%s>\n", script.dllPath.c_str());
#endif // _ENGINE
                return;
            }
            break;
        }
    }

    if (newScriptObject.scriptName == "")
    {
#if _ENGINE
        DebugLogger::GetInstance()->Log("Registering Script Failure <%s>\n", _scriptName.c_str());
#endif // _ENGINE
        return;
    }

    newScriptObject.image = mono_assembly_get_image(newScriptObject.assembly);

    //Find the script class
    newScriptObject.scriptClass = mono_class_from_name(newScriptObject.image, "", newScriptObject.scriptName.c_str());
    if (!newScriptObject.scriptClass)
    {
#if _ENGINE
        DebugLogger::GetInstance()->Log("Failed to find script class <%s>\n", newScriptObject.scriptName.c_str());
#endif // _ENGINE
        return;
    }

    //Create the script object
    newScriptObject.scriptObject = mono_object_new(g_appDomain, newScriptObject.scriptClass);
    mono_runtime_object_init(newScriptObject.scriptObject);

    //To get MonoBehaviour's functions
    MonoClass* mono = mono_class_from_name(newScriptObject.image, "", "MonoBehaviour");
    MonoMethod* scriptEngineInit = mono_class_get_method_from_name(mono, "EngineInit", 1);
    if (!scriptEngineInit) {
#if _ENGINE
        DebugLogger::GetInstance()->Log("Failed to find EngineInit\n");
#endif // _ENGINE
        return;
    }

    newScriptObject.scriptEngineUpdate = mono_class_get_method_from_name(mono, "EngineUpdate", 0);
    if (!newScriptObject.scriptEngineUpdate) {
#if _ENGINE
        DebugLogger::GetInstance()->Log("Failed to find EngineUpdate\n");
#endif // _ENGINE
        return;
    }

    newScriptObject.scriptTriggerEnter = mono_class_get_method_from_name(mono, "EngineOnTriggerEnter", 1);
    if (!newScriptObject.scriptTriggerEnter) {
#if _ENGINE
        DebugLogger::GetInstance()->Log("Failed to find EngineOnTriggerEnter\n");
#endif // _ENGINE
        return;
    }

    newScriptObject.scriptTriggerStay = mono_class_get_method_from_name(mono, "EngineOnTriggerStay", 1);
    if (!newScriptObject.scriptTriggerStay) {
#if _ENGINE
        DebugLogger::GetInstance()->Log("Failed to find EngineOnTriggerStay\n");
#endif // _ENGINE
        return;
    }

    newScriptObject.scriptTriggerExit = mono_class_get_method_from_name(mono, "EngineOnTriggerExit", 1);
    if (!newScriptObject.scriptTriggerExit) {
#if _ENGINE
        DebugLogger::GetInstance()->Log("Failed to find EngineOnTriggerExit\n");
#endif // _ENGINE
        return;
    }

    void* args[1];
    args[0] = &_entID;
    mono_runtime_invoke(scriptEngineInit, newScriptObject.scriptObject, args, nullptr);//run script engineInit



    //Get the main functions
    newScriptObject.scriptStart = mono_class_get_method_from_name(newScriptObject.scriptClass, "Start", 0);
    if (!newScriptObject.scriptStart) {
#if _ENGINE
        DebugLogger::GetInstance()->Log("Failed to find Start\n");
#endif // _ENGINE
        return;
    }

    //Set up Serialized values
    void* iter = 0;
    MonoClassField* field = mono_class_get_fields(newScriptObject.scriptClass, &iter);
    while (field)
    {
        if (mono_field_get_flags(field) & 0x0006)
        {
            newScriptObject.fields.push_back(field);//save field
            std::string type{ mono_type_full_name(mono_field_get_type(field)) };
            std::string name{ mono_field_get_name(field) };
            bool exist = false;
            for (CSScriptComponent::SerializedInfo info : newScriptObject.component->serializedValues)
            {
                if (info.name == name)//already exists
                {
                    //so we update instead
                    mono_field_set_value(newScriptObject.scriptObject, field, &info.value);
                    exist = true;
                    break;
                }
            }

            if (exist)
            {
                field = mono_class_get_fields(newScriptObject.scriptClass, &iter);
                continue;
            }
            if (type == "int")
            {
                int value;
                mono_field_get_value(newScriptObject.scriptObject, field, &value);
                newScriptObject.component->serializedValues.push_back(CSScriptComponent::SerializedInfo(name, value));
            }
            else if (type == "single")
            {
                float value;
                mono_field_get_value(newScriptObject.scriptObject, field, &value);
                newScriptObject.component->serializedValues.push_back(CSScriptComponent::SerializedInfo(name, value));
            }
            else if (type == "bool")
            {
                bool value;
                mono_field_get_value(newScriptObject.scriptObject, field, &value);
                newScriptObject.component->serializedValues.push_back(CSScriptComponent::SerializedInfo(name, value));
            }
            else if (type == "Engine.Vector3")
            {
                Vector3 value;
                mono_field_get_value(newScriptObject.scriptObject, field, &value);
                newScriptObject.component->serializedValues.push_back(CSScriptComponent::SerializedInfo(name, value));
            }
            else if (type == "Engine.Vector4")
            {
                Vector4 value;
                mono_field_get_value(newScriptObject.scriptObject, field, &value);
                newScriptObject.component->serializedValues.push_back(CSScriptComponent::SerializedInfo(name, value));
            }
           

            //C# - C++
            //single - float | int - int | Engine.Vector3 - Vector3 | bool - bool
        }
        field = mono_class_get_fields(newScriptObject.scriptClass, &iter);
    }

    if (_scriptName == "ControllerConnectedScript")
        newScriptObject.updateThroughPause = true;

    mono_gchandle_new(newScriptObject.scriptObject, true);
    g_objectsList.emplace_back(std::move(newScriptObject));
}

void MonoManager::DeregisterScript(uint64_t _entID, std::string _scriptName)
{
    std::vector<ScriptObject>::iterator itr = g_objectsList.begin();
    while (itr != g_objectsList.end())
    {
        if (itr->entityID == _entID && itr->scriptName == _scriptName)
        {
            g_objectsList.erase(itr);
            break;
        }
        ++itr;
    }
}

void MonoManager::OnNotify(EventObject* _eventObj)
{
    EventPlayPause *playEvent = dynamic_cast<EventPlayPause*>(_eventObj);
    if (playEvent)
    {
        switch (playEvent->mEventType)
        {
        case EventPlayPause::PLAY:
            if (g_monoState == MONO_STATE::STOPPED)
                g_monoState = MONO_STATE::FIRST_RUN;
            else
                g_monoState = MONO_STATE::RUNNING;

            break;
        case EventPlayPause::PAUSE:
            g_monoState = MONO_STATE::PAUSED;
            break;
        case EventPlayPause::STOP:
            g_monoState = MONO_STATE::STOPPED;
            Reload();
            break;
        }
    }
}