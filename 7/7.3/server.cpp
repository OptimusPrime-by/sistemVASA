#include <WinSock2.h>
#include <iostream>
#include<Windows.h>
#include<string>
#pragma comment(lib,"WS2_32.lib")
#pragma warning(disable: 4996)
using namespace std;
#define DEBUG
#define maxConnections 50
HANDLE hThreads[maxConnections / 2];
int countGames = 0;
class TicTacToe {
	SOCKET clients[2];
	char field[10];
	bool allGood = true;
	void errorProcessing(int index, string text) {
		//cout << "Error " << text << " with code: " << GetLastError() << endl;
		allGood = false;
	}
	void mySend(int index, char* message) {
		if (send(clients[index], message, strlen(message), NULL) == SOCKET_ERROR)
			errorProcessing(index, message);
	}
	void mySend(int index, int message) {
		if (send(clients[index], (char*)&message, sizeof(message), NULL) == SOCKET_ERROR)
			errorProcessing(index, "int mySend");
	}
	bool myRecv(int index, int *number) {
		int numberRead = recv(clients[index], (char*)number, sizeof(int), NULL);
		if (numberRead == SOCKET_ERROR) {
			errorProcessing(index, "myRecv int");
			allGood = false;
			return false;
		}
		return true;
	}
	bool myRecv(int index, char*message) {
		int numberRead = recv(clients[index], message, sizeof(message), NULL);
		if (numberRead == SOCKET_ERROR) {
			errorProcessing(index, "myRecv int");
			allGood = false;
			return false;
		}
		return true;
	}
	int slotIsBusy(int slotNumber) {
		if (field[slotNumber] == 'x' || field[slotNumber] == 'o')
			return 1;
		return 0;
	}
	void oneMove(int index) {
		int menuItem = 1;//код который говорит клиенту что сейчас надо будет выбрать ячейку
		mySend(index, menuItem);
		int bSlotIsBusy = 1;//переменная которая отвечает занята ли эта клетка		
		do {
			int numberSlot;
			if (myRecv(index, &numberSlot)) {
				bSlotIsBusy = slotIsBusy(numberSlot);
				if (!bSlotIsBusy) {//если не занята присваеваем на его место нужный символ 
					if (index == 0)
						field[numberSlot] = 'x';
					else
						field[numberSlot] = 'o';
				}
				mySend(index, bSlotIsBusy);//отправляем результат проверки клиенту 
			}
		} while (bSlotIsBusy&&allGood);//будет продолжаться пока что хорошо и пока клиент не выберет свободный слот
	}
	bool thisClientWin(int indexClient) {
		char symbol;
		if (indexClient == 0)
			symbol = 'x';
		if (indexClient == 1)
			symbol = 'o';
		if (field[0] == symbol&& field[1] == symbol && field[2] == symbol)
			return true;
		if (field[0] == symbol&& field[3] == symbol && field[6] == symbol)
			return true;
		if (field[0] == symbol&& field[4] == symbol && field[8] == symbol)
			return true;
		if (field[1] == symbol&& field[4] == symbol && field[7] == symbol)
			return true;
		if (field[2] == symbol&& field[4] == symbol && field[6] == symbol)
			return true;
		if (field[2] == symbol&& field[5] == symbol && field[8] == symbol)
			return true;
		if (field[3] == symbol&& field[4] == symbol && field[5] == symbol)
			return true;
		if (field[6] == symbol&& field[7] == symbol && field[8] == symbol)
			return true;
		return false;
	}
	void endGame(int indexWin, bool wasWin, bool wasDisconect) {//принимает идекс победителя и два флага 
		int menuItem = 0;
		if (wasDisconect) {
			mySend(abs(indexWin - 1), 3);//если был дисконект то уведомляем об этом друго			
			return;
		}
		else {
			char message[] = "d";
			for (int i = 0; i < 2; i++) {
				mySend(i, menuItem);//отправляем все код о конце игры				
			}
			for (int i = 0; i < 2 && !wasWin; i++) {//если была ничья уведомляем об этом пользователя
				mySend(i, message);
			}
			if (!wasWin)//если была ничья дальше нет смысла
				return;
			message[0] = 'w';
			mySend(indexWin, message);//уведомлем победителя 
			message[0] = 'l';
			mySend(abs(indexWin - 1), message);//уведомляем проигравшего
		}
	}
	void sendStats() {
		for (int i = 0; i < 2 && allGood; i++) {
			mySend(i, 2);//уведомляем что сейчас будем посылать статистику
			char any[9];
			mySend(i, field);//посылаем статистику
		}
	}
public:
	TicTacToe(SOCKET firstClien, SOCKET secondClien) {
		this->clients[0] = firstClien;
		this->clients[1] = secondClien;
		for (int i = 0; i < 9; i++)
			field[i] = '-';
		field[9] = '\0';
	}
	~TicTacToe() {
		shutdown(clients[0], SD_BOTH);
		closesocket(clients[0]);
		shutdown(clients[1], SD_BOTH);
		closesocket(clients[1]);
	}
	void startGame() {
		bool endGame = false;
		int i = 0;
		for (; i < 9 && allGood && !endGame; i++) {
			sendStats();
			oneMove(i % 2);
			if (i > 2)//начиная с третьего хода есть шанс что кто-нибудь победит 
				endGame = thisClientWin(i % 2);
		}
		this->endGame((i - 1) % 2, endGame, !allGood);
	}
	bool continueToPlay() {
		int choose = 1;
		int menuItem = 1;
		for (int i = 0; i < 2 && allGood; i++) { //принимаем решение обоих игроков и если кто-нибудь против choose станет = 0 и цикл закончится
			myRecv(i, &choose);
			if (choose == 0)
				menuItem = 0;
		}
		for (int i = 0; i < 2; i++)//отсылаем решение каждому игроку 
			mySend(i, menuItem);
		if (allGood)
			for (int i = 0; i < 9; i++)//обнуляем поле
				field[i] = '-';
		return allGood;
	}
};
bool choose = 1;
DWORD WINAPI theardNewGame(void* param) {
	TicTacToe *ticTacToe = (TicTacToe*)param;

	do {
		ticTacToe->startGame();
	} while (ticTacToe->continueToPlay());//если все проголосовали за новую игру то игра продолжается, если нет то поток завершает работу
	delete ticTacToe;
	return 0;
}

DWORD WINAPI broadCastThread(LPVOID param) {
	SOCKET sockUDP = socket(AF_INET, SOCK_DGRAM, 0);
	int AllowBroadcast = 1;
	if (setsockopt(sockUDP, SOL_SOCKET, SO_BROADCAST, (char*)&AllowBroadcast, sizeof(AllowBroadcast)) == SOCKET_ERROR) {
		cout << "Error setsockopt() with code: " << WSAGetLastError();
		system("pause");
		closesocket(sockUDP);
		return 1;
	}
	sockaddr_in sAddrUDP;
	sAddrUDP.sin_family = AF_INET;
	sAddrUDP.sin_addr.S_un.S_addr = 0;
	sAddrUDP.sin_port = htons(2010);	
	if (bind(sockUDP, (SOCKADDR*)&sAddrUDP, sizeof(sAddrUDP)) == SOCKET_ERROR) {
		cout << "Error in bind() with code: " << GetLastError() << endl;
		Sleep(1000);		
		closesocket(sockUDP);
		WSACleanup();
		return -1;
	}
	sAddrUDP.sin_port = htons(2020);
	sAddrUDP.sin_addr.S_un.S_addr = inet_addr("10.1.0.255");
	while (choose) {
		if (sendto(sockUDP, "a", 2, NULL, (SOCKADDR*)&sAddrUDP, sizeof(sAddrUDP)) == SOCKET_ERROR) {
			cout << "Error sendto() with code: " << WSAGetLastError();			
			continue;
		}		
	}
	closesocket(sockUDP);
	return 0;
}
void waitingConnect(SOCKET sock, sockaddr_in cAddr) {
	int sizeofSAdrr = sizeof(cAddr);
	SOCKET coupleClients[2];
	for (int i = 0; i < 2; ) {
		cout << "Waiting " << i + 1 << " client" << endl;
		coupleClients[i] = accept(sock, (SOCKADDR*)&cAddr, &sizeofSAdrr);
		if (coupleClients[i] == INVALID_SOCKET) {
			cout << "failed to connect: " << endl;
			cerr << "Code error " << GetLastError() << endl;
		}
		else {
			cout << "Client " << i + 1 << " connected " << endl;
			i++;
		}
	}
	TicTacToe *ticTacToe = new TicTacToe(coupleClients[0], coupleClients[1]);
	if ((hThreads[countGames] = CreateThread(NULL, NULL, theardNewGame, (void*)ticTacToe, NULL, NULL)) == INVALID_HANDLE_VALUE) {
		cout << "Eroor in CreateThread for " << countGames + 1 << endl;
	}
	else
		countGames++;
}

int main() {

	WSADATA WSAData;
	int codeError = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (codeError != 0) {
		cout << "Error in WSAStartup with code: " << codeError << endl;
		Sleep(1000);
		return -1;
	}
	sockaddr_in sAddr, cAddr;
	ZeroMemory(&sAddr, sizeof(sAddr));
	ZeroMemory(&cAddr, sizeof(cAddr));
	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.S_un.S_addr = ADDR_ANY;//inet_addr("127.0.0.1");inet_addr("10.1.0.34");
	sAddr.sin_port = htons(101010);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		cout << "Error in socket() with code: " << GetLastError() << endl;
		Sleep(1000);
		WSACleanup();
		return -1;
	}

	if (bind(sock, (SOCKADDR*)&sAddr, sizeof(sAddr)) == SOCKET_ERROR) {
		cout << "Error in bind() with code: " << GetLastError() << endl;
		Sleep(1000);
		shutdown(sock, SD_BOTH);
		closesocket(sock);
		WSACleanup();
		return -1;
	}
	if (listen(sock, maxConnections) == SOCKET_ERROR) {
		cout << "Error in bind() with code: " << GetLastError() << endl;
		Sleep(1000);
		shutdown(sock, SD_BOTH);
		closesocket(sock);
		WSACleanup();
		return -1;
	}
	
	HANDLE hBroadcastThread = CreateThread(NULL,NULL,broadCastThread,NULL,NULL,NULL);
	if (hBroadcastThread == INVALID_HANDLE_VALUE) {
		cout << "failed to create a BroadcastThread "<<GetLastError() << endl;
		system("pause");
		shutdown(sock, SD_BOTH);
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	
	for (int i = 0; i < maxConnections / 2 && choose; i++) {
		waitingConnect(sock, cAddr);
		cout << endl;
		cout << "Wait for new players?(Yes(1) No(0))" << endl;
		cin >> choose;
	}
	
	
	cout << "We are waiting for all the games to end" << endl;
	WaitForMultipleObjects(countGames, hThreads, true, INFINITE);
	for (int i = 0; i < countGames; i++) {
		CloseHandle(hThreads[i]);
	}
	CloseHandle(hBroadcastThread);
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	WSACleanup();
	return 0;
}