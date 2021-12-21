#include<iostream>
#include<windows.h>
using namespace std;

int main()
{
	typedef void(*Function)();
	// ���������� ����������� dll
	HMODULE hMyDll = LoadLibrary("SPLab10_3");
	if (hMyDll == NULL)
	{
		cout << "Failed To Load Libruary" << endl;
		system("pause");
		return 0;
	}

	/*
	������� GetProcAddress �������� ����� ������� function � ����������� ��� ��������� DllFunc � ����� ����������� �����. 
	��� ���������� �.�. GetProcAddress ���������� ���������� far-���������
	*/
	Function func = (Function)GetProcAddress(hMyDll, "function");
	if (hMyDll == NULL) {
		cout << "Error" << endl;
		system("pause");
		return 0;
	}

	else
		func();

	FreeLibrary(hMyDll);

	system("pause");
	return 0;
}

/*
����� ���������� ��������� ��� ��� ���������� - ����� ���������� ���������� ����. 
������������ ������ �������� ������������ � �������� DLL � ����������� �������������� �������
(�� ������� ��� ���� � �������� ��� ��������, ����� � ���� ���������� ������ ���� �������� ���������� �������).
���� ����� ���������� ��������� ���������� DLL �� ���� ������������� � ���� ������������ ����������� �������������� ������������ �������� � ����������� DLL.
����� ����� � ������ - �� �������� ��� DLL � ��������� ����, � ����������� �����-�� ������� �� ������� ������� ������������ ��������� � ���������� ��� ��������� � ����������.
*/