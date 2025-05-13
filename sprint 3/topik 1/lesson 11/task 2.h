#pragma once

#include <vector>

class IntArray {
public:
    IntArray(std::vector<int> data): data_(data) {}
    
    void push_back(int value) {
        data_.push_back(value);
    }

    int& operator[](int index){
        if (index < 0) {
            return data_[index + data_.size()];
        }
        else {
            return data_[index];
        }
    };
private:
    std::vector<int> data_;
};
