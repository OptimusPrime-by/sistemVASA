#include <windows.h>
#include <iostream>
#pragma comment(lib, "WS2_32.lib")

using namespace std;

#define SERVER_PORT 1111
#define CLIENT 0
#define SERVER 1


int moves = 0;


char fSoc[20] = { NULL };
char sSoc[20] = { NULL };
char whowin[2];
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
			return 9;
			break;
		}
	}
	else return 9;
	if (moves == 9)
	{
		return 8;
	}
	else return 0;
}

bool winCheck() {
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

SOCKET FirstClientSocket;
SOCKET SecondClientSocket;

int main() {
	setlocale(0, "");
	WSADATA data;
	int res = WSAStartup(MAKEWORD(2, 2), &data); //инициализация динамической библиотеки,MAKEWORD задаёт старшую версию библиотеки
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //AF_INET для IPv4,SOCK_STREAM для потокового соединения,протокол TCP
	if (serverSocket == INVALID_SOCKET) {
		cerr << "Сокет не был создан. Ошибка: " << WSAGetLastError() << endl;
		cin.get();
		return 0;
	}

	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//IP-адрес интерфейса, с которого будут приниматься входящие подключения
	sa.sin_port = htons(SERVER_PORT); //задаёт прослушиваемый порт,
	if (bind(serverSocket, (SOCKADDR*)&sa, sizeof(sa)) == SOCKET_ERROR) {   //устанавливает для сокета связь с определенным сетевым интерфейсом(или всеми интерфейсами) и номером порта
		cerr << "Связывание не произошло. Ошибка: " << WSAGetLastError() << endl;
		cin.get();
		return 0;
	}

	if (listen(serverSocket, 2) == SOCKET_ERROR) { //режим ожидания подключений клиентов
		cerr << "Сокет не прослушан. Ошибка: " << WSAGetLastError() << endl;
		cin.get();
		return 0;
	}

	int sockAddrLen = sizeof(sockaddr_in);
	sockaddr_in saClient;
	cout << "Сервер ожидает подключения." << endl;
    FirstClientSocket = accept(serverSocket, (SOCKADDR*)&saClient, &sockAddrLen); //создает новый сокет для входящего подключения
	if (FirstClientSocket == INVALID_SOCKET) {
		cerr << "Сообщение не принято. Ошибка: " << WSAGetLastError() << endl;
		cin.get();
		return 0;
	}
	else cout << "Клиент подключён." << endl;

	SecondClientSocket = accept(serverSocket, (SOCKADDR*)&saClient, &sockAddrLen);
	if (SecondClientSocket == INVALID_SOCKET) {
		cerr << "Сообщение не принято. Ошибка: " << WSAGetLastError() << endl;
		cin.get();
		return 0;
	}
	else cout << "Клиент подключён." << endl;

	if (recv(FirstClientSocket, fSoc, sizeof(fSoc), 0) == SOCKET_ERROR) {
		cerr << "Связывание не произошло. Ошибка: " << WSAGetLastError() << endl;
		cin.get();
		return 0;
	}
	if (recv(SecondClientSocket, sSoc, sizeof(sSoc), 0) == SOCKET_ERROR) {
		cerr << "Связывание не произошло. Ошибка: " << WSAGetLastError() << endl;
		cin.get();
		return 0;
	}

	if (send(FirstClientSocket, sSoc, sizeof(sSoc), 0) == SOCKET_ERROR) {
		cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
		cin.get();
		return 0;
	}
	if (send(SecondClientSocket, fSoc, sizeof(fSoc), 0) == SOCKET_ERROR) {
		cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
		cin.get();
		return 0;
	}

	char number = '1';
	if (send(FirstClientSocket, &number, sizeof(number), 0) == SOCKET_ERROR) {    //кто начинает первым
		cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
		cin.get();
		return 0;
	}
	number = '2';
	if (send(SecondClientSocket, &number, sizeof(number), 0) == SOCKET_ERROR) {
		cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
		cin.get();
		return 0;
	}

	char error = '9';
	char point[3];
	cout << field;
	while (true) {
		if (moves <= 9) {

			if (send(FirstClientSocket, field, sizeof(field), 0) == SOCKET_ERROR){
				cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
			    cin.get();
			    return 0;
		    }
			if (send(SecondClientSocket, field, sizeof(field), 0) == SOCKET_ERROR) {
				cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
				cin.get();
				return 0;
			}
			if (recv(FirstClientSocket, point, sizeof(point), 0) == SOCKET_ERROR) {
				cerr << "Приём не выполнен. Ошибка: " << WSAGetLastError() << endl;
				cin.get();
				return 0;
			}
			int errorCode = fillField(atoi(point), CLIENT);
			if (errorCode == 8&&winCheck()==false) {
				field[33] = '8';
				if (send(FirstClientSocket, field, sizeof(field), 0) == SOCKET_ERROR) {
					cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
					cin.get();
					return 0;
				}
				if (send(SecondClientSocket, field, sizeof(field), 0) == SOCKET_ERROR) {
					cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
					cin.get();
					return 0;
				}
				return 0;
			}
			while (errorCode == 9) {
				if (send(FirstClientSocket, &error, sizeof(error), 0) == SOCKET_ERROR) {
					cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
					cin.get();
					return 0;
				}
				if (recv(FirstClientSocket, point, sizeof(point), 0) == SOCKET_ERROR) {
					cerr << "Приём не выполнен. Ошибка: " << WSAGetLastError() << endl;
					cin.get();
					return 0;
				}
				errorCode = fillField(atoi(point), CLIENT);
			}
			cout << field;
			if ((moves > 4 || moves==9) && winCheck()) {
				if (atoi(whowin) == 5) {
					field[33] = '5';
					if (send(FirstClientSocket, field, sizeof(field), 0) == SOCKET_ERROR) {
						cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
						cin.get();
						return 0;
					}
					field[33] = '4';
					if (send(SecondClientSocket, field, sizeof(field), 0) == SOCKET_ERROR) {
						cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
						cin.get();
						return 0;
					}
					break;
				}
			}
			if ((moves > 4 || moves==9)&& winCheck()) {
				if (atoi(whowin) == 4) {
					field[33] = '4';
					if (send(FirstClientSocket, field, sizeof(field), 0) == SOCKET_ERROR) {
						cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
						cin.get();
						return 0;
					}
					field[33] = '5';
					if (send(SecondClientSocket, field, sizeof(field), 0) == SOCKET_ERROR) {
						cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
						cin.get();
						return 0;
					}
					break;
				}
			}

			if (send(FirstClientSocket, field, sizeof(field), 0) == SOCKET_ERROR) {
				cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
				cin.get();
				return 0;
			}
			if (send(SecondClientSocket, field, sizeof(field), 0) == SOCKET_ERROR) {
				cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
				cin.get();
				return 0;
			}
			if (recv(SecondClientSocket, point, sizeof(point), 0) == SOCKET_ERROR) {
				cerr << "Приём не выполнен. Ошибка: " << WSAGetLastError() << endl;
				cin.get();
				return 0;
			}
			errorCode = fillField(atoi(point), SERVER);
			if (errorCode == 8 && winCheck()==false) {
				field[33] = '8';
				if (send(FirstClientSocket, field, sizeof(field), 0) == SOCKET_ERROR) {
					cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
					cin.get();
					return 0;
				}
				if (send(SecondClientSocket, field, sizeof(field), 0) == SOCKET_ERROR) {
					cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
					cin.get();
					return 0;
				}
				return 0;
			}
			while (errorCode == 9) {
				if (send(SecondClientSocket, &error, sizeof(error), 0) == SOCKET_ERROR) {
					cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
					cin.get();
					return 0;
				}
				if (recv(SecondClientSocket, point, sizeof(point), 0) == SOCKET_ERROR) {
					cerr << "Приём не выполнен. Ошибка: " << WSAGetLastError() << endl;
					cin.get();
					return 0;
				}
				errorCode = fillField(atoi(point), SERVER);
			}

			cout << field;
			if ((moves > 4 || moves == 9) && winCheck()) {
				if (atoi(whowin) == 5) {
					field[33] = '5';
					if (send(FirstClientSocket, field, sizeof(field), 0) == SOCKET_ERROR) {
						cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
						cin.get();
						return 0;
					}
					field[33] = '4';
					if (send(SecondClientSocket, field, sizeof(field), 0) == SOCKET_ERROR) {
						cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
						cin.get();
						return 0;
					}
					break;
				}
			}
			if ((moves > 4 || moves == 9) && winCheck()) {
				if (atoi(whowin) == 4) {
					field[33] = '4';
					if (send(FirstClientSocket, field, sizeof(field), 0) == SOCKET_ERROR) {
						cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
						cin.get();
						return 0;
					}
					field[33] = '5';
					if (send(SecondClientSocket, field, sizeof(field), 0) == SOCKET_ERROR) {
						cerr << "Сообщение не отправлено. Ошибка: " << WSAGetLastError() << endl;
						cin.get();
						return 0;
					}
					break;
				}
			}
		
		}

	}
	closesocket(serverSocket);
	system("pause");
	return 0;
}