#include <iostream>
#include <vector>
#include <Windows.h>
#include <random>
#include <string>

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_int_distribution<int> uid(0, 2);

void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

class CMatrix {
public:
	float m_f4X4_float32[4][4];

public:
	CMatrix() {};

	virtual ~CMatrix() {};
	/*CMatrix(CMatrix& other);
	CMatrix& operator=(CMatrix&& other);*/

	void initialize(float values[9]) {
		m_f4X4_float32[0][0] = values[0];
		m_f4X4_float32[0][1] = values[1];
		m_f4X4_float32[0][2] = values[2];
		m_f4X4_float32[1][0] = values[3];
		m_f4X4_float32[1][1] = values[4];
		m_f4X4_float32[1][2] = values[5];
		m_f4X4_float32[2][0] = values[6];
		m_f4X4_float32[2][1] = values[7];
		m_f4X4_float32[2][2] = values[8];
	};

	void showMatrix3X3(int x, int y) {
		gotoxy(x, y);
		printf("%3.0f %3.0f %3.0f", m_f4X4_float32[0][0], m_f4X4_float32[0][1], m_f4X4_float32[0][2]);
		gotoxy(x, y + 1);
		printf("%3.0f %3.0f %3.0f", m_f4X4_float32[1][0], m_f4X4_float32[1][1], m_f4X4_float32[1][2]);
		gotoxy(x, y + 2);
		printf("%3.0f %3.0f %3.0f", m_f4X4_float32[2][0], m_f4X4_float32[2][1], m_f4X4_float32[2][2]);
	}
	void showMatrix4X4(int x, int y) {
		gotoxy(x, y);
		printf("%3.0f %3.0f %3.0f %3.0f", m_f4X4_float32[0][0], m_f4X4_float32[0][1], m_f4X4_float32[0][2], m_f4X4_float32[0][3]);
		gotoxy(x, y + 1);
		printf("%3.0f %3.0f %3.0f %3.0f", m_f4X4_float32[1][0], m_f4X4_float32[1][1], m_f4X4_float32[1][2], m_f4X4_float32[1][3]);
		gotoxy(x, y + 2);
		printf("%3.0f %3.0f %3.0f %3.0f", m_f4X4_float32[2][0], m_f4X4_float32[2][1], m_f4X4_float32[2][2], m_f4X4_float32[2][3]);
		gotoxy(x, y + 3);
		printf("%3.0f %3.0f %3.0f %3.0f", m_f4X4_float32[3][0], m_f4X4_float32[3][1], m_f4X4_float32[3][2], m_f4X4_float32[3][3]);
	}
};

CMatrix identify() {
	CMatrix matrix;
	memset(&matrix, 0, sizeof(CMatrix));
	matrix.m_f4X4_float32[0][0] = 1;
	matrix.m_f4X4_float32[1][1] = 1;
	matrix.m_f4X4_float32[2][2] = 1;
	matrix.m_f4X4_float32[3][3] = 1;

	return matrix;
};

inline void MatrixReset(CMatrix& matrix) {
	matrix = identify();
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			matrix.m_f4X4_float32[i][j] = (float)(uid(dre));
		}
	}
};

namespace Matrix3X3 {
	CMatrix multiply(const CMatrix& lhs, const CMatrix& rhs) {
		CMatrix result;
		memset(&result, 0, sizeof(CMatrix));
		auto mul = [&](int x, int y) {
			for (int i = 0; i < 3; ++i) {
				result.m_f4X4_float32[x][y] += lhs.m_f4X4_float32[x][i] * rhs.m_f4X4_float32[i][y];
			}
		};

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				mul(i, j);
			}
		}

		return result;
	}
	CMatrix add(const CMatrix& lhs, const CMatrix& rhs) {
		CMatrix result;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				result.m_f4X4_float32[i][j] = lhs.m_f4X4_float32[i][j] + rhs.m_f4X4_float32[i][j];
			}
		}

		return result;
	}
	CMatrix subtract(const CMatrix& lhs, const CMatrix& rhs) {
		CMatrix result;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				result.m_f4X4_float32[i][j] = lhs.m_f4X4_float32[i][j] - rhs.m_f4X4_float32[i][j];
			}
		}

		return result;
	}
	CMatrix scalarProduct(const CMatrix& matrix, float value) {
		CMatrix result;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				result.m_f4X4_float32[i][j] = matrix.m_f4X4_float32[i][j] * value;
			}
		}

		return result;
	}
	float determinant(const CMatrix& matrix) {
		float result;
		result = matrix.m_f4X4_float32[0][0] * matrix.m_f4X4_float32[1][1] * matrix.m_f4X4_float32[2][2] +
			matrix.m_f4X4_float32[0][1] * matrix.m_f4X4_float32[1][2] * matrix.m_f4X4_float32[2][0] +
			matrix.m_f4X4_float32[0][2] * matrix.m_f4X4_float32[1][0] * matrix.m_f4X4_float32[2][1] -
			(
				matrix.m_f4X4_float32[0][0] * matrix.m_f4X4_float32[1][2] * matrix.m_f4X4_float32[2][1] +
				matrix.m_f4X4_float32[0][1] * matrix.m_f4X4_float32[1][0] * matrix.m_f4X4_float32[2][2] +
				matrix.m_f4X4_float32[0][2] * matrix.m_f4X4_float32[1][1] * matrix.m_f4X4_float32[2][0]
				);

		return result;
	}
	CMatrix transpose(const CMatrix& matrix) {
		CMatrix result;
		result.m_f4X4_float32[0][0] = matrix.m_f4X4_float32[0][0]; result.m_f4X4_float32[1][0] = matrix.m_f4X4_float32[0][1]; result.m_f4X4_float32[2][0] = matrix.m_f4X4_float32[0][2];
		result.m_f4X4_float32[0][1] = matrix.m_f4X4_float32[1][0]; result.m_f4X4_float32[1][1] = matrix.m_f4X4_float32[1][1]; result.m_f4X4_float32[2][1] = matrix.m_f4X4_float32[1][2];
		result.m_f4X4_float32[0][2] = matrix.m_f4X4_float32[2][0]; result.m_f4X4_float32[1][2] = matrix.m_f4X4_float32[2][1]; result.m_f4X4_float32[2][2] = matrix.m_f4X4_float32[2][2];

		return result;
	}
};

namespace Matrix4X4 {
	CMatrix;
	float determinant(const CMatrix& matrix) {
		CMatrix m1;
		CMatrix m2;
		CMatrix m3;
		CMatrix m4;
		float result = 0;

		float values[9];

		values[0] = matrix.m_f4X4_float32[1][1];
		values[1] = matrix.m_f4X4_float32[1][2];
		values[2] = matrix.m_f4X4_float32[1][3];
		values[3] = matrix.m_f4X4_float32[2][1];
		values[4] = matrix.m_f4X4_float32[2][2];
		values[5] = matrix.m_f4X4_float32[2][3];
		values[6] = matrix.m_f4X4_float32[3][1];
		values[7] = matrix.m_f4X4_float32[3][2];
		values[8] = matrix.m_f4X4_float32[3][3];
		m1.initialize(values);
		result += Matrix3X3::determinant(m1) * matrix.m_f4X4_float32[0][0];

		values[0] = matrix.m_f4X4_float32[1][0];
		values[1] = matrix.m_f4X4_float32[1][2];
		values[2] = matrix.m_f4X4_float32[1][3];
		values[3] = matrix.m_f4X4_float32[2][0];
		values[4] = matrix.m_f4X4_float32[2][2];
		values[5] = matrix.m_f4X4_float32[2][3];
		values[6] = matrix.m_f4X4_float32[3][0];
		values[7] = matrix.m_f4X4_float32[3][2];
		values[8] = matrix.m_f4X4_float32[3][3];
		m2.initialize(values);
		result -= Matrix3X3::determinant(m2) * matrix.m_f4X4_float32[0][1];

		values[0] = matrix.m_f4X4_float32[1][0];
		values[1] = matrix.m_f4X4_float32[1][1];
		values[2] = matrix.m_f4X4_float32[1][3];
		values[3] = matrix.m_f4X4_float32[2][0];
		values[4] = matrix.m_f4X4_float32[2][1];
		values[5] = matrix.m_f4X4_float32[2][3];
		values[6] = matrix.m_f4X4_float32[3][0];
		values[7] = matrix.m_f4X4_float32[3][1];
		values[8] = matrix.m_f4X4_float32[3][3];
		m3.initialize(values);
		result += Matrix3X3::determinant(m3) * matrix.m_f4X4_float32[0][2];

		values[0] = matrix.m_f4X4_float32[1][0];
		values[1] = matrix.m_f4X4_float32[1][1];
		values[2] = matrix.m_f4X4_float32[1][2];
		values[3] = matrix.m_f4X4_float32[2][0];
		values[4] = matrix.m_f4X4_float32[2][1];
		values[5] = matrix.m_f4X4_float32[2][2];
		values[6] = matrix.m_f4X4_float32[3][0];
		values[7] = matrix.m_f4X4_float32[3][1];
		values[8] = matrix.m_f4X4_float32[3][2];
		m4.initialize(values);
		result -= Matrix3X3::determinant(m4) * matrix.m_f4X4_float32[0][3];

		return result;
	}

};


static const char KM_multiply = 'm';
static const char KM_subtract = 'd';
static const char KM_add = 'a';
static const char KM_determinant = 'r';
static const char KM_transpose = 't';
static const char KM_change4X4 = 'h';
static const char KM_reset = 's';
static const char KM_quit = 'q';

struct POSITION {
	int x;
	int y;
};

int main() {
	CMatrix lhs;
	lhs = identify();
	MatrixReset(lhs);
	CMatrix rhs;
	rhs = identify();
	MatrixReset(rhs);

	POSITION base1{ 1,2 };
	POSITION base2{ 15,2 };
	POSITION result1{ 1, 12 };
	POSITION result2{ 15, 12 };
	POSITION result3{ 30, 12 };

	char command;
	while (true)
	{
		try
		{
			system("cls");

			CMatrix result;

			lhs.showMatrix3X3(1, 2);
			rhs.showMatrix3X3(15, 2);

			gotoxy(0, 10);
			std::cout << "input : ";
			std::cin >> command;
			std::cin.ignore();

			switch (command)
			{
			case KM_multiply:
				result = Matrix3X3::multiply(lhs, rhs);
				lhs.showMatrix3X3(result1.x, result1.y);
				rhs.showMatrix3X3(result2.x, result2.y);
				result.showMatrix3X3(result3.x, result3.y);
				break;
			case KM_add:
				result = Matrix3X3::add(lhs, rhs);
				lhs.showMatrix3X3(result1.x, result1.y);
				rhs.showMatrix3X3(result2.x, result2.y);
				result.showMatrix3X3(result3.x, result3.y);
				break;
			case KM_subtract:
				result = Matrix3X3::subtract(lhs, rhs);
				lhs.showMatrix3X3(result1.x, result1.y);
				rhs.showMatrix3X3(result2.x, result2.y);
				result.showMatrix3X3(result3.x, result3.y);
				break;
			case KM_determinant:
			{
				float D = Matrix3X3::determinant(lhs);
				lhs.showMatrix3X3(result1.x, result1.y);
				gotoxy(result2.x, result2.y);
				std::cout << " = " << D << "   ";
				gotoxy(0, 60);

			}
			break;
			case KM_transpose:
				result = Matrix3X3::transpose(lhs);
				lhs.showMatrix3X3(result1.x, result1.y);
				result.showMatrix3X3(result2.x, result2.y);

				result = Matrix3X3::transpose(rhs);
				rhs.showMatrix3X3(result1.x, result1.y + 10);
				result.showMatrix3X3(result2.x, result2.y + 10);
				break;
			case KM_change4X4:
			{
				lhs.showMatrix4X4(result1.x, result1.y);
				float D = Matrix4X4::determinant(lhs);
				gotoxy(result2.x + 5, result2.y);
				std::cout << " = " << D << "   ";
				gotoxy(0, 60);
			}
			break;
			case KM_reset:
				MatrixReset(lhs);
				MatrixReset(rhs);
				break;
			case KM_quit:
				return 0;
			default:
				break;
			}

			if (command > '1' && command <= '9') {
				int value = atoi(&command);
				result = Matrix3X3::scalarProduct(lhs, value);
				lhs.showMatrix3X3(result1.x, result1.y);
				result.showMatrix3X3(result2.x, result2.y);
			}

			system("pause");
		}
		catch (const std::exception& e)
		{

		}
	}
}