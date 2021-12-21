#include "iostream"
#include "windows.h"

using namespace std;

DWORD hSize = 0;
HANDLE File;

int main()
{
	setlocale(LC_ALL, "rus");
	HANDLE firstFile = CreateFile("D:\\SPLab9\\EvenParts.jpg",
		GENERIC_READ,  // ������ ��� ������
		0,             // ��������� ������ ��������� ��������� ���� ��� ����������
		NULL,          // ���� �������� ���������� ������������ �� ���������.
		OPEN_EXISTING, // ��������� ������ ����� ���� ����������, ����������� ������� ERROR_FILE_NOT_FOUND
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (firstFile == INVALID_HANDLE_VALUE)
	{
		cout << "Error(1): " << GetLastError() << endl;
		system("pause");
		return 0;
	}
	DWORD firstFileSize = GetFileSize(firstFile, NULL);

	HANDLE secondFile = CreateFile("D:\\SPLab9\\OddParts.jpg", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (secondFile == INVALID_HANDLE_VALUE)
	{
		cout << "Error(2): " << GetLastError() << endl;
		system("pause");
		return 0;
	}
	DWORD secondFileSize = GetFileSize(secondFile, NULL);

	HANDLE firstMapFile = CreateFileMapping(firstFile,
		NULL,           //�������� ���������� ������������ �� ���������
		PAGE_READONLY, // ������ ��� ������
		0, // high dw size (0, current size)
		0,   // low dw size
		NULL); // ���

	if (!firstMapFile)
	{
		cerr << "������ �������� �����! " << endl;
		system("pause");
		return 0;
	}
	HANDLE secondMapFile = CreateFileMapping(secondFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (!secondMapFile)
	{
		cerr << "������ �������� �����! " << endl;
		system("pause");
		return 0;
	}

	hSize = firstFileSize + secondFileSize;

	char* hAll = (char*)VirtualAlloc(NULL, //������� ���� ����������, ��� �������� ������ ������
		hSize,// ������ ���� ������
		MEM_COMMIT, //������������ ������� ������ ��� ��������� ����������������� ������� ������.
					//������� ����� �����������, ���, ����� ���������� ������� ������������ ������� ������ � ������, �� ���������� ����� ����� ����.
					//����������� ���������� �������� �� ��������������, ���� / ���� ����������� ����� ���������� �� ��������.
		PAGE_READWRITE); // �������� ��� ������

	char* FirstFile = (char*)MapViewOfFile(firstMapFile,  //���������� ������� ����������� 
		FILE_MAP_COPY, // ����������� ����� ��� ����������� ��� ������ ��������������
		0,
		0,
		0);
	if (FirstFile == NULL)
	{
		cout << "Error(3)" << GetLastError() << endl;
		system("pause");
		return 0;
	}
	char* SecondFile = (char*)MapViewOfFile(secondMapFile, FILE_MAP_COPY, 0, 0, 0);
	if (SecondFile == NULL)
	{
		cout << "Error(3)" << GetLastError() << endl;
		system("pause");
		return 0;
	}

	File = CreateFile("D:\\SPLab9\\ResultFile.jpg", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (File == INVALID_HANDLE_VALUE)
	{
		cout << "Error(3) " << GetLastError() << endl;
		system("pause");
		return 0;
	}

	int a = hSize / 2;

	for (int i = 0; i < a; i += 32)
	{
		CopyMemory((hAll + 2 * i), (SecondFile + i), 32);
		CopyMemory((hAll + 2 * i + 32), (FirstFile + i), 32);
	}

	DWORD buffout;
	if (!WriteFile(File, hAll, hSize, &buffout, NULL)) {
		cout << "������ ������!" << endl;
		system("pause");
		return 0;
	}


	CloseHandle(File);
	UnmapViewOfFile(SecondFile);
	UnmapViewOfFile(FirstFile);
	VirtualFree(hAll, hSize, MEM_RELEASE);
	CloseHandle(secondMapFile);
	CloseHandle(firstMapFile);
	CloseHandle(secondFile);
	CloseHandle(firstFile);
	system("pause");
	return 0;
}
