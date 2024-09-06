#include "EvaluateString.h"

template<typename T, typename TB>
void EvaluateString<T, TB>::bindVar(const std::string& key, T value)
{
    auto a = &m_vars[key];
    auto it = m_vars.find(key);
    if (it != m_vars.end()) {
        it->second = value;
    }
    else {
        throw std::runtime_error("There is no variable with this name");
    }
}

template<typename T, typename TB>
T EvaluateString<T, TB>::calculate()
{
    return root->calculate();
}

template class EvaluateString<int>;

template class EvaluateString<double>;