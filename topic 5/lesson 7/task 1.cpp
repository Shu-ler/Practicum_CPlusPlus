#include <string>
#include <iostream>

void DoOliverTask(int count) {
    // Место для вашего решения.
}

void DoPenelopeTask(int count) {
    // Место для вашего решения.
}

int main() {
    std::string target;
    int count;
    std::cin >> target >> count;

    if (target == "Oliver") {
        DoOliverTask(count);
    } else if (target == "Penelope") {
        DoPenelopeTask(count);
    } else {
        std::cout << "Unknown addressee: " << target << std::endl;
    }
}