//#    define CUSTOMTYPES_D __declspec(dllexport)

#ifdef __EXP_NMS_UTILITIES
#    define CUSTOMTYPES_D __declspec(dllexport)
#else
#    define CUSTOMTYPES_D __declspec(dllimport)
#endif


#ifndef NMS_CUSTOMTYPES
#define NMS_CUSTOMTYPES

typedef CUSTOMTYPES_D unsigned char BYTE;
typedef CUSTOMTYPES_D unsigned int U32;

#endif