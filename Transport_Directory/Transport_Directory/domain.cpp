#include "domain.h"

namespace trans_cat {

    bool Stop::operator==(const Stop& other) const {
        return name == other.name;
    }

    bool Route::operator==(const Route& other) const {
        return name == other.name;
    }

} // namespace trans_cat
