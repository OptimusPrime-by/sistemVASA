#define UNICODE
#include <windows.h>
#include <iostream>

using namespace std;
int main() {
	//OVERLAPPED overlapped = { sizeof(overlapped) };
	//overlapped.Offset = 0;
	//overlapped.OffsetHigh = 0;
	//overlapped.hEvent = NULL;
	OVERLAPPED overlapped;
	//overlapped.hEvent = hEvent;
	overlapped.Offset = 271014;
	overlapped.OffsetHigh = 0;
	setlocale(LC_ALL, "Russian");
	int const nby=63;
	TCHAR cBuffer[nby];
	DWORD BytesRead,k;
	HANDLE hOut1, hOut2;

	if (
		!CreateDirectory(
			L"D:\\Temp\\",	//���� ������������ ��������.
			NULL			//���� lpSecurityAttributes �������� NULL , ������� �������� ���������� ������������ �� ���������. (MSDN)
		)
		) {
		cout << "�� ������� ������� �������" << endl;
		system("pause");
		return 0;
	}
	else cout << "������� ������" << endl;

	if (
		!CopyFile(
			L"D:\\test.info",			// ��� ������������� �����.
			L"D:\\Temp\\test.info",		// ��� ������ �����.
			false
		)
		) {
		cout << "�� ������� ����������� ���� �� ��������� �������" << endl;
		system("pause");
		return 0;
	}
	else cout << "���� ����������!" << endl;

	overlapped.hEvent = CreateEvent( // ������� ������ �������   hEvent-���������� �������,������� ������ ���� ����������� � ���������� ���������, ����� �������� �����������
		NULL,	// ���� ���� �������� ����� NULL, ���������� �� ����� ���� ����������� ��������� ����������.
		TRUE,	// ���� ������ , ������� ������� ������ ������� � ������ �������, ������� ������� ������������� ������� ResetEvent, ����� ���������� ��������� ������� � ���������������(nonsignaled). 
		false,	// ���� ������ , ��������������� ��������� ��������� ������� �������; ����� �� �� ����� ��������.
		NULL	// ��� ������� �������. ��� ���������� ��������� MAX_PATH . ��������� ���� ������������� � ��������. NULL - no name.
	);



	hOut1 = CreateFile(
		L"D:\\Temp\\test.info",			//��� ����� ��� ����������, ������� ����� ������� ��� �������. �� ������ ������������ ���� ����� ����� (/), ���� �������� ����� ����� (\) � ���� �����.
		GENERIC_READ,					//����������� ������ � ����� ��� ����������, ������� ����� ����������� ��� ������, ������, ��� ��� �� ������
		0,								//����������� ����� ����������� ������� � ����� ��� ����������
		NULL,							//�������������� ���������� ������������ � ���������� ��������, ������������, ����� �� ������������ ���������� ������������� ��������� ����������.
		OPEN_EXISTING,					//�������� �������. 
		FILE_FLAG_OVERLAPPED,			//�������� � ����� ����� ��� ����������. ����� �������������
		0								//���������� ���������� ����� ������� � ������ ������� GENERIC_READ . ��� �������� ������������� ����� CreateFile ���������� ���� ��������.
	);

	if (hOut1 == NULL) {
		cout << "�� ������� ������� ����" << endl;
		system("pause");
		return 0;
	}
	else cout << "���� ������ ��� ������" << endl;


	

	//overlapped.Offset = 10; �������������� ����� � ������� ���������� ��������

	ReadFile(
		hOut1,			//Handle � ����������
		cBuffer,		//��������� �� �����, ������� ��������� ������, ��������� �� ����� ��� ����������.
		sizeof(cBuffer),//������������ ���������� ������ ��� ������.
		&BytesRead,		//��������� �� ����������, ������� �������� ���������� ������, ��������� ��� �������������  ��������� hOut1 .
		&overlapped		//��������� �� ��������� OVERLAPPED ���������, ���� �������� hOut1 ��� ������ � ������� FILE_FLAG_OVERLAPPED , ����� �� ����� ���� NULL .
	);

	// ������������� ���������� ��������� �� ��� ��� ���� ������� �� �������� � ���������� ���������
	//while (true) {//WAIT_OBJECT_0 ��� �������� ������� � ���������� ���������
	if (GetOverlappedResult(hOut1, &overlapped, &BytesRead, TRUE)) {
		//	break;                       //GetOverlappedResult ��������� ���������� ���������� �������� � �������� ������(=0 ����� ����������� �������)
	//}
		if (BytesRead == sizeof(cBuffer))
		{
			cout << "������ ������!" << endl;
		}
		else {
			cout << "������ ������!" << endl;
		}
	}

	hOut2 = CreateFile(
		L"D:\\Temp\\answer.txt",
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hOut2 == 0) {
		cout << "�� ������� ������� ���� answer.txt" << endl;
		system("pause");
		return 0;
	}
	else cout << "���� answer.txt ������" << endl;

	/*
	DWORD Offset = SetFilePointer(	//���������� ��������� ����� � ��������� ����.
		hOut2,				//����� � �����.
		0,			
		0,					//A pointer to the high order 32-bits of the signed 64-bit distance to move. If you do not need - NULL
		FILE_BEGIN			//
	);
	*/
	if (
		!WriteFile( //������ ������
			hOut2,
			cBuffer,
			BytesRead,
			&k,
			NULL
		)) {
		cout << "�� ������� �������� � ���� answer.txt" << endl;
		system("pause");
		return 0;
	}
	else cout << "������ ������������" << endl;
	/*while (true) {//WAIT_OBJECT_0 ��� �������� ������� � ���������� ���������
		if (GetOverlappedResult(hOut2, &overlapped, &BytesRead, TRUE) != 0)
			break;                       //GetOverlappedResult ��������� ���������� ���������� �������� � �������� ������(=0 ����� ����������� �������)
	}*/
	CloseHandle(hOut1);		//�������� ������
	CloseHandle(hOut2);
	system("pause");
	return 0;
}