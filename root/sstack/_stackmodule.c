#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

typedef struct STACKNODE
{
    PyObject* item;
    struct STACKNODE* next;
} snode;

typedef struct
{
    PyObject_VAR_HEAD
    snode* head;
    PyObject* weakreflist;
} stackobject;

PyObject* stack_new(PyTypeObject* type, PyObject args, PyObject* kwargs)
{
    stackobject* self;

    self = (stackobject*)type->tp_alloc(type, 0);
    if (self == NULL)
    {
        return NULL;
    }
    self->head = NULL;
    self->weakreflist = NULL;
    Py_SET_SIZE(self, 0);
    return (PyObject*)self;
}

static int stack_init(stackobject* self, PyObject* args, PyObject* kwargs)
{
    return 0;
}

int stack_traverse(stackobject* self, visitproc visit, void* arg)
{
    Py_VISIT(Py_TYPE(self));
    snode* tmp;

    tmp = self->head;
    while (tmp != NULL)
    {
        Py_VISIT(tmp->item);
        tmp = tmp->next;
    }
    return 0;
}

int stack_clear(stackobject* self)
{
    snode* tmp;

    tmp = self->head;
    while (tmp != NULL)
    {
        Py_CLEAR(tmp->item);
        self->head = self->head->next;
        PyMem_FREE(tmp);
        tmp = self->head;
    }
    Py_SET_SIZE(self, 0);
    return 0;
}

PyDoc_STRVAR(stack_clear_doc, "Clears the stack.");

static PyObject* stack_clearmethod(stackobject* self, PyObject* Py_UNUSED(ignored))
{
    stack_clear(self);
    Py_RETURN_NONE;
}

PyDoc_STRVAR(stack_push_doc, "Adds an item to the stack.");

static PyObject* stack_push(stackobject* self, PyObject* object)
{
    snode* node = NULL;
    node = (snode*)PyMem_Malloc(sizeof(snode));
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

PyDoc_STRVAR(stack_peek_doc, "Returns the last element from the stack.");

static PyObject* stack_peek(stackobject* self, PyObject* Py_UNUSED(ignored))
{
    if (self->head == NULL)
    {
        PyErr_SetString(PyExc_IndexError, "peek from empty stack");
        return NULL;
    }
    Py_INCREF(self->head->item);
    return self->head->item;
}

PyDoc_STRVAR(stack_pop_doc, "Returns and removes the last element from the stack.");

static PyObject* stack_pop(stackobject* self, PyObject* Py_UNUSED(ignored))
{
    snode* tmp;
    PyObject* object;

    if (self->head == NULL)
    {
        PyErr_SetString(PyExc_IndexError, "pop from empty stack");
        return NULL;
    }

    tmp = self->head;
    self->head = tmp->next;
    object = tmp->item;
    PyMem_Free(tmp);
    Py_SET_SIZE(self, Py_SIZE(self) - 1);
    return object;
}

static Py_ssize_t stack_len(stackobject* self)
{
    return Py_SIZE(self);
}

static void stack_dealloc(stackobject* self)
{
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

static PyMethodDef stack_methods[] = {
    { "push", (PyCFunction)stack_push, METH_O, stack_push_doc},
    { "pop", (PyCFunction)stack_pop, METH_NOARGS, stack_pop_doc},
    { "peek", (PyCFunction)stack_peek, METH_NOARGS, stack_peek_doc},
    { "clear", (PyCFunction)stack_clearmethod, METH_NOARGS, stack_clear_doc},
    { NULL, NULL, 0, NULL },
};

static PyMemberDef stack_members[] = {
    {"__weaklistoffset__", T_PYSSIZET, offsetof(stackobject, weakreflist), READONLY},
    {NULL},
};

PyDoc_STRVAR(stack_doc,
"Stack() --> stack object\n\
stack container based on a single-linked list.");

static PyType_Slot stack_slots[] = {
    {Py_tp_doc, stack_doc},
    {Py_tp_new, stack_new},
    {Py_tp_init, stack_init},
    {Py_tp_dealloc, stack_dealloc},
    {Py_tp_methods, stack_methods},
    {Py_tp_members, stack_members},
    {Py_tp_traverse, stack_traverse},
    {Py_tp_clear, stack_clear},

    {Py_sq_length, (lenfunc)stack_len},
    {0, NULL},
};

static PyType_Spec stack_spec = {
    .name = "stack.Stack",
    .basicsize = sizeof(stackobject),
    .flags = (Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_HEAPTYPE),
    .itemsize = sizeof(snode),
    .slots = stack_slots,
};

static int stackmodule_exec(PyObject* module)
{
    PyObject* type = PyType_FromModuleAndSpec(module, &stack_spec, NULL);
    if (PyModule_AddType(module, type) < 0)
    {
        Py_XDECREF(&type);
        Py_XDECREF(&module);
        return 0;
    }
    return 0;
};

static struct PyModuleDef_Slot stackmodule_slots[] = {
    {Py_mod_exec, stackmodule_exec},
    {0, NULL},
};

PyDoc_STRVAR(stackmodule_doc, "The simple module with stack container");

static struct PyModuleDef stackmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "stack",
    .m_doc = stackmodule_doc,
    .m_size = 0,
    .m_slots = stackmodule_slots,
};

PyMODINIT_FUNC PyInit__stack(void)
{
    PyModuleDef_Init(&stackmodule);
}
