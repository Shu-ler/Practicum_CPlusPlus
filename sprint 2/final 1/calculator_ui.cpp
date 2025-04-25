#include "calculator_ui.h"

using namespace std::literals;

CalculatorUI::CalculatorUI(Calculator &calc, std::ostream &out, std::ostream &err)
    : calc_(calc),
      outstream_(out),
      errstream_(err)
{
}

bool CalculatorUI::Parse(std::istream &input) const
{
    Number operand;
    if (!ReadNumber(input, operand))
    {
        return false;
    }
    calc_.Set(operand);

    std::string token;
    while (input >> token)
    {
        Number operand_next;
        if (token == "+"s)
        {
            if (!ReadNumber(input, operand_next))
            {
                break;
            }
            calc_.Add(operand_next);
        }
        else if (token == "-"s)
        {
            if (!ReadNumber(input, operand_next))
            {
                break;
            }
            calc_.Sub(operand_next);
        }
        else if (token == "*"s)
        {
            if (!ReadNumber(input, operand_next))
            {
                break;
            }
            calc_.Mul(operand_next);
        }
        else if (token == "/"s)
        {
            if (!ReadNumber(input, operand_next))
            {
                break;
            }
            calc_.Div(operand_next);
        }
        else if (token == "**"s)
        {
            if (!ReadNumber(input, operand_next))
            {
                break;
            }
            calc_.Pow(operand_next);
        }
        else if (token == "s"s)
        {
            calc_.Save();
        }
        else if (token == "l"s)
        {
            if (!calc_.HasMem())
            {
                errstream_ << "Error: Memory is empty"s << std::endl;
                break;
            }
            calc_.Load();
        }
        else if (token == "="s)
        {
            outstream_ << calc_.GetNumberRepr() << std::endl;
        }
        else if (token == "c"s)
        {
            calc_.Set(0);
        }
        else if (token == "n"s)
        {
            calc_.Set(-calc_.GetNumber());
        }
        else if (token == ":"s)
        {
            if (!ReadNumber(input, operand_next))
            {
                break;
            }
            calc_.Set(operand_next);
        }
        else if (token == "q"s)
        {
            return true;
        }
        else
        {
            errstream_ << "Error: Unknown token "s << token << std::endl;
            break;
        }
    }
    return false;

    return !input.fail();
}

bool CalculatorUI::ReadNumber(std::istream &input, Number &result) const
{
    if (!(input >> result))
    {
        errstream_ << "Error: Numeric operand expected"s << std::endl;
        return false;
    }
    return true;
}
