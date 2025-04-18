#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

class Duck
{
public:
    void SetName(const std::string &name)
    {
        name_ = name;
    }
    void Fly(int distance)
    {
        distance_ += distance;
        std::cout << name_ << " flies "s << distance << "km. Total flight distance is "s << distance_ << "km."s << std::endl;
    }

private:
    std::string name_;
    int distance_ = 0;
};

int main()
{
    std::vector<Duck> ducks; // Команда уток
    std::string name;        // Буфер имени утки
    int ducks_count = 0;     // Количество уток
    int cmd_count = 0;       // Количество команд

    // Создание команды уток
    std::cin >> ducks_count >> std::ws;
    for (size_t i = 0; i < ducks_count; ++i)
    {
        Duck new_duck;
        std::cin >> name;
        new_duck.SetName(name);
        ducks.push_back(new_duck);
    }

    // Ввод и обработка команд
    std::cin >> cmd_count;
    for (size_t i = 0; i < cmd_count; ++i)
    {
        int duck_number, distance;
        std::cin >> duck_number >> distance;
        ducks[duck_number - 1].Fly(distance);
    }
}
