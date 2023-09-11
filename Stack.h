typedef struct Stacknode SNode;

struct StackNode
{
    PyObject* item;
    struct StackNode* next;
};
