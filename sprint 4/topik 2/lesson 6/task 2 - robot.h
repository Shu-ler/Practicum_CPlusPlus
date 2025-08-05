#include <ostream>
#include <vector>
#include <format>

struct Point {
    size_t x;
    size_t y;
};

inline Point operator+(Point lhs, Point rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline Point operator-(Point lhs, Point rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

class Robot {
public:
    explicit Robot(size_t columns, size_t rows) : field_(columns, std::vector<int>(rows, 0)) {
    }
    
    void GoDown(size_t n_pos) {
        if ((current_point_.y + n_pos) < field_[0].size()) {
            current_point_.y += n_pos;
        }
    }
    
    void GoUp(size_t n_pos) {
        if (current_point_.y >= n_pos) {
            current_point_.y -= n_pos;
        }
    }
    
    void GoRight(size_t n_pos) {
        if ((current_point_.x + n_pos) < field_.size()) {
            current_point_.x += n_pos;
        }
    }
    
    void GoLeft(size_t n_pos) {
        if (current_point_.x >= n_pos) {
            current_point_.x -= n_pos;
        }
    }
    
    void PutBalls(int num) {
        field_[current_point_.x][current_point_.y] += num;
    }
    
    int CalculateMaxValue() const {
        int max = field_[0][0];
        for (auto& row : field_) {
            for (auto& column : row) {
                if (max < column) {
                    max = column;
                }
            }
        }
        return max;
    }
    
    void PrintField(std::ostream& out) {
        char space_left, space_rigth;
        auto rows = field_[0].size();
        auto columns = field_.size();
        for ( size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j) {
                auto its_current_pos = (i == current_point_.y && j == current_point_.x);
                space_left = its_current_pos ? '[' : ' ';
                space_rigth = its_current_pos ? ']' : ' ';
                out << space_left << std::format("{:02}", field_[j][i]) << space_rigth;
            }
            out << std::endl;
        }
    }
    
    // Этот метод нужен для тестов.
    Point GetCurrentPos() const {
        return current_point_;
    }
    
private:
    std::vector<std::vector<int>> field_;
    Point current_point_ = {0, 0};
};
