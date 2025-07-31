#pragma once

#include <iostream>
#include <map>
#include <optional>
#include <vector>

class RecipeBook {
public:
    RecipeBook() = default;
    RecipeBook(std::map<std::string, std::map<std::string, int>> recipes) :
        recipes_(recipes) {}

    bool AddRecipe(const std::string& name, const std::map<std::string, int>& ingredients);
    bool DropRecipe(const std::string& name);
    std::vector<std::string> GetAllRecipes() const;
    std::optional<std::map<std::string, int>> GetRecipe(const std::string& name) const;
    std::vector<std::string> FindByIngredient(const std::string& name) const;
private:
    std::map<std::string, std::map<std::string, int>> recipes_;
};

inline bool RecipeBook::AddRecipe(const std::string &name, const std::map<std::string, int> &ingredients) {
    auto [iterator, inserted] = recipes_.insert({name, ingredients});
    return inserted;
}

inline bool RecipeBook::DropRecipe(const std::string &name) {
    return recipes_.erase(name);
}

inline std::vector<std::string> RecipeBook::GetAllRecipes() const {
    std::vector<std::string> recipes;
    for (const auto& [recipe_name, ingredients] : recipes_) {
        recipes.push_back(recipe_name);
    }
    return recipes;
}

inline std::optional<std::map<std::string, int> > RecipeBook::GetRecipe(const std::string &name) const {
    auto iterator = recipes_.find(name);
    if (iterator == recipes_.end()) {
        return std::nullopt;
    } else {
        return iterator->second;
    }
}

inline std::vector<std::string> RecipeBook::FindByIngredient(const std::string &name) const {
    std::vector<std::string> recipes;
    for (const auto& [recipe_name, ingredients] : recipes_) {
        if (ingredients.find(name) != ingredients.end()) {
            recipes.push_back(recipe_name);
        }
    }
    return recipes;
}
