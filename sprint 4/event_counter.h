#include <queue>

// Это заготовка класса для отслеживания событий в интервале interval_.
// Можно добавлять новые методы (лучше приватные), члены класса и новый код.
// Нельзя менять интерфейс существующих методов.
class EventCounter {
public:
    // В конструкторе вы получаете интервал, события в котором нужно будет считать
    // и начальное время.
    EventCounter(const int interval, const int initial_time = 0):
        interval_(interval),
        current_time_(initial_time) {
    }

    void UpdateTime(const int time) {
        current_time_ = time;
        const int min_time = current_time_ - interval_;

        while (!events_.empty() && events_.front() < min_time)
            events_.pop();
    }

    void OnEvent() {
        events_.push(current_time_);
    }

    size_t EventsInInterval() const {
        return events_.size();
    }

private:
    const int interval_;
    int current_time_;
    std::queue<int> events_;
};
