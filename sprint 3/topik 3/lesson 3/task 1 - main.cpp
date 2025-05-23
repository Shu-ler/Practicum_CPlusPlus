#include <iostream>

void PrintStrStatistics(const std::string &str) {

    auto char_cnt = [&](char sign)
    {
        int count = 0;
        for (char ch : str) {
            if (ch == sign) {
                count++;
            }
        }
        return count;
    };

    int num_space = char_cnt(' ');
    int num_dot = char_cnt('.');
    int num_comma = char_cnt(',');

    std::cout << "Пробелов: " << num_space
              << ", точек: " << num_dot
              << ", запятых: " << num_comma << std::endl;
}

int main() {
    std::string str;
    std::getline(std::cin, str);
    PrintStrStatistics(str);
}
