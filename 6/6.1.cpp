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
			L"D:\\Temp\\",	//Путь создаваемого каталога.
			NULL			//Если lpSecurityAttributes является NULL , каталог получает дескриптор безопасности по умолчанию. (MSDN)
		)
		) {
		cout << "Не удалось создать каталог" << endl;
		system("pause");
		return 0;
	}
	else cout << "Каталог создан" << endl;

	if (
		!CopyFile(
			L"D:\\test.info",			// Имя существующего файла.
			L"D:\\Temp\\test.info",		// Имя нового файла.
			false
		)
		) {
		cout << "Не удалось скопировать файл во временный каталог" << endl;
		system("pause");
		return 0;
	}
	else cout << "Файл скопирован!" << endl;

	overlapped.hEvent = CreateEvent( // Создаем объект события   hEvent-дискриптор события,которое должно быть установлено в сигнальное состояние, когда операция завершилась
		NULL,	// Если этот параметр равен NULL, дескриптор не может быть унаследован дочерними процессами.
		TRUE,	// Если ИСТИНА , функция создает объект события с ручным сбросом, который требует использования функции ResetEvent, чтобы установить состояние события в несогласованное(nonsignaled). 
		false,	// Если ИСТИНА , сигнализируется начальное состояние объекта события; иначе он не имеет значения.
		NULL	// Имя объекта события. Имя ограничено символами MAX_PATH . Сравнение имен чувствительно к регистру. NULL - no name.
	);



	hOut1 = CreateFile(
		L"D:\\Temp\\test.info",			//Имя файла или устройства, которое будет создано или открыто. Вы можете использовать либо косые черты (/), либо обратную косую черту (\) в этом имени.
		GENERIC_READ,					//Запрошенный доступ к файлу или устройству, который можно суммировать как чтение, запись, оба или ни одного
		0,								//Запрошенный режим совместного доступа к файлу или устройству
		NULL,							//Необязательный дескриптор безопасности и логическое значение, определяющее, может ли возвращенный дескриптор наследоваться дочерними процессами.
		OPEN_EXISTING,					//Действие функции. 
		FILE_FLAG_OVERLAPPED,			//Атрибуты и флаги файла или устройства. Можно комбинировать
		0								//Правильный дескриптор файла шаблона с правом доступа GENERIC_READ . При открытии существующего файла CreateFile игнорирует этот параметр.
	);

	if (hOut1 == NULL) {
		cout << "Не удалось открыть файл" << endl;
		system("pause");
		return 0;
	}
	else cout << "Файл открыт для чтения" << endl;


	

	//overlapped.Offset = 10; местоположение файла в котором начинается передача

	ReadFile(
		hOut1,			//Handle к устройству
		cBuffer,		//Указатель на буфер, который принимает данные, считанные из файла или устройства.
		sizeof(cBuffer),//Максимальное количество байтов для чтения.
		&BytesRead,		//Указатель на переменную, которая получает количество байтов, считанных при использовании  параметра hOut1 .
		&overlapped		//Указатель на структуру OVERLAPPED требуется, если параметр hOut1 был открыт с помощью FILE_FLAG_OVERLAPPED , иначе он может быть NULL .
	);

	// Останавливает выполнение программы до тех пор пока объекты не окажутся в сигнальном состоянии
	//while (true) {//WAIT_OBJECT_0 при переходе обьекта в сигнальное состояние
	if (GetOverlappedResult(hOut1, &overlapped, &BytesRead, TRUE)) {
		//	break;                       //GetOverlappedResult извлекает результаты асинхроной операции с указаным файлом(=0 когда завершается ошибкой)
	//}
		if (BytesRead == sizeof(cBuffer))
		{
			cout << "Чтение удачно!" << endl;
		}
		else {
			cout << "Ошибка чтения!" << endl;
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
		cout << "Не удалось создать файл answer.txt" << endl;
		system("pause");
		return 0;
	}
	else cout << "Файл answer.txt создан" << endl;

	/*
	DWORD Offset = SetFilePointer(	//Перемещает указатель файла в указанный файл.
		hOut2,				//Хэндл к файлу.
		0,			
		0,					//A pointer to the high order 32-bits of the signed 64-bit distance to move. If you do not need - NULL
		FILE_BEGIN			//
	);
	*/
	if (
		!WriteFile( //запись данных
			hOut2,
			cBuffer,
			BytesRead,
			&k,
			NULL
		)) {
		cout << "Не удалось записать в файл answer.txt" << endl;
		system("pause");
		return 0;
	}
	else cout << "Запись осуществлена" << endl;
	/*while (true) {//WAIT_OBJECT_0 при переходе обьекта в сигнальное состояние
		if (GetOverlappedResult(hOut2, &overlapped, &BytesRead, TRUE) != 0)
			break;                       //GetOverlappedResult извлекает результаты асинхроной операции с указаным файлом(=0 когда завершается ошибкой)
	}*/
	CloseHandle(hOut1);		//закрыйть хендлы
	CloseHandle(hOut2);
	system("pause");
	return 0;
}