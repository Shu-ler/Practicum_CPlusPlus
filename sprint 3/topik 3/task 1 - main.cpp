#include "coffeebar.h"
#include <cassert>

int DoRecipe(const std::vector<RecipeAction>& recipe, Cup& cup) {
    int total_time = 0;
    
    for (const auto& action: recipe) {
        total_time += action(cup);		
    }
    
    return total_time;
}

int AddCoffee(Cup& cup) {
    cup.coffee += 50;
    return 15;
}

int AddWater(Cup& cup) {
    cup.water += 50;
    return 15;
}

int AddMilk(Cup& cup) {
    cup.milk += 50;
    return 10;
}

int AddSteamedMilk(Cup& cup) {
    cup.steamed_milk += 50;
    return 20;
}

int AddCocoaPowder(Cup& cup) {
    cup.cocoa += 2;
    return 15;
}

int AddTeabag(Cup& cup) {
    cup.teabag += 1;
    return 3;
}

std::vector<RecipeAction> CreateRecipe(DrinkType drink_type) {
    switch (drink_type) {
    case DrinkType::Latte :
        return {AddCoffee, AddMilk, AddMilk, AddSteamedMilk};
    case DrinkType::Americano :
        return {AddCoffee, AddWater, AddWater};
    case DrinkType::Cappuccino :
        return {AddCoffee, AddMilk, AddSteamedMilk};
    case DrinkType::Cocoa :
        return {AddCocoaPowder, AddMilk, AddMilk, AddMilk, AddMilk};
    case DrinkType::Tea :
        return {AddTeabag, AddWater, AddWater, AddWater, AddWater};
    }
    return {};
}

int main() {
{
    auto recipe = CreateRecipe(DrinkType::Americano);
    Cup cup;
    
    int time = DoRecipe(recipe, cup);
    assert(time == 45);
    assert((cup == Cup{
        .coffee = 50,
        .water = 100
    }));   
}
{
    auto recipe = CreateRecipe(DrinkType::Cocoa);
    Cup cup;
    
    int time = DoRecipe(recipe, cup);
    assert(time == 55);
    assert((cup == Cup{
        .cocoa = 2,
        .milk = 200
    }));  
}     
}
