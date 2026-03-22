#include "../inc/Polynimial.hpp"

template <typename T>
Polynomial<T>& Polynomial<T>::operator=(const Polynomial<T>& other)
{
    if(this!=&other)
    {
        variables = other.variables;
        root = cloneTree(other.root.get());
    }
    return *this;
}