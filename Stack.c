#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include "Stack.h"

#define DEBUG

/*
 * Implements an example function.
 */
PyDoc_STRVAR(Stack_example_doc, "example(obj, number)\
\
Example function");

typedef struct
{
    PyObject_HEAD
    struct StackNode* head;
} Stack;

/*static PyMemberDef Custom_members[] = {
    {"item", T_OBJECT_EX, offsetof(StackNode, item), 0, "any python object"},
    {"next", T_OBJECT_EX, offsetof(CustomObject, last), 0, "last name"},
    {"number", T_INT, offsetof(CustomObject, number), 0, "custom number"},
    {NULL}
};*/

PyObject* Stack_new(PyTypeObject* type, PyObject* args, PyObject* kwargs)
{
#ifdef DEBUG
    printf("Stack_new method called\n");
#endif // DEBUG
    Stack* self;
    self = (Stack*)type->tp_alloc(type, 0);
    if (self != NULL)
    {
        self->head = NULL;
    }
    return (PyObject*)self;
}

static int Stack_init(Stack* self, PyObject* args, PyObject* kwargs)
{
#ifdef DEBUG
    printf("Stack_init method called\n");
#endif // DEBUG
    return 0;
}

static PyObject* Stack_push(Stack* self, PyObject* object)
{
    struct StackNode* node = NULL;
    node = (struct StackNode*)PyMem_Malloc(sizeof(struct StackNode));
    if (node == NULL)
    {
        PyErr_SetString(PyExc_MemoryError, "Can't allocate memory for new element");
        return NULL;
    }
    node->item = object;
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

    Py_RETURN_NONE;
}

static PyObject* Stack_pop(Stack* self, PyObject* PyUnused(ignored))
{
    if (self->head == NULL)
    {
        Py_RETURN_NONE;
    }
    struct StackNode* tmp;
    tmp = self->head;
    self->head = (struct StackNode*) tmp->next;
    
    PyObject* object = tmp->item;
    PyMem_Free(tmp);

    return object;
}

static void Stack_dealloc(Stack* self)
{
#ifdef DEBUG
    printf("Stack_dealloc method called\n");
#endif // DEBUG
    struct StackNode* tmp;
    while (self->head != NULL)
    {
        Py_XDECREF(self->head->item);
        tmp = self->head->next;
        PyMem_Free(self->head);
        self->head = tmp;
    }



    Py_TYPE(self)->tp_free((PyObject*)self);
}

/*
 * List of functions to add to Stack in exec_Stack().
 */
static PyMethodDef Stack_methods[] = {
    { "push", (PyCFunction)Stack_push, METH_O, PyDoc_STR("put the element to stack top")},
    { "pop", (PyCFunction)Stack_pop, METH_NOARGS, PyDoc_STR("return and delete the top stack element")},
    { NULL, NULL, 0, NULL } /* marks end of array */
};

static PyTypeObject Stack_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "stack.Stack",
    .tp_doc = PyDoc_STR("The simple stack"),
    .tp_basicsize = sizeof(Stack),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = Stack_new,
    .tp_init = (initproc) Stack_init,
    .tp_dealloc = (destructor) Stack_dealloc,
    .tp_methods = Stack_methods,
};

/*
 * Documentation for Stack.
 */
PyDoc_STRVAR(Stack_doc, "The Stack module");


static PyModuleDef Stack_def = {
    PyModuleDef_HEAD_INIT,
    .m_name = "stack",
    .m_doc = Stack_doc,
    .m_size = -1,              
};

PyMODINIT_FUNC PyInit_stack() {
#ifdef DEBUG
    printf("PyInit_stack method called\n");
#endif // DEBUG

    PyObject* m;

    if (PyType_Ready(&Stack_type) < 0)
    {
        return NULL;
    }

    m = PyModule_Create(&Stack_def);
    if (m == NULL)
    {
        return NULL;
    }

    Py_INCREF(&Stack_type);

    if (PyErr_Occurred() != NULL)
    {
        printf("Some error occured in module stack on itinialication\n");
    }


    if (PyModule_AddObject(m, "stack", (PyObject*) &Stack_type) < 0)
    {
        Py_DECREF(&Stack_type);
        Py_DECREF(&m);
        return NULL;
    }

#ifdef DEBUG
    printf("Module stack was initialized\n");
#endif // DEBUG

    return m;
}
