#include <iostream>
#include <vector>
#include <algorithm>

enum class STATE
{
	STATE_VAILD,
	STATE_INVAILD,
};

struct Point {
	int	x;
	int y;
	int z;
	STATE state = STATE::STATE_VAILD;

	Point() = default;
	Point(int _x, int _y, int _z) : x{ _x }, y{ _y }, z{ _z } {};
	Point(int value[3]) : x{ value[0] }, y{ value[1] }, z{ value[2] } {};

	float GetLength() const {
		return sqrt(x * x + y * y + z * z);
	}
};

#define MAX_POINTS 10

int main() {

	std::vector<Point> pointList;

	auto showList = [&]() {
		for (int i = MAX_POINTS - 1; i >= 0; --i) {
			std::cout << i << "\t";
			if (i < pointList.size()) {
				if (pointList[i].state == STATE::STATE_VAILD)
					std::cout << pointList[i].x << " " << pointList[i].y << " " << pointList[i].z << std::endl;
				else
					std::cout << std::endl;
			}
			else
				std::cout << std::endl;
		}
	};


	while (true)
	{
		try
		{
			showList();
			char command;
			std::cin >> command;

			switch (command)
			{
			case '+':
			{
				int x, y, z;
				std::cin >> x >> y >> z;
				if (pointList.size() < MAX_POINTS)
					pointList.emplace_back(x, y, z);
				else {

				}
			}
			break;
			case '-':
				pointList.pop_back();
				break;
			case 'e':
			{
				int x, y, z;
				std::cin >> x >> y >> z;
				if (pointList.front().state == STATE::STATE_INVAILD)
					pointList.front() = Point(x, y, z);
				else {
					if (pointList.size() < MAX_POINTS)
						pointList.insert(pointList.begin(), Point(x, y, z));
				}
			}
			break;
			case 'd':
				for (Point& p : pointList) {
					if (p.state == STATE::STATE_INVAILD)
						continue;
					p.state = STATE::STATE_INVAILD;
					break;
				}
				break;
			case 'l':
			{
				int count = 0;
				for (Point& p : pointList) {
					if (p.state == STATE::STATE_VAILD)
						count++;
				}
				std::cout << count << std::endl;
			}
			break;
			case 'c':
				pointList.clear();
				break;
			case 'm':
			{
				Point farthestPoint = Point(0, 0, 0);
				for (const Point& p : pointList) {
					if (farthestPoint.GetLength() < p.GetLength())
						farthestPoint = p;
				}
				std::cout << farthestPoint.x << " " << farthestPoint.y << " " << farthestPoint.z << std::endl;
			}

			break;
			case 'n':
			{
				Point nearestPoint = Point(INT_MAX, INT_MAX, INT_MAX);
				for (const Point& p : pointList) {
					if (nearestPoint.GetLength() > p.GetLength())
						nearestPoint = p;
				}
				std::cout << nearestPoint.x << " " << nearestPoint.y << " " << nearestPoint.z << std::endl;
			}

			break;
			case 'a':
				while (pointList.front().state == STATE::STATE_INVAILD)
				{
					pointList.erase(pointList.begin());
				}
				std::sort(pointList.begin(), pointList.end(), [](const Point& lhs, const Point& rhs) {
					return lhs.GetLength() < rhs.GetLength();
					});
				break;
			case 's':
				while (pointList.front().state == STATE::STATE_INVAILD)
				{
					pointList.erase(pointList.begin());
				}
				std::sort(pointList.begin(), pointList.end(), [](const Point& lhs, const Point& rhs) {
					return lhs.GetLength() > rhs.GetLength();
					});
				break;
				break;
			default:
				break;
			}

			system("pause");
			system("cls");
		}
		catch (const std::exception&)
		{

		}
	}

	return 0;
}