#include <cassert>
#include <functional>
#include <string>
#include <optional>
#include <mutex>

using namespace std;

template <typename T>
class LazyValue {
public:
    explicit LazyValue(std::function<T()> init)
        : init_(std::move(init))
    {
    }

    bool HasValue() const {
        return static_cast<bool>(value_);
    }

    const T& Get() const {
        std::call_once(flag_, [this]() {
            value_.emplace(init_());
            });
        return value_.value();
    }

private:
    mutable std::function<T()> init_;
    mutable std::optional<T> value_;
    mutable std::once_flag flag_;
};

void UseExample() {
    const string big_string = "Giant amounts of memory"s;

    LazyValue<string> lazy_string([&big_string] {
        return big_string;
        });

    assert(!lazy_string.HasValue());
    assert(lazy_string.Get() == big_string);
    assert(lazy_string.Get() == big_string);
}

void TestInitializerIsntCalled() {
    bool called = false;

    {
        LazyValue<int> lazy_int([&called] {
            called = true;
            return 0;
            });
    }
    assert(!called);
}

int main() {
    UseExample();
    TestInitializerIsntCalled();
}
