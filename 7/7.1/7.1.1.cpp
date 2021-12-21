#include <windows.h>
#include <iostream>

using namespace std;

int main() {
	char dir[256];
	while (strcmp(dir, "end") != 0) {
		cout << "Input directory name:" << endl;
		cin >> dir;
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));
		STARTUPINFO si;
		LPSECURITY_ATTRIBUTES sa = new SECURITY_ATTRIBUTES;//атрибуты защиты 
		sa->nLength = sizeof(sa);
		sa->lpSecurityDescriptor = NULL;//дескриптор безопасности
		sa->bInheritHandle = TRUE;//наследование
		HANDLE hIInputPipe, hOInputPipe, hIOutputPipe, hOOutputPipe;
		if (!CreatePipe(&hIInputPipe, &hOInputPipe, sa, NULL)) {
			cerr << "Error" << GetLastError << endl;
		}
		if (!CreatePipe(&hIOutputPipe, &hOOutputPipe, sa, NULL)) {
			cerr << "Error" << GetLastError << endl;
		}

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(STARTUPINFO);
		si.hStdInput = hIInputPipe;
		si.hStdOutput = hOOutputPipe;
		si.dwFlags |= STARTF_USESTDHANDLES;

		char c[] = "cmd.exe";
		if (!CreateProcess(NULL, c, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
			cerr << "Error " << GetLastError() << endl;
		}

		CloseHandle(hIInputPipe);
		CloseHandle(hOOutputPipe);

		char lpBuffer[50];
		wsprintf(lpBuffer, "cd /d %s \r\n dir \r\n", dir);
		if (!WriteFile(hOInputPipe, lpBuffer, strlen(lpBuffer), 0, NULL)) {
			cerr << "Error " << GetLastError() << endl;
		}

		char output[2];
		int n = 2;
		while (n != 0) {
			if (!ReadFile(hIOutputPipe, output, 1, 0, NULL)) {
				cerr << "Error " << GetLastError() << endl;
			}
			output[1] = '\0';
			n = GetFileSize(hIOutputPipe, 0);
			cout << output;
		}
		cout << endl;
		TerminateProcess(pi.hProcess, 0);
	}

	system("pause");
	return 0;
}