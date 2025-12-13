#pragma once

#include "budget_manager.h"
#include <string_view>

// Обрабатывает одну строку запроса
void ParseAndProcessQuery(BudgetManager& manager, std::string_view line);
