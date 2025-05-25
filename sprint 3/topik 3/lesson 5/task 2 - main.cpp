#include "student.h"

#include <algorithm>
#include <iostream>
#include <optional>
#include <vector>

template<typename Comparator>
void PrintTop(std::vector<Student> all_students, 
                       std::optional<int> max_top, 
                       Comparator comparator) {
    std::stable_sort(all_students.begin(), all_students.end(), comparator);
    auto upper_limit = max_top.value_or(all_students.size());
    if (upper_limit > all_students.size()) {
        upper_limit = all_students.size();
    }
    
    for (size_t i = 0; i < upper_limit; i++) {
        std::cout << all_students[i] << std::endl;
    }
}

int Count5(const Student& s0) {
    return std::count(s0.all_scores.begin(), s0.all_scores.end(), 5);    
}

bool CompareSurnameAsc(const Student& s1, const Student& s2) {
    return s1.last_name < s2.last_name;
}

bool CompareSurnameDesc(const Student& s1, const Student& s2) {
    return s1.last_name > s2.last_name;
}

bool CompareMeanScoreDesc(const Student& s1, const Student& s2) {
    return s1.mean_score > s2.mean_score;
}

bool CompareBestScoreDesc(const Student& s1, const Student& s2) {
    auto count1 = Count5(s1);
    auto count2 = Count5(s2);
    return count1 > count2;
}

bool LexicographicalScoreCompare(const Student& s1, const Student& s2) {
    auto count1 = Count5(s1);
    auto count2 = Count5(s2);
    return std::tie(count1, s1.mean_score) > 
           std::tie(count2, s2.mean_score);
}

int main() {
    std::vector<Student> students;
    Student student;
    while(std::cin >> student) {
        students.push_back(student);
    }
    std::cout << "- Топ студентов с компаратором CompareSurnameAsc" 
              << std::endl;
    PrintTop(students, std::nullopt, CompareSurnameAsc);
    
    std::cout << "- Топ-10 студентов с компаратором CompareSurnameDesc" 
              << std::endl;		
    PrintTop(students, 10, CompareSurnameDesc);
    
    std::cout << "- Топ-4 студентов с компаратором CompareMeanScoreDesc" 
              << std::endl;
    PrintTop(students, 4, CompareMeanScoreDesc);
    
    std::cout << "- Топ-3 студентов с компаратором LexicographicalScoreCompare" 
              << std::endl;
    PrintTop(students, 3, LexicographicalScoreCompare);
}
