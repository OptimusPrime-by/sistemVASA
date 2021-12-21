// ������
#include <windows.h>
#include <iostream>
#include <string.h>

using namespace std;

int main()
{
	setlocale(0, "");
	char computerName[80];
	char pipeName[80];
	HANDLE hNamedPipe;
	DWORD dwBytesWritten;
	DWORD dwBytesRead;
	char Message[100];
	int nMessageLength;

	cout << "Enter Servername: ";
	cin.getline(computerName, 80);

	wsprintf(pipeName, "\\\\%s\\pipe\\MyPipe", computerName);  //����������� �������� ��� ������ � ������ ������

	if (!WaitNamedPipe(pipeName,     //����� ����� pipeName ����� �������� ��� ����������
		NMPWAIT_WAIT_FOREVER)) {
		cout << "The pipe doesn't exist or is already taken" << endl
			<< "Error: " << GetLastError() << endl;
		cin.get();
		return 0;
	}

	hNamedPipe = CreateFile( //����������� � ������
		pipeName,
		GENERIC_READ | GENERIC_WRITE,
		0, 
		NULL,             //security attributes
		OPEN_EXISTING,  
		0,    //attribute file
		NULL);   //inherited another file attributes

	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Failed to connect to the pipe" << endl
			<< "Error: " << GetLastError() << endl;
		cin.get();
		system("pause");
		return 0;
	}
	else cout << "You have connected successfully\nYou: " << endl;


	while (true)
	{
		cin.getline(Message, 100);             //���� ���������

		nMessageLength = strlen(Message) + 1;

		if (strcmp(Message, "Exit") == 0) {
			cout << "The chat has been closed" << endl;
			break;
		}
		if (!WriteFile(hNamedPipe, Message, sizeof(char) * strlen(Message) + 1,	&dwBytesWritten, NULL)) { //�������� ���������    
			cerr << "Failed to send the message" << endl
				<< "Error: " << GetLastError() << endl;
			CloseHandle(hNamedPipe);
			cin.get();
			system("pause");

			return 0;
		}
		if (!ReadFile(hNamedPipe, Message, sizeof(Message),	&dwBytesRead, NULL)){  //������ ���������
			cerr << "Failed to read the message" << endl << "Error: " << GetLastError() << endl;
			CloseHandle(hNamedPipe);
			cin.get();
			system("pause");
			return 0;
		}
		else {
			if (strcmp(Message, "Exit") == 0)
			{
				cout << "The chat has been closed" << endl;
				break;
			}
			else {
				cout << "Interlocutor: " << Message << endl << "You: ";
			}
		}

	}
	DisconnectNamedPipe(hNamedPipe);
	CloseHandle(hNamedPipe);
	system("pause");
	return 0;

}