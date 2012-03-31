#include "script.hpp"
#include "math.hpp"
#include "pickler.hpp"
namespace KBEngine{ 
namespace script{

//-------------------------------------------------------------------------------------
Script::Script():
m_module_(NULL),
m_pyStdouterr_(NULL)
{
}

//-------------------------------------------------------------------------------------
Script::~Script()
{
}

//-------------------------------------------------------------------------------------
int Script::Run_SimpleString(std::string command, std::string* retBufferPtr)
{
	if(retBufferPtr != NULL)
	{
		if(!m_pyStdouterrHook_->install()){												
			ERROR_MSG("Script::Run_SimpleString::m_pyStdouterrHook_->install() is failed!\n");
			SCRIPT_ERROR_CHECK();
			return -1;
		}
			
		m_pyStdouterrHook_->setHookBuffer(retBufferPtr);
		PyRun_SimpleString(command.c_str());
		SCRIPT_ERROR_CHECK();														// ����Ƿ��д������
		m_pyStdouterrHook_->uninstall();
		return 0;
	}

	PyRun_SimpleString(command.c_str());
	SCRIPT_ERROR_CHECK();															// ����Ƿ��д������
	return 0;
}

//-------------------------------------------------------------------------------------
bool Script::install(Py_UNICODE* pythonHomeDir, std::wstring pyPaths, const char* moduleName)
{
	Py_SetPythonHome(pythonHomeDir);												// ������python�Ļ�������
	Py_Initialize();                      											// python�������ĳ�ʼ��  
    if (!Py_IsInitialized())
    {
    	ERROR_MSG("Script::install::Py_Initialize is failed!\n");
        return false;
    } 
	
	PySys_SetPath((pyPaths + SCRIPT_PATH).c_str());
	PyObject *m = PyImport_AddModule("__main__");

	m_module_ = PyImport_AddModule(moduleName);										// ����һ���ű�����ģ��
	if (m_module_ == NULL)
		return false;
	
	ScriptStdOutErr::installScript(NULL);											// ��װpy�ض���ģ��
	ScriptStdOutErrHook::installScript(NULL);

	static struct PyModuleDef moduleDesc =   
	{  
			 PyModuleDef_HEAD_INIT,  
			 moduleName,  
			 "This module is created by KBEngine!",  
			 -1,  
			 NULL  
	};  

	PyModule_Create(&moduleDesc);													// ��ʼ������ģ��
	PyObject_SetAttrString(m, moduleName, m_module_);								// ��ģ��������main

	m_pyStdouterr_ = new ScriptStdOutErr();											// �ض���python���
	m_pyStdouterrHook_ = new ScriptStdOutErrHook();
	
	if(!m_pyStdouterr_->install()){													// ��װpy�ض���ű�ģ��
		ERROR_MSG("Script::install::m_pyStdouterr_->install() is failed!\n");
		SCRIPT_ERROR_CHECK();
		return false;
	}
	
	Pickler::initialize();
	math::installModule("Math");
	INFO_MSG("Script::install is successfully!\n");
	return true;
}

//-------------------------------------------------------------------------------------
bool Script::uninstall()
{
	math::uninstallModule();
	Pickler::finalise();
	SCRIPT_ERROR_CHECK();															// ����Ƿ��д������

	if(m_pyStdouterr_->isInstall() && !m_pyStdouterr_->uninstall())					// ж��py�ض���ű�ģ��
		ERROR_MSG("Script::uninstall::m_pyStdouterr_->uninstall() is failed!\n");
	else
		Py_DECREF(m_pyStdouterr_);

	if(m_pyStdouterrHook_->isInstall() && !m_pyStdouterrHook_->uninstall())
		ERROR_MSG("Script::uninstall::m_pyStdouterrHook_->uninstall() is failed!\n");
	else
		Py_DECREF(m_pyStdouterrHook_);

	ScriptStdOutErr::uninstallScript();	
	ScriptStdOutErrHook::uninstallScript();

	Py_Finalize();																	// ж��python������
	INFO_MSG("Script::uninstall is successfully!\n");
	return true;	
}

//-------------------------------------------------------------------------------------

}
}