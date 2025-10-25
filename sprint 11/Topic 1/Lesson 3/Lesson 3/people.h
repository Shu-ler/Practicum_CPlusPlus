#pragma once
#include <string>
#include <unordered_set>

enum class Gender { MALE, FEMALE };

class Person {
public:
    Person(const std::string& name, int age, Gender gender);

    // ������� ������ ������
    const std::string& GetName() const;
    int GetAge() const;
    Gender GetGender() const;

private:
    std::string name_{};
    int age_ = 0;
    Gender gender_;
};

enum class ProgrammingLanguage { CPP, JAVA, PYTHON, PHP };

// �����������. ����� ��������� ������ ����������������
class Programmer : public Person {
public:
    Programmer(const std::string& name, int age, Gender gender);

    // ��������� ������������ ������ ����� ���������������� language
    // ����� ������ ����� ������ ����������� ����� ��������������� �� ���� �����
    // � �� ����� �����������
    void AddProgrammingLanguage(ProgrammingLanguage language);

    // ��������, ����� �� ����������� ��������������� �� ����� ���������������� language
    // ���������� ����������� �� ������� �� ����� ������ ����������������
    bool CanProgram(ProgrammingLanguage language) const;

private:
    std::unordered_set<ProgrammingLanguage> programming_languages_;
};

enum class WorkerSpeciality { BLACKSMITH, CARPENTER, WOOD_CHOPPER, ENGINEER, PLUMBER };

// �������. ������� ����������� ���������������
class Worker : public Person {
public:
    Worker(const std::string& name, int age, Gender gender);

    // ��������� �������� ����������� �������� �� ������������� speciality
    void AddSpeciality(WorkerSpeciality speciality);

    // ��������, ����� �� ������� �������� �� ��������� �������������.
    // ����� ����� ������ �������� ������� �� ������� �������� ���������������
    bool HasSpeciality(WorkerSpeciality speciality) const;

private:
    std::unordered_set<WorkerSpeciality> worker_speciality_;
};
