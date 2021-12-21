#include <WinSock2.h>
#include <iostream>
#include<Windows.h>
#include<string>
#pragma comment(lib,"WS2_32.lib")
#pragma warning(disable: 4996)
using namespace std;

char message[1024];
class TicTacToe {
	bool allGood;
	SOCKET connectedSock;
	void errorProcessing(string text) {
		//cout << "Error " << text << " with code: " << GetLastError() << endl;
		allGood = false;
	}
	void makeMove() {
		int slotBusy = 1;
		int numberOfCells = -1;
		do {
			cout << "Choose where to go: ";
			cin >> numberOfCells;
			if (numberOfCells >= 0 && numberOfCells <= 8) {
				if (send(connectedSock, (char*)&numberOfCells, sizeof(int), NULL) == SOCKET_ERROR) {
					errorProcessing("during send numberOfCells in makeMove");
					return;
				}
				if (recv(connectedSock, (char*)&slotBusy, sizeof(slotBusy), NULL) == SOCKET_ERROR) {
					errorProcessing("during recv slotBusy in makeMove");
					return;
				}

				if (slotBusy)
					cout << "This slot is busy" << endl;
			}
			else {
				cout << "Incorrect data, try again" << endl;
			}
		} while (slotBusy);
	}
	void printStats() {
		system("cls");
		char stats[9];
		if (recv(connectedSock, stats, 9, NULL) == SOCKET_ERROR) {
			errorProcessing("during recv slotBusy in printStats");
			return;
		}
		for (int i = 0; i < 9; i++) {
			cout << "[" << i << "]: " << stats[i] << " ";
			if (i == 2 || i == 5 || i == 8)
				cout << endl;
		}
	}
	void endGame() {
		char symbol[2];
		if (recv(connectedSock, symbol, sizeof(symbol), NULL) == SOCKET_ERROR) {//прнимаем сообщение в котором говориться о победе или проигрыше 
			errorProcessing("during recv symbol in endGame");
			return;
		}
		if (symbol[0] == 'w')
			cout << "You are win" << endl;
		if (symbol[0] == 'l')
			cout << "You are lose" << endl;
		if (symbol[0] == 'd')
			cout << "Draw" << endl;

	}
	bool continueToPlay() {
		int choose;
		cout << "Want to start a new game? (Yes(!=0) No(0))" << endl;
		cin >> choose;
		cout << "Waiting for the second player" << endl;
		if (send(connectedSock, (char*)&choose, sizeof(int), NULL) == SOCKET_ERROR) {
			errorProcessing("during send choose in continueToPlay");
			return false;
		}
		if (recv(connectedSock, (char*)&choose, sizeof(int), NULL) == SOCKET_ERROR) {//получения ответа от сервера
			errorProcessing("during recv choose in continueToPlay");
			return false;
		}
		if (!choose)//если второй клиент отказался играть choose будет равен 0
			cout << "One player doesn't want to play" << endl;
		return choose;
	}
public:
	TicTacToe(SOCKET sock) {
		this->connectedSock = sock;
		cout << "Connected" << endl;
		cout << "Waiting for the second player" << endl;
		allGood = true;
	}
	TicTacToe() {
		shutdown(connectedSock, SD_BOTH);
		closesocket(connectedSock);
		WSACleanup();
	}
	void startGame() {
		int menuItem;
		bool continueGame = true;
		do {

			if (recv(connectedSock, (char*)&menuItem, sizeof(menuItem), NULL) == SOCKET_ERROR)
				errorProcessing("during recv menuItem in main");
			if (menuItem == 0 && allGood) {//игра закончилась
				endGame();
				continueGame = continueToPlay();
			}
			if (menuItem == 3 && allGood) {//кто-то отключился
				cout << "Second player  disconnected" << endl;
				continueGame = false;
			}
			if (menuItem == 1 && allGood)//сделать шаг
				makeMove();
			if (menuItem == 2 && allGood)//вывести статистику игры
				printStats();
		} while (continueGame&&allGood);
		if (!allGood)
			cout << "Server disconnected" << endl;
	}
};
int main() {
	WSADATA WSAData;
	int codeError = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (codeError != 0) {
		cout << "Error in WSAStartup with code: " << codeError << endl;
		system("pause");
		return -1;
	}



	//========================lab 7.3 ===================================
	SOCKET sockUDP = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockUDP == INVALID_SOCKET) {
		cout << "Error in socket() with code: " << GetLastError() << endl;
		system("pause");
		WSACleanup();
		return -1;
	}
	char AllowBroadcast = '1';
	
	sockaddr_in cAddr;
	cAddr.sin_family = AF_INET;
	cAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	cAddr.sin_port = htons(2020);
	if (bind(sockUDP, (sockaddr*)&cAddr, sizeof(cAddr)) == SOCKET_ERROR) {
		cout << "Error in setsockopt() with code: " << WSAGetLastError() << endl;
		system("pause");
		WSACleanup();
		closesocket(sockUDP);
		return -1;
	}

	char answer[2];
	sockaddr_in sAddrUDP;

	int sizeofAddr = sizeof(cAddr);
	if (recvfrom(sockUDP, answer, sizeof(answer), NULL, (sockaddr*)&sAddrUDP, &sizeofAddr) == SOCKET_ERROR) {
		cout << "Error in recvfrom() with code: " << WSAGetLastError() << endl;
		system("pause");
		WSACleanup();
		shutdown(sockUDP, SD_BOTH);
		closesocket(sockUDP);
		return -1;
		
	}
	cout << inet_ntoa(cAddr.sin_addr) << endl;
	shutdown(sockUDP, SD_BOTH);
	closesocket(sockUDP);
	//========================end ===================================
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		cout << "Error in socket() with code: " << GetLastError() << endl;
		system("pause");
		return -1;
	}
	
	sockaddr_in sAddr;
	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.S_un.S_addr = sAddrUDP.sin_addr.S_un.S_addr;
	sAddr.sin_port = htons(101010);
	if (connect(sock, (SOCKADDR*)&sAddr, sizeof(sAddr)) == SOCKET_ERROR) {
		cout << "Error in connect() with code: " << WSAGetLastError() << endl;
		system("pause");
		WSACleanup();
		shutdown(sock, SD_BOTH);
		closesocket(sock);
		return -1;
	}
	TicTacToe ticTacToe(sock);
	ticTacToe.startGame();
	
	system("pause");
	return 0;
}