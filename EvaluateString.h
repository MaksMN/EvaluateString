#pragma once
#include <string>
#include <stdexcept>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include <queue>
#include "SortedQueue.h"

template <typename T>
class EvaluateString
{
public:
    EvaluateString(std::map<std::string, T>* vars_, const std::string& expression_) : vars(vars_), expression(expression_) {}
    ~EvaluateString() {
        free();
    }
private:
    std::map<std::string, T>* vars;

    // символы которые в исходной строке разрешаются как операторы
    const std::set<char> op_chars = { '~', '!', '%', '^', '&', '=', '|', '<', '>', '/', '*', '-', '+', '\'' };

    // символы которые в исходной строке разрешаются как переменные
    const std::set<char> var_chars =
    {
        '_', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };

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
        {"-'", 11},
        {"~'", 11},
        {"bracket", 100}
    };
    const std::string expression;

    // получает приоритет оператора из карты операторов
    int operator_priority(const std::string& op);

    // получает значение переменной из карты переменных
    T* find_vars(const std::string& var);

    // добавляет символ к последнему элементу вектора
    void vector_add_back(std::vector<std::string>& v, const std::string s);

    // разбивает строку выражения на операторы и переменные в вектор
    std::vector<std::string> expression_to_vector();

    // дерево в строку (в разработке)
    std::string tree_to_string() {
        return std::string();
    };

    int error = 0;
    std::string error_message =
        std::string("EvaluateString error:") +
        std::string("\n\tSource expression: ") + expression;

    // базовая структура узла
    struct node
    {
        int index = 0;
        virtual T calculate() = 0;
        virtual void free() = 0;
    };

    // корень дерева
    node* root = nullptr;

    // узел переменной
    struct node_var final : public node
    {
        node_var(T* value) : value_(value) {}
    private:
        T* value_ = 0;
    public:
        void free() override
        {
            return;
        }
        T calculate() override
        {
            return *value_;
        }
    };

    // базовая структура операторов
    struct node_op : public node
    {
    private:
        int priority = -1;

        node* left_ = nullptr;
        node* right_ = nullptr;
    public:
        void free() override
        {
            if (left_) {
                left_->free();
                delete left_;
            }
            if (right_) {
                right_->free();
                delete right_;
            }
        }
        void add(node* n) {
            if (n == nullptr)
                return;
            if (n->index > this->index) {
                if (right_)
                    static_cast<node_op*>(right_)->add(n);
                else
                    setRightNode(n);
            }
            else if (n->index < this->index) {
                if (left_)
                    static_cast<node_op*>(left_)->add(n);
                else
                    setLeftNode(n);
            }
        }
        void setPriority(int p) {
            priority = p;
        }
        int getPriority() {
            return priority;
        }
        T getLeftValue()
        {
            return left_ ? left_->calculate() : 0;
        };
        T getRightValue() {
            return right_ ? right_->calculate() : 0;
        }
        void setLeftNode(node* value)
        {
            left_ = value;
        };
        void setRightNode(node* value)
        {
            right_ = value;
        };
        node* getLeftNode() {
            return left_;
        }
        node* getRightNode() {
            return right_;
        }
    };

    // узлы операторов
    struct node_logical_or final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() || this->getRightValue();
        }
    };
    struct node_logical_and final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() && this->getRightValue();
        }
    };
    struct node_bitwise_or final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() | this->getRightValue();
        }
    };
    struct node_bitwise_xor final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() ^ this->getRightValue();
        }
    };
    struct node_bitwise_and final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() & this->getRightValue();
        }
    };
    struct node_equal final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() == this->getRightValue();
        }
    };
    struct node_not_equal final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() != this->getRightValue();
        }
    };
    struct node_less final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() < this->getRightValue();
        }
    };
    struct node_less_equal final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() <= this->getRightValue();
        }
    };
    struct node_greater final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() > this->getRightValue();
        }
    };
    struct node_greater_equal final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() >= this->getRightValue();
        }
    };
    struct node_bitwise_left_shift final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() << this->getRightValue();
        }
    };
    struct node_bitwise_right_shift final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() >> this->getRightValue();
        }
    };
    struct node_addition final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() + this->getRightValue();
        }
    };
    struct node_subtraction final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() - this->getRightValue();
        }
    };
    struct node_multiplication final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() * this->getRightValue();
        }
    };
    struct node_division final : public node_op
    {
        node_division(EvaluateString<T>* es) :es_(es) {}
        EvaluateString<T>* es_;
        T calculate() override
        {
            if (this->getRightValue() == 0) {
                es_->error = 1;
                es_->error_message +=
                    "\n\tTree expression: " + es_->tree_to_string() +
                    "\n\tError: \"Division by zero\"";
                throw std::runtime_error(es_->error_message);
            }
            return this->getLeftValue() / this->getRightValue();
        }
    };
    struct node_remainder final : public node_op
    {
        T calculate() override
        {
            return this->getLeftValue() % this->getRightValue();
        }
    };
    struct node_prefix_minus final : public node_op
    {
        T calculate() override
        {
            return -(this->getRightValue());
        }
    };
    struct node_prefix_bitwise_not final : public node_op
    {
        T calculate() override
        {
            return ~(this->getRightValue());
        }
    };

public:
    void BuildTree()
    {
        if (root)
            return;
        SortedQueue<node> nodes_operators;
        std::queue<node*> nodes_vars;
        std::vector<std::string> v;
        try
        {
            v = expression_to_vector();
        }
        catch (const std::exception& e)
        {
            throw; // Переброс исключения
        }
        int i = 0;
        int increase_priority = 0;
        for (auto&& s : v)
        {
            i++;
            bool is_operator = op_chars.contains(s.front());
            bool is_var_char = var_chars.contains(s.front());
            bool is_left_bracket = s.front() == '(';
            bool is_right_bracket = s.front() == ')';
            bool is_bracket = is_left_bracket || is_right_bracket;
            bool is_not_brackets = !is_left_bracket && !is_right_bracket;

            if (is_left_bracket)
            {
                increase_priority += operator_priority("bracket");
                continue;
            }
            if (is_right_bracket)
            {
                increase_priority -= operator_priority("bracket");
                continue;
            }

            if (is_operator) {
                node* n = nullptr;
                if (s == "||") {
                    n = new node_logical_or();
                }
                else if (s == "&&") {
                    n = new node_logical_and();
                }
                else if (s == "|") {
                    n = new node_bitwise_or();
                }
                else if (s == "^") {
                    n = new node_bitwise_xor();
                }
                else if (s == "&") {
                    n = new node_bitwise_and();
                }
                else if (s == "==") {
                    n = new node_equal();
                }
                else  if (s == "!=") {
                    n = new node_not_equal();
                }
                else  if (s == "<") {
                    n = new node_less();
                }
                else  if (s == "<=") {
                    n = new node_less_equal();
                }
                else  if (s == ">") {
                    n = new node_greater();
                }
                else  if (s == ">=") {
                    n = new node_greater_equal();
                }
                else  if (s == "<<") {
                    n = new node_bitwise_left_shift();
                }
                else  if (s == ">>") {
                    n = new node_bitwise_right_shift();
                }
                else  if (s == "+") {
                    n = new node_addition();
                }
                else  if (s == "-") {
                    n = new node_subtraction();
                }
                else  if (s == "*") {
                    n = new node_multiplication();
                }
                else  if (s == "/") {
                    n = new node_division(this);
                }
                else  if (s == "%") {
                    n = new node_remainder();
                }
                else  if (s == "-'") {
                    n = new node_prefix_minus();
                }
                else  if (s == "~'") {
                    n = new node_prefix_bitwise_not();
                }
                if (n) {
                    n->index = i;
                    int priority = operator_priority(s) + increase_priority;
                    static_cast<node_op*>(n)->setPriority(priority);
                    nodes_operators.add(n, priority);
                    continue;
                }

            }
            if (is_var_char) {
                T* var_value = find_vars(s);
                if (var_value) {
                    node* n = new node_var(var_value);
                    n->index = i;
                    nodes_vars.push(n);
                    continue;
                }
                else {
                    error = 3;
                    error_message += "\n\tModule: CreateTree(): \"Variable " + s + " not initialized.\"";
                    throw std::runtime_error(error_message);
                }
            }

        }
        if (nodes_operators.is_empty()) {
            error = 4;
            error_message += "\n\tModule: CreateTree(): \"No operators in expression.\"";
            throw std::runtime_error(error_message);
        }
        if (nodes_vars.empty()) {
            error = 5;
            error_message += "\n\tModule: CreateTree(): \"No variables in expression.\"";
            throw std::runtime_error(error_message);
        }

        root = nodes_operators.get();
        while (!nodes_operators.is_empty())
        {
            node* n = nodes_operators.get();
            static_cast<node_op*>(root)->add(n);
        }
        while (!nodes_vars.empty())
        {
            node* n = nodes_vars.front();
            nodes_vars.pop();
            static_cast<node_op*>(root)->add(n);
        }
    }
    T calculate() {
        if (root)
            return root->calculate();
        else
            return 0;
    }
    void free() {
        if (root)
        {
            root->free();
            delete root;
        }
    }
};

template <typename T>
inline int EvaluateString<T>::operator_priority(const std::string& op)
{
    auto res = operators.find(op);
    return (res != operators.end()) ? res->second : -1;
}

template <typename T>
T* EvaluateString<T>::find_vars(const std::string& var)
{
    auto it = vars->find(var);
    return (it != vars->end()) ? &it->second : nullptr;
}

template<typename T>
inline void EvaluateString<T>::vector_add_back(std::vector<std::string>& v, const std::string s)
{
    if (v.empty()) {
        v.push_back(s);
    }
    else {
        v.back() += s;
    }
}

template<typename T>
inline std::vector<std::string> EvaluateString<T>::expression_to_vector()
{
    std::stack<char> bracketsStack;
    std::vector<std::string> v;
    bool imbalance = false;

    for (char c : expression) {
        if (c == ' ')
            continue;
        char v_back = '\0';
        if (!v.empty())
            v_back = v.back().back();

        bool is_operator = op_chars.contains(c);
        bool is_var_char = var_chars.contains(c);
        bool previous_is_operator = op_chars.contains(v_back);
        bool previous_is_var_char = var_chars.contains(v_back);

        bool is_left_bracket = c == '(';
        bool is_right_bracket = c == ')';
        bool is_bracket = is_left_bracket || is_right_bracket;
        bool is_not_brackets = !is_left_bracket && !is_right_bracket;
        bool previous_is_bracket = v_back == '(' || v_back == ')';
        bool is_minus = c == '-';
        bool is_bitwise_not = c == '~';

        if (!(is_var_char || is_operator || is_bracket)) {
            continue;
        }

        if (is_var_char && !previous_is_operator && !previous_is_bracket)
            vector_add_back(v, std::string(1, c));
        else if (is_var_char && (previous_is_operator || previous_is_bracket))
            v.push_back(std::string(1, c));

        if (is_operator) {
            if ((is_minus || is_bitwise_not) && previous_is_operator) {
                std::string s;
                s.push_back(c);
                s.push_back('\'');
                v.push_back(s);
            }
            else if (previous_is_operator)
                vector_add_back(v, std::string(1, c));
            else
                v.push_back(std::string(1, c));
        }
        if (is_bracket)
            v.push_back(std::string(1, c));

        if (is_left_bracket) {
            bracketsStack.push(c);
        }

        else if (is_right_bracket) {
            if (bracketsStack.empty() || bracketsStack.top() != '(') {
                imbalance = true;
                break;
            }
            bracketsStack.pop();
        }

    }
    if (!bracketsStack.empty() || imbalance) {
        error = 2;
        error_message += "\n\tModule: expression_to_vector(): \"Unbalanced brackets\"";
        throw std::runtime_error(error_message);
    }
    return v;
}
