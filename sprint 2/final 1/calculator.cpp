#include "calculator.h"
#include <cmath>

void Calculator::Set(Number n) {
    num_in_calc_ = n;    
}

Number Calculator::GetNumber() const {
    return num_in_calc_;
}

void Calculator::Add(Number n) {
    num_in_calc_ += n;    
}

void Calculator::Sub(Number n){
    num_in_calc_ -= n;    
}

void Calculator::Div(Number n) {
    num_in_calc_ /= n;    
}

void Calculator::Mul(Number n) {
    num_in_calc_ *= n;    
}

void Calculator::Pow(Number n){
    num_in_calc_ = pow(num_in_calc_, n);    
}

void Calculator::Save() {
    calc_memory_ = num_in_calc_;
    num_in_memory_ = true;
}

void Calculator::Load() {
    if (num_in_memory_) {
        num_in_calc_ = calc_memory_;
    }
}

bool Calculator::HasMem() const {
    return num_in_memory_;
}

std::string Calculator::GetNumberRepr() const {
    return std::to_string(num_in_calc_);
}
