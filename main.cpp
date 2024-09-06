#include "EvaluateString.h"

int main() {
    EvaluateString<double> esi("((C + D) * (E + F)) * H * (K / L)");

    esi.bindVar("C", 455);
    esi.bindVar("D", 222);
    esi.bindVar("E", 333);
    esi.bindVar("F", 444);
    esi.bindVar("H", 555);
    esi.bindVar("K", 666);
    esi.bindVar("L", 2);

    auto r = esi.calculate();

    return 0;

}