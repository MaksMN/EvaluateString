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
#include <iomanip>

/// @brief Анализ строки арифметического выражения
/// @tparam T тип для основных арифметических операций
/// @tparam TB Тип в который преобразуется T в битовых операциях. Должен быть целочисленным
template <typename T = double, typename TB = long long int>
class EvaluateString
{
private:
#pragma region Base structures
    /// @brief Базовая структура узла дерева
    struct node
    {
        int index = 0;
        virtual T calculate() = 0;
        virtual void add(std::unique_ptr<node> n) = 0;
    };

    /// @brief Базовая структура операторов
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

    /// @brief Структура операнда
    struct operand : public node
    {
        operand(T* value, bool* bind_var) : value_(value), const_value(0), bind(bind_var) {}
        operand(T value) : const_value(value), value_(&const_value), bind(&const_bind) {}
    private:
        bool const_bind = true;
        bool* bind = nullptr;
        const T const_value;
        const T* value_;
    public:
        T calculate() override
        {
            if (bind != nullptr && !*bind)
                throw std::runtime_error("Variable not bound");
            return *value_;
        }
        void add(std::unique_ptr<node> n)  override {}
    };
#pragma endregion

#pragma region operators

    // Структуры операторов

    struct node_logical_or final : public node_operator
    {
        node_logical_or() : node_operator(1) {}
        T calculate() override
        {
            return this->getLeftValue() || this->getRightValue();
        }
    };

    struct node_logical_and final : public node_operator
    {
        node_logical_and() : node_operator(2) {}
        T calculate() override
        {
            return this->getLeftValue() && this->getRightValue();
        }
    };

    struct node_bitwise_or final : public node_operator
    {
        node_bitwise_or() : node_operator(3) {}
        T calculate() override
        {
            return (TB)this->getLeftValue() | (TB)this->getRightValue();
        }
    };

    struct node_bitwise_xor final : public node_operator
    {
        node_bitwise_xor() : node_operator(4) {}
        T calculate() override
        {
            return (TB)this->getLeftValue() ^ (TB)this->getRightValue();
        }
    };

    struct node_bitwise_and final : public node_operator
    {
        node_bitwise_and() : node_operator(5) {}
        T calculate() override
        {
            return (TB)this->getLeftValue() & (TB)this->getRightValue();
        }
    };

    struct node_equal final : public node_operator
    {
        node_equal() : node_operator(6) {}
        T calculate() override
        {
            return this->getLeftValue() == this->getRightValue();
        }
    };

    struct node_not_equal final : public node_operator
    {
        node_not_equal() : node_operator(6) {}
        T calculate() override
        {
            return this->getLeftValue() != this->getRightValue();
        }
    };

    struct node_less final : public node_operator
    {
        node_less() : node_operator(7) {}
        T calculate() override
        {
            return this->getLeftValue() < this->getRightValue();
        }
    };

    struct node_less_equal final : public node_operator
    {
        node_less_equal() : node_operator(7) {}
        T calculate() override
        {
            return this->getLeftValue() <= this->getRightValue();
        }
    };

    struct node_greater final : public node_operator
    {
        node_greater() : node_operator(7) {}
        T calculate() override
        {
            return this->getLeftValue() > this->getRightValue();
        }
    };

    struct node_greater_equal final : public node_operator
    {
        node_greater_equal() : node_operator(7) {}
        T calculate() override
        {
            return this->getLeftValue() >= this->getRightValue();
        }
    };

    struct node_bitwise_left_shift final : public node_operator
    {
        node_bitwise_left_shift() : node_operator(8) {}
        T calculate() override
        {
            return (TB)this->getLeftValue() << (TB)this->getRightValue();
        }
    };

    struct node_bitwise_right_shift final : public node_operator
    {
        node_bitwise_right_shift() : node_operator(8) {}
        T calculate() override
        {
            return (TB)this->getLeftValue() >> (TB)this->getRightValue();
        }
    };

    struct node_addition final : public node_operator
    {
        node_addition() : node_operator(9) {}
        T calculate() override
        {
            return this->getLeftValue() + this->getRightValue();
        }
    };

    struct node_subtraction final : public node_operator
    {
        node_subtraction() : node_operator(9) {}
        T calculate() override
        {
            return this->getLeftValue() - this->getRightValue();
        }
    };

    struct node_multiplication final : public node_operator
    {
        node_multiplication() : node_operator(10) {}
        T calculate() override
        {
            auto v1 = this->getLeftValue();
            auto v2 = this->getRightValue();
            return v1 * v2;
        }
    };

    struct node_division final : public node_operator
    {
        node_division() : node_operator(10) {}
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
        node_remainder() : node_operator(10) {}
        T calculate() override
        {
            return (TB)this->getLeftValue() % (TB)this->getRightValue();
        }
    };

    struct node_prefix_minus final : public node_operator
    {
        node_prefix_minus() : node_operator(11) {}
        T calculate() override
        {
            return -(this->getRightValue());
        }
    };

    struct node_prefix_bitwise_not final : public node_operator
    {
        node_prefix_bitwise_not() : node_operator(11) {}
        T calculate() override
        {
            return ~(TB)(this->getRightValue());
        }
    };

#pragma endregion

    // Вершина дерева
    std::unique_ptr<node> root = nullptr;
    // Стек операторов
    std::stack<std::unique_ptr<node_operator>> operators_stack;
    // Основной стек
    std::stack<std::unique_ptr<node>> common_stack;
    // Строка выражения
    const std::string expression;

    // Карта переменных выражения
    std::map<std::string, T> m_vars;
    // Карта инициализированных переменных выражения
    std::map<std::string, bool> bind_vars;

    // Символы которые в исходной строке разрешаются как операторы
    const std::set<char> operator_chars = { '~', '!', '%', '^', '&', '=', '|', '<', '>', '/', '*', '-', '+' };
    // Символы которые в исходной строке разрешаются как многосимвольные операторы
    const std::set<std::string> operators_str = { "||", "&&", "==", "!=", "<=", ">=", "<<", ">>" };

    // Карта для создания узлов операторов
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
        { "~", []() -> node_operator* { return new node_prefix_bitwise_not(); } },
    };

    // создает узел оператора
    std::unique_ptr<node_operator> createOperator(const std::string& key)
    {
        auto it = registry_operators.find(key);
        if (it != registry_operators.end()) {
            return std::unique_ptr<node_operator>((it->second)());
        }
        return nullptr;
    }

    bool isOperator(const char ch) {
        return operator_chars.contains(ch);
    }

    bool isVariable(const char ch) {
        return isalnum(ch) || ch == '_' || ch == '.' || ch == ',';
    }

    bool isBracket(const char ch) {
        return ch == '(' || ch == ')';
    }

#pragma region Tokens actions
    enum TokenType {
        operators,
        variables,
        brackets,
        no_init
    };
    struct Token
    {
        std::string value;
        TokenType type_ = TokenType::no_init;
        bool isInit() {
            return type_ != TokenType::no_init;
        }
    };
    std::queue<std::unique_ptr<Token>> tokens;
    TokenType getTokenType(const char ch) {
        if (isVariable(ch))
            return TokenType::variables;
        if (isOperator(ch))
            return TokenType::operators;
        if (ch == '(' || ch == ')')
            return TokenType::brackets;
        return TokenType::no_init;
    }
    void createTokens() {
        std::unique_ptr<Token> token;
        std::stack<char> bracketsStack;
        bool imbalance = false;

        for (int i = 0; i < expression.size(); ++i) {
            char c = expression[i];
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
                if (i == expression.size() - 1) {
                    tokens.push(std::move(token));
                    break;
                }
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
#pragma endregion

    // Создает стек операций
    void createStack() {

        int increase = 0;
        int i = 0;
        while (!tokens.empty())
        {
            auto t = std::move(tokens.front());
            i++;
            tokens.pop();

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
                auto var = convertString(t->value);
                std::unique_ptr<operand> v = nullptr;
                if (var)
                    v = std::make_unique<operand>(*var);
                else
                {
                    bind_vars[t->value] = false;
                    v = std::make_unique<operand>(&m_vars[t->value], &bind_vars[t->value]);
                }
                v->index = i;
                common_stack.push(std::move(v));
                continue;
            }

            if (t->type_ == TokenType::operators) {
                auto op = std::move(createOperator(t->value));
                if (!op)
                    throw std::runtime_error("Expression wrong");
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

        while (!operators_stack.empty())
        {
            common_stack.push(std::move(operators_stack.top()));
            operators_stack.pop();
        }
    }

    // Создает дерево выражений
    void createTree() {
        if (common_stack.empty()) {
            throw std::runtime_error("Bad expression");
        }
        root = std::move(common_stack.top());
        common_stack.pop();
        while (!common_stack.empty())
        {
            root->add(std::move(common_stack.top()));
            common_stack.pop();
        }
        int s = 0;
    }

    // Функции общего назначения

    // Преобразует строку в тип Т. При неудаче возвращает nullptr
    std::unique_ptr<T> convertString(std::string& str)
    {
        str = str_replace(",", ".", str);
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

    // Функция str_replace для замены подстроки в строке
    std::string str_replace(const std::string& search, const std::string& replace, const std::string& subject) {
        std::string result = subject;
        size_t pos = 0;

        while ((pos = result.find(search, pos)) != std::string::npos) {
            result.replace(pos, search.length(), replace);
            pos += replace.length();  // сдвигаем позицию для дальнейшего поиска
        }

        return result;
    }

    // Перезаписывает в строке символы по шаблону карты <string, T>, меняет string на T
    std::string apply_replacements(const std::map<std::string, T>& replacements, const std::string& subject) {
        std::string result = subject;

        for (const auto& pair : replacements) {
            std::string search = pair.first;
            std::string replace = to_string(pair.second);
            result = str_replace(search, replace, result);
        }

        return result;
    }

    // преобразует T в строку удаляя лишние нули в дробной части
    std::string to_string(T value) {
        std::ostringstream oss;
        // Устанавливаем формат вывода: фиксированное количество знаков после запятой, но без лишних нулей
        oss << std::fixed << std::setprecision(10) << value;

        std::string str = oss.str();

        // Убираем лишние нули в конце дробной части
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);

        // Если число заканчивается на точку, убираем ее
        if (str.back() == '.') {
            str.pop_back();
        }

        return str;
    }
public:
    EvaluateString(const std::string& exp) : expression(exp)
    {
        createTokens();
        createStack();
        createTree();
    }

    void bindVar(const std::string& key, T value)
    {
        auto a = &m_vars[key];
        auto it = m_vars.find(key);
        if (it != m_vars.end()) {
            it->second = value;
            bind_vars[key] = true;
        }
        else {
            throw std::runtime_error("There is no variable with this name");
        }
    }

    std::string getExpression() {
        return apply_replacements(m_vars, expression);
    }

    T calculate() {
        return root->calculate();
    }

};


