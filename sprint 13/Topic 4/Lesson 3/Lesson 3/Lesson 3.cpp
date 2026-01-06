#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;

class RouteManager {
public:
    void AddRoute(int start, int finish) {
        reachable_sets_[start].insert(finish);
        reachable_sets_[finish].insert(start);
    }

    int FindNearestFinish(int start, int finish) const {
        int result = abs(start - finish);

        map<int, set<int>>::const_iterator it_map = reachable_sets_.find(start);
        if (it_map == reachable_sets_.end()) {
            return result;
        }

        const set<int>& reachable = it_map->second;
        if (reachable.empty()) {
            return result;
        }

        set<int>::const_iterator lb = reachable.lower_bound(finish);

        // Проверяем lb — первый элемент >= finish
        if (lb != reachable.end()) {
            int dist = abs(*lb - finish);
            if (dist < result) {
                result = dist;
            }
        }

        // Проверяем элемент перед lb — он < finish
        if (lb != reachable.begin()) {
            set<int>::const_iterator prev_it = lb;
            --prev_it;
            int dist = abs(*prev_it - finish);
            if (dist < result) {
                result = dist;
            }
        }

        return result;
    }

private:
    map<int, set<int>> reachable_sets_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    RouteManager routes;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int start, finish;
        cin >> start >> finish;

        if (query_type == "ADD") {
            routes.AddRoute(start, finish);
        }
        else if (query_type == "GO") {
            cout << routes.FindNearestFinish(start, finish) << '\n';
        }
    }

    return 0;
}