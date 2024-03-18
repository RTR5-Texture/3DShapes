#pragma once

#ifdef AMC_TEXTURE_EXPORT

#if _MSC_VER
#define AMC_TEXTURE_AAKAAR_API __declspec(dllexport)
#else
#define AMC_TEXTURE_AAKAAR_API __attribute__((visibility("default")))
#endif	// _MSC_VER

#else

#ifdef _MSC_VER
#define AMC_TEXTURE_AAKAAR_API __declspec(dllimport)
#else
#define AMC_TEXTURE_AAKAAR_API
#endif // _MSC_VER

#endif	// AMC_TEXTURE_EXPORT