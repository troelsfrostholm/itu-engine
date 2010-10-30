#define LOG_FUNCTIONS_D __declspec(dllexport)

#ifndef NMS_LOGFUNCTIONS
	#define NMS_LOGFUNCTIONS


#include "NMS_CustomTypes.h"

#include <stdio.h>
#include <iostream>

typedef enum 
{LOG_ERROR, 
 LOG_DEBUG, 
 LOG_RUN}logLevel;

#define LOG			 NMS_LogFunctions::GetSingleton()
#define DESTROY_LOG	 NMS_LogFunctions::Destroy();

class LOG_FUNCTIONS_D NMS_LogFunctions
{
	public :
		 NMS_LogFunctions (void);
		 ~NMS_LogFunctions (void);
		 static NMS_LogFunctions& GetSingleton (void);

	private :
		 static void Initialize (void);

	public :
		 static void Destroy (void);
		 static void write(char* sMessage,logLevel level);


	private:
	    static NMS_LogFunctions* m_Singleton;
		FILE* m_FErrorLog;
		FILE* m_FDebugLog;
		FILE* m_FRunLog;
};
#endif