#include <algorithm>
#include <deque>
#include <string>

using namespace std;

struct Ticket {
    int id;
    string name;
};

class TicketOffice {
public:
    // добавить билет в систему
    void PushTicket(const string& name) {
        tickets_.push_back(Ticket{++last_id_, name });
    }

    // получить количество доступных билетов
    int GetAvailable() const {
        return tickets_.size();
    }

    // получить количество доступных билетов определённого типа
    int GetAvailable(const string& name) const {
        return count_if(tickets_.begin(), tickets_.end(), [&](const Ticket& ticket) {
            return ticket.name == name;
            });
    }

    // отозвать старые билеты (до определённого id)
    void Invalidate(int minimum) {
        while (!tickets_.empty() && tickets_.front().id <= minimum) {
            tickets_.pop_front();
        };
    }

private:
    int last_id_ = 0;
    deque<Ticket> tickets_;
};
