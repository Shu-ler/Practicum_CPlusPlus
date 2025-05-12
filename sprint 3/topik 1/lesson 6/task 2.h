#pragma once

enum class Weekday {Sun, Mon, Tue, Wed, Thu, Fri, Sat};

inline Weekday operator++(Weekday& day) { 
    day = static_cast<Weekday>((static_cast<int>(day) + 1) % 7);
    return day;
}

inline Weekday operator--(Weekday& day) { 
    day = static_cast<Weekday>((static_cast<int>(day) + 6) % 7);
    return day;
}

inline Weekday operator++(Weekday& day, int) { 
    auto day_tmp = day;
    ++day;
    return day_tmp;
}

inline Weekday operator--(Weekday& day, int) { 
    auto day_tmp = day;
    --day;
    return day_tmp;
}
