#include <WinSock2.h>
#include<fstream>
#include<Windows.h>
#include<string>

#pragma comment(lib,"WS2_32.lib")
#pragma warning(disable: 4996)
using namespace std;
#define maxConnections 50
HANDLE hThreads[maxConnections / 2];
int countGames = 0;
ofstream out;
char service_name[] = "TicTacToeService";
char fileLogPath[] = "D:\\Probnic\\Lab11\\Debug\\ServiceLog.txt";
HANDLE hBroadcastThread;

SOCKET sock;
SERVICE_STATUS service_status;
SERVICE_STATUS_HANDLE hServiceStatus;

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
		int menuItem = 1;//код который говорит клиенту что сейчас надо будет выбрать €чейку
		mySend(index, menuItem);
		int bSlotIsBusy = 1;//переменна€ котора€ отвечает зан€та ли эта клетка		
		do {
			int numberSlot;
			if (myRecv(index, &numberSlot)) {
				bSlotIsBusy = slotIsBusy(numberSlot);
				if (!bSlotIsBusy) {//если не зан€та присваеваем на его место нужный символ 
					if (index == 0)
						field[numberSlot] = 'x';
					else
						field[numberSlot] = 'o';
				}
				mySend(index, bSlotIsBusy);//отправл€ем результат проверки клиенту 
			}
		} while (bSlotIsBusy&&allGood);//будет продолжатьс€ пока что хорошо и пока клиент не выберет свободный слот
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
	void endGame(int indexWin, bool wasWin, bool wasDisconect) {//принимает идекс победител€ и два флага 
		int menuItem = 0;
		if (wasDisconect) {
			mySend(abs(indexWin - 1), 3);//если был дисконект то уведомл€ем об этом друго			
			return;
		}
		else {
			char message[] = "d";
			for (int i = 0; i < 2; i++) {
				mySend(i, menuItem);//отправл€ем все код о конце игры				
			}
			for (int i = 0; i < 2 && !wasWin; i++) {//если была ничь€ уведомл€ем об этом пользовател€
				mySend(i, message);
			}
			if (!wasWin)//если была ничь€ дальше нет смысла
				return;
			message[0] = 'w';
			mySend(indexWin, message);//уведомлем победител€ 
			message[0] = 'l';
			mySend(abs(indexWin - 1), message);//уведомл€ем проигравшего
		}
	}
	void sendStats() {
		for (int i = 0; i < 2 && allGood; i++) {
			mySend(i, 2);//уведомл€ем что сейчас будем посылать статистику
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
			if (service_status.dwCurrentState != SERVICE_RUNNING)
				allGood = false;
			sendStats();
			oneMove(i % 2);
			if (i > 2)//начина€ с третьего хода есть шанс что кто-нибудь победит 
				endGame = thisClientWin(i % 2);
		}
		this->endGame((i - 1) % 2, endGame, !allGood);
	}
	bool continueToPlay() {
		int choose = 1;
		int menuItem = 1;
		for (int i = 0; i < 2 && allGood; i++) { //принимаем решение обоих игроков и если кто-нибудь против choose станет = 0 и цикл закончитс€
			myRecv(i, &choose);
			if (choose == 0)
				menuItem = 0;
		}
		for (int i = 0; i < 2; i++)//отсылаем решение каждому игроку 
			mySend(i, menuItem);
		if (allGood)
			for (int i = 0; i < 9; i++)//обнул€ем поле
				field[i] = '-';
		return allGood;
	}
};
bool choose = 1;

DWORD WINAPI theardNewGame(void* param) {
	TicTacToe *ticTacToe = (TicTacToe*)param;

	do {
		ticTacToe->startGame();
	} while (ticTacToe->continueToPlay()&&service_status.dwCurrentState==SERVICE_RUNNING);//если все проголосовали за новую игру то игра продолжаетс€, если нет то поток завершает работу
	delete ticTacToe;
	return 0;
}

DWORD WINAPI broadCastThread(LPVOID param) {
	SOCKET sockUDP = socket(AF_INET, SOCK_DGRAM, 0);
	int AllowBroadcast = 1;
	if (setsockopt(sockUDP, SOL_SOCKET, SO_BROADCAST, (char*)&AllowBroadcast, sizeof(AllowBroadcast)) == SOCKET_ERROR) {
		out << "Error setsockopt() with code: " << WSAGetLastError();
		system("pause");
		closesocket(sockUDP);
		return 1;
	}
	sockaddr_in sAddrUDP;
	sAddrUDP.sin_family = AF_INET;
	sAddrUDP.sin_addr.S_un.S_addr = 0;
	sAddrUDP.sin_port = htons(2010);	
	if (bind(sockUDP, (SOCKADDR*)&sAddrUDP, sizeof(sAddrUDP)) == SOCKET_ERROR) {
		out << "Error in bind() with code: " << GetLastError() << endl;
		Sleep(1000);		
		closesocket(sockUDP);
		WSACleanup();
		return -1;
	}
	sAddrUDP.sin_port = htons(2020);
	sAddrUDP.sin_addr.S_un.S_addr = INADDR_BROADCAST;//inet_addr("10.1.0.255")
	while (service_status.dwCurrentState==SERVICE_RUNNING) {
		if (sendto(sockUDP, "a", 2, NULL, (SOCKADDR*)&sAddrUDP, sizeof(sAddrUDP)) == SOCKET_ERROR) {
			out << "Error sendto() with code: " << WSAGetLastError();
			continue;
		}		
	}
	closesocket(sockUDP);
	return 0;
}

void waitingConnect(SOCKET sock, sockaddr_in cAddr) {
	int sizeofSAdrr = sizeof(cAddr);
	SOCKET coupleClients[2];
	for (int i = 0; i < 2 && service_status.dwCurrentState==SERVICE_RUNNING; ) {
		out << "Waiting " << i + 1 << " client" << endl;
		coupleClients[i] = accept(sock, (SOCKADDR*)&cAddr, &sizeofSAdrr);//€ не знаю как убрать задержку здесь
		if (coupleClients[i] == INVALID_SOCKET) {
			out << "failed to connect: " << endl;
			out << "Code error " << GetLastError() << endl;
		}
		else {
			out << "Client " << i + 1 << " connected " << endl;
			i++;
		}
	}
	TicTacToe *ticTacToe = new TicTacToe(coupleClients[0], coupleClients[1]);
	if ((hThreads[countGames] = CreateThread(NULL, NULL, theardNewGame, (void*)ticTacToe, NULL, NULL)) == INVALID_HANDLE_VALUE) {
		out << "Eroor in CreateThread for " << countGames + 1 << endl;
	}
	else
		countGames++;
}

//	Enter Service
void errorHandler(char * errorArea) {
	char errorStr[1024];
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errorStr, sizeof(errorStr), NULL);
	out << "ќшибка в " << errorArea << "  од ошибки " << GetLastError() << " ѕо€снение ошибки " << errorStr << endl;
	service_status.dwCurrentState = SERVICE_CONTROL_STOP;
	SetServiceStatus(hServiceStatus, &service_status);
}
//ќбработчик управл€ющих команд дл€ сервиса
DWORD WINAPI ServiceCtrlHandlerEx(DWORD dwControl,DWORD dwEventType,LPVOID lpEventData,LPVOID lpContext) {
	switch (dwControl) {
	case SERVICE_CONTROL_STOP://Stop Service
		service_status.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(hServiceStatus, &service_status);

		WaitForMultipleObjects(countGames, hThreads, true, 29000);
		for (int i = 0; i < countGames; i++) {
			CloseHandle(hThreads[i]);
		}
		CloseHandle(hBroadcastThread);
		shutdown(sock, SD_BOTH);
		closesocket(sock);
		WSACleanup();
		out << "Service closed" << endl;
		out.close();
		service_status.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(hServiceStatus, &service_status);
		break;	
	default:
		service_status.dwCheckPoint++;
		SetServiceStatus(hServiceStatus, &service_status);

	}
	return 0;
}
void WINAPI ServiceMain(DWORD dwArgs, LPSTR *lpszArgv) {
	//регестрируем ќбработчик управл€ющих команд дл€ сервиса 
	hServiceStatus = RegisterServiceCtrlHandlerEx(service_name, ServiceCtrlHandlerEx,NULL);	
	if (!hServiceStatus) {
		out.open(fileLogPath);
		out << "Mistake in hServiceStatus.";
		out.close();
		return;
	}
	//Iticilization structure service
	service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	service_status.dwCurrentState = SERVICE_START_PENDING;
	service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP ;
	service_status.dwWin32ExitCode = NO_ERROR;
	service_status.dwCheckPoint = 0;
	service_status.dwWaitHint = 5000;
	out.open(fileLogPath, ios::app);
	//Set up Status
	if (!SetServiceStatus(hServiceStatus, &service_status)) {
		errorHandler("SetServiceStatus");
		return;
	}
	
	//локальна€ инициализаци€ сервиса
	WSADATA WSAData;
	int codeError = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (codeError != 0) {
		errorHandler("WSAStartup");
		return;
	}
	sockaddr_in sAddr, cAddr;
	ZeroMemory(&sAddr, sizeof(sAddr));
	ZeroMemory(&cAddr, sizeof(cAddr));
	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.S_un.S_addr = ADDR_ANY;//inet_addr("127.0.0.1");inet_addr("10.1.0.34");
	sAddr.sin_port = htons(101010);

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		errorHandler("socket");
		WSACleanup();
		return;
	}

	if (bind(sock, (SOCKADDR*)&sAddr, sizeof(sAddr)) == SOCKET_ERROR) {
		errorHandler("bind");
		shutdown(sock, SD_BOTH);
		closesocket(sock);
		WSACleanup();
		return;
	}
	if (listen(sock, maxConnections) == SOCKET_ERROR) {
		errorHandler("listen");
		shutdown(sock, SD_BOTH);
		closesocket(sock);
		WSACleanup();
		return ;
	}
	
	hBroadcastThread = CreateThread(NULL,NULL,broadCastThread,NULL,NULL,NULL);
	if (hBroadcastThread == INVALID_HANDLE_VALUE) {
		errorHandler("CreateThread for broadCastThread");
		shutdown(sock, SD_BOTH);
		closesocket(sock);
		WSACleanup();
		return;
	}
	
	//”становка рабочего состо€ни€ сервиса
	service_status.dwCurrentState = SERVICE_RUNNING;
	
	//устанавливаем состо€ние сервиса
	if (!SetServiceStatus(hServiceStatus, &service_status)) {
		errorHandler("SetServiceStatus");
		return;
	}
	
	out << "Server Works."<<endl;


	for (int i = 0; i < maxConnections / 2 &&service_status.dwCurrentState==SERVICE_RUNNING; i++) {
		waitingConnect(sock, cAddr);		
	}
	
	return ;
}

int main() {
	SERVICE_TABLE_ENTRY service_table[] = { { service_name/*Name process*/,ServiceMain },{NULL,NULL} };//for service
	//запуск диспетчера сервиса
	if (!StartServiceCtrlDispatcher(service_table)) {//ƒиспетчер сервиса
		out.open(fileLogPath,ios::app);
		char errorStr[256];
		
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errorStr, sizeof(errorStr), NULL);
		out << "ќшибка при запуске диспетчера сервиса(StartServiceCtrlDispatcher). "<<errorStr<<"  од ошибки "<<GetLastError();
		out.close();
	}
	return 0;
}