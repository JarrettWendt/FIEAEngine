# Libray.Python

Python bindings for Library.Shared.

### "Hacky" things being done
Class bindings are being put into an actual class rather than a group of free functions. For example, rather than having the free function
```c++
PyObject* MyClass_DoSomething(MyClass* self, PyObject* args);
```
We have a method
```c++
PyObject* MyClass::DoSomething(PyObject* args);
```

The `self` pointer becomes the `this` pointer. To make this work, a `union` cast is being done for forcibly cast a pointer to a method into a pointer to a free function.

As long as we're force-casting, we may as well discard function parameters not being used. For example, all getters/setters take a `void*` closure as a parameter. Rather than omitting the variable name or marking it with '[[maybe_unused]]' we can actually omit the variable entirely. The "undefined behavior" here is actually very well defined:
- The pointer will be passed anyway. It will exist on the stack. We just won't have a variable to reference it.
- We don't have to worry about a destructor not being called because it is simply a pointer. It has no destructor anyway.
