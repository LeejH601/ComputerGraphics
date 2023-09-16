#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <ranges>
#include <random>
#include <chrono>

void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


enum class CARD_COLOR {
	COLOR_1 = 1,
	COLOR_2,
	COLOR_3,
	COLOR_4,
	COLOR_5,
	COLOR_6,
	COLOR_7 = 8,
	COLOR_8,
};

std::map<char, int> COLOR_MAP{

};

class CCard {
	char m_cType;
	CARD_COLOR m_eColor;
	bool m_bIsReverse = true;

public:

public:
	CCard() = default;
	CCard(char type) {
		m_cType = type;
		m_eColor = (CARD_COLOR)COLOR_MAP.find(m_cType)->second;
	}
	void SetIsReverse(bool flag) { m_bIsReverse = flag; };
	bool IsReverse() { return m_bIsReverse; };
	char GetType() { return m_cType; };
	void Render() {
		if (m_bIsReverse) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			std::cout << "*";
		}
		else {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)m_eColor);
			std::cout << m_cType;
		}
	}
};

#define BOARD_WIDTH 4
#define BOARD_OFFSET 2
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

int main() {
	std::random_device rd;
	std::default_random_engine dre(rd());

	COLOR_MAP.emplace(std::make_pair<char, int>('A', (int)CARD_COLOR::COLOR_1));
	COLOR_MAP.emplace('B', (int)CARD_COLOR::COLOR_2);
	COLOR_MAP.emplace('C', (int)CARD_COLOR::COLOR_3);
	COLOR_MAP.emplace('D', (int)CARD_COLOR::COLOR_4);
	COLOR_MAP.emplace('E', (int)CARD_COLOR::COLOR_5);
	COLOR_MAP.emplace('F', (int)CARD_COLOR::COLOR_6);
	COLOR_MAP.emplace('G', (int)CARD_COLOR::COLOR_7);
	COLOR_MAP.emplace('H', (int)CARD_COLOR::COLOR_8);

	std::vector<CCard> CardLists;



	CCard board[BOARD_WIDTH][BOARD_WIDTH];




	auto showBoardFrame = [&]() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		int x = BOARD_OFFSET * 2;
		char c = 'a';
		for (int i = 0; i < BOARD_WIDTH; ++i) {
			gotoxy(x, 0);
			std::cout << c++;
			x += BOARD_OFFSET * 2;
		}
		int y = BOARD_OFFSET;
		for (int i = 0; i < BOARD_WIDTH; ++i) {
			gotoxy(0, y);
			std::cout << 1 + i;
			y += BOARD_OFFSET;
		}
	};

	auto showBoard = [&]() {
		showBoardFrame();
		int x = BOARD_OFFSET * 2;
		int y = BOARD_OFFSET;
		for (int i = 0; i < BOARD_WIDTH; ++i) {
			y = BOARD_OFFSET;
			for (int j = 0; j < BOARD_WIDTH; ++j) {
				gotoxy(x, y);
				board[i][j].Render();
				y += BOARD_OFFSET;
			}
			x += BOARD_OFFSET * 2;
		}
	};

	auto showCurrentCursor = [](int x, int y) {
		gotoxy(x * BOARD_WIDTH + BOARD_WIDTH, y * BOARD_WIDTH / 2 + BOARD_WIDTH / 2);
		//std::cout << 'o';
	};

	auto CheckEnd = [&]() {
		for (int i = 0; i < BOARD_WIDTH; ++i) {
			for (int j = 0; j < BOARD_WIDTH; ++j) {
				if (board[i][j].IsReverse())
					return false;
			}
		}
		return true;
	};

	int openedCard = 0;
	int lastX, lastY;
	int x = 0, y = 0;
	int boardSize = BOARD_WIDTH * BOARD_WIDTH;

	while (true)
	{
		for (int i = 0; i < boardSize / 2; ++i) {
			CardLists.emplace_back('A' + i);
			CardLists.emplace_back('A' + i);
		}
		std::ranges::shuffle(CardLists, dre);

		for (int i = 0; i < BOARD_WIDTH; ++i) {
			for (int j = 0; j < BOARD_WIDTH; ++j) {
				board[i][j] = CardLists.back();
				CardLists.pop_back();
			}
		}


		showBoard();
		auto startTime = std::chrono::high_resolution_clock::now();
		auto endTime = startTime;
		int nReverseCount = 0;
		bool resetFlag = false;
		bool clearCheat = false;
		while (!CheckEnd() && !resetFlag && !clearCheat)
		{

			try
			{
				int command = _getch();
				int dir;
				switch (command)
				{
				case 13:
					if (!board[x][y].IsReverse())
						break;
					nReverseCount++;
					if (openedCard == 0) {
						board[x][y].SetIsReverse(false);
						openedCard = board[x][y].GetType();
						lastX = x;
						lastY = y;
					}
					else {
						if (lastX == x && lastY == y)
							break;
						if (board[x][y].GetType() == openedCard) {
							openedCard = 0;
							board[x][y].SetIsReverse(false);
						}
						else {
							openedCard = 0;
							board[x][y].SetIsReverse(false);
							system("cls");
							showBoard();
							showCurrentCursor(x, y);
							Sleep(1000);
							//system("pause");
							board[x][y].SetIsReverse(true);
							board[lastX][lastY].SetIsReverse(true);
						}
						lastX = x;
						lastY = y;
					}
					break;
				case 224:
					dir = _getch();
					switch (dir)
					{
					case LEFT:
						x = max(x - 1, 0);
						break;
					case RIGHT:
						x = min(x + 1, BOARD_WIDTH - 1);
						break;
					case UP:
						y = max(y - 1, 0);
						break;
					case DOWN:
						y = min(y + 1, BOARD_WIDTH - 1);
						break;
					default:
						break;
					}
					break;
				case 'r':
					resetFlag = true;
					break;
				case 'e':
					clearCheat = true;
					break;
				default:
					break;
				}
				system("cls");
				showBoard();
				showCurrentCursor(x, y);
			}
			catch (const std::exception&)
			{

			}

		}
		

		if (!resetFlag) {
			if (clearCheat) {
				for (int i = 0; i < BOARD_WIDTH; ++i) {
					for (int j = 0; j < BOARD_WIDTH; ++j) {
						board[i][j].SetIsReverse(false);
					}
				}
			}
			showBoard();
			endTime = std::chrono::high_resolution_clock::now();

			gotoxy(0, 2 * BOARD_WIDTH + BOARD_WIDTH / 2);
			std::cout << "Clear" << std::endl;
			std::cout << "걸린 시간 : " << std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count() << std::endl;
			std::cout << "뒤집은 횟수 : " << nReverseCount << std::endl;
			//nReverseCount = 18;
			float result = nReverseCount - 16;
			result /= 5.0f;
			result = (-5.f * log10(result + 2.f) + 6) / 4.0f;
			result *= 100.0f;
			result = max(0.0f, min(result, 100.0f));
			std::wcout << "총점 : " << result << std::endl;

			std::cout << "r 키를 눌러 재시작";
			int command = _getch();
			if (command == 'r')
				continue;
			else
				break;
		}
		else {
			gotoxy(0, 2 * BOARD_WIDTH + BOARD_WIDTH / 2);
			std::cout << "게임을 리셋합니다." << std::endl;
			system("pause");
		}
	}


}