#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "log_duration.h"

using namespace std;

class RandomContainer {
public:
    void Insert(int val) {
        if (index_map_.find(val) != index_map_.end()) {
            return;
        }
        values_.push_back(val);
        index_map_[val] = values_.size() - 1;
    }

    void Remove(int val) {
        auto it = index_map_.find(val);
        if (it == index_map_.end()) {
            return;
        }

        size_t remove_idx = it->second;
        int last_val = values_.back();

        values_[remove_idx] = last_val;
        index_map_[last_val] = remove_idx;

        values_.pop_back();
        index_map_.erase(val);
    }

    bool Has(int val) const {
        return index_map_.find(val) != index_map_.end();
    }

    int GetRand() const {
        uniform_int_distribution<int> distr(0, values_.size() - 1);
        int rand_index = distr(engine_);
        return values_[rand_index];
    }

private:
    vector<int> values_;
    unordered_map<int, size_t> index_map_;
    mutable mt19937 engine_;
};

int main() {
    RandomContainer container;
    int query_num = 0;
    cin >> query_num;
    {
        LOG_DURATION("Requests handling"s);
        for (int query_id = 0; query_id < query_num; query_id++) {
            string query_type;
            cin >> query_type;
            if (query_type == "INSERT"s) {
                int value = 0;
                cin >> value;
                container.Insert(value);
            }
            else if (query_type == "REMOVE"s) {
                int value = 0;
                cin >> value;
                container.Remove(value);
            }
            else if (query_type == "HAS"s) {
                int value = 0;
                cin >> value;
                if (container.Has(value)) {
                    cout << "true"s << endl;
                }
                else {
                    cout << "false"s << endl;
                }
            }
            else if (query_type == "RAND"s) {
                cout << container.GetRand() << endl;
            }
        }
    }
}
