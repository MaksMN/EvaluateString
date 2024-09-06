#include <vector>
#include <map>
#include <stack>
#include <string>
#include <stdexcept>
#include <memory>
#include <functional>
#include <set>
#include <queue>
#include <cctype>
#include <sstream> 

template <typename T, typename TB = int>
class EvaluateString
{
private:

    // базовая структура
    struct node
    {
        int index = 0;
        virtual T calculate() = 0;
        virtual void add(std::unique_ptr<node> n) = 0;
    };

    // структура оператора
    struct node_operator : public node
    {
    protected:
        int priority;
        std::unique_ptr<node> left_ = nullptr;
        std::unique_ptr<node> right_ = nullptr;
        T default_r = 0;
        T default_l = 0;

    public:
        node_operator(int _priority) : priority(_priority) {}
        virtual ~node_operator() = default;

        void setLeftNode(std::unique_ptr<node> value)
        {
            left_ = std::move(value);
        };
        void setRightNode(std::unique_ptr<node> value)
        {
            right_ = std::move(value);
        };
        std::unique_ptr<node> getLeftNode() {
            return std::move(left_);
        }
        std::unique_ptr<node> getRightNode() {
            return std::move(right_);
        }

        void add(std::unique_ptr<node> n)  override
        {
            if (n == nullptr)
                return;
            if (n->index > this->index) {
                if (right_)
                    right_->add(std::move(n));
                else
                    setRightNode(std::move(n));
            }
            else if (n->index < this->index) {
                if (left_)
                    left_->add(std::move(n));
                else
                    setLeftNode(std::move(n));
            }
        }

        void setPriority(int p) {
            priority = p;
        }
        int getPriority() {
            return priority;
        }

        void increasePriority(int increase_) {
            priority += increase_;
        }

        T getLeftValue()
        {
            return left_ ? left_->calculate() : default_l;
        };
        T getRightValue() {
            return right_ ? right_->calculate() : default_r;
        }
    };

    struct operand : public node
    {
        operand(T* value) : value_(value) {}
    private:
        T* value_ = 0;
    public:
        T calculate() override
        {
            return *value_;
        }
        void add(std::unique_ptr<node> n)  override {}
    };

#pragma region operators

    struct node_logical_or final : public node_operator
    {
        node_logical_or(int increase = 0) : node_operator(1 + increase) {}
        T calculate() override
        {
            return this->getLeftValue() || this->getRightValue();
        }
    };

    struct node_logical_and final : public node_operator
    {
        node_logical_and(int increase = 0) : node_operator(2 + increase) {}
        T calculate() override
        {
            return this->getLeftValue() && this->getRightValue();
        }
    };

    struct node_bitwise_or final : public node_operator
    {
        node_bitwise_or(int increase = 0) : node_operator(3 + increase) {}
        T calculate() override
        {
            return (TB)this->getLeftValue() | (TB)this->getRightValue();
        }
    };

    struct node_bitwise_xor final : public node_operator
    {
        node_bitwise_xor(int increase = 0) : node_operator(4 + increase) {}
        T calculate() override
        {
            return (TB)this->getLeftValue() ^ (TB)this->getRightValue();
        }
    };

    struct node_bitwise_and final : public node_operator
    {
        node_bitwise_and(int increase = 0) : node_operator(5 + increase) {}
        T calculate() override
        {
            return (TB)this->getLeftValue() & (TB)this->getRightValue();
        }
    };

    struct node_equal final : public node_operator
    {
        node_equal(int increase = 0) : node_operator(6 + increase) {}
        T calculate() override
        {
            return this->getLeftValue() == this->getRightValue();
        }
    };

    struct node_not_equal final : public node_operator
    {
        node_not_equal(int increase = 0) : node_operator(6 + increase) {}
        T calculate() override
        {
            return this->getLeftValue() != this->getRightValue();
        }
    };

    struct node_less final : public node_operator
    {
        node_less(int increase = 0) : node_operator(7 + increase) {}
        T calculate() override
        {
            return this->getLeftValue() < this->getRightValue();
        }
    };

    struct node_less_equal final : public node_operator
    {
        node_less_equal(int increase = 0) : node_operator(7 + increase) {}
        T calculate() override
        {
            return this->getLeftValue() <= this->getRightValue();
        }
    };

    struct node_greater final : public node_operator
    {
        node_greater(int increase = 0) : node_operator(7 + increase) {}
        T calculate() override
        {
            return this->getLeftValue() > this->getRightValue();
        }
    };

    struct node_greater_equal final : public node_operator
    {
        node_greater_equal(int increase = 0) : node_operator(7 + increase) {}
        T calculate() override
        {
            return this->getLeftValue() >= this->getRightValue();
        }
    };

    struct node_bitwise_left_shift final : public node_operator
    {
        node_bitwise_left_shift(int increase = 0) : node_operator(8 + increase) {}
        T calculate() override
        {
            return (TB)this->getLeftValue() << (TB)this->getRightValue();
        }
    };

    struct node_bitwise_right_shift final : public node_operator
    {
        node_bitwise_right_shift(int increase = 0) : node_operator(8 + increase) {}
        T calculate() override
        {
            return (TB)this->getLeftValue() >> (TB)this->getRightValue();
        }
    };

    struct node_addition final : public node_operator
    {
        node_addition(int increase = 0) : node_operator(9 + increase) {}
        T calculate() override
        {
            return this->getLeftValue() + this->getRightValue();
        }
    };

    struct node_subtraction final : public node_operator
    {
        node_subtraction(int increase = 0) : node_operator(9 + increase) {}
        T calculate() override
        {
            return this->getLeftValue() - this->getRightValue();
        }
    };

    struct node_multiplication final : public node_operator
    {
        node_multiplication(int increase = 0) : node_operator(10 + increase) {}
        T calculate() override
        {
            auto v1 = this->getLeftValue();
            auto v2 = this->getRightValue();
            return v1 * v2;
        }
    };

    struct node_division final : public node_operator
    {
        node_division(int increase = 0) : node_operator(10 + increase) {}
        T calculate() override
        {
            auto v1 = this->getLeftValue();
            auto v2 = this->getRightValue();
            if (v2 == 0) {
                std::string error_message =
                    "\n\tError: \"Division by zero\"";
                throw std::runtime_error(error_message);
            }
            return v1 / v2;
        }
    };

    struct node_remainder final : public node_operator
    {
        node_remainder(int increase = 0) : node_operator(10 + increase) {}
        T calculate() override
        {
            return (TB)this->getLeftValue() % (TB)this->getRightValue();
        }
    };

    struct node_prefix_minus final : public node_operator
    {
        node_prefix_minus(int increase = 0) : node_operator(11 + increase) {}
        T calculate() override
        {
            return -(this->getRightValue());
        }
    };

    struct node_prefix_bitwise_not final : public node_operator
    {
        node_prefix_bitwise_not(int increase = 0) : node_operator(11 + increase) {}
        T calculate() override
        {
            return ~(TB)(this->getRightValue());
        }
    };

#pragma endregion

    std::unique_ptr<node> root = nullptr;

    std::stack<std::unique_ptr<node_operator>> operators_stack;
    std::stack<std::unique_ptr<node>> common_stack;
    const std::string expression;

    std::vector<T> vars;
    std::map<std::string, T> m_vars;

    // символы которые в исходной строке разрешаются как операторы
    const std::set<char> operator_chars = { '~', '!', '%', '^', '&', '=', '|', '<', '>', '/', '*', '-', '+' };
    const std::set<std::string> operators_str = { "||", "&&", "==", "!=", "<=", ">=", "<<", ">>" };

    const std::map<std::string, std::function<node_operator* ()>> registry_operators
    {
        { "||", []() -> node_operator* { return new node_logical_or(); } },
        { "&&", []() -> node_operator* { return new node_logical_and(); } },
        { "|", []() -> node_operator* { return new node_bitwise_or(); } },
        { "^", []() -> node_operator* { return new node_bitwise_xor(); } },
        { "&", []() -> node_operator* { return new node_bitwise_and(); } },
        { "==", []() -> node_operator* { return new node_equal(); } },
        { "!=", []() -> node_operator* { return new node_not_equal(); } },
        { "<", []() -> node_operator* { return new node_less(); } },
        { "<=", []() -> node_operator* { return new node_less_equal(); } },
        { ">", []() -> node_operator* { return new node_greater(); } },
        { ">=", []() -> node_operator* { return new node_greater_equal(); } },
        { "<<", []() -> node_operator* { return new node_bitwise_left_shift(); } },
        { ">>", []() -> node_operator* { return new node_bitwise_right_shift(); } },
        { "+", []() -> node_operator* { return new node_addition(); } },
        { "-", []() -> node_operator* { return new node_subtraction(); } },
        { "*", []() -> node_operator* { return new node_multiplication(); } },
        { "/", []() -> node_operator* { return new node_division(); } },
        { "%", []() -> node_operator* { return new node_remainder(); } },
        //{ "-'", []() -> node* { return new node_prefix_minus(); } },
        { "~", []() -> node_operator* { return new node_prefix_bitwise_not(); } },
    };

    bool isOperator(const char ch) {
        return operator_chars.contains(ch);
    }

    bool isVariable(const char ch) {
        return isalnum(ch) || ch == '_';
    }

    bool isBracket(const char ch) {
        return ch == '(' || ch == ')';
    }

    // создает узел оператора
    std::unique_ptr<node_operator> createOperator(const std::string& key)
    {
        auto it = registry_operators.find(key);
        if (it != registry_operators.end()) {
            return std::unique_ptr<node_operator>((it->second)());
        }
        return nullptr;
    }

    // преобразует строку в тип Т. При неудаче возвращает nullptr
    std::unique_ptr<T> convertString(const std::string& str) {
        std::istringstream iss(str);
        T value;
        iss >> value;
        if (!iss.fail() && iss.eof()) {
            return std::make_unique<T>(value);
        }
        else {
            return nullptr;
        }
    }

    enum TokenType {
        operators,
        variables,
        brackets,
        no_init
    };

    TokenType getTokenType(const char ch) {
        if (isVariable(ch))
            return TokenType::variables;
        if (isOperator(ch))
            return TokenType::operators;
        if (ch == '(' || ch == ')')
            return TokenType::brackets;
        return TokenType::no_init;
    }

    struct Token
    {
        std::string value;
        TokenType type_ = TokenType::no_init;
        bool isInit() {
            return type_ != TokenType::no_init;
        }
    };
    std::queue<std::unique_ptr<Token>> tokens;

    void createTree() {
        root = std::move(common_stack.top());
        common_stack.pop();
        while (!common_stack.empty())
        {
            root->add(std::move(common_stack.top()));
            common_stack.pop();
        }
        int s = 0;
    }

    void createStack() {

        int increase = 0;
        int i = 0;
        while (!tokens.empty())
        {
            auto t = std::move(tokens.front());
            i++;
            tokens.pop();

            while (tokens.empty() && !operators_stack.empty())
            {
                common_stack.push(std::move(operators_stack.top()));
                operators_stack.pop();
            }

            if (t->value == "(")
            {
                increase += 100;
                continue;
            }
            if (t->value == ")")
            {
                increase -= 100;
                continue;
            }

            if (t->type_ == TokenType::variables) {
                auto v = std::make_unique<operand>(&m_vars[t->value]);
                v->index = i;
                common_stack.push(std::move(v));
                continue;
            }

            if (t->type_ == TokenType::operators) {
                auto op = std::move(createOperator(t->value));
                op->index = i;
                op->increasePriority(increase);

                while (!operators_stack.empty() && op->getPriority() <= operators_stack.top()->getPriority()) {
                    common_stack.push(std::move(operators_stack.top()));
                    operators_stack.pop();
                }
                if (operators_stack.empty()) {
                    operators_stack.push(std::move(op));
                    continue;
                }
                if (op->getPriority() > operators_stack.top()->getPriority()) {
                    operators_stack.push(std::move(op));
                    continue;
                }
            }
        }
    }

    void createTokens() {
        std::unique_ptr<Token> token;
        std::stack<char> bracketsStack;
        bool imbalance = false;

        for (char c : expression) {
            if (c == ' ')
                continue;

            TokenType c_type = getTokenType(c);
            if (c_type == TokenType::no_init)
                throw std::runtime_error("Character type could not be determined");

            if (!token)
            {
                token = std::make_unique<Token>();
                token->type_ = c_type;
            }

            // если токен инициирован и тип символа не равен типу токена, 
            // значит создание текущего токена завершено
            if (token->type_ != getTokenType(c) && token->isInit()) {
                tokens.push(std::move(token));
                token = std::make_unique<Token>();
                token->type_ = c_type;
            }

            // если символ принадлежит переменным
            // буквы цифры _
            if (isVariable(c))
            {
                token->value += c;
                token->type_ = TokenType::variables;
                continue;
            }

            if (isOperator(c))
            {
                // двусимвольные операторы
                std::string op = token->value + c;
                if (operators_str.contains(op))
                {
                    token->value = op;
                    tokens.push(std::move(token));
                    continue;
                }
                // односимвольные

                // если токен не пустой значит он не прошел проверку двусимвольного оператора
                // его можно закидывать в очередь
                if (!token->value.empty())
                {
                    tokens.push(std::move(token));

                    // текущий оператор может быть унарным, идти после другого оператора
                    token = std::make_unique<Token>();
                    token->value = c;
                    token->type_ = c_type;
                    continue;
                }
                token->value = c;
                token->type_ = c_type;
            }

            if (isBracket(c))
            {
                // баланс скобок
                if (c == '(') {
                    bracketsStack.push(c);
                }
                else if (c == ')') {
                    if (bracketsStack.empty() || bracketsStack.top() != '(') {
                        imbalance = true;
                        break;
                    }
                    bracketsStack.pop();
                }
                token->value += c;
                token->type_ = TokenType::brackets;
                tokens.push(std::move(token));
            }

        }
        if (!bracketsStack.empty() || imbalance) {
            std::string error_message = "\"Unbalanced brackets\"";
            throw std::runtime_error(error_message);
        }
    }

public:
    EvaluateString(const std::string& exp) : expression(exp)
    {
        createTokens();
        createStack();
        createTree();
    }

    void bindVar(const std::string& key, T value);

    T calculate();

};


