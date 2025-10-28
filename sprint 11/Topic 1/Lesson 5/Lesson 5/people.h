#pragma once

#include <string>

class Person;

// ����������� �� ���������� ��������.
class PersonObserver {
public:
	// ���� ����� ����������, ����� �������� ��������� ���������������� ��������
	virtual void OnSatisfactionChanged(Person& /*person*/, int /*old_value*/, int /*new_value*/) {
		// ���������� ������ �������� ������ ������ �� ������
	}

protected:
	// ����� PersonObserver �� ������������ ��� �������� ��������
	~PersonObserver() = default;
};

/*
	�������.
	��� ��������� ������ ���������������� ����������
	���������� � ��� �����������
*/
class Person {
public:
	Person(const std::string& name, int age) 
		: name_{ name }
		, age_{ age } {
	}

	virtual ~Person() = default;

	int GetSatisfaction() const {
		return satisfaction_;
	}

	const std::string& GetName() const {
		return name_;
	}

	// ������������ ����������� � ��������. ����������� �����������
	// ������������ �� ��������� ������ ���������������� ��������
	// ����� ����������� �������� ����� ����� ������������
	// ���� �������� nullptr � �������� �����������, ��� ������������ ���������� �����������
	void SetObserver(PersonObserver* observer) {
		observer_ = observer;
	}

	PersonObserver* GetObserver() {
		return observer_;
	}

	int GetAge() const {
		return age_;
	}

	// ����������� �� 1 ���������� ������� �� �����
	// ����������� ���������������� �� 1
	void Dance() {
		++dance_count_;
		AfterDance();
	}

	int GetDanceCount() const {
		return dance_count_;
	}

	// ������� ����. ���������� � ������� ������ ������ �� ������
	virtual void LiveADay() {
		// ��������� ����� �������������� ���� �����
	}

protected:
	virtual void AfterDance() {
		SetSatisfaction(satisfaction_ + 1);
	}

	void SetSatisfaction(int sat) {
		if (satisfaction_ != sat) {
			int old_satisfaction = satisfaction_;
			satisfaction_ = sat;
			if (observer_) {
				observer_->OnSatisfactionChanged(*this, old_satisfaction, satisfaction_);
			}
		}
	}

private:
	std::string name_;
	PersonObserver* observer_ = nullptr;
	int satisfaction_ = 100;
	int age_;
	int dance_count_ = 0;
};

// �������.
// ���� �������� �������� �� �������
class Worker : public Person {
public:
	Worker(const std::string& name, int age) : Person(name, age) {
	}

	// ���� �������� �������� �� �������
	void LiveADay() override {
		Work();
	}

	// ����������� ������� ��������� ������ �� 1, ��������� ���������������� �� 5
	void Work() {
		++work_done_;
		SetSatisfaction(GetSatisfaction() - 5);
	}

	// ���������� �������� �������� ��������� ������
	int GetWorkDone() const {
		return work_done_;
	}

protected:

	void AfterDance() override {
		const int age = GetAge();
		int incr = (age > 30 && age < 40) ? 2 : 1;
		SetSatisfaction(GetSatisfaction() + incr);
	}

private:
	int work_done_ = 0;
};

// �������.
// ���� �������� �������� �� ������
class Student : public Person {
public:
	Student(const std::string& name, int age) : Person(name, age) {
	}

	// ���� �������� �������� �� ������
	void LiveADay() override {
		Study();
	}

	// ����� ����������� ������� ������ �� 1, ��������� ������� ���������������� �� 3
	void Study() {
		++knowledge_;
		SetSatisfaction(GetSatisfaction() - 3);
	}

	// ���������� ������� ������
	int GetKnowledgeLevel() const {
		return knowledge_;
	}

private:
	int knowledge_ = 0;
};
