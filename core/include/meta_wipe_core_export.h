#pragma once

#ifdef _WIN32
    #ifdef META_WIPER_CORE_EXPORTS
        #define META_WIPER_CORE_EXPORT_FLAG __declspec(dllexport)
    #else
        #define META_WIPER_CORE_EXPORT_FLAG __declspec(dllimport)
    #endif
#else
    #define MMETA_WIPER_CORE_EXPORT_FLAG
#endif