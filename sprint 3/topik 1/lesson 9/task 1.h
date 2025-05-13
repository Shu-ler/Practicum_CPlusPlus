#pragma once

#include <string>

class Person {
public:
    Person() = default;
    explicit Person(const std::string& name) 
        : name_(name) {
        };

    std::string GetName() {
        return name_;
    }
private:
    std::string name_;
};
