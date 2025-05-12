#pragma once

#include <cmath>

class Interval {
public:
    Interval(int start, int end): start_(start), end_(end) {};
    int GetStart() const { 
        return start_; 
    }
    int GetEnd() const { 
        return end_; 
    }
    int GetLength() const {
        return abs(end_ - start_);
    }
private:
    int start_, end_;
};

inline bool operator<(const Interval& lhs, const Interval& rhs) {
    return lhs.GetLength() < rhs.GetLength();
} 

inline bool operator>(const Interval& lhs, const Interval& rhs) {
    return rhs < lhs;
} 

inline bool operator==(const Interval& lhs, const Interval& rhs) {
    return !(lhs < rhs) && !(lhs > rhs);
} 

inline bool operator!=(const Interval& lhs, const Interval& rhs) {
    return !(lhs == rhs);
} 

inline bool operator<=(const Interval& lhs, const Interval& rhs) {
    return !(lhs > rhs);
} 

inline bool operator>=(const Interval& lhs, const Interval& rhs) {
    return !(lhs < rhs);
} 
