#include <vector>
#include <map>
#include <stack>
#include <string>
#include <stdexcept>

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
    const std::string expression;

    std::vector<T> vars;
    std::map<std::string, T*> m_vars;

    // допустимые операторы и их приоритеты
    const std::map<std::string, int> operators
    {
        { "||", 1 },
        { "&&", 2 },
        { "|", 3 },
        { "^", 4 },
        { "&", 5 },
        { "==", 6 },
        { "!=", 6 },
        { "<", 7 },
        { "<=", 7 },
        { ">", 7 },
        { ">=", 7 },
        { "<<", 8 },
        { ">>", 8 },
        { "+", 9 },
        { "-", 9 },
        { "*", 10 },
        { "/", 10 },
        { "%", 10 },
        { "-'", 11 }, // унарный минус
        { "~'", 11 }, // унарный bitwise not
        { "(", 100 },
        { ")", -100 }
    };

public:
    EvaluateString(const std::string& exp) : expression(exp) {}

    void bindVars(std::vector<T>& vars) {
        if (this->vars.size() < vars.size()) {
            throw std::length_error("Vector exceeds maximum allowed length.");
        }
        for (size_t i = 0; i < this->vars.size(); i++)
        {
            this->vars[i] = vars[i];
        }
    }

};
