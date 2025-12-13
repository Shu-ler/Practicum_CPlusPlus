#include "parser.h"
#include "budget_manager.h"

#include <map>
#include <functional>
#include <sstream>
#include <string>
#include <iostream>

namespace {

    int ParseDateToIndex(const Date& date) {
        return Date::ComputeDistance(BudgetManager::START_DATE, date);
    }

    class CommandHandler {
    public:
        explicit CommandHandler(BudgetManager& manager) : manager_(manager) {
            InitializeHandlers();
        }

        void Process(const std::string& line) {
            if (line.empty()) return;

            std::istringstream is(line);
            std::string cmd;
            is >> cmd;

            auto it = request_handlers_.find(cmd);
            if (it != request_handlers_.end()) {
                it->second(line);
            }
        }

    private:
        BudgetManager& manager_;
        using HandlerFunc = std::function<void(const std::string&)>;
        std::map<std::string, HandlerFunc> request_handlers_;

        void InitializeHandlers();
        void HandleEarn(const std::string& line);
        void HandleSpend(const std::string& line);
        void HandlePayTax(const std::string& line);
        void HandleComputeIncome(const std::string& line);
    };

    void CommandHandler::InitializeHandlers() {
        request_handlers_["Earn"] = [this](const std::string& line) { HandleEarn(line); };
        request_handlers_["Spend"] = [this](const std::string& line) { HandleSpend(line); };
        request_handlers_["PayTax"] = [this](const std::string& line) { HandlePayTax(line); };
        request_handlers_["ComputeIncome"] = [this](const std::string& line) { HandleComputeIncome(line); };
    }

    void CommandHandler::HandleEarn(const std::string& line) {
        std::istringstream is(line);
        std::string cmd, from_str, to_str;
        double value;
        is >> cmd >> from_str >> to_str >> value;

        Date from(from_str), to(to_str);
        int from_idx = ParseDateToIndex(from);
        int to_idx = ParseDateToIndex(to);

        manager_.Earn(from_idx, to_idx, value);
    }

    void CommandHandler::HandleSpend(const std::string& line) {
        std::istringstream is(line);
        std::string cmd, from_str, to_str;
        double value;
        is >> cmd >> from_str >> to_str >> value;

        Date from(from_str), to(to_str);
        int from_idx = ParseDateToIndex(from);
        int to_idx = ParseDateToIndex(to);

        manager_.Spend(from_idx, to_idx, value);
    }

    void CommandHandler::HandlePayTax(const std::string& line) {
        std::istringstream is(line);
        std::string cmd, from_str, to_str;
        int rate;
        is >> cmd >> from_str >> to_str >> rate;

        Date from(from_str), to(to_str);
        int from_idx = ParseDateToIndex(from);
        int to_idx = ParseDateToIndex(to);

        manager_.PayTax(from_idx, to_idx, rate);
    }

    void CommandHandler::HandleComputeIncome(const std::string& line) {
        std::istringstream is(line);
        std::string cmd, from_str, to_str;
        is >> cmd >> from_str >> to_str;

        Date from(from_str), to(to_str);
        int from_idx = ParseDateToIndex(from);
        int to_idx = ParseDateToIndex(to);

        double income = manager_.ComputeIncome(from_idx, to_idx);
        std::cout << income << std::endl;
    }

} // namespace

// Единожды создаём обработчик для первого manager
CommandHandler& GetHandler(BudgetManager& manager) {
    static CommandHandler handler(manager);
    return handler;
}

void ParseAndProcessQuery(BudgetManager& manager, std::string_view line) {
    std::string line_copy(line);
    GetHandler(manager).Process(line_copy);
}
