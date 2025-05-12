#pragma once

enum class Weapons { ROCK, PAPER, SCISSORS };

inline bool operator<(const Weapons& lhs, const Weapons& rhs) {
    return (lhs == Weapons::SCISSORS && rhs == Weapons::ROCK)
        || (lhs == Weapons::ROCK && rhs == Weapons::PAPER)
        || (lhs == Weapons::PAPER && rhs == Weapons::SCISSORS);
} 
