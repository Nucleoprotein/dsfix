#pragma once

#include <string>

#include <windows.h>
#include <xinput.h>

const static char* dinput8dll = "dinput8.dll";

class DirectInputModuleManager : NonCopyable
{
public:
    HRESULT(WINAPI* DirectInput8Create)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter);
    HRESULT(WINAPI* DllCanUnloadNow)(void);
    HRESULT(WINAPI* DllGetClassObject)(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv);
    HRESULT(WINAPI* DllRegisterServer)(void);
    HRESULT(WINAPI* DllUnregisterServer)(void);

    DirectInputModuleManager()
    {
        m_module = LoadLibraryFromSystemDir(dinput8dll);

        if (!m_module)
        {
            HRESULT hr = GetLastError();
            std::unique_ptr<char[]> error_msg(new char[MAX_PATH]);
            sprintf_s(error_msg.get(), MAX_PATH, "Cannot load \"%s\" error: 0x%x", dinput8dll, hr);
            MessageBoxA(NULL, error_msg.get(), "Error", MB_ICONERROR);
            exit(hr);
        }

        GetProcedureAddress("DirectInput8Create", &DirectInput8Create);
        GetProcedureAddress("DllCanUnloadNow", &DllCanUnloadNow);
        GetProcedureAddress("DllGetClassObject", &DllGetClassObject);
        GetProcedureAddress("DllRegisterServer", &DllRegisterServer);
        GetProcedureAddress("DllUnregisterServer", &DllUnregisterServer);
    }

    ~DirectInputModuleManager()
    {
        if (m_module)
            FreeLibrary(m_module);
    }

    static DirectInputModuleManager& Get()
    {
        static DirectInputModuleManager instance;
        return instance;
    }

private:
    template<typename T>
    inline void GetProcedureAddress(const char* funcname, T* ppfunc)
    {
        *ppfunc = reinterpret_cast<T>(::GetProcAddress(m_module, funcname));
    }

    HMODULE m_module;
};