#include <vector>
#include <map>
#include <stack>
#include <string>

template <typename T>
class EvaluateString
{
private:
    struct node
    {
        /* data */
    };
    struct node_operator : public node
    {

    };
    struct node_value : public node
    {
        /* data */
    };

    std::stack<node> operators_stack;
    std::stack<node> common_stack;
    std::string expression;

    std::vector<T> vars;
    std::map<std::string, T> m_vars;

    // допустимые операторы и их приоритеты
    const std::map<std::string, int> operators
    {
        {"||", 1},
        {"&&", 2},
        {"|", 3},
        {"^", 4},
        {"&", 5},
        {"==", 6},
        {"!=", 6},
        {"<", 7},
        {"<=", 7},
        {">", 7},
        {">=", 7},
        {"<<", 8},
        {">>", 8},
        {"+", 9},
        {"-", 9},
        {"*", 10},
        {"/", 10},
        {"%", 10},
        {"-'", 11}, // унарный минус
        {"~'", 11}, // унарный bitwise not
        {"(", 100},
        {")", -100}
    };


public:
    EvaluateString(/* args */);

};
