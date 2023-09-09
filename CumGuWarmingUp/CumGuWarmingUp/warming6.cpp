#include <iostream>
#include <Windows.h>

#define MAX_BOARD_SIZE 30
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

struct MYRECT {
	Point leftTop;
	Point rightBottom;

	MYRECT() = default;
	MYRECT(Point p1, Point p2) : leftTop{ p1 }, rightBottom{ p2 } {};

	void Render() {
		if (leftTop.x == -1)
			return;
		for (int i = leftTop.x; i <= rightBottom.x; ++i) {
			for (int j = leftTop.y; j <= rightBottom.y; ++j) {
				int x = i;
				int y = j;

				x %= MAX_BOARD_SIZE;
				y %= MAX_BOARD_SIZE;

				gotoxy(x * BOARD_OFFSET, y * BOARD_OFFSET / 2);
				std::cout << 'o';
			}
		}
	}

	void update() {
		if (leftTop.x >= MAX_BOARD_SIZE) {
			leftTop.x -= MAX_BOARD_SIZE;
			rightBottom.x -= MAX_BOARD_SIZE;
		}
		if (leftTop.x < 0) {
			leftTop.x += MAX_BOARD_SIZE;
			rightBottom.x += MAX_BOARD_SIZE;
		}
		if (leftTop.y < 0) {
			leftTop.y += MAX_BOARD_SIZE;
			rightBottom.y += MAX_BOARD_SIZE;
		}

		if (leftTop.y > MAX_BOARD_SIZE) {
			leftTop.y -= MAX_BOARD_SIZE;
			rightBottom.y -= MAX_BOARD_SIZE;
		}
	}

	void moveX(bool dir) {
		if (dir) {
			leftTop.x++;
			rightBottom.x++;

			
		}
		else {
			leftTop.x--;
			rightBottom.x--;

			
		}
	}

	void moveY(bool dir) {
		if (dir) {
			leftTop.y--;
			rightBottom.y--;

			
		}
		else {
			leftTop.y++;
			rightBottom.y++;

		}
	}

	void scale(bool isBig) {
		if (isBig) {
			scaleX(true);
			scaleY(true);
		}
		else {
			scaleX(false);
			scaleY(false);
		}
	}

	void scaleX(bool isBig) {
		if (isBig) {
			leftTop.x--;
			rightBottom.x++;
		}
		else {
			if (rightBottom.x - leftTop.x >= 2) {
				leftTop.x++;
				rightBottom.x--;
			}
			else if (rightBottom.x - leftTop.x == 1) {
				rightBottom.x--;
			}
		}
	}

	void scaleY(bool isBig) {
		if (isBig) {
			leftTop.y--;
			rightBottom.y++;
		}
		else {
			if (rightBottom.y - leftTop.y >= 2) {
				leftTop.y++;
				rightBottom.y--;
			}
			else if (rightBottom.y - leftTop.y == 1) {
				rightBottom.y--;
			}
		}
	}
};

int main() {
	int board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];

	MYRECT rect;

	auto RenderBoard = [&]() {
		for (int i = 0; i < MAX_BOARD_SIZE; ++i) {
			for (int j = 0; j < MAX_BOARD_SIZE; ++j) {
				gotoxy(i * BOARD_OFFSET, j * BOARD_OFFSET / 2);
				std::cout << '*';
			}
		}
	};

	auto RestRect = [&]() {
		int x1, y1, x2, y2;

		gotoxy(0, MAX_BOARD_SIZE * (BOARD_OFFSET / 2));
		std::cout << "input coord value : ";
		std::cin >> x1 >> y1 >> x2 >> y2;

		rect = MYRECT(Point(x1 - 1, y1 - 1), Point(x2 - 1, y2 - 1));
	};

	RestRect();

	while (true)
	{
		system("cls");
		rect.update();
		RenderBoard();
		rect.Render();
		try
		{
			char command;
			gotoxy(0, MAX_BOARD_SIZE * (BOARD_OFFSET / 2));
			std::cout << "input : ";
			std::cin >> command;
			switch (command)
			{
			case 'R':
			{
				RestRect();
			}
			break;
			case 'd':
				rect.moveX(true);
				break;
			case 'a':
				rect.moveX(false);
				break;
			case 'w':
				rect.moveY(true);
				break;
			case 's':
				rect.moveY(false);
				break;
			case 't':
				rect.scale(false);
				break;
			case 'T':
				rect.scale(true);
				break;
			case 'i':
				rect.scaleX(true);
				break;
			case 'j':
				rect.scaleX(false);
				break;
			case 'k':
				rect.scaleY(true);
				break;
			case 'l':
				rect.scaleY(false);
				break;
			default:
				break;
			}

		}
		catch (const std::exception&)
		{

		}
	}
}