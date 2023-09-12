#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <Windows.h>
#include <queue>
#include <list>
#include <stack>
#include <conio.h>

#define MAX_BOARD_SIZE 50
#define BOARD_OFFSET 2
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

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
				std::cout << std::string("бс");
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

int board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
std::vector<Point> Route;
std::vector<MYRECT> Obstacles;
std::vector<Point> GoalList;
std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_int_distribution<int> uid(0, 3);
std::uniform_int_distribution<int> uid_obscount(5, 10);
std::uniform_int_distribution<int> uid_obs(2, MAX_BOARD_SIZE - 8);
std::uniform_int_distribution<int> uid_obsSize(1, 4);
std::uniform_int_distribution<int> uid_point(1, MAX_BOARD_SIZE - 2);

std::map<int, int> RouteDirectionMap{};


float distance(Point p1, Point p2) {
	return sqrt(
		pow(abs(p1.x - p2.x), 2) + pow(abs(p1.y - p2.y), 2)
	);
}

class NODE {
	friend Point operator+(const NODE& lhs, const NODE& rhs);

	Point p;
	float fScore;
	float gScore;
	float hScore;
	Point parent;

public:
	NODE() = default;
	NODE(Point _p, float g, float h, Point _parent) {
		p = _p;

		gScore = g;
		hScore = h;

		fScore = g + h;

		parent = _parent;
	}

	Point GetPoint() const { return p; };
	float GetDistancefromParent() const {
		return distance(p, parent);
	}
	float GetHScore() const {
		return hScore;
	}
	float GetFScore() const {
		return fScore;
	}
	float GetGScore() const {
		return gScore;
	}
	void SetGHScore(float g, float h) {
		gScore = g;
		hScore = h;
		fScore = g + h;
	}
	void SetParent(Point _parent) { parent = _parent; };
	Point GetParent() { return parent; };
};

bool CheckPointVaild(const Point& p) {
	if (p.x < 0)
		return false;
	if (p.x >= MAX_BOARD_SIZE)
		return false;
	if (p.y < 0)
		return false;
	if (p.y >= MAX_BOARD_SIZE)
		return false;

	/*POINT P;
	P.x = p.x;
	P.y = p.y;

	for (MYRECT& obs : Obstacles) {
		RECT rect;
		rect.left = obs.leftTop.x;
		rect.top = obs.leftTop.y;
		rect.right = obs.rightBottom.x;
		rect.bottom = obs.rightBottom.y;

		if (PtInRect(&rect, P))
			return false;
	}*/

	return true;
}

bool operator<(const NODE& lhs, const NODE& rhs) {
	return lhs.GetFScore() < rhs.GetFScore();
}

bool operator==(const Point& lhs, const Point& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

Point operator+(const Point& lhs, const Point& rhs) {
	return Point(lhs.x + rhs.x, lhs.y + rhs.y);
}

bool makeRouteAStar() {
	Obstacles.clear();
	int nObstacles = uid_obscount(dre);
	for (int i = 0; i < nObstacles; ++i) {
		Point p{ uid_obs(dre),uid_obs(dre) };
		Point size{ uid_obsSize(dre),uid_obsSize(dre) };
		MYRECT rect{ p, Point(p.x + size.x, p.y + size.y)};

		Obstacles.emplace_back(rect);
	}

	Route.clear();

	static std::uniform_int_distribution<int> uid_goals(4, 20);

	GoalList.clear();
	int nGoals = uid_goals(dre);
	GoalList.emplace_back(0, 0);
	for (int i = 1; i < nGoals - 1; ++i) {
		GoalList.emplace_back(uid_point(dre), uid_point(dre));
	}
	GoalList.emplace_back(MAX_BOARD_SIZE - 1, MAX_BOARD_SIZE - 1);

	std::vector<NODE> globalClosed;
	for (MYRECT& obs : Obstacles) {
		for (int i = obs.leftTop.x; i <= obs.rightBottom.x; ++i) {
			for (int j = obs.leftTop.y; j <= obs.rightBottom.y; ++j) {
				globalClosed.emplace_back(Point(i, j), FLT_MAX, FLT_MAX, Point(-1, -1));

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				gotoxy(i * BOARD_OFFSET, j * BOARD_OFFSET / 2);
				std::cout << std::string("бс");
			}
		}
	}

	for (int i = 0; i < nGoals - 1; ++i) {
		std::list<NODE> OpenNodes;
		std::vector<NODE> CloseNodes = globalClosed;
		int nContinueCount = 0;
		int prevDir = -1;

		Point startPoint = GoalList[i];
		Point nextGoal = GoalList[i + 1];
		Point PrevPoint = startPoint;

		OpenNodes.emplace_back(startPoint, 0, 0, Point(-1, -1));

		for (auto& nd : CloseNodes) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			gotoxy(nd.GetPoint().x * BOARD_OFFSET, nd.GetPoint().y * BOARD_OFFSET / 2);
			std::cout << std::string("бс");
		}

		while (true)
		{
			if (OpenNodes.empty())
				return false;
			auto node_it = OpenNodes.begin();
			for (auto it = OpenNodes.begin(); it != OpenNodes.end(); ++it) {
				if (node_it->GetFScore() > it->GetFScore())
					node_it = it;
			}
			NODE node = *node_it;
			OpenNodes.erase(node_it);

			CloseNodes.push_back(node);

			/*SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
			gotoxy(node.GetPoint().x * BOARD_OFFSET, node.GetPoint().y * BOARD_OFFSET / 2);
			std::cout << std::string("бс");*/
			//Sleep(1);

			if (node.GetPoint() == nextGoal) {
				break;
			}

			Point p = node.GetPoint();
			static Point dirPoints[4] = { {1,0}, {0,-1}, {-1,0}, {0,1} };
			for (int i = 0; i < 4; ++i) {
				Point newP = p + dirPoints[i];
				if (CheckPointVaild(newP) &&
					std::find_if(CloseNodes.begin(), CloseNodes.end(), [&](const NODE& _node) {
						return _node.GetPoint() == newP;
						}) == CloseNodes.end()
							)
				{

					auto it = std::find_if(OpenNodes.begin(), OpenNodes.end(), [&](const NODE& _node) {
						return _node.GetPoint() == newP;
						});
					if (it == OpenNodes.end()) {
						OpenNodes.emplace_back(newP, node.GetGScore() + 1, distance(newP, nextGoal), p);

						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
						gotoxy(node.GetPoint().x * BOARD_OFFSET, node.GetPoint().y * BOARD_OFFSET / 2);
						std::cout << std::string("бс");

					}
					else {
						float g = node.GetGScore() + 1;
						float h = distance(newP, nextGoal);
						float f = g + h;
						if (f < it->GetFScore()) {
							it->SetGHScore(g, h);
							it->SetParent(p);
						}
					}
				}
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
			gotoxy(nextGoal.x * BOARD_OFFSET, nextGoal.y * BOARD_OFFSET / 2);
			std::cout << std::string("бс");
			//Sleep(1);
		}

		std::stack<Point> reversePath;
		NODE node = CloseNodes.back();

		while (true)
		{
			reversePath.push(node.GetPoint());

			if (node.GetPoint() == startPoint)
				break;

			node = *std::find_if(CloseNodes.begin(), CloseNodes.end(), [&](const NODE& other) {
				return other.GetPoint() == node.GetParent();
				});
		}

		if (i != 0)
			reversePath.pop();

		while (reversePath.size() > 0)
		{
			Route.push_back(reversePath.top());
			reversePath.pop();
		}

		system("cls");
		for (auto& p : Route) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			gotoxy(p.x * BOARD_OFFSET, p.y * BOARD_OFFSET / 2);
			std::cout << std::string("бр");
		}

		for (int j = 0; j <= i; ++j) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
			gotoxy(GoalList[j].x * BOARD_OFFSET, GoalList[j].y * BOARD_OFFSET / 2);
			std::cout << std::string("бс");
		}
	}

	

	return true;
}


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

void RenderGoals() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
	for (Point& p : GoalList) {
		gotoxy(p.x * BOARD_OFFSET, p.y * BOARD_OFFSET / 2);
		std::cout << std::string("бс");
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
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	for (MYRECT& rect : Obstacles) {
		rect.Render();
	}
}

void RenderRoute() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	for (Point& p : Route) {
		gotoxy(p.x * BOARD_OFFSET, p.y * BOARD_OFFSET / 2);
		std::cout << std::string("бр");
	}
}

int main() {
	system("mode con cols=200 lines=200 | title warmingUp7");

	RouteDirectionMap.emplace(0, 0);
	RouteDirectionMap.emplace(1, 0);
	RouteDirectionMap.emplace(2, 0);
	RouteDirectionMap.emplace(3, 0);

	Point player{ -1,-1 };
	Point OldPlayerPos = player;
	int nPlayerIndex = -1;

	auto CreateRoute = []() {
		while (true)
		{
			if (makeRouteAStar())
				break;
		}
	};

	//makeRoute();
	
	CreateRoute();
	RenderBoard();
	RenderRoute();
	RenderGoals();

	auto RenderPlayer = [&]() {
		if (nPlayerIndex == -1)
			return;

		gotoxy(OldPlayerPos.x * BOARD_OFFSET, OldPlayerPos.y * BOARD_OFFSET / 2);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		std::cout << std::string("бр");

		player.x = Route[nPlayerIndex].x;
		player.y = Route[nPlayerIndex].y;

		gotoxy(player.x * BOARD_OFFSET, player.y * BOARD_OFFSET / 2);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		std::cout << std::string("б┌");

		OldPlayerPos = player;
	};

	while (true)
	{
		int command;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		gotoxy(0, MAX_BOARD_SIZE * BOARD_OFFSET / 2);
		command = _getch();
		switch (command)
		{
		case 'n':
			RouteDirectionMap[0] = 0;
			RouteDirectionMap[1] = 0;
			RouteDirectionMap[2] = 0;
			RouteDirectionMap[3] = 0;

			CreateRoute();

			RenderBoard();
			RenderRoute();
			RenderGoals();

			player.x = -1;
			player.y = -1;

			nPlayerIndex = -1;

			break;
		case 'r':
			player.x = Route.front().x;
			player.y = Route.front().y;
			nPlayerIndex = 0;
			break;
		case 224:
		{
			int dir = _getch();
			switch (dir)
			{
			case LEFT:
				nPlayerIndex = max(0, nPlayerIndex - 1);
				break;
			case RIGHT:
				nPlayerIndex = min(nPlayerIndex + 1, Route.size() - 1);
				break;
			default:
				break;
			}
		}
		break;
		case 'q':
			return 0;
		default:
			break;
		}

		RenderGoals();
		RenderPlayer();
	}

	return 0;
}