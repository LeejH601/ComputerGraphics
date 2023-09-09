#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ranges>
#include <string_view>
#include <iomanip>

using std::operator""sv;
constexpr auto delim{ " "sv };

bool isNumeric(std::string& str) {
	for (char c : str)
		if (!std::isdigit(c)) {
			return false;
		}
	return true;
}

int main() {

	std::string fileName;
	std::cin >> fileName;
	std::ifstream in{ fileName };

	std::vector<std::string> vStrings;
	std::string str;
	str.resize(100);
	while (true)
	{
		char data[100];
		in.getline(data, 100);
		if (in.eof())
			break;
		vStrings.emplace_back(data);
	}

	auto printStr = [](std::vector<std::string>& Strings) {
		for (std::string& s : Strings) {
			std::cout << s << std::endl;
		}
	};



	while (true)
	{
		printStr(vStrings);

		char command;
		try
		{
			std::cout << "input : ";
			std::cin >> command;

			switch (command)
			{
			case 'd':
				for (std::string& str : vStrings) {
					str = std::string(str.rbegin(), str.rend());
					std::cout << str << std::endl;
				}
				break;
			case 'e':
			{
				std::string insertString = "@@";
				for (std::string& str : vStrings) {
					std::string oldStr = str;
					str.clear();
					int cnt = 0;
					for (auto it = oldStr.begin(); it != oldStr.end(); ++it) {
						str += *it;
						cnt++;
						if (cnt >= 3) {
							str += "@@";
							cnt = 0;
						}
					}
				}
				printStr(vStrings);
			}

			break;
			case 'f':
				for (std::string& str : vStrings) {
					std::string newStr{ "" };

					for (const auto word : std::views::split(str, delim)) {
						//std::cout << std::quoted(std::string_view(word)) << std::endl;
						std::string s{ word.begin(), word.end() };
						for (auto c = s.crbegin(); c != s.crend(); ++c) {
							newStr += *c;
						}
						newStr += " ";
					}
					str = newStr;
					std::cout << str << std::endl;
				}
				break;
			case 'g':
				char src;
				char newChar;
				std::cin >> src >> newChar;
				for (std::string& str : vStrings) {
					for (char& c : str) {
						if (c == src)
							c = newChar;
					}
				}
				printStr(vStrings);
				break;
			case 'h':
				break;
			case '+':
				for (std::string& str : vStrings) {
					std::string newStr{ "" };

					for (const auto word : std::views::split(str, delim)) {
						std::string s{ word.begin(), word.end() };
						if (isNumeric(s)) {
							int num = std::atoi(s.c_str());
							num++;
							if (num < 0)
								num = 0;
							s = std::to_string(num);
						}
						newStr += s + " ";
					}
					str = newStr;
					std::cout << str << std::endl;
				}
				break;
			case '-':
				for (std::string& str : vStrings) {
					std::string newStr{ "" };

					for (const auto word : std::views::split(str, delim)) {
						std::string s{ word.begin(), word.end() };
						if (isNumeric(s)) {
							int num = std::atoi(s.c_str());
							num--;
							s = std::to_string(num);
						}
						newStr += s + " ";
					}
					str = newStr;
					std::cout << str << std::endl;
				}
				break;
			case 'q':
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
}