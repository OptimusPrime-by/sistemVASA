#include <iostream>
#include <windows.h>

using namespace std;

BOOL WINAPI DllMain(HINSTANCE hDll,				// handle to DLL module
					DWORD dwReason,				// reason for calling function
					LPVOID lpReserved) {		// reserved
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH: // �������� DLL 
		break;
	case DLL_THREAD_ATTACH: // �������� ������ 
		break;
	case DLL_THREAD_DETACH: // ���������� ������ 
		break;
	case DLL_PROCESS_DETACH: // ���������� DLL 
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) char Message[] = "Hello,World!";

extern "C" __declspec(dllexport) void SayMessage(char* message) {
	cout << "DLL say> " << message << endl;
	return;
}
