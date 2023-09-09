#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <Windows.h>
#include <queue>
#include <list>
#include <stack>

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
std::uniform_int_distribution<int> uid_point(0, MAX_BOARD_SIZE - 1);

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

void makeRouteAStar() {
	Route.clear();

	static std::uniform_int_distribution<int> uid_goals(4, 20);

	std::vector<Point> GoalList;
	int nGoals = uid_goals(dre);
	GoalList.emplace_back(0, 0);
	for (int i = 1; i < nGoals - 1; ++i) {
		GoalList.emplace_back(uid_point(dre), uid_point(dre));
	}
	GoalList.emplace_back(MAX_BOARD_SIZE - 1, MAX_BOARD_SIZE - 1);


	for (int i = 0; i < nGoals - 1; ++i) {
		std::list<NODE> OpenNodes;
		std::vector<NODE> CloseNodes;

		Point startPoint = GoalList[i];
		Point nextGoal = GoalList[i + 1];

		OpenNodes.emplace_back(startPoint, 0, 0, Point(-1, -1));

		while (true)
		{
			auto node_it = OpenNodes.begin();
			for (auto it = OpenNodes.begin(); it != OpenNodes.end(); ++it) {
				if (node_it->GetFScore() > it->GetFScore())
					node_it = it;
			}
			NODE node = *node_it;
			OpenNodes.erase(node_it);

			CloseNodes.push_back(node);

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
					if (it == OpenNodes.end())
						OpenNodes.emplace_back(newP, node.GetGScore() + 1, distance(newP, nextGoal), p);
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
	}

	return;
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

	//makeRoute();
	makeRouteAStar();
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

			makeRouteAStar();

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