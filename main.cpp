#include "EvaluateString.h"
#include <iostream>

/// @brief Пример работы класса анализа математической строки
int main() {

    // Выражение с константами и переменными 
    EvaluateString<long double, int> es("((222 + var1) * (var2 + var3)) * var4 * (var_5 / var_6)");

    // Константные выражения
    EvaluateString es2("2+2*2");
    EvaluateString es3("(2+2)*2");

    // Присваивание значений переменным
    es.bindVar("var1", 55);
    es.bindVar("var2", 25);
    es.bindVar("var3", 3);
    es.bindVar("var4", 2);
    es.bindVar("var_5", 8);
    es.bindVar("var_6", 2);
    std::cout << "Выражение: " << es.getExpression() << " = " << es.calculate() << std::endl;

    // Изменение значений переменных
    es.bindVar("var1", 888);
    es.bindVar("var2", 255);
    es.bindVar("var3", 777);
    es.bindVar("var4", 999);
    es.bindVar("var_5", 666);
    es.bindVar("var_6", 777);
    std::cout << "Выражение: " << es.getExpression() << " = " << std::to_string(es.calculate()) << std::endl;

    std::cout << "Выражение: " << es2.getExpression() << " = " << es2.calculate() << std::endl;
    std::cout << "Выражение: " << es3.getExpression() << " = " << es3.calculate() << std::endl;

    // Побитовые операции
    EvaluateString esB("(A | B | C) & (D | E)");
    esB.bindVar("A", 32);
    esB.bindVar("B", 1024);
    esB.bindVar("C", 512);
    esB.bindVar("D", 32);
    esB.bindVar("E", 64);
    std::cout << "Выражение: " << esB.getExpression() << " = " << esB.calculate() << std::endl;

    EvaluateString esB2("(4 | 16 | 32 | 64) & ~(64)");
    std::cout << "Выражение: " << esB2.getExpression() << " = " << esB2.calculate() << std::endl;

    // Консольный калькулятор
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
            std::cout << std::to_string(esi.calculate());
            std::cout << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

    } while (1);
    return 0;
}