#include "NMS_LogFunctions.h"

NMS_LogFunctions* NMS_LogFunctions::m_Singleton = 0;

NMS_LogFunctions::NMS_LogFunctions (void) {
}

NMS_LogFunctions::~NMS_LogFunctions (void) {

}

NMS_LogFunctions& NMS_LogFunctions::GetSingleton (void) {
	if (!m_Singleton) {
		m_Singleton = new(STATIC_ALLOC, MEM_SINGLETON) NMS_LogFunctions;
		Initialize ();
	}

	return *m_Singleton;
}

void NMS_LogFunctions::Initialize (void) {
	errno_t err;
	err=fopen_s (&m_Singleton->m_FErrorLog,"NMS_ErrorLog.txt","w+");
    if (err!=0)
    {
		//throw "Error when trying to load access NMS_ErrorLog.txt!\n";
		//Destroy();
		printf( "Error when trying to load access NMS_ErrorLog.txt!\n" );
		Destroy();
		return;
    }
	err=fopen_s(&m_Singleton->m_FDebugLog,"NMS_DebugLog.txt","w+");
	if (err!=0)
    {
		printf( "Error when trying to load access NMS_DebugLog.txt!\n" );
		Destroy();
		return;
    }
	err=fopen_s(&m_Singleton->m_FRunLog,"NMS_RunLog.txt","w+");
	if (err!=0)
    {
		printf( "Error when trying to load access NMS_RunLog.txt!\n" );
		Destroy();
		return;
    }
}

void NMS_LogFunctions::Destroy (void) {
	if (m_Singleton) {	
		fclose(m_Singleton->m_FErrorLog);
		fclose(m_Singleton->m_FDebugLog);
		fclose(m_Singleton->m_FRunLog);
		delete m_Singleton;
		m_Singleton = 0;
	}
}

void NMS_LogFunctions::write(char* sMessage,logLevel level)
{
	switch(level)
	{
		case LOG_ERROR:fprintf(m_Singleton->m_FErrorLog,sMessage); 
			break;
		case LOG_DEBUG:fprintf(m_Singleton->m_FDebugLog,sMessage);
			break;
		case LOG_RUN:fprintf(m_Singleton->m_FRunLog,sMessage); 
			break;
	}
}

