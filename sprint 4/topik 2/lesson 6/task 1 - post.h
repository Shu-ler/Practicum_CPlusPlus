#pragma once

#include <iostream>
#include <vector>

class Post {
    struct Package {
        std::string sender{};
        std::string addressee{};
        double weight = 0;
    };
    
public:
    Post() {
    }
    
    // Резервирует место под посылки.
    void SetPostCapacity(size_t capacity) {
        packages_.reserve(capacity);
        if (packages_.size() > capacity) {
            packages_.resize(capacity);
            packages_.shrink_to_fit();
        }
    }
    
    // Функция добавления посылки.
    // Ничего не делает, если посылка не вписывается в capacity() вектора.
    void AddPackage(const std::string& sender, const std::string& addressee, double weight) {
        if (GetFreePlace() > 0) {
            Package new_pack(sender, addressee, weight);
            packages_.push_back(new_pack);
        }
    }
    
    // Возвращает свободное место: вместимость минус количество посылок.
    int GetFreePlace() const {
        return packages_.capacity() - packages_.size();
    }
    
    // Возвращает количество посылок.
    int GetPackagesCount() const {
        return packages_.size();
    }
    
    // Этот метод будет использоваться в тестах.
    const auto& GetUnderlying() const {
        return packages_;
    }
    
private:
    std::vector<Package> packages_;
};
