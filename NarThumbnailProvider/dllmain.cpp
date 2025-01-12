#include <windows.h>
#include <Guiddef.h>
#include <shlobj.h> // For SHChangeNotify
#include "ClassFactory.h" // For the class factory

/* 67c1e27f-dfcb-4f4c-b4b8-1b80c5fde495 */
const CLSID CLSID_RecipeThumbnailProvider ={ 
	0x67c1e27f,
	0xdfcb,
	0x4f4c,
	{0xb4, 0xb8, 0x1b, 0x80, 0xc5, 0xfd, 0xe4, 0x95}
};

HINSTANCE g_hInst = NULL;
long g_cDllRef = 0;

__declspec(dllexport) BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		// Hold the instance of this DLL module, we will use it to get the 
		// path of the DLL to register the component.
		g_hInst = hModule;
		DisableThreadLibraryCalls(hModule);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

_Check_return_
STDAPI  DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID FAR* ppv) {
	HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;

	if (IsEqualCLSID(CLSID_RecipeThumbnailProvider, rclsid)) {
		hr = E_OUTOFMEMORY;

		ClassFactory *pClassFactory = new ClassFactory();
		if (pClassFactory) {
			hr = pClassFactory->QueryInterface(riid, ppv);
			pClassFactory->Release();
		}
	}

	return hr;
}

__control_entrypoint(DllExport)
STDAPI DllCanUnloadNow(void) {
	return g_cDllRef > 0 ? S_FALSE : S_OK;
}

STDAPI DllRegisterServer(void) {
	return S_OK;
}

STDAPI DllUnregisterServer(void) {
	return S_OK;
}
