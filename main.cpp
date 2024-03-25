#include "EvaluateString.h"
#include <map>
#include <string>
#include <iostream>

int main() {
    // карта перменных
    std::map<std::string, int> vars{
        {"A", 2},
        {"B", 2},
        {"C", 2},
        {"D", 1},
        {"E", 7},
        {"F", 5},
        {"G", 6},
        {"H", 1},
        {"J", 2},
        {"K", 1},
        {"L", 77},
        {"M", 1}
    };

    // выражение 1
    std::string expression1 = "A + B - ((C - D - E) + F | G) + (H - J) - K * (L + M)";
    EvaluateString<int> es1(&vars, expression1);
    es1.BuildTree();
    int v1 = es1.calculate();
    std::cout << expression1 << " = " << v1 << std::endl;

    // выражение 2
    std::string expression2 = "A + B * C";
    EvaluateString<int> es2(&vars, expression2);
    es2.BuildTree();
    int v2 = es2.calculate();
    std::cout << expression2 << " = " << v2 << std::endl;

    // меняем значение переменной A
    vars["A"] = 555;

    std::cout << "После измнения перменной A" << std::endl;
    v1 = es1.calculate();
    std::cout << expression1 << " = " << v1 << std::endl;

    v2 = es2.calculate();
    std::cout << expression2 << " = " << v2 << std::endl;

    // Побитовые
    vars["A"] = 1;
    vars["B"] = 2;
    vars["C"] = 4;
    vars["D"] = 8;
    std::string expression4 = "(A|B|C)&D|B";
    EvaluateString<int> es4(&vars, expression4);
    es4.BuildTree();
    int v4 = es4.calculate();
    std::cout << "Побитовые операции с флагами" << std::endl;
    std::cout << expression4 << " = " << v4 << std::endl;

    return 0;
}