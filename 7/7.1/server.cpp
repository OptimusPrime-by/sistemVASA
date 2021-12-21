//������
#include <Windows.h>
#include <iostream>
#include <string.h>

using namespace std;

int main()
{
	setlocale(0, "");

	HANDLE hNamedPipe;
	DWORD dwBytesRead;
	DWORD dwBytesWrite;
	char Message[100];
	int nMessageLength;

	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;

	// ��������� �������� ������������, 
	// ��������� ������������ ������������

	sa.bInheritHandle = FALSE;
	sa.nLength = sizeof(sa);
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE); //������������� ������(�������� ����)
	sa.lpSecurityDescriptor = &sd;   // ��������� ���������� ���������� � SECURITY_ATTRIBUTES

	hNamedPipe = CreateNamedPipe("\\\\.\\pipe\\MyPipe", // name pipe
		PIPE_ACCESS_DUPLEX, // channel attributes
		PIPE_TYPE_MESSAGE | PIPE_WAIT, //data transfer mode
		1,//maximum number of channel instances
		0,//output buffer size
		0,//INPUT
		INFINITE,//CLIENT WAITING TIME
		&sa//security attributes
	);

	if (hNamedPipe == INVALID_HANDLE_VALUE) {
		cerr << "Failed to create the pipe" << endl
			<< "Error: " << GetLastError() << endl;
		cin.get();
		return 0;
	}

	cout << "Waiting for connection..." << endl;

	if (!ConnectNamedPipe(
		hNamedPipe,
		NULL  // asynchronous
	))
	{
		cerr << "Failed to connect to the pipe" << endl
			<< "Error: " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cin.get();
		system("pause");
		return 0;
	}
	else cout << "The Client has connected" << endl;

	while (true) {

		if (!ReadFile(hNamedPipe, Message, sizeof(Message), &dwBytesRead,NULL)) {
			if (GetLastError() == 109) {
				cout << "The chat has been closed" << endl;
				break;

			}
			cerr << "Failed to read from the pipe" << endl
				<< "Error: " << GetLastError() << endl;
			CloseHandle(hNamedPipe);
			cin.get();
			system("pause");

			return 0;
		}
		else {
			if (strcmp(Message, "Exit") == 0) {
				cout << "The chat has been closed" << endl;
				break;
			}
			else {
				cout << "Interlocutor: " << Message << endl << "You: ";
			}
		}
		cin.getline(Message, 100);

		if (!WriteFile(hNamedPipe, Message, sizeof(char) * strlen(Message) + 1,	&dwBytesWrite,NULL)) {
			cerr << "Failed to write" << endl
				<< "Error: " << GetLastError() << endl;

			CloseHandle(hNamedPipe);
			cin.get();
			system("pause");
			return 0;
		}

		if (strcmp(Message, "Exit") == 0) {
			cout << "The chat has been closed" << endl;
		}

	}

	DisconnectNamedPipe(hNamedPipe);
	CloseHandle(hNamedPipe);
	system("pause");
	return 0;
}













/*
channel attributes
PIPE_ACCESS_DUPLEX � ������ � ������ � �����;
PIPE_ACCESS_INBOUND � ������ �����, � ������ ������ ������
PIPE_ACCESS_OUTBOUND � ������ �����, � ������ ������ ������

����� �������� ������
PIPE_TYPE_BYTE � ������ ������ �������;
PIPE_TYPE_MESSAGE � ������ ������ �����������;
PYPE_READMODE_BYTE � ������ ������ �������;
PYPE_READMODE_MESSAGE � ������ ������ �����������
*/