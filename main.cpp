#include "EvaluateString.h"
#include <iostream>

int main() {
    EvaluateString es("((222 + var1) * (var2 + var3)) * var4 * (var_5 / var_6)");

    es.bindVar("var1", 55);
    es.bindVar("var2", 25);
    es.bindVar("var3", 3);
    es.bindVar("var4", 2);
    es.bindVar("var_5", 8);
    es.bindVar("var_6", 2);

    EvaluateString es2("2+2*2");

    EvaluateString es3("(2+2)*2");

    std::cout << "Выражение: " << es.getExpression() << " = " << es.calculate() << std::endl;
    std::cout << "Выражение: " << es2.getExpression() << " = " << es2.calculate() << std::endl;
    std::cout << "Выражение: " << es3.getExpression() << " = " << es3.calculate() << std::endl;

    do
    {
        std::string input;
        std::cout << "Введите выражение (x - выход): ";
        std::getline(std::cin, input);
        if (input == "x")
            break;
        try
        {
            EvaluateString esi(input);
            std::cout << esi.calculate();
            std::cout << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }



    } while (1);


    return 0;

}