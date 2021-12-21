#define UNICODE
#include "windows.h"
#include <iostream>

using namespace std;

int main() {
	setlocale(0, "");
	TCHAR str[MAX_PATH]; // ������ ��� �������� �������� �������� �����

	
	
	if (!GetCurrentDirectory(sizeof(str), str)) // ��������� �������� �������� 
		cout << "������" << endl;
	else
		wcout << str << endl;

	

if (!CreateDirectory(L"D:\\Temp\\", // �������� ��������
		NULL // �������=�� ������������
	))
		cout << "������ �������� ��������." << endl;
	else
		cout << "��������� ������� ������." << endl;

SetCurrentDirectory(TEXT("D:\\"));	// ������ �������
GetCurrentDirectory(sizeof(str), str);

	

	const TCHAR* name = L"test.info";
	const TCHAR* Name = L"D:\\Temp\\test.info";

	
	
	
	if (!CopyFile(name,  // ��� ����������� �����
				  Name,			 // ��� �����, � ������� ����� ���������� ������������ ����
				  FALSE			 // �������� � ������ ������������� ����� FALSE - ������������
	))
		cout << "������ ����������� �����." << endl;
	else
		cout << "����������� ���������." << endl;
	

	HANDLE hFile = CreateFile(name, // ��� �����
		GENERIC_READ,				// ����� ������� (������ ��� ������)
		0,							// ������ ����������� �������������
		NULL,						// ��������� �������������
		OPEN_EXISTING,				// ������� ������������ ����, ���� ���� � ��������� ������ �� ����������, ������� ���������� �������
		FILE_ATTRIBUTE_NORMAL,		// �������� ����� (������� ����, �� ������� ������ ���������)
		NULL						// �������� �����
	);

	if (hFile == NULL)
		cout << "������ �������� �����." << endl;
	else
		cout << "���� ������." << endl;

	if (!SetFilePointer(hFile,		// �������� ��������� �����������
		271014,					    // 32-��������� �������� ������� ������� �� ������, ������������ ���������� ������ ��� ����������� ��������� �����.
		0,							// ����� ������� 32-������ ����� ������������ �����, ����������� ���������� �������� ������ �����
		FILE_BEGIN					// ��������� ����� ��� ����������� ��������� ����� (������)
	))
		cout << "������ ��������� �������." << endl;
	else
		cout << "����������� ������� �����������" << endl;

	TCHAR buffer[63];
	DWORD n, k;

	if (!ReadFile(hFile,			// ����������
		buffer,						// ��������� �� �����, ������� ��������� ������, ��������� �� ����� ��� ����������.
		sizeof(buffer),				// ������������ ����� ����������� ������.
		&n,							// ��������� �� ����������, ������� �������� ���������� ������, ����������� ��� ������������� ����������� ��������� hFile
		NULL						// ������������ ��� ����������� ������
	))
		cout << "������ ������ ������ �� �����." << endl;
	else
		cout << "������ ���������." << endl;

	const TCHAR* Name2 = L"D:\\Temp\\answer.txt";

	HANDLE hFile2 = CreateFile(Name2,	 //file name
		GENERIC_WRITE,					 //access method (write-only)
		0,								 //sharing modes
		NULL,							 //security atributes
		CREATE_NEW,						 //create an existing file if the file with the specified if the name does not exist, the function fails
		FILE_ATTRIBUTE_NORMAL,			 //file attributes (a regular file that has no other attributes')
		NULL							 //attribute file
	);

	if (hFile2 == NULL)
		cout << "������ �������� �����." << endl;
	else
		cout << "���� ������." << endl;

	if (!WriteFile(hFile2,				 //file descriptor
		buffer,							 //pointer to data buffer
		n,								 //number of bytes to write
		&k,								 //number of bytes to write
		NULL							 //used for asynchronous recording
	))
		cout << "������ ������ ������ � ����" << endl;
	else
		cout << "������ ���������" << endl;

	system("pause");
	return 0;
}
