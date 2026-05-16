/*!
All content � 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       EngineScript.cs
@author     HEAR Li Heng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      The C# script that will be linked with when compiling the user created
            scripts to enable the scripts to run our engine functions.
*//*______________________________________________________________________*/

using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Engine;
using System;

/*!
 *   Engine namespace to specify using our engine code with C# code.
 *   Currently available
 *   - Input
 *   - Transform
*/
namespace Engine
{
    public static class Time
    {
        public static float deltaTime
        {
            get
            {
                return (float)GetDeltaTime();
            }
        }

        public static double systemRunTime
        {
            get
            {
                return GetRunTime();
            }
        }

        //External calls
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern double GetDeltaTime();
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern double GetRunTime();
    }

    public static class Debug
    {
        public static void Log<T>(T msg)
        {
            ConsoleLog(msg.ToString());
        }

        //External calls
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void ConsoleLog(string message);
    }

    //Input class so scripters can call Input.GetKey()
    public class Input
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetKey(KeyCode keyCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetKeyDown(KeyCode keyCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetKeyUp(KeyCode keyCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetAction(string action);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetActionDown(string action);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetActionUp(string action);
    }

    //SoundManager class
    public static class SoundManager
    {
        private static float mChannelVolume;
        private static float mChannelPitch;

        public static float channelVolume
        {
            get
            {
                GetChannelVolume(out mChannelVolume);
                return mChannelVolume;
            }
            set
            {
                mChannelVolume = value;
                SetChannelVolume(ref mChannelVolume);

            }
        }

        public static float channelPitch
        {
            get
            {
                GetChannelPitch(out mChannelPitch);
                return mChannelPitch;
            }
            set
            {
                mChannelPitch = value;
                SetChannelPitch(ref mChannelPitch);
            }
        }



        public static void PlaySound(string soundName, float volume, float pitch)
        {
            PlaySoundExternal(soundName, volume, pitch);
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void PlaySoundExternal(string soundName, float volume, float pitch);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetChannelVolume(out float volume);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetChannelVolume(ref float volume);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetChannelPitch(out float pitch);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetChannelPitch(ref float pitch);
    }

    //Camera class
    public static class Camera
    {
        private static Vector3 mPosition;
        private static Vector3 mClampHoriRange;
        private static Vector3 mClampVertRange;
        private static Vector3 mWorldSize;
        private static float mZoom;

        public static Vector3 position
        {
            get
            {
                GetPosition(out mPosition);
                return mPosition;
            }
            set
            {
                mPosition = value;
                SetPosition(ref mPosition);

            }
        }

        public static Vector3 horizontalClampRange
        {
            get
            {
                GetHorizontalClampRange(out mClampHoriRange);
                return mClampHoriRange;
            }
            set
            {
                mClampHoriRange = value;
                SetHorizontalClampRange(ref mClampHoriRange);

            }
        }

        public static Vector3 verticalClampRange
        {
            get
            {
                GetVerticalClampRange(out mClampVertRange);
                return mClampVertRange;
            }
            set
            {
                mClampVertRange = value;
                SetVerticalClampRange(ref mClampVertRange);

            }
        }

        public static Vector3 worldSize
        {
            get
            {
                GetWorldSize(out mWorldSize);
                return mWorldSize;
            }
            set
            {
                mWorldSize = value;
                SetWorldSize(ref mWorldSize);

            }
        }

        public static float zoom
        {
            get
            {
                GetZoom(out mZoom);
                return mZoom;
            }
            set
            {
                mZoom = value;
                SetZoom(ref mZoom);

            }
        }

        public static void DoScreenshake()
        {
            ScreenShakeExternal();
        }

        public static void SetTarget(Transform target)
        {
            UpdateCameraTarget(target.mEntID);
        }

        public static void ClearTarget()
        {
            UpdateCameraTarget(UInt64.MaxValue);
        }

        //External calls
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetPosition(out Vector3 pos);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetPosition(ref Vector3 pos);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetHorizontalClampRange(out Vector3 range);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetHorizontalClampRange(ref Vector3 range);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetVerticalClampRange(out Vector3 range);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetVerticalClampRange(ref Vector3 range);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetWorldSize(out Vector3 size);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetWorldSize(ref Vector3 size);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetZoom(out float zoom);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetZoom(ref float zoom);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void ScreenShakeExternal();
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void UpdateCameraTarget();
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void UpdateCameraTarget(UInt64 entID);
    }

    //Particle system
    public static class ParticleSystem
    {
        public static void SpawnParticles(Vector3 pos, Vector3 offset)
        {
            SpawnParticlesExternal(pos, offset);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SpawnParticlesExternal(Vector3 pos, Vector3 offset);
    }

    //DialogueTrigger struct
    [StructLayout(LayoutKind.Sequential)]
    public struct DialogueTrigger
    {
        private IntPtr mVPtr;
        private UInt64 mEntID;

        public void PlayTrigger()
        {
            if (mEntID != 0)
            {
                PlayTriggerExternal(mEntID);
            }
        }

        //External calls
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void PlayTriggerExternal(UInt64 entID);
    }

    //CamWaypoint struct
    public struct CamWaypoint
    {
        public Vector3 position;
        public Vector3 playerSpawnPos;

        public CamWaypoint(Vector3 pos, Vector3 spawn)
        {
            position = pos;
            playerSpawnPos = spawn;
        }

        public static CamWaypoint GetCurrentCamWaypoint()
        {
            Vector3 pos, spawn;
            GetCurrentCamWaypointExternal(out pos, out spawn);
            return new CamWaypoint(pos, spawn);
        }

        public static void NextCamWaypoint()
        {
            NextCamWaypointExternal();
        }
        

        //External calls
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetCurrentCamWaypointExternal(out Vector3 pos, out Vector3 spawn);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void NextCamWaypointExternal();

        
    }

    //C# Renderer class
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public class Renderer
    {
        public IntPtr mVPtr;
        public UInt64 mEntID;
        public int mModelType;
        public int mDebugModelType;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 9)]
        public float[] mMdl_to_ndc_xform;
        public bool mGfxDebugCollided;

        //cannot be done by marshalling
        //private string mShaderName;
        //private string mTextureName;

        //public string spriteName;
        public string shaderName
        {
            get
            {
                string retString = "";
                if (mEntID != 0)
                {
                    GetShaderName(mEntID, out retString);
                }
                return retString;
            }
            set
            {
                if (mEntID != 0)
                {
                    SetShaderName(mEntID, ref value);
                }
            }
        }

        public string textureName
        {
            get
            {
                string retString = "";
                if (mEntID != 0)
                {
                    GetTextureName(mEntID, out retString);
                }
                return retString;
            }
            set
            {
                if (mEntID != 0)
                {
                    SetTextureName(mEntID, ref value);
                }
            }
        }

        public void SetUniform(string uniformName, float val)
        {
            if (mEntID != 0)
            {
                SetUniformFloat(mEntID, uniformName, val);
            }
        }

        public void SetUniform(string uniformName, Vector4 val)
        {
            if (mEntID != 0)
            {
                SetUniformVec4(mEntID, uniformName, val);
            }
        }

        public static uint GetTextureID(string texName)
        {
            uint ret = 0;
            GetTextureIDExternal(out ret, texName);
            return ret;
        }



        //Externals
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetShaderName(UInt64 entID, out string shdName);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetShaderName(UInt64 entID, ref string shdName);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetTextureName(UInt64 entID, out string texName);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTextureName(UInt64 entID, ref string texName);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetUniformFloat(UInt64 entID, string uniformName, float val);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetUniformVec4(UInt64 entID, string uniformName, Vector4 val);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetTextureIDExternal(out uint id, string uniformName);
    }

    //C# version of Vector3 struct
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
        public float x;
        public float y;
        public float z;

        public Vector3(float _x, float _y, float _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        public static Vector3 operator +(Vector3 lhs, Vector3 rhs)
        {
            return new Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
        }
        public static Vector3 operator -(Vector3 lhs, Vector3 rhs)
        {
            return new Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
        }
        public static Vector3 operator *(Vector3 lhs, float rhs)
        {
            return new Vector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
        }
        public static Vector3 operator /(Vector3 lhs, float rhs)
        {
            return new Vector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
        }


        public static Vector3 operator *(float lhs, Vector3 rhs)
        {
            return rhs * lhs;
        }

        public float Length()
        {
            return (float)Math.Sqrt(LengthSquared());
        }

        public float LengthSquared()
        {
            return x * x + y * y + z * z;
        }

    }

    //C# version of Vector3 struct
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector4
    {
        public float x;
        public float y;
        public float z;
        public float w;

        public Vector4(float _x, float _y, float _z, float _w)
        {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }

        public static Vector4 operator +(Vector4 lhs, Vector4 rhs)
        {
            return new Vector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
        }
        public static Vector4 operator -(Vector4 lhs, Vector4 rhs)
        {
            return new Vector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
        }
    }

    //C# Transform struct
    [StructLayout(LayoutKind.Sequential)]
    public struct Transform
    {
        private IntPtr mVPtr;
        public UInt64 mEntID;
        private Vector3 mPosition;
        private Vector3 mLocalPos;//can only get
        private Vector3 mScale;
        private float mRotation;
        private UInt64 mParentID;

        //Scripter's variables
        public Vector3 position
        {
            get
            {
                if (mEntID != 0)
                {
                    GetPosition(mEntID, out mPosition);
                }
                return mPosition;
            }
            set
            {
                mPosition = value;
                if (mEntID != 0)
                {
                    SetPosition(mEntID, ref mPosition);
                }
            }
        }

        public Vector3 localPosition
        {
            get
            {
                if (mEntID != 0)
                {
                    GetLocalPosition(mEntID, out mLocalPos);
                }
                return mLocalPos;
            }
            set
            {
                mLocalPos = value;
                if (mEntID != 0)
                {
                    Debug.Log("Does not affect Engine's side");
                }
            }
        }

        public Vector3 scale
        {
            get
            {
                if (mEntID != 0)
                {
                    GetScale(mEntID, out mScale);
                }
                return mScale;
            }
            set
            {
                mScale = value;
                if (mEntID != 0)
                {
                    SetScale(mEntID, ref mScale);
                }
            }
        }

        public float rotation
        {
            get
            {
                if (mEntID != 0)
                {
                    GetRotation(mEntID, out mRotation);
                }
                return mRotation;
            }
            set
            {
                mRotation = value;
                if (mEntID != 0)
                {
                    SetRotation(mEntID, ref mRotation);
                }
            }
        }

        public Transform parent
        {
            get
            {
                if (mEntID != 0)
                {
                    GetParent(mEntID, out mParentID);
                }
                return new Transform(mParentID);
            }
            set
            {
                mParentID = value.mEntID;
                if (mEntID != 0)
                {
                    SetParent(mEntID, ref mParentID);
                }
            }

        }

        public Transform(UInt64 entID) { mEntID = entID; }

        public GameObject GetChild(UInt64 id)
        {
            UInt64 childEntID = 0;
            if (mEntID != 0)
            {
                GetChildExternal(mEntID, out id, ref childEntID);
            }
            return new GameObject(childEntID);
        }

        public UInt64 GetChildCount()
        {
            UInt64 childEntID = 0;
            if (mEntID != 0)
            {
                GetChildCountExternal(mEntID, out childEntID);
            }
            return childEntID;
        }


        //External calls
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetPosition(UInt64 entID, out Vector3 pos);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetPosition(UInt64 entID, ref Vector3 pos);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetLocalPosition(UInt64 entID, out Vector3 pos);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetScale(UInt64 entID, out Vector3 scale);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetScale(UInt64 entID, ref Vector3 scale);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetRotation(UInt64 entID, out float rot);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetRotation(UInt64 entID, ref float rot);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetChildExternal(UInt64 entID, out UInt64 childID, ref UInt64 childEntityID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetChildCountExternal(UInt64 entID, out UInt64 childCount);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetParent(UInt64 entID, out UInt64 parentID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetParent(UInt64 entID, ref UInt64 parentID);
    }

    //GameObject class
    public class GameObject
    {
        UInt64 mEntID;
        public Transform transform;
        string mName;

        public GameObject(UInt64 id)
        {
            mEntID = id;
            transform.mEntID = id;
        }

        public static GameObject Instantiate()
        {
            UInt64 entID;
            SpawnEntity(out entID);
            return new GameObject(entID);
        }

        public static GameObject Instantiate(string prefabName)
        {
            UInt64 entID;
            SpawnEntityPrefab(out entID, ref prefabName);
            return new GameObject(entID);
        }


        public static void Destroy(GameObject obj)
        {
            if (obj.mEntID != 0)
                DeleteEntityExternal(obj.mEntID);
        }

        public void Destroy()
        {
            if (mEntID != 0)
                DeleteEntityExternal(mEntID);
        }

        public void RunFunction(string funcName)
        {
            if (mEntID != 0)
                RunFuncExternal(mEntID, funcName);
        }

        public static GameObject Find(string name)
        {
            UInt64 id;
            FindEntity(out id, name);
            return new GameObject(id);
        }

        public string name
        {
            get
            {
                if (mEntID != 0)
                {
                    GetName(mEntID, out mName);
                }
                return mName;
            }
            set
            {
                mName = value;
                if (mEntID != 0)
                {
                    SetName(mEntID, ref mName);
                }
            }
        }

        /*!
         * @brief 
         * The function called by the scripter to get a component attached to the same entity
         * after the script's update is called.
        */
        unsafe public T GetComponent<T>()
        {
            IntPtr ptr = Engine.Components.Engine_GetComponent(mEntID, typeof(T).Name);
            if (ptr == null)
                throw new Exception("MissingComponentException: Entity " + mEntID + " has no component <" + typeof(T).Name + "> but a script is trying to access it.");
            return (T)Marshal.PtrToStructure(ptr, typeof(T));
            // return Unsafe.AsRef<T>(ptr);
        }

        //External calls
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SpawnEntity(out UInt64 entID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SpawnEntityPrefab(out UInt64 entID, ref string prefabName);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void DeleteEntityExternal(UInt64 entID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetName(UInt64 entID, out string name);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetName(UInt64 entID, ref string name);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void RunFuncExternal(UInt64 entID, string funcName);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void FindEntity(out UInt64 entID, string name);

    }

    //C# Physics struct
    [StructLayout(LayoutKind.Sequential)]
    public struct PhysicsComponent
    {
        private IntPtr mVPtr;
        private UInt64 mEntID;
        private Vector3 mCurrentVelocity;
        private Vector3 mGravity;
        private Vector3 mCurrentAcceleration;
        private Vector3 mMoveForce;
        private Vector3 mJumpForce;
        private Vector3 mObjBigScale;

        private float mCurrentA;
        private float mForce;
        private float mMass;
        private float mObjCurrentMass;
        private float mJumpMultiplier;
        private float mJumpVelocity;
        private float mCoefficientOfFriction;
        private bool mIsCharacterSmall;

        //Scripter's variables
        public Vector3 velocity
        {
            get
            {
                if (mEntID != 0)
                {
                    GetVelocity(mEntID, out mCurrentVelocity);
                }
                return mCurrentVelocity;
            }
            set
            {
                mCurrentVelocity = value;
                if (mEntID != 0)
                {
                    SetVelocity(mEntID, ref mCurrentVelocity);
                }
            }
        }
        public Vector3 gravity
        {
            get
            {
                if (mEntID != 0)
                {
                    GetGravity(mEntID, out mGravity);
                }
                return mGravity;
            }
            set
            {
                mGravity = value;
                if (mEntID != 0)
                {
                    SetGravity(mEntID, ref mGravity);
                }
            }
        }
        public Vector3 acceleration
        {
            get
            {
                if (mEntID != 0)
                {
                    GetAcceleration(mEntID, out mCurrentAcceleration);
                }
                return mCurrentAcceleration;
            }
            set
            {
                mCurrentAcceleration = value;
                if (mEntID != 0)
                {
                    SetAcceleration(mEntID, ref mCurrentAcceleration);
                }
            }
        }

        public Vector3 jumpForce
        {
            get
            {
                if (mEntID != 0)
                {
                    GetJumpForce(mEntID, out mJumpForce);
                }
                return mJumpForce;
            }
            set
            {
                mJumpForce = value;
                if (mEntID != 0)
                {
                    SetJumpForce(mEntID, ref mJumpForce);
                }
            }
        }
        public Vector3 objBigScale
        {
            get
            {
                if (mEntID != 0)
                {
                    GetObjectBig(mEntID, out mObjBigScale);
                }
                return mObjBigScale;
            }
            set
            {
                mObjBigScale = value;
                if (mEntID != 0)
                {
                    SetObjectBig(mEntID, ref mObjBigScale);
                }
            }
        }


        public bool isCharacterSmall
        {
            get
            {
                if (mEntID != 0)
                {
                    GetCharacterSmall(mEntID, out mIsCharacterSmall);
                }
                return mIsCharacterSmall;
            }
            set
            {
                mIsCharacterSmall = value;
                if (mEntID != 0)
                {
                    SetCharacterSmall(mEntID, ref mIsCharacterSmall);
                }
            }
        }

        public float mass
        {
            get
            {
                if (mEntID != 0)
                {
                    GetMass(mEntID, out mMass);
                }
                return mMass;
            }
            set
            {
                mMass = value;
                if (mEntID != 0)
                {
                    SetMass(mEntID, ref mMass);
                }
            }
        }


        //External calls
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetVelocity(UInt64 entID, out Vector3 vel);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetVelocity(UInt64 entID, ref Vector3 vel);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetGravity(UInt64 entID, out Vector3 grav);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetGravity(UInt64 entID, ref Vector3 grav);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetAcceleration(UInt64 entID, out Vector3 acc);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetAcceleration(UInt64 entID, ref Vector3 acc);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetJumpForce(UInt64 entID, out Vector3 jump);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetJumpForce(UInt64 entID, ref Vector3 jump);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetObjectBig(UInt64 entID, out Vector3 big);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetObjectBig(UInt64 entID, ref Vector3 big);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetMass(UInt64 entID, out float mass);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetMass(UInt64 entID, ref float mass);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetWindTunnelForce(UInt64 entID, out Vector3 force);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetWindTunnelForce(UInt64 entID, ref Vector3 force);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetCharacterSmall(UInt64 entID, out bool isSmall);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetCharacterSmall(UInt64 entID, ref bool isSmall);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetCharacterSize(UInt64 entID, out float size);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetCharacterSize(UInt64 entID, ref float size);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetCharacterWind(UInt64 entID, out float wind);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetCharacterWind(UInt64 entID, ref float wind);
    }

    //C# Collider struct
    [StructLayout(LayoutKind.Sequential)]
    public struct BoxCollider
    {
        private IntPtr mVPtr;
        private UInt64 mEntID;
        private Vector3 mOffset;
        private Vector3 mScale;
        private Vector3 mFinalpos;
        private Vector3 mMin;
        private Vector3 mMax;

        //Renderer mRdr;
        //ColliderType mColliderType;

        //Scripter's variables
        public Vector3 offset
        {
            get
            {
                if (mEntID != 0)
                {
                    GetOffset(mEntID, out mOffset);
                }
                return mOffset;
            }
            set
            {
                mOffset = value;
                if (mEntID != 0)
                {
                    SetOffset(mEntID, ref mOffset);
                }
            }
        }

        public Vector3 scale
        {
            get
            {
                if (mEntID != 0)
                {
                    GetScale(mEntID, out mScale);
                }
                return mScale;
            }
            set
            {
                mScale = value;
                if (mEntID != 0)
                {
                    SetScale(mEntID, ref mScale);
                }
            }
        }

        //External calls
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetOffset(UInt64 entID, out Vector3 offset);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetOffset(UInt64 entID, ref Vector3 offset);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetScale(UInt64 entID, out Vector3 scale);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetScale(UInt64 entID, ref Vector3 scale);
    }

    //C# TileComponent struct
    [StructLayout(LayoutKind.Sequential)]
    public struct TileComponent
    {
        public TileComponent() { }
        public IntPtr mVPtr;
        public UInt64 mEntID;


        public static int FLAG_ACTIVE = 0x00000001;
        public static int FLAG_VISIBLE = 0x00000002;
        public static int FLAG_NON_COLLIDABLE = 0x00000004;
         
        public static int COLLISION_LEFT = 0x00000001;	//0001
        public static int COLLISION_RIGHT = 0x00000002;	//0010
        public static int COLLISION_TOP = 0x00000004;		//0100
        public static int COLLISION_BOTTOM = 0x00000008;    //1000

        static Vector3 mMinBound;

        static int mTileSize;
        static int mMapWidth;
        static int mMapHeight;



        int mGridCollisionFlag;

        public static Vector3 minBound
        {
            get
            {
                GetMinBound(out mMinBound);
                return mMinBound;
            }
            set
            {
                mMinBound = value;
                SetMinBound(ref mMinBound);
            }
        }
        public static int tileSize
        {
            get
            {
                GetTileSize(out mTileSize);
                return mTileSize;
            }
            set
            {
                mTileSize = value;
                SetTileSize(ref mTileSize);
            }
        }
        public static int mapWidth
        {
            get
            {
                GetMapWidth(out mMapWidth);
                return mMapWidth;
            }
            set
            {
                mMapWidth = value;
                SetMapWidth(ref mMapWidth);
            }
        }
        public static int mapHeight
        {
            get
            {
                GetMapHeight(out mMapHeight);
                return mMapHeight;
            }
            set
            {
                mMapHeight = value;
                SetMapHeight(ref mMapHeight);
            }
        }

        public int gridCollisionFlag
        {
            get
            {
                if (mEntID != 0)
                {
                    GetCollisionFlag(mEntID, out mGridCollisionFlag);
                }
                return mGridCollisionFlag;
            }
            set
            {
                mGridCollisionFlag = value;
                if (mEntID != 0)
                {
                    SetCollisionFlag(mEntID, ref mGridCollisionFlag);
                }
            }
        }

        public static int GetBinaryMap(int x, int y)
        {
            int data;
            GetBinaryMapExternal(x, y, out data);
            return data;
        }

        public static void SetBinaryMap(int x, int y, int data)
        {
            SetBinaryMapExternal(x, y, ref data);
        }

        public static void GetCellValue_YX_Second(int x, int y, out int valueX, out int valueY)
        {
            GetCellValue_YX_SecondExternal(x, y, out valueX, out valueY);
        }
        //External calls
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetMinBound(out Vector3 min);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetMinBound(ref Vector3 min);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetTileSize(out int size);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTileSize(ref int size);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetMapWidth(out int width);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetMapWidth(ref int width);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetMapHeight(out int height);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetMapHeight(ref int height);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetCollisionFlag(UInt64 entID, out int size);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetCollisionFlag(UInt64 entID, ref int size);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetBinaryMapExternal(int x, int y, out int height);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetBinaryMapExternal(int x, int y, ref int height);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetCellValue_YX_SecondExternal (int x, int y, out int valueX, out int valueY);
    }
    

    //Keycode to use with C# Input
    public enum KeyCode : int
    {
        /*The unknown key*/
        KEY_UNKNOWN = -1,
        /* mouse buttons
        * GLFW_MOUSE_BUTTON_1   0
        GLFW_MOUSE_BUTTON_2   1
        GLFW_MOUSE_BUTTON_3   2
        GLFW_MOUSE_BUTTON_4   3
        GLFW_MOUSE_BUTTON_5   4
        GLFW_MOUSE_BUTTON_6   5
        GLFW_MOUSE_BUTTON_7   6
        GLFW_MOUSE_BUTTON_8   7
        */
        /* Printable keys */
        SPACE = 32,
        APOSTROPHE = 39, /* ' */
        COMMA = 44, /* , */
        MINUS = 45, /* - */
        PERIOD = 46, /* . */
        SLASH = 47, /* / */
        ALPHA_0 = 48,
        ALPHA_1 = 49,
        ALPHA_2 = 50,
        ALPHA_3 = 51,
        ALPHA_4 = 52,
        ALPHA_5 = 53,
        ALPHA_6 = 54,
        ALPHA_7 = 55,
        ALPHA_8 = 56,
        ALPHA_9 = 57,
        SEMICOLON = 59, /* ; */
        EQUAL = 61, /* = */
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        LEFT_BRACKET = 91,  /* [ */
        BACKSLASH = 92,  /* \ */
        GRAVE_ACCENT = 96,  /* ` */
        /* Function keys */
        ESCAPE = 256,
        ENTER = 257,
        TAB = 258,
        BACKSPACE = 259,
        INSERT = 260,
        DELETE_ = 261,
        RIGHT = 262,
        LEFT = 263,
        DOWN = 264,
        UP = 265,
        PAGE_UP = 266,
        PAGE_DOWN = 267,
        HOME = 268,
        END = 269,
        CAPS_LOCK = 280,
        SCROLL_LOCK = 281,
        NUM_LOCK = 282,
        PRINT_SCREEN = 283,
        PAUSE = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,
        KEYPAD_0 = 320,
        KEYPAD_1 = 321,
        KEYPAD_2 = 322,
        KEYPAD_3 = 323,
        KEYPAD_4 = 324,
        KEYPAD_5 = 325,
        KEYPAD_6 = 326,
        KEYPAD_7 = 327,
        KEYPAD_8 = 328,
        KEYPAD_9 = 329,
        KEYPAD_DECIMAL = 330,
        KEYPAD_DIVIDE = 331,
        KEYPAD_MULTIPLY = 332,
        KEYPAD_SUBTRACT = 333,
        KEYPAD_ADD = 334,
        KEYPAD_ENTER = 335,
        KEYPAD_EQUAL = 336,
        LEFT_SHIFT = 340,
        LEFT_CONTROL = 341,
        LEFT_ALT = 342,
        LEFT_SUPER = 343,
        RIGHT_SHIFT = 344,
        RIGHT_CONTROL = 345,
        RIGHT_ALT = 346,
        RIGHT_SUPER = 347,
        MENU = 348
    }


    public class Components
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        unsafe public static extern IntPtr Engine_GetComponent(UInt64 entityID, string type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        unsafe public static extern bool Engine_GetTriggerEntered(UInt64 entityID);
    }
}


/*!
 *   MonoBehaviour class that all user created script should inherit so that
 *   the engine can call the Update function.
*/
public unsafe class MonoBehaviour
{
    public Transform transform;
    public GameObject gameobject;
    public UInt64 mEntityId;

    /*!
     * @brief 
     * The init function called by the engine to set the 
     * attached entity id
     * @param id
     * The attached entity id
    */
    public virtual void EngineInit(UInt64 id)
    {
        mEntityId = id;
        try
        {
            transform = new Transform(mEntityId);
            gameobject = new GameObject(mEntityId);
            Awake();
        }
        catch (Exception e)
        {
            Debug.Log(e.Message);
        }
    }

    /*!
     * @brief 
     * The awake function called once the moment the script is registered
    */
    public virtual void Awake() { }

    /*!
     * @brief 
     * The start function called once before Update() is called
    */
    public virtual void Start() { }


    /*!
     * @brief 
     * The update function called every frame.
    */
    public virtual void Update() { }

    /*!
     * @brief 
     * The function called by the engine to sync relevant info
     * after the script's update is called.
    */
    public virtual void EngineUpdate()
    {
        try
        {
            //Call base update
            Update();
        }
        catch (Exception e)
        {
            Debug.Log(e.Message);
        }
    }

    public virtual void EngineOnTriggerEnter(UInt64 otherID)
    {
        OnTriggerEnter(new GameObject(otherID));
    }

    public virtual void EngineOnTriggerStay(UInt64 otherID)
    {
        OnTriggerStay(new GameObject(otherID));
    }

    public virtual void EngineOnTriggerExit(UInt64 otherID)
    {
        OnTriggerExit(new GameObject(otherID));
    }

    public virtual void OnTriggerEnter(GameObject other) { }
    public virtual void OnTriggerExit(GameObject other) { }
    public virtual void OnTriggerStay(GameObject other) { }
}