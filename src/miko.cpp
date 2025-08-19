#include "miko/miko.h"
#include <windows.h>
#include <comdef.h>

namespace miko {

static bool g_isInitialized = false;

bool Initialize() {
    if (g_isInitialized) {
        return true; // Already initialized
    }
    
    // Initialize COM for Direct2D and other Windows APIs
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        // COM initialization failed
        return false;
    }
    
    g_isInitialized = true;
    return true;
}

void Shutdown() {
    if (!g_isInitialized) {
        return; // Not initialized
    }
    
    // Uninitialize COM
    CoUninitialize();
    
    g_isInitialized = false;
}

bool IsInitialized() {
    return g_isInitialized;
}

std::string GetVersion() {
    return std::to_string(VERSION_MAJOR) + "." + 
           std::to_string(VERSION_MINOR) + "." + 
           std::to_string(VERSION_PATCH);
}

} // namespace miko