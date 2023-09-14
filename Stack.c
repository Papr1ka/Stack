#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

//#define DEBUG

PyDoc_STRVAR(stack_example_doc, "example(obj, number)\
\
Example function");

struct StackNode
{
    PyObject* item;
    struct StackNode* next;
};

typedef struct
{
    PyObject_VAR_HEAD
    struct StackNode* head;
    PyObject* weakreflist;
} Stack;

PyObject* stack_new(PyTypeObject* type, PyObject* args, PyObject* kwargs)
{
#ifdef DEBUG
    printf("stack_new method called\n");
#endif // DEBUG
    Stack* self;
    self = (Stack*)type->tp_alloc(type, 0);
    if (self != NULL)
    {
        self->head = NULL;
        self->weakreflist = NULL;
        Py_SET_SIZE(self, 0);
    }
    return (PyObject*)self;
}

static int stack_init(Stack* self, PyObject* args, PyObject* kwargs)
{
#ifdef DEBUG
    printf("stack_init method called\n");
#endif // DEBUG

    return 0;
}

int stack_traverse(Stack* self, visitproc visit, void* arg)
{
#ifdef DEBUG
    printf("traverse method called\n");
#endif // DEBUG
    Py_VISIT(Py_TYPE(self));
    struct StackNode* tmp;
    tmp = self->head;
    while (tmp != NULL)
    {
        Py_VISIT(tmp->item);
        tmp = self->head->next;
    }
    return 0;
}

int stack_clear(Stack* self)
{
#ifdef DEBUG
    printf("clear method called\n");
#endif // DEBUG
    struct StackNode* tmp;
    tmp = self->head;
    while (self->head != NULL)
    {
        Py_CLEAR(self->head->item);
        tmp = self->head->next;
        PyMem_FREE(self->head);
        self->head = tmp;
    }
    return 0;
}

static PyObject* stack_push(Stack* self, PyObject* object)
{
#ifdef DEBUG
    printf("push method called\n");
#endif // DEBUG
    struct StackNode* node = NULL;
    node = (struct StackNode*)PyMem_Malloc(sizeof(struct StackNode));
    if (node == NULL)
    {
        PyErr_NoMemory();
        return NULL;
    }
    node->item = object;
    node->next = NULL;
    Py_INCREF(object);
    if (self->head != NULL)
    {
        node->next = self->head;
        self->head = node;
    }
    else
    {
        self->head = node;
    }
    Py_SET_SIZE(self, Py_SIZE(self) + 1);
    Py_RETURN_NONE;
}

static PyObject* stack_peek(Stack* self, PyObject* Py_UNUSED(ignored))
{
#ifdef DEBUG
    printf("peek method called\n");
#endif // DEBUG
    if (self->head == NULL)
    {
        Py_RETURN_NONE;
    }
    return self->head->item;
}

static PyObject* stack_pop(Stack* self, PyObject* PyUnused(ignored))
{
    if (self->head == NULL)
    {
        PyErr_SetString(PyExc_IndexError, "pop from empty stack");
        return NULL;
    }
    struct StackNode* tmp;
    tmp = self->head;
    self->head = (struct StackNode*) tmp->next;
    
    PyObject* object = tmp->item;
    PyMem_Free(tmp);
    Py_DECREF(object);
    Py_SET_SIZE(self, Py_SIZE(self) - 1);
    return object;
}

static void stack_dealloc(Stack* self)
{
#ifdef DEBUG
    printf("stack_dealloc method called\n");
#endif // DEBUG

    PyTypeObject* tp = Py_TYPE(self);

    PyObject_GC_UnTrack(self);

    if (self->weakreflist != NULL)
    {
        PyObject_ClearWeakRefs((PyObject*)self);
    }

    stack_clear(self);

    tp->tp_free((PyObject*)self);
    Py_DECREF(tp);
}

/*
 * List of functions to add to stackobject in exec_stackobject().
 */
static PyMethodDef stack_methods[] = {
    { "push", (PyCFunction)stack_push, METH_O, PyDoc_STR("put the element to stack top")},
    { "pop", (PyCFunction)stack_pop, METH_NOARGS, PyDoc_STR("return and delete the top element from stack")},
    { "peek", (PyCFunction)stack_peek, METH_NOARGS, PyDoc_STR("return to the top stack element")},
    { NULL, NULL, 0, NULL } /* marks end of array */
};

static Py_ssize_t stack_len(Stack* self)
{
    return Py_SIZE(self);
}

static PySequenceMethods stack_as_sequence = {
    .sq_length = (lenfunc) stack_len,
};

static PyTypeObject stack_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "stack.Stack",
    .tp_doc = PyDoc_STR("The simple stack"),
    .tp_basicsize = sizeof(Stack),
    .tp_itemsize = NULL,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC
    //Py_TPFLAGS_HEAPTYPE,
    ,
    .tp_new = stack_new,
    .tp_init = (initproc)stack_init,
    .tp_dealloc = (destructor)stack_dealloc,
    .tp_methods = stack_methods,
    .tp_as_sequence = &stack_as_sequence,
    .tp_traverse = stack_traverse,
    .tp_clear = stack_clear,
    .tp_weaklistoffset = offsetof(Stack, weakreflist),
};

/*
 * Documentation for stackobject.
 */
PyDoc_STRVAR(stack_doc, "The stackobject module");


static PyModuleDef stack_def = {
    PyModuleDef_HEAD_INIT,
    .m_name = "stack",
    .m_doc = stack_doc,
    .m_size = -1,
};

PyMODINIT_FUNC PyInit_stack() {
#ifdef DEBUG
    printf("PyInit_stack method called\n");
#endif // DEBUG

    PyObject* m;

    if (PyType_Ready(&stack_type) < 0)
    {
        return NULL;
    }

    m = PyModule_Create(&stack_def);
    if (m == NULL)
    {
        return NULL;
    }

    Py_INCREF(&stack_type);

    if (PyErr_Occurred() != NULL)
    {
        printf("Some error occured in module stack on inialication\n");
    }

    if (PyModule_AddType(m, &stack_type) < 0)
    {
        Py_XDECREF(&stack_type);
        Py_XDECREF(&m);
        return NULL;
    }

#ifdef DEBUG
    printf("Module stack was initialized\n");
#endif // DEBUG

    return m;
}
