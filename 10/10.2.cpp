#include<iostream>
#include<windows.h>

using namespace std;

int main() {
	HMODULE hMyDll;
	void(*Say)(char*);
	char* word;

	hMyDll = LoadLibrary(L"lab10z1"); // load ddl (name dll); handle/false
	if (hMyDll == NULL) {
		wcerr << L"Error loading library!" << endl;
		system("pause");
		return 3;
	}

	word = (char*)GetProcAddress(hMyDll, "Message");  // get address dll (handle dll, name function/function number); address/false
	if (!word) {
		wcerr << L"Error loading char array!" << endl;
		system("pause");
		return 4;
	}

	Say = (void(*)(char*))GetProcAddress(hMyDll, "SayMessage");	// get address dll (handle dll, name function/function number);
	if (!Say) {
		wcerr << L"Error loading function!" << endl;
		system("pause");
		return 5;
	}

	Say(word);
	FreeLibrary(hMyDll);	// free dll (handle dll)

	system("pause");
	return 0;
}