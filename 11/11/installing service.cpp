#include <iostream>
#include <Windows.h>
using namespace std;

void errorHandler(char * errorArea) {
	char errorStr[1024];	
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errorStr, sizeof(errorStr), NULL);	
	printf("Ошибка в %s код ошибки: %d Пояснение ошибки :%s", errorArea,GetLastError(),errorStr);
	system("pause");
}

int main(int argc, char**argv) {
	setlocale(0,"");
	char service_name[] = "TicTacToeService";

	SC_HANDLE hSCM = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);//Open ManagerService
	if (hSCM==NULL) {
		errorHandler("OpenSCManager");
		return 1;
	}

	SC_HANDLE hService;
	hService = OpenService(hSCM, service_name, SERVICE_ALL_ACCESS);//Open Servece
	if (hService == NULL) {
	
		hService = CreateService(hSCM,service_name,// Дескриптор базы данных service control manager.;
			service_name,//имя, отображаемое в реестре и утилите Services Панели управления;
			SERVICE_ALL_ACCESS,SERVICE_WIN32_OWN_PROCESS,SERVICE_DEMAND_START,SERVICE_ERROR_NORMAL,"D:\\Probnic\\Lab11\\Debug\\Server",NULL,NULL,NULL,NULL,NULL);

		if (hService==NULL) {
			errorHandler("CreateService");
			CloseServiceHandle(hSCM);
			return 1;
		}
		cout << "Сервер установлен." << endl;
	}	
	
		SERVICE_STATUS service_statuc;
		char choose = '0';
		do {
			cout << "Delete and out(0) " << endl;		
			cout << "Start(1)" << endl;
			cin >> choose;
			cin.clear();
			switch(choose)
			{
//Delete
			case '0':
				ControlService(hService, SERVICE_CONTROL_STOP, &service_statuc);
				Sleep(500);
				if (service_statuc.dwCurrentState != SERVICE_STOPPED) {
					errorHandler("ControlService with SERVICE_CONTROL_STOP");					
				}
				if (!DeleteService(hService)) {
					errorHandler("DeleteService");
				}
				else
					cout << "Сервис удален." << endl;
				break;

//Start
			case '1':
				if (!StartService(hService, argc, (LPCSTR*)argv)) {
					errorHandler("StartService");
				}
				else
					cout << "Сервис запущен." << endl;
				break;
			default:
				cout << "Я такой комманды не знаю." << endl;
				break;
			}
		} while (choose != '0');
		
		
	

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCM);
	system("pause");
	return 0;
}