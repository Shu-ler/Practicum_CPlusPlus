#pragma once

#include "people.h"

/*
    Ќадзиратель за уровнем удовлетворЄнности.
    —пособен наблюдать за состо€нием человека
    ≈сли уровень удовлетворЄнности человека опуститс€ ниже минимального уровн€, Ќадзиратель
    побуждает человека танцевать до тех пор, пока уровень уровень удовлетворЄнности
    не станет больше или равен максимальному значению
*/
class SatisfactionSupervisor : public PersonObserver {
public:
    //  онструктор принимает значение нижнего и верхнего уровн€ удовлетворЄнности
    SatisfactionSupervisor(int min_satisfaction, int max_satisfaction)
        : min_satisfaction_(min_satisfaction), max_satisfaction_(max_satisfaction) {
    }
    
    void OnSatisfactionChanged(Person& person, int old_value, int new_value) override {
        // ≈сли уровень удовлетворЄнности опустилс€ ниже нижней границы
        if (new_value < old_value && new_value < min_satisfaction_) {
            // ѕобуждаем человека танцевать, пока уровень удовлетворЄнности не подниметс€ до верхнего уровн€
            while (person.GetSatisfaction() < max_satisfaction_) {
                person.Dance();
            }
        }
    }

private:
    int min_satisfaction_;
    int max_satisfaction_;
};
