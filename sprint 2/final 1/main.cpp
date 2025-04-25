#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "calculator_ui.h"

void Test() {
    {
        std::istringstream input{"15 q"};
        std::ostringstream output;
        std::ostringstream errors;
        
        Calculator calc;
        CalculatorUI ui{calc, output, errors};
        
        assert(ui.Parse(input));
        assert(output.str().empty());
        assert(errors.str().empty());
    }
    {
        std::istringstream input{"42 / 6 + 3 = q"};
        std::ostringstream output;
        std::ostringstream errors;
        
        Calculator calc;
        CalculatorUI ui{calc, output, errors};
        ui.Parse(input);

        assert(calc.GetNumberRepr() == "10.000000");
        assert(output.str() == "10.000000\n");
        assert(errors.str().empty());
    }
    {
        std::istringstream input{"2 + 2 * 2 = q"};
        std::ostringstream output;
        std::ostringstream errors;
        
        Calculator calc;
        CalculatorUI ui{calc, output, errors};
        ui.Parse(input);

        assert(output.str() == "8.000000\n");
        assert(errors.str().empty());
    }
    {
        std::istringstream input{"2 + 2 * 2 = q"};
        std::ostringstream output;
        std::ostringstream errors;
        
        Calculator calc;
        CalculatorUI ui{calc, output, errors};
        ui.Parse(input);

        assert(output.str() == "8.000000\n");
        assert(errors.str().empty());
    }
    {
        std::istringstream input{"-1 * -1 * -1 - -1 * -1 * -1 = q"};
        std::ostringstream output;
        std::ostringstream errors;
        
        Calculator calc;
        CalculatorUI ui{calc, output, errors};
        ui.Parse(input);

        assert(output.str() == "0.000000\n");
        assert(errors.str().empty());
    }
    {
        std::istringstream input{"10 / 0 + 15 = q"};
        std::ostringstream output;
        std::ostringstream errors;
        
        Calculator calc;
        CalculatorUI ui{calc, output, errors};
        ui.Parse(input);

        assert(output.str() == "inf\n");
        assert(errors.str().empty());
    }
    {
        std::istringstream input{"10 / -0 + 15 = q"};
        std::ostringstream output;
        std::ostringstream errors;
        
        Calculator calc;
        CalculatorUI ui{calc, output, errors};
        ui.Parse(input);

        assert(output.str() == "-inf\n");
        assert(errors.str().empty());
    }
    {
        std::istringstream input{"10 + 5 =\n"
                                 ": 5 ** 5 =\n"
                                 "s + 5 =\n"
                                 "l =\n"
                                 "+ 10 =\n"
                                 "l =\n"
                                 "q"};
        std::ostringstream output;
        std::ostringstream errors;
        
        Calculator calc;
        CalculatorUI ui{calc, output, errors};
        ui.Parse(input);

        assert(output.str() == "15.000000\n3125.000000\n3130.000000\n"
                               "3125.000000\n3135.000000\n3125.000000\n");
        assert(errors.str().empty());
    }
    {
        std::istringstream input{"10 + 5 =\nl q"};
        std::ostringstream output;
        std::ostringstream errors;
        
        Calculator calc;
        CalculatorUI ui{calc, output, errors};
        ui.Parse(input);

        assert(output.str() == "15.000000\n");
        assert(errors.str() == "Error: Memory is empty\n");
    }
    {
        std::istringstream input{"15 hello"};
        std::ostringstream output;
        std::ostringstream errors;
        
        Calculator calc;
        CalculatorUI ui{calc, output, errors};
        ui.Parse(input);

        assert(output.str().empty());
        assert(errors.str() == "Error: Unknown token hello\n");
    }
    {
        std::istringstream input{"hello"};
        std::ostringstream output;
        std::ostringstream errors;
        
        Calculator calc;
        CalculatorUI ui{calc, output, errors};
        ui.Parse(input);

        assert(output.str().empty());
        assert(errors.str() == "Error: Numeric operand expected\n");
    }
}

constexpr int CALCULATION_ERROR = 1;

int main() {
    Test();

    Calculator calc;
    CalculatorUI ui{calc, std::cout, std::cerr};
    if (!ui.Parse(std::cin)) {
        std::cerr << "Program failed! Error code: " << CALCULATION_ERROR << std::endl;
    }
}
