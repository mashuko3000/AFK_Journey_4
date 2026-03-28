#include "../inc/Polynimial.hpp"

template <typename T>
Polynomial<T>& Polynomial<T>::operator=(const Polynomial<T>& other)
{
    if (this != &other)
    {

        Polynomial<T> temp(other);
        std::swap(this->variables, temp.variables);
        std::swap(this->root, temp.root);
    }

    return *this;
}

