#include <windows.h>
#include <iostream>
#pragma comment(lib, "WS2_32.lib")

using namespace std;

#define SERVER_PORT 1112
#define UDP_SERVER_PORT 1111

char point[3];
char buff;

SOCKET clientSocket;

char error[] = { "Error" };

int errorFunc() {
	send(clientSocket, error, sizeof(error), 0);
	cout << "Error: " << WSAGetLastError() << endl;
	system("pause");
	return 0;
}

int receive(SOCKET clientSocket) {
	buff = '1';
	while (buff != NULL) {
		if (recv(clientSocket, &buff, sizeof(buff), 0) == SOCKET_ERROR)
			return errorFunc();
		if (atoi(&buff) == 5) {
			cout << "Ты выиграл." << endl;
			system("pause");
			return 0;
		}
		if (atoi(&buff) == 4) {
			cout << "Ты проиграл." << endl;
			system("pause");
			return 0;
		}
		if (atoi(&buff) == 8) {
			cout << "Ничья." << endl;
			system("pause");
			return 0;
		}
		if (atoi(&buff) == 9) {
			cout << "Неправильный ход, попробуй ещё раз:" << endl;
			cin >> point;
			if (send(clientSocket, point, sizeof(point), 0) == SOCKET_ERROR)
				return errorFunc();
			receive(clientSocket);
		}
		else cout << buff;
	}
	return 1;
}

int main() {
	SetConsoleOutputCP(1251);

	WSADATA wsadata;
	int res = WSAStartup(MAKEWORD(2, 2), &wsadata);

	char ca[2];

	char nick[20] = { NULL };

	cout << "Добро пожаловать в игру крестики-нолики." << endl;

	SOCKET serverUdpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serverUdpSocket == INVALID_SOCKET)
		return errorFunc();

	int one = 1;
	if (setsockopt(serverUdpSocket, SOL_SOCKET, SO_BROADCAST, (char*)&one, sizeof(one)) < 0)
		return errorFunc();

	sockaddr_in UdpSa;
	UdpSa.sin_family = AF_INET;
	UdpSa.sin_addr.S_un.S_addr= inet_addr("255.255.255.255");
	//UdpSa.sin_addr.S_un.S_addr = htonl(INADDR_BROADCAST); //    inet_addr("10.1.11.122");
	UdpSa.sin_port = htons(UDP_SERVER_PORT);

	if (sendto(serverUdpSocket, " ", 2, 0, (SOCKADDR*)&UdpSa, sizeof(UdpSa)) == SOCKET_ERROR)
		return errorFunc();
	int a = sizeof(UdpSa);
	if (recvfrom(serverUdpSocket, ca, sizeof(ca), 0, (SOCKADDR*)&UdpSa, &a) == SOCKET_ERROR)
		return errorFunc();

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
		return errorFunc();
	closesocket(serverUdpSocket);
	sockaddr_in sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.S_un.S_addr = UdpSa.sin_addr.S_un.S_addr;
	sa.sin_port = htons(SERVER_PORT);

	int connection_status = connect(clientSocket, (SOCKADDR*)&sa, sizeof(sa));
	if (connection_status == SOCKET_ERROR) 
		return errorFunc();
	else 
		cout << "Соединение установлено" << endl;
	cout << "Вы подключились к " << inet_ntoa(sa.sin_addr) << endl;
	cout << "Введите ваш ход (11-33): " << endl;
	char S;

	if (send(clientSocket, nick, sizeof(nick), 0) == SOCKET_ERROR)
		return errorFunc();

	if (recv(clientSocket, nick, sizeof(nick), 0) == SOCKET_ERROR)
		return errorFunc();

	if (recv(clientSocket, &S, sizeof(S), 0) == SOCKET_ERROR)
		return errorFunc();

	if (atoi(&S) == 1) {
		while (true) {
			if (receive(clientSocket) == 0) return 0;

			cout << "Ваш ход: " << endl;
			cin >> point;

			if (send(clientSocket, point, sizeof(point), 0) == SOCKET_ERROR)
				return errorFunc();

			receive(clientSocket);

			cout << endl;
			cout << "Ход противника. " << endl;
		}
	}
	else if (atoi(&S) == 2) {
		while (true) {
			receive(clientSocket);
			cout << "Ход противника. " << endl;

			receive(clientSocket);
			cout << "Ваш ход: " << endl;
			cin >> point;

			if (send(clientSocket, point, sizeof(point), 0) == SOCKET_ERROR)
				return errorFunc();
		}
	}


	system("pause");
	return 0;
}
