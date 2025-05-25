#include "student.h"
#include <algorithm>
#include <iostream>
#include <cassert>

bool HasBadGrade(const Student& student) {
    auto has_2 = std::find_if(student.all_scores.begin(), student.all_scores.end(), [](int score){ 
        return score < 3;
    });
    return (has_2 != student.all_scores.end()) || student.mean_score < 4;
}

// Функция принимает вектор по значению, чтобы
// модифицировать его без побочного эффекта.
std::vector<Student> FilterBadGrades(std::vector<Student> students) {
    std::erase_if(students, HasBadGrade);
    return students;
}

void TestEmptyInput() {
    std::vector<Student> students = {};
    auto result = FilterBadGrades(students);
    assert(result.empty());
}

void TestEmptyOutput() {
    std::vector<Student> students = {
        Student{.mean_score = 4.2, .first_name = "Petr", .last_name = "Ivanov",  .all_scores = {2, 5, 5, 5, 4}},
        Student{.mean_score = 4.4, .first_name = "Kate", .last_name = "Ivanova", .all_scores = {2, 5, 5, 5, 5}}};
    auto result = FilterBadGrades(students);
    assert(result.empty());
}

void TestPositiveResult() {
    std::vector<Student> students = {
        Student{.mean_score = 4.4, .first_name = "Petr", .last_name = "Ivanov",  .all_scores = {5, 5, 4, 4, 4}},
        Student{.mean_score = 4.6, .first_name = "Kate", .last_name = "Ivanova", .all_scores = {5, 5, 5, 4, 4}}};
    auto result = FilterBadGrades(students);
    assert(result.size() == 2);
}

void TestMeanMark() {
    std::vector<Student> students = {
        Student{.mean_score = 3.2, .first_name = "Petr", .last_name = "Ivanov",  .all_scores = {3, 4, 3, 3, 3 }},
        Student{.mean_score = 4.6, .first_name = "Kate", .last_name = "Ivanova", .all_scores = {5, 5, 5, 4, 4}}};
    auto result = FilterBadGrades(students);
    assert(result.size() == 1 && 
           result[0].first_name == students[1].first_name);
}

void TestMultipleStudents() {
    std::vector<Student> students = {
        Student{.mean_score = 5, .first_name = "Petr", .last_name = "Ivanov",  .all_scores = {5, 5, 5}},
        Student{.mean_score = 3.5, .first_name = "Ivan", .last_name = "Petrov", .all_scores = {3, 4}},
        Student{.mean_score = 4, .first_name = "Alexey", .last_name = "Sergeev", .all_scores = {2, 5, 5}},
        Student{.mean_score = 4, .first_name = "Sergey", .last_name = "Alexeev", .all_scores = {3, 4, 5}},
    };
    auto result = FilterBadGrades(students);
    assert(result.size() == 2 && 
           result[0].first_name == "Petr" && 
           result[1].first_name == "Sergey");
}

int main()
{
   TestEmptyInput();
   TestEmptyOutput();
   TestPositiveResult();
   TestMeanMark();
   TestMultipleStudents();
   std::cout  << "ok!" << std::endl;
}
