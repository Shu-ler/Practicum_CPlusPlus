#pragma once
#include <string>

using Number = double;

bool RunCalculatorCycle();
bool ReadNumber(Number &numbuffer);
bool NeedSecondNumber(std::string command);
bool ReadNumberIfNecessary(std::string command, Number &next_number, bool next_number_ok);
bool ReadNumber(Number &numbuffer);
