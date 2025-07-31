#pragma once

#include <string>
#include <utility>
#include <vector>
#include <algorithm>

using Note = std::string;

// Это заготовка класса для хранения и поиска заметок.
// Можно добавлять новые методы (лучше приватные), члены класса и другой код.
// Менять интерфейс существующих методов нельзя.
class NoteStorage {
public:
    // Здесь нужно обозначить тип итераторов, которые возвращает метод SearchNotes.
    // Тип зависит от контейнера, который вы будете использовать для хранения заметок.
    using Iterator = std::vector<Note>::const_iterator;

    NoteStorage () = default;

    // Реализуйте этот метод.
    void AddNote(const int page, const Note& note) {
        pages_.push_back(page);
        notes_.push_back(note);
    }

    // Реализуйте этот метод.
    std::pair<Iterator, Iterator> SearchNotes(int page_from, int page_to) const {
        auto pages_lb = std::lower_bound(pages_.begin(), pages_.end(), page_from);
        auto pages_ub = std::upper_bound(pages_lb, pages_.end(), page_to);
        auto note_lb = notes_.begin() + std::distance(pages_.begin(), pages_lb);
        auto note_ub = notes_.begin() + std::distance(pages_.begin(), pages_ub);
        return std::make_pair(note_lb, note_ub);    }

private:
    std::vector<int> pages_;
    std::vector<Note> notes_;
};
