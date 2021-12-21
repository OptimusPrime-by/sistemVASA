#define UNICODE
#include <windows.h>
#include <iostream>

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	int size = 3;
	TCHAR *buffer = new TCHAR[size]; // Массив для хранения пути

	int temp = GetCurrentDirectory(
		size,		// Указатель размера буфера для занесения
		buffer		// Указатель на буфер который получает текущую строку каталога
	);
	// Если функция завершается успешно, возвращаемое значение указывает количество символов, которые записываются в буфер, не включая завершающий нулевой символ.
	// Если буфер, на который указывает lpBuffer, недостаточно велик, возвращаемое значение указывает требуемый размер буфера в символах, включая нуль-завершающий символ.  (MSDN)
	// Поэтому записываем в temp переменную


	if (size >= temp) { // Если размер буфера достаточно велик
		wcout << buffer << endl;
		SetCurrentDirectory(TEXT("D:\\Temp"));	// Меняем каталог
		GetCurrentDirectory(	// Возвращаем путь к текущему каталогу
			size,
			buffer
		);
		wcout << buffer << endl;
	}
	else {
		delete[] buffer;
		buffer = new TCHAR[temp];	// Пересоздаём буфер пути с нужной длиной (вернули первым вызовом GetCurrentDirectory)
		GetCurrentDirectory(
			temp,
			buffer
		);
		wcout << buffer << endl;	// Вывели первый каталог

		SetCurrentDirectory(TEXT("D:\\Temp"));  //меняем каталог
		GetCurrentDirectory(
			temp,
			buffer
		);
		wcout << buffer << endl;	// Вывели изменённый каталог 
	}

	system("pause");
	return 0;
}