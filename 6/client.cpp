#include <windows.h>
#include <iostream>
#pragma comment(lib, "WS2_32.lib")

using namespace std;

#define SERVER_PORT 1111

char point[3];
char result;

int getMessage(SOCKET clientSocket) {
	result = '1';
	while (result != NULL) {
		if (recv(clientSocket, &result, sizeof(result), 0) == SOCKET_ERROR) {    //получаем код результата
			cerr << "Приём не выполнен. Ошибка: " << WSAGetLastError() << endl;
			cin.get();
			return 0;
		}
		if (atoi(&result) == 5) {
			cout << "Вы выиграли." << endl;
			system("pause");
			return 0;
		}
		if (atoi(&result) == 4) {
			cout << "Вы проиграли." << endl;
			system("pause");
			return 0;
		}
		if (atoi(&result) == 8) {
			cout << "Ничья." << endl;
			system("pause");
			return 0;
		}
		if (atoi(&result) == 9) {
			cout << "Неправильный ход, попробуйте ещё раз:" << endl;
			cin >> point;
			if (send(clientSocket, point, sizeof(point), 0) == SOCKET_ERROR) {
				cerr << "Отправка сообщения не выполнена. ERROR: " << WSAGetLastError() << endl;
				cin.get();
				return 0;
			}
			getMessage(clientSocket);
		}
		else cout << result;
	}
	return 1;
}

int main() {
	setlocale(0, "");
	char nickName[20] = { NULL };
	cout << "Добро пожаловать в игру крестики-нолики." << endl;
	cout << "Введите IP сервера: " << endl;
	char addr[20];
	cin.getline(addr, sizeof(addr));   //вводим IP
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);   //указываем версию библиотеки winsock
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		cout << "Сокет не был создан. Ошибка: " << WSAGetLastError() << endl;
		cin.get();
		return 0;
	}
	sockaddr_in sa;
	ZeroMemory(&sa, sizeof(sa)); // очищаем sa
	sa.sin_family = AF_INET;
	sa.sin_addr.S_un.S_addr = inet_addr(addr);
	sa.sin_port = htons(SERVER_PORT);

	int connection_status = connect(clientSocket, (SOCKADDR*)&sa, sizeof(sa)); //установка соединения
	if (connection_status == SOCKET_ERROR) {
		cerr << "Соединение не установлено. Ошибка: " << WSAGetLastError() << endl;
		cin.get();
		return 0;
	}
	else cout << "Соединение установлено. " << endl;

	cout << "Введите ваш ход (11-33): " << endl;
	char S;

	if (send(clientSocket, nickName, sizeof(nickName), 0) == SOCKET_ERROR) {  //устанавливаем связь с сервером
		cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
		cin.get();
		return 0;
	}

	if (recv(clientSocket, nickName, sizeof(nickName), 0) == SOCKET_ERROR) {
		cerr << "Приём не выполнен. Ошибка: " << WSAGetLastError() << endl;
		cin.get();
		return 0;
	}

	if (recv(clientSocket, &S, sizeof(S), 0) == SOCKET_ERROR) {
		cerr << "Приём не выполнен. Ошибка: " << WSAGetLastError() << endl;
		cin.get();
		return 0;
	}

	if (atoi(&S) == 1) {
		while (true) {
			if (getMessage(clientSocket) == 0)
				return 0;

			cout << "Ваш ход. " << endl;
			cin >> point;

			if (send(clientSocket, point, sizeof(point), 0) == SOCKET_ERROR) {
				cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
				cin.get();
				return 0;
			}

			if (getMessage(clientSocket) == 0)
				return 0;

			cout << endl;
			cout << "Ход противника. " << endl;
		}
	}
	else if (atoi(&S) == 2) {
		while (true) {
			getMessage(clientSocket);
			cout << "Ход противника. " << endl;

			if (getMessage(clientSocket) == 0)
				return 0;
			cout << "Ваш ход. " << endl;
			cin >> point;

			if (send(clientSocket, point, sizeof(point), 0) == SOCKET_ERROR) {
				cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
				cin.get();
				return 0;
			}

		}

	}


	system("pause");
	return 0;
}