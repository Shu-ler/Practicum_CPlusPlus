#include <iostream>

class HelloMsg {
public:
	explicit HelloMsg(std::ostream& out) noexcept {
		using namespace std::literals;
		out << "Hello World!"s
			<< std::endl;
	};
	~HelloMsg() {};
};

int main() {
    HelloMsg hello(std::cout);
}
