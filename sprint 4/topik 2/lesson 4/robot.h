#include <vector>
#include <optional>

struct MaxCell {
    size_t pos;
    int num;
};

class Robot {
public:
    explicit Robot(size_t max_cells) {
        line_.resize(max_cells);        // Используя метод вектора resize(), задать размер
        current_it_ = line_.begin();    // Итератор, который хранит текущую позицию робота
    }

    void GoRight(size_t n_pos) {
        if (n_pos < (line_.size() - GetCurrentIndex() - 1)) {
            current_it_ += n_pos;
        }
    }

    void GoLeft(size_t n_pos) {
        if (n_pos <= GetCurrentIndex()) {
            current_it_ -= n_pos;
        }
    }

    void SetPos(size_t start) {
        if (start < line_.size()) {
            current_it_ = line_.begin() + start;
        }
    }

    void PutBalls(int num) {
        (*current_it_) += num;
    }
    
    // Нужно возвратить std::nullopt, если вектор пуст.
    std::optional<MaxCell> CalculateMaxValue() const {
        if (line_.empty()) {
            return std::nullopt;
        }
        
        MaxCell res{0,0};
        size_t cur_pos = 0;
        for (auto i = line_.begin(); i != line_.end(); ++i, ++cur_pos) {
            if (*i > res.num) {
                res.num = *i;
                res.pos = cur_pos;
            }
        }
        
        return std::optional {res};
    }
    
    // Возвращает текущий порядковый номер позиции
    size_t GetCurrentIndex() const {
        return current_it_ - line_.begin();
    }

private:
    std::vector<int> line_{};
    std::vector<int>::iterator current_it_{};
};
