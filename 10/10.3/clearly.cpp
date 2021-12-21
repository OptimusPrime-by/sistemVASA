#include<iostream>
#include<windows.h>
using namespace std;

int main()
{
	typedef void(*Function)();
	// Дескриптор загружаемой dll
	HMODULE hMyDll = LoadLibrary("SPLab10_3");
	if (hMyDll == NULL)
	{
		cout << "Failed To Load Libruary" << endl;
		system("pause");
		return 0;
	}

	/*
	Вызовом GetProcAddress получаем адрес функции function и присваиваем его указателю DllFunc с явным приведением типов. 
	Это необходимо т.к. GetProcAddress возвращает бестиповой far-указатель
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
Явная компоновка устраняет все эти недостатки - ценой некоторого усложнения кода. 
Программисту самому придется позаботиться о загрузке DLL и подключении экспортируемых функций
(не забывая при этом о контроле над ошибками, иначе в один прекрасный момент дело кончится зависанием системы).
Зато явная компоновка позволяет подгружать DLL по мере необходимости и дает программисту возможность самостоятельно обрабатывать ситуации с отсутствием DLL.
Можно пойти и дальше - не задавать имя DLL в программе явно, а сканировать такой-то каталог на предмет наличия динамических библиотек и подключать все найденные к приложению.
*/