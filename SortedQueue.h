/// @brief Создает сортированную очередь. Максимальная сложность O(n^2)
/// @details повторяющиеся элементы ставит в обратном порядке
/// @details ...2a...2b...2c... сортирует в  ...2c2b2a...
/// @details сортировка производится по полю priority в структуре Node
/// @tparam T 
template<typename T>
class SortedQueue
{
private:
    struct Node
    {
        Node(T* v, int priority_) : value(v), priority(priority_) {};
        T* value = nullptr;
        int priority = -1;
        Node* next = nullptr;
    };
    Node* head = nullptr;

public:
    ~SortedQueue(){
        free();
    }
    void free() {
        if (is_empty())
            return;
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    bool is_empty() {
        return head == nullptr;
    }
    T* get(){
        if (is_empty())
            return nullptr;
        T* res = head->value;
        Node* temp = head;
        head = head->next;
        delete temp;
        return res;
    }
    void add(T* value, int p) {
        Node* n = new Node(value, p);
        // если очередь пустая, кладем в нее элемент и выходим
        if (is_empty()) {
            head = n;
            return;
        }
        // если в очереди есть элементы
        Node* current = head;
        Node* previous = head;
        Node* next = current->next;
        while (current)
        {
            // при одинаковом приоритете добавить элемент перед.
            // если текущий был первым - сделать вставляемый первым.
            if (n->priority <= current->priority) {
                n->next = current;
                if (current == head) {
                    head = n;
                }
                else {
                    previous->next = n;
                }
                return;
            }
            if (n->priority > current->priority) {
                if (next) {
                    previous = current;
                    current = next;
                    next = current->next;
                    continue;
                }
                else {
                    current->next = n;
                    return;
                }
            }
        }
    }
};