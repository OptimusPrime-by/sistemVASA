#include "header.h"

using namespace std;

void function() {
	setlocale(0, "");

	TCHAR str[MAX_PATH];
	if (!GetCurrentDirectory(sizeof(str), str))
		cout << "������" << endl;
	else
		wcout << str << endl;

	if (!CreateDirectory("D:\\Temp", //name
		NULL //atributes security
	))
	{
		cout << "������ �������� ��������." << GetLastError() << endl;
		system("pause");
		return;
	}
	else
		cout << "��������� ������� ������." << endl;

	TCHAR* name = "D:\\test.info";
	TCHAR* Name = "D:\\Temp\\test.info";
	if (!CopyFile(name, //the name of the file to be copied
		Name, //the name of the file to which the existing file will be copied
		FALSE //action in the event of the existence of the file
	))
		cout << "������ ����������� �����." << endl;
	else
		cout << "����������� ���������." << endl;

	HANDLE hFile = CreateFile(Name, //file name
		GENERIC_READ, //access method (read-only)
		0, //sharing modes
		NULL, //security atributes
		OPEN_EXISTING, //open an existing file if the file with the specified if the name does not exist, the function fails
		FILE_ATTRIBUTE_NORMAL, //file attributes (a regular file that has no other attributes')
		NULL //attribute file
	);

	if (hFile == NULL)
		cout << "������ �������� �����." << endl;
	else
		cout << "���� ������." << endl;

	if (!SetFilePointer(hFile, //open file descriptor 
		271014, //the lower 32-bit portion of the signed number that specifies the transition distance within the file
		0, //the highest 32-bit part of the signed number that specifies the transition distance within the file
		FILE_BEGIN //points is counted as the beginning of a transition
	))
		cout << "������ ��������� �������." << endl;
	else
		cout << "����������� ������� �����������" << endl;

	TCHAR buffer[126];
	DWORD n, k;

	if (!ReadFile(hFile, //file descriptor 
		buffer, //pointer to data buffer
		sizeof(buffer), //number of bytes to read
		&n, //number of bytes to read
		NULL //used for asynchronous recording
	))
		cout << "������ ������ ������ �� �����." << endl;
	else
		cout << "������ ���������." << endl;

	TCHAR* Name2 = "D:\\Temp\\answer.txt";

	HANDLE hFile2 = CreateFile(Name2, //file name
		GENERIC_WRITE, //access method (write-only)
		0, //sharing modes
		NULL, //security atributes
		CREATE_NEW, //create an existing file if the file with the specified if the name does not exist, the function fails
		FILE_ATTRIBUTE_NORMAL, //file attributes (a regular file that has no other attributes')
		NULL //attribute file
	);

	if (hFile2 == NULL)
		cout << "������ �������� �����." << endl;
	else
		cout << "���� ������." << endl;

	if (!WriteFile(hFile2, //file descriptor
		buffer, //pointer to data buffer
		n, //number of bytes to write
		&k, //number of bytes to write
		NULL //used for asynchronous recording
	))
		cout << "������ ������ ������ � ����." << endl;
	else
		cout << "������ ���������." << endl;

}
