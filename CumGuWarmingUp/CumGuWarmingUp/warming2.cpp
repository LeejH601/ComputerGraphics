#include <string>
#include <iostream>
#include <fstream>
#include <vector>

int main() {

	std::string fileName;
	std::cin >> fileName;
	std::ifstream in{ fileName };

	std::vector<std::string> vStrings;
	std::string str;
	while (in >> str)
	{
		vStrings.push_back(str);
	}

	for (std::string& s : vStrings) {
		std::cout << s << " ";
	}

	
	int nNumCount = 0;
	for (std::string& s : vStrings) {
		bool flag = true;
		for (char c : s) {
			if (!(c >= '0' && c <= '9'))
				flag = false;
		}
		if(flag)
			nNumCount++;
	}
	std::cout << std::endl;
	std::cout << "word count : " << vStrings.size() - nNumCount << std::endl;

	std::cout << "number count : " << nNumCount << std::endl;

	int nCapCount = 0;
	for (std::string& s : vStrings) {
		bool flag = false;
		for (char c : s) {
			if (!(c > 'A' && c < 'Z'))
				continue;
			flag = true;
		}
		if (flag) {
			nCapCount++;
			std::cout << s << " ";
		}
	}
	std::cout << std::endl;
	std::cout << "Capital count : " << nCapCount << std::endl;
}