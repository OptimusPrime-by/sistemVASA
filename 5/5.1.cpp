#define UNICODE
#include <windows.h>
#include <iostream>

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	int size = 3;
	TCHAR *buffer = new TCHAR[size]; // ������ ��� �������� ����

	int temp = GetCurrentDirectory(
		size,		// ��������� ������� ������ ��� ���������
		buffer		// ��������� �� ����� ������� �������� ������� ������ ��������
	);
	// ���� ������� ����������� �������, ������������ �������� ��������� ���������� ��������, ������� ������������ � �����, �� ������� ����������� ������� ������.
	// ���� �����, �� ������� ��������� lpBuffer, ������������ �����, ������������ �������� ��������� ��������� ������ ������ � ��������, ������� ����-����������� ������.  (MSDN)
	// ������� ���������� � temp ����������


	if (size >= temp) { // ���� ������ ������ ���������� �����
		wcout << buffer << endl;
		SetCurrentDirectory(TEXT("D:\\Temp"));	// ������ �������
		GetCurrentDirectory(	// ���������� ���� � �������� ��������
			size,
			buffer
		);
		wcout << buffer << endl;
	}
	else {
		delete[] buffer;
		buffer = new TCHAR[temp];	// ���������� ����� ���� � ������ ������ (������� ������ ������� GetCurrentDirectory)
		GetCurrentDirectory(
			temp,
			buffer
		);
		wcout << buffer << endl;	// ������ ������ �������

		SetCurrentDirectory(TEXT("D:\\Temp"));  //������ �������
		GetCurrentDirectory(
			temp,
			buffer
		);
		wcout << buffer << endl;	// ������ ��������� ������� 
	}

	system("pause");
	return 0;
}