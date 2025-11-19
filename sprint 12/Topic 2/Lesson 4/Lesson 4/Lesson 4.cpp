#include <iostream>
#include <string>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <cstdint>

using namespace std;

struct Booking {
    int64_t time;
    int client_id;
    int room_count;
};

class HotelManager {
private:
    static constexpr int64_t DAY = 86400;

    struct HotelStats {
        deque<Booking> bookings;
        unordered_map<int, int> client_freq;  // client_id → количество броней
        int total_rooms = 0;

        // Удалить брони, которые вышли за пределы [time - DAY + 1, time]
        void Prune(int64_t current_time) {
            int64_t oldest_allowed = current_time - DAY + 1;

            while (!bookings.empty() && bookings.front().time < oldest_allowed) {
                const Booking& b = bookings.front();
                total_rooms -= b.room_count;
                if (--client_freq[b.client_id] == 0) {
                    client_freq.erase(b.client_id);
                }
                bookings.pop_front();
            }
        }
    };

    map<string, HotelStats> hotels_;
    int64_t last_time_ = 0;

public:
    void Book(int64_t time, const string& hotel_name, int client_id, int room_count) {
        last_time_ = time;

        HotelStats& hotel = hotels_[hotel_name];
        hotel.Prune(time);  // удаляем устаревшие

        hotel.bookings.push_back({ time, client_id, room_count });
        hotel.client_freq[client_id]++;
        hotel.total_rooms += room_count;
    }

    int ComputeClientCount(const string& hotel_name) {
        auto it = hotels_.find(hotel_name);
        if (it == hotels_.end()) {
            return 0;
        }

        HotelStats& hotel = it->second;
        hotel.Prune(last_time_);  // используем last_time_ — время последнего BOOK

        return hotel.client_freq.size();
    }

    int ComputeRoomCount(const string& hotel_name) {
        auto it = hotels_.find(hotel_name);
        if (it == hotels_.end()) {
            return 0;
        }

        HotelStats& hotel = it->second;
        hotel.Prune(last_time_);  // чистим

        return hotel.total_rooms;
    }
};

int main() {
    HotelManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;

        if (query_type == "BOOK") {
            int64_t time;
            cin >> time;
            string hotel_name;
            cin >> hotel_name;
            int client_id, room_count;
            cin >> client_id >> room_count;
            manager.Book(time, hotel_name, client_id, room_count);
        }
        else {
            string hotel_name;
            cin >> hotel_name;
            if (query_type == "CLIENTS") {
                cout << manager.ComputeClientCount(hotel_name) << "\n";
            }
            else if (query_type == "ROOMS") {
                cout << manager.ComputeRoomCount(hotel_name) << "\n";
            }
        }
    }

    return 0;
}
