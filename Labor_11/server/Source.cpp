#include <windows.h>
#include <iostream>
#pragma comment(lib, "WS2_32.lib")

using namespace std;

#define SERVER_PORT 1112
#define UDP_SERVER_PORT 1111
#define CLIENT 0
#define SERVER 1

int moves = 0;

char error[] = { "Error" };

SOCKET FirstClientSocket;
SOCKET SecondClientSocket;

int errorFunc() {
	send(FirstClientSocket, error, sizeof(error), 0);
	send(SecondClientSocket, error, sizeof(error), 0);
	cout << "Error: " << WSAGetLastError() << endl;
	system("pause");
	return 0;
}

char whowin[34];
char musor[20];
char fSoc[20];
char sSoc[20];

char field[] = {
		' ', ' ', '1', ' ', '2', ' ', '3', '\n',
	'1', ' ', '.', ' ', '.', ' ', '.', '\n',
	'2', ' ', '.', ' ', '.', ' ', '.', '\n',
	'3', ' ', '.', ' ', '.', ' ', '.', ' ', '\n', '\0' };
int numField[3][3] = {
{ 0, 0, 0 },
{ 0, 0, 0 },
{ 0, 0, 0 }
};

int fillField(int a, int player) {
	if (a == 11 || a == 12 || a == 13 ||
		a == 21 || a == 22 || a == 23 ||
		a == 31 || a == 32 || a == 33) {
		switch (a) {
		case 11:
			switch (player) {
			case CLIENT:
				if (numField[0][0] == 0) {
					field[10] = 'O';
					numField[0][0] = 1;
				}
				else return 9;
				break;
			case SERVER:
				if (numField[0][0] == 0) {
					field[10] = 'X';
					numField[0][0] = 2;
				}
				else return 9;
				break;
			default:
				field[10] = '-';
				break;
			}
			moves++;
			break;
		case 12:
			switch (player) {
			case CLIENT:
				if (numField[0][1] == 0) {
					field[12] = 'O';
					numField[0][1] = 1;
				}
				else return 9;
				break;
			case SERVER:
				if (numField[0][1] == 0) {
					field[12] = 'X';
					numField[0][1] = 2;
				}
				else return 9;
				break;
			default:
				field[12] = '-';
				break;
			}
			moves++;
			break;
		case 13:
			switch (player) {
			case CLIENT:
				if (numField[0][2] == 0) {
					field[14] = 'O';
					numField[0][2] = 1;
				}
				else return 9;
				break;
			case SERVER:
				if (numField[0][2] == 0) {
					field[14] = 'X';
					numField[0][2] = 2;
				}
				else return 9;
				break;
			default:
				field[14] = '-';
				break;
			}
			moves++;
			break;
		case 21:
			switch (player) {
			case CLIENT:
				if (numField[1][0] == 0) {
					field[18] = 'O';
					numField[1][0] = 1;
				}
				else return 9;
				break;
			case SERVER:
				if (numField[1][0] == 0) {
					field[18] = 'X';
					numField[1][0] = 2;
				}
				else return 9;
				break;
			default:
				field[18] = '-';
				break;
			}
			moves++;
			break;
		case 22:
			switch (player) {
			case CLIENT:
				if (numField[1][1] == 0) {
					field[20] = 'O';
					numField[1][1] = 1;
				}
				else return 9;
				break;
			case SERVER:
				if (numField[1][1] == 0) {
					field[20] = 'X';
					numField[1][1] = 2;
				}
				else return 9;
				break;
			default:
				field[20] = '-';
				break;
			}
			moves++;
			break;
		case 23:
			switch (player) {
			case CLIENT:
				if (numField[1][2] == 0) {
					field[22] = 'O';
					numField[1][2] = 1;
				}
				else return 9;
				break;
			case SERVER:
				if (numField[1][2] == 0) {
					field[22] = 'X';
					numField[1][2] = 2;
				}
				else return 9;
				break;
			default:
				field[22] = '-';
				break;
			}
			moves++;
			break;
		case 31:
			switch (player) {
			case CLIENT:
				if (numField[2][0] == 0) {
					field[26] = 'O';
					numField[2][0] = 1;
				}
				else return 9;
				break;
			case SERVER:
				if (numField[2][0] == 0) {
					field[26] = 'X';
					numField[2][0] = 2;
				}
				else return 9;
				break;
			default:
				field[26] = '-';
				break;
			}
			moves++;
			break;
		case 32:
			switch (player) {
			case CLIENT:
				if (numField[2][1] == 0) {
					field[28] = 'O';
					numField[2][1] = 1;
				}
				else return 9;
				break;
			case SERVER:
				if (numField[2][1] == 0) {
					field[28] = 'X';
					numField[2][1] = 2;
				}
				else return 9;
				break;
			default:
				field[28] = '-';
				break;
			}
			moves++;
			break;
		case 33:
			switch (player) {
			case CLIENT:
				if (numField[2][2] == 0) {
					field[30] = 'O';
					numField[2][2] = 1;
				}
				else return 9;
				break;
			case SERVER:
				if (numField[2][2] == 0) {
					field[30] = 'X';
					numField[2][2] = 2;
				}
				else return 9;
				break;
			default:
				field[30] = '-';
				break;
			}
			moves++;
			break;
		default:
			break;
		}
	}
	else return 9;
	if (moves == 9) return 8;
	else return 0;
}

bool isFinish() {
	for (int i = 0; i < 3; i++) {
		if (numField[i][1] == numField[i][0] && numField[i][1] == numField[i][2]) {
			if (numField[i][1] == 1) {
				whowin[0] = '5'; // 5 - код победы игрока
				return true;
			}
			if (numField[i][1] == 2) {
				whowin[0] = '4'; // 4 - код победы сервера
				return true;
			}
		}
	}
	for (int j = 0; j < 3; j++) {
		if (numField[1][j] == numField[0][j] && numField[1][j] == numField[2][j]) {
			if (numField[1][j] == 1) {
				whowin[0] = '5'; // 5 - код победы игрока
				return true;
			}
			if (numField[1][j] == 2) {
				whowin[0] = '4'; // 4 - код победы сервера
				return true;
			}
		}
	}
	if (numField[1][1] == numField[0][0] && numField[1][1] == numField[2][2]) {
		if (numField[1][1] == 1) {
			whowin[0] = '5'; // 5 - код победы игрока
			return true;
		}
		if (numField[1][1] == 2) {
			whowin[0] = '4'; // 4 - код победы сервера
			return true;
		}
	}
	if (numField[1][1] == numField[0][2] && numField[1][1] == numField[2][0]) {
		if (numField[1][1] == 1) {
			whowin[0] = '5'; // 5 - код победы игрока
			return true;
		}
		if (numField[1][1] == 2) {
			whowin[0] = '4'; // 4 - код победы сервера
			return true;
		}
	}

	return 0;
}


int main() {
	SetConsoleOutputCP(1251);

	WSADATA wsadata;
	int res = WSAStartup(MAKEWORD(2, 2), &wsadata);

	int sockAddrLen = sizeof(sockaddr_in);

	SOCKET serverUdpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serverUdpSocket == INVALID_SOCKET)
		return errorFunc();

	sockaddr_in UdpSa;
	UdpSa.sin_family = AF_INET;
	UdpSa.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	UdpSa.sin_port = htons(UDP_SERVER_PORT);
	if (bind(serverUdpSocket, (SOCKADDR*)&UdpSa, sizeof(UdpSa)) == SOCKET_ERROR)
		return errorFunc();

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
		return errorFunc();

	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(SERVER_PORT);
	if (bind(serverSocket, (SOCKADDR*)&sa, sizeof(sa)) == SOCKET_ERROR)
		return errorFunc();

	if (listen(serverSocket, 2) == SOCKET_ERROR)
		return errorFunc();

	sockaddr_in sAddr;
	if (recvfrom(serverUdpSocket, fSoc, sizeof(fSoc), 0, (SOCKADDR*)&sAddr, &sockAddrLen) == SOCKET_ERROR)
		return errorFunc();
	if (sendto(serverUdpSocket, " ", 2, 0, (SOCKADDR*)&sAddr, sockAddrLen) == SOCKET_ERROR)
		return errorFunc();

	sockaddr_in saClient;
	cout << "ќжидание клиентов" << endl;
	SOCKET FirstClientSocket = accept(serverSocket, (SOCKADDR*)&saClient, &sockAddrLen);
	if (FirstClientSocket == INVALID_SOCKET)
		return errorFunc();
	else cout << " лиент подключен" << endl;

	if (recvfrom(serverUdpSocket, fSoc, sizeof(fSoc), 0, (SOCKADDR*)&sAddr, &sockAddrLen) == SOCKET_ERROR)
		return errorFunc();
	if (sendto(serverUdpSocket, " ", 2, 0, (SOCKADDR*)&sAddr, sockAddrLen) == SOCKET_ERROR)
		return errorFunc();

	closesocket(serverUdpSocket);
	SOCKET SecondClientSocket = accept(serverSocket, (SOCKADDR*)&saClient, &sockAddrLen);
	if (SecondClientSocket == INVALID_SOCKET)
		return errorFunc();
	else cout << " лиент подключен" << endl;

	if (recv(FirstClientSocket, fSoc, sizeof(fSoc), 0) == SOCKET_ERROR)
		return errorFunc();
	if (recv(SecondClientSocket, sSoc, sizeof(sSoc), 0) == SOCKET_ERROR)
		return errorFunc();

	if (send(FirstClientSocket, sSoc, sizeof(sSoc), 0) == SOCKET_ERROR)
		return errorFunc();
	if (send(SecondClientSocket, fSoc, sizeof(fSoc), 0) == SOCKET_ERROR)
		return errorFunc();

	char buff = '1';
	if (send(FirstClientSocket, &buff, sizeof(buff), 0) == SOCKET_ERROR)
		return errorFunc();
	buff = '2';
	if (send(SecondClientSocket, &buff, sizeof(buff), 0) == SOCKET_ERROR)
		return errorFunc();
	char error = '9';
	char point[3];
	cout << field;
	int i = 0;
	while (true) {
		if (moves < 9) {

			if (send(FirstClientSocket, field, sizeof(field), 0) == SOCKET_ERROR)
				return errorFunc();
			if (send(SecondClientSocket, field, sizeof(field), 0) == SOCKET_ERROR)
				return errorFunc();

			if (recv(FirstClientSocket, point, sizeof(point), 0) == SOCKET_ERROR)
				return errorFunc();

			int errorCode = fillField(atoi(point), CLIENT);
			if (errorCode == 8 && isFinish() == false) {
				char code = '8';
				if (send(FirstClientSocket, &code, sizeof(code), 0) == SOCKET_ERROR)
					return errorFunc();
				if (send(SecondClientSocket, &code, sizeof(code), 0) == SOCKET_ERROR)
					return errorFunc();
				system("pause");
				return 0;
			}
			while (errorCode == 9) {
				if (send(FirstClientSocket, &error, sizeof(error), 0) == SOCKET_ERROR)
					return errorFunc();
				if (recv(FirstClientSocket, point, sizeof(point), 0) == SOCKET_ERROR)
					return errorFunc();
				errorCode = fillField(atoi(point), CLIENT);
			}
			cout << field;
			if ((moves > 4 || moves == 9) && isFinish()) {
				if (atoi(whowin) == 5) {
					char code = '5';
					if (send(FirstClientSocket, &code, sizeof(code), 0) == SOCKET_ERROR)
						return errorFunc();
					code = '4';
					if (send(SecondClientSocket, &code, sizeof(code), 0) == SOCKET_ERROR)
						return errorFunc();
					break;
				}
			}
			if ((moves > 4 || moves == 9) && isFinish()) {
				if (atoi(whowin) == 4) {
					char code = '4';
					if (send(FirstClientSocket, &code, sizeof(code), 0) == SOCKET_ERROR)
						return errorFunc();
					code = '5';
					if (send(SecondClientSocket, &code, sizeof(code), 0) == SOCKET_ERROR)
						return errorFunc();
					break;
				}
			}

			if (send(FirstClientSocket, field, sizeof(field), 0) == SOCKET_ERROR)
				return errorFunc();
			if (send(SecondClientSocket, field, sizeof(field), 0) == SOCKET_ERROR)
				return errorFunc();

			if (recv(SecondClientSocket, point, sizeof(point), 0) == SOCKET_ERROR)
				return errorFunc();

			errorCode = fillField(atoi(point), SERVER);
			if (errorCode == 8 && isFinish() == false) {
				char code = '8';
				if (send(FirstClientSocket, &code, sizeof(code), 0) == SOCKET_ERROR)
					return errorFunc();
				if (send(SecondClientSocket, &code, sizeof(code), 0) == SOCKET_ERROR)
					return errorFunc();
				system("pause");
				return 0;
			}
			while (errorCode == 9) {
				if (send(SecondClientSocket, &error, sizeof(error), 0) == SOCKET_ERROR)
					return errorFunc();
				if (recv(SecondClientSocket, point, sizeof(point), 0) == SOCKET_ERROR)
					return errorFunc();
				errorCode = fillField(atoi(point), SERVER);
			}

			cout << field;
			if ((moves > 4 || moves == 9) && isFinish()) {
				if (atoi(whowin) == 5) {
					char code = '5';
					if (send(FirstClientSocket, &code, sizeof(code), 0) == SOCKET_ERROR)
						return errorFunc();
					code = '4';
					if (send(SecondClientSocket, &code, sizeof(code), 0) == SOCKET_ERROR)
						return errorFunc();
					break;
				}
			}
			if ((moves > 4 || moves == 9) && isFinish()) {
				if (atoi(whowin) == 4) {
					char code = '4';
					if (send(FirstClientSocket, &code, sizeof(code), 0) == SOCKET_ERROR)
						return errorFunc();
					code = '5';
					if (send(SecondClientSocket, &code, sizeof(code), 0) == SOCKET_ERROR)
						return errorFunc();
					break;
				}
			}
		}
	}
	closesocket(serverSocket);
	system("pause");
	return 0;
}