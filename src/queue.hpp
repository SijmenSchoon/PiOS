#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

template <class T>
class queue
{
public:
    bool empty();
    size_t size();
    void push(T val);
    T pop();

private:
    size_t _size;
    size_t _alloc;
    T *_elements;
};

template <class T>
inline bool queue<T>::empty()
{
    return _size == 0;
}

template <class T>
inline size_t queue<T>::size()
{
    return _size;
}

template <class T>
inline void queue<T>::push(T val)
{
    if (_size == _alloc)
    {
        T elements[_alloc * 2];
        for (size_t i = 0; i < _size; i++)
        {
            elements[i] = _elements[i];
        }
        _elements = elements;
        _alloc *= 2;
    }

    _elements[_size++] = val;
}

template <class T>
inline T queue<T>::pop()
{
    T val = _elements[0];
    for (size_t i = 0; i < _size - 1; i++)
    {
        _elements[i] = _elements[i + 1];
    }
    return val;
}

#endif /* end of include guard: QUEUE_H */
