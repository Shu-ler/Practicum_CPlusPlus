#pragma once

#include <string>
#include <set>

class GuestsList
{
public:
    GuestsList() = default;
    GuestsList(const std::set<std::string>& guests) :
        guests_list_(guests) {};

    bool CheckGuest(const std::string& name) const;
private:
    std::set<std::string> guests_list_;
};

inline bool GuestsList::CheckGuest(const std::string &name) const {
    return guests_list_.find(name) != guests_list_.end();
}
