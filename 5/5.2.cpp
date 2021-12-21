#define UNICODE
#include "windows.h"
#include <iostream>

using namespace std;

int main() {
	setlocale(0, "");
	TCHAR str[MAX_PATH]; // Массив для хранения текущего каталога файла

	
	
	if (!GetCurrentDirectory(sizeof(str), str)) // Получение текущего каталога 
		cout << "Ошибка" << endl;
	else
		wcout << str << endl;

	

if (!CreateDirectory(L"D:\\Temp\\", // Название каталога
		NULL // Атрибуи=ты безопасности
	))
		cout << "Ошибка создания каталога." << endl;
	else
		cout << "Временный каталог создан." << endl;

SetCurrentDirectory(TEXT("D:\\"));	// Меняем каталог
GetCurrentDirectory(sizeof(str), str);

	

	const TCHAR* name = L"test.info";
	const TCHAR* Name = L"D:\\Temp\\test.info";

	
	
	
	if (!CopyFile(name,  // Имя копируемого файла
				  Name,			 // Имя файла, в который будет скопирован существующий файл
				  FALSE			 // Действие в случае существования файла FALSE - перезаписать
	))
		cout << "Ошибка копирования файла." << endl;
	else
		cout << "Копирование произошло." << endl;
	

	HANDLE hFile = CreateFile(name, // Имя файла
		GENERIC_READ,				// Режим доступа (только для чтения)
		0,							// Режимы совместного использования
		NULL,						// Параметры безопастности
		OPEN_EXISTING,				// Откроет существующий файл, если файл с указанным именем не существует, функция завершится ошибкой
		FILE_ATTRIBUTE_NORMAL,		// Атрибуты файла (обычный файл, не имеющий других атрибутов)
		NULL						// Атрибуты файлы
	);

	if (hFile == NULL)
		cout << "Ошибка открытия файла." << endl;
	else
		cout << "Файл открыт." << endl;

	if (!SetFilePointer(hFile,		// Открытие файлового дескриптора
		271014,					    // 32-разрядное значение низкого порядка со знаком, определяющее количество байтов для перемещения указателя файла.
		0,							// Самая высокая 32-битная часть подписанного числа, указывающая расстояние перехода внутри файла
		FILE_BEGIN					// Начальная точка для перемещения указателя файла (начало)
	))
		cout << "Ошибка установки позиции." << endl;
	else
		cout << "Необходимая позиция установлена" << endl;

	TCHAR buffer[63];
	DWORD n, k;

	if (!ReadFile(hFile,			// Дескриптор
		buffer,						// Указатель на буфер, который принимает данные, считанные из файла или устройства.
		sizeof(buffer),				// Максимальное число считываемых байтов.
		&n,							// Указатель на переменную, которая получает количество байтов, считываемых при использовании синхронного параметра hFile
		NULL						// Используется для асинхронной записи
	))
		cout << "Ошибка чтения данных из файла." << endl;
	else
		cout << "Чтение произошло." << endl;

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
		cout << "Ошибка создания файла." << endl;
	else
		cout << "Файл создан." << endl;

	if (!WriteFile(hFile2,				 //file descriptor
		buffer,							 //pointer to data buffer
		n,								 //number of bytes to write
		&k,								 //number of bytes to write
		NULL							 //used for asynchronous recording
	))
		cout << "Ошибка записи данных в файл" << endl;
	else
		cout << "Запись произошла" << endl;

	system("pause");
	return 0;
}
