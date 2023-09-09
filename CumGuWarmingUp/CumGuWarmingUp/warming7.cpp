#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <Windows.h>

#define MAX_BOARD_SIZE 50
#define BOARD_OFFSET 2

void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
};


struct Point {
	int x = -1;
	int y = -1;

	Point() = default;
	Point(int _x, int _y) : x{ _x }, y{ _y } {};

};

int board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
std::vector<Point> Route;
std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_int_distribution<int> uid(0, 3);

std::map<int, int> RouteDirectionMap{};


void makeRoute() {
	Route.clear();
	Route.emplace_back(0, 0);

	int RouteCount = 0;
	while (!(Route.back().x == MAX_BOARD_SIZE - 1 && Route.back().y == MAX_BOARD_SIZE - 1))
	{
		int dir;

		dir = uid(dre);
		if (RouteCount % 10 == 0) {
			int minimumVisited = INT_MAX;
			for (auto p : RouteDirectionMap) {
				if (p.second < minimumVisited) {
					minimumVisited = p.first;
					dir = p.first;
				}
			}
		}

		Point newPoint = Route.back();
		switch (dir)
		{
		case 0:
			newPoint.x++;
			break;
		case 1:
			newPoint.y--;
			break;
		case 2:
			newPoint.x--;
			break;
		case 3:
			newPoint.y++;
			break;
		default:
			break;
		}
		if (newPoint.x >= 0 && newPoint.x < MAX_BOARD_SIZE && newPoint.y >= 0 && newPoint.y < MAX_BOARD_SIZE) {
			RouteDirectionMap[dir]++;
			Route.emplace_back(newPoint.x, newPoint.y);
		}
		RouteCount++;
	}
}

void RenderBoard() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	for (int i = 0; i < MAX_BOARD_SIZE; ++i) {
		for (int j = 0; j < MAX_BOARD_SIZE; ++j) {
			gotoxy(i * BOARD_OFFSET, j * BOARD_OFFSET / 2);
			std::cout << std::string("бс");
		}
	}
}

void RenderRoute() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	for (Point& p : Route) {
		gotoxy(p.x * BOARD_OFFSET, p.y * BOARD_OFFSET / 2);
		std::cout << std::string("бр");
	}
}

int main() {
	RouteDirectionMap.emplace(0, 0);
	RouteDirectionMap.emplace(1, 0);
	RouteDirectionMap.emplace(2, 0);
	RouteDirectionMap.emplace(3, 0);

	Point player{ -1,-1 };
	int nPlayerIndex = -1;

	makeRoute();
	RenderBoard();
	RenderRoute();

	auto RenderPlayer = [&]() {
		if (nPlayerIndex == -1)
			return;

		player.x = Route[nPlayerIndex].x;
		player.y = Route[nPlayerIndex].y;

		gotoxy(player.x * BOARD_OFFSET, player.y * BOARD_OFFSET / 2);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		std::cout << std::string("б┌");
	};

	while (true)
	{
		char command;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		gotoxy(0, MAX_BOARD_SIZE * BOARD_OFFSET / 2);
		std::cout << "input : ";
		std::cin >> command;
		switch (command)
		{
		case 'n':
			RouteDirectionMap[0] = 0;
			RouteDirectionMap[1] = 0;
			RouteDirectionMap[2] = 0;
			RouteDirectionMap[3] = 0;

			makeRoute();

			player.x = -1;
			player.y = -1;

			nPlayerIndex = -1;

			break;
		case 'r':
			player.x = Route.front().x;
			player.y = Route.front().y;
			nPlayerIndex = 0;
			break;
		case '+':
			nPlayerIndex = min(nPlayerIndex + 1, Route.size() - 1);
			break;
		case '-':
			nPlayerIndex = max(0, nPlayerIndex - 1);
			break;
		case 'q':
			return 0;
		default:
			break;
		}

		system("cls");
		RenderBoard();
		RenderRoute();

		RenderPlayer();
	}

	return 0;
}