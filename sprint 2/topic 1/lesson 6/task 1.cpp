#include <iostream>
#include <string>

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
    Duck duck1;
    duck1.SetName("Whisper Quack"s);    // Тихокряк 
    Duck duck2;
    duck2.SetName("Fire Wing"s);        // Огнекрыл 

    int num_commands = 0;
    std::cin >> num_commands;

    for (int i = 0; i < num_commands; ++i)
    {
        int duck_number, distance;
        std::cin >> duck_number >> distance;
        if (duck_number == 1)
        {
            duck1.Fly(distance);
        }
        else if (duck_number == 2)
        {
            duck2.Fly(distance);
        }
    }
}
