#include <iostream>
#include <map>
#include <set>
#include <string>

class Json {};

class Builder {
    class BaseContext;
    class DictValueContext;
    class ArrayItemContext;
public:
    Builder() = default;
    DictValueContext StartDict();
    ArrayItemContext StartArray();
    BaseContext EndDict();
    BaseContext EndArray();

private:
    Json* json;
    class BaseContext {
    public:
        BaseContext(Builder& builder)
            : builder_(builder)
        {
        }
        DictValueContext StartDict() {
            return builder_.StartDict();
        }
        ArrayItemContext StartArray() {
            return builder_.StartArray();
        }
        BaseContext EndDict() {
            return builder_.EndDict();
        }
        BaseContext EndArray() {
            return builder_.EndArray();
        }

    private:
        Builder& builder_;
    };

    class DictValueContext : public BaseContext {
    public:
        DictValueContext(BaseContext base)
            : BaseContext(base)
        {
        }
        BaseContext EndArray() = delete;
        DictValueContext StartDict() = delete;
        ArrayItemContext StartArray() = delete;
    };

    class ArrayItemContext : public BaseContext {
    public:
        ArrayItemContext(BaseContext base) : BaseContext(base) {}
        BaseContext EndDict() = delete;
    };
};

Builder::DictValueContext Builder::StartDict() {
    return BaseContext{ *this };
}

Builder::ArrayItemContext Builder::StartArray() {
    return BaseContext{ *this };
}

Builder::BaseContext Builder::EndDict() {
    return *this;
}

Builder::BaseContext Builder::EndArray() {
    return *this;
}

int main() {
    auto doc = Builder().StartArray().EndDict();
}
