#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H

#include <iterator>

template<typename T>
struct vector {
    static const size_t STARTING_ARRAY_CAPACITY = 64;

    typedef T value_type;
    typedef T *iterator;
    typedef T const *const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    vector() : is_small(false), big(nullptr) {
    }

    vector(vector<T> const &rhs) : is_small(rhs.is_small) {
        if (rhs.is_small) {
            new(&small) T(rhs.small);
        } else {
            big = rhs.big;
            if (big != nullptr) {
                big->users++;
            }
        }
    }

    ~vector() {
        if (is_small) {
            small.~T();
        } else if (big && --big->users == 0) {
            for (size_t i = 0; i < size(); i++) {
                big->elements[i].~T();
            }
            operator delete(big);
            big = nullptr;
        }
    }

    template<typename InputIterator>
    vector(InputIterator first, InputIterator last) : vector() {
        while (first != last) {
            push_back(*first);
            first++;
        }
    }

    vector &operator=(vector const &rhs) {
        if (!is_small) {
            if (big && --big->users == 0) {
                for (size_t i = 0; i < size(); i++) {
                    big->elements[i].~T();
                }
                operator delete(big);
            }
            big = nullptr;
        }
        is_small = rhs.is_small;
        if (rhs.is_small) {
            new(&small) T(rhs.small);
        } else {
            big = rhs.big;
            if (big != nullptr) {
                big->users++;
            }
        }
        return *this;
    }

    template<typename InputIterator>
    void assign(InputIterator first, InputIterator last) {
        get_control();
        clear();
        while (first != last) {
            push_back(*first);
            first++;
        }
    }

    T const &operator[](size_t ind) const {
        return is_small ? small : big->elements[ind];
    }

    T &operator[](size_t ind) {
        get_control();
        return is_small ? small : big->elements[ind];
    }

    T const &front() const {
        return operator[](0);
    }

    T &front() {
        return operator[](0);
    }

    T const &back() const {
        return operator[](size() - 1);
    }

    T &back() {
        return operator[](size() - 1);
    }

    void push_back(T const &value) {
        get_control();
        if (size() == 0 && !big) {
            new(&small) T(value);
            is_small = true;
        } else if (is_small) {
            auto *new_big = static_cast<big_object *> (operator new(
                    sizeof(big_object) + STARTING_ARRAY_CAPACITY * sizeof(T)));
            new_big->size = 2;
            new_big->capacity = STARTING_ARRAY_CAPACITY;
            new_big->users = 1;
            try {
                new(&new_big->elements[0]) T(small);
                try {
                    new(&new_big->elements[1]) T(value);
                } catch (std::exception const &e) {
                    new_big->elements[0].~T();
                    throw;
                }
            } catch (std::exception const &e) {
                operator delete(new_big);
                throw;
            }
            small.~T();
            big = new_big;
            is_small = false;
        } else {
            if (big->size == big->capacity) {
                auto *new_big = static_cast<big_object *> (operator new(
                        sizeof(big_object) + 2 * big->capacity * sizeof(T)));
                new_big->size = big->size + 1;
                new_big->capacity = 2 * big->capacity;
                new_big->users = 1;
                for (size_t i = 0; i < size(); i++) {
                    new(&new_big->elements[i]) T(big->elements[i]);
                    big->elements[i].~T();
                }
                operator delete(big);
                big = new_big;
            }
            new(&big->elements[size()]) T(value);
            big->size++;
        }
    }

    size_t size() const noexcept {
        return is_small ? 1 : big ? big->size : 0;
    }

    void pop_back() {
        get_control();
        if (is_small) {
            small.~T();
            is_small = false;
        } else {
            big->elements[--big->size].~T();
        }
    }

    T const *data() const {
        return is_small ? &small : big ? big->elements : nullptr;
    }

    T *data() {
        return is_small ? &small : big ? big->elements : nullptr;
    }

    iterator begin() {
        return data();
    }

    iterator end() {
        return data() + size();
    }

    reverse_iterator rbegin() {
        get_control();
        return reverse_iterator(end());
    }

    reverse_iterator rend() {
        get_control();
        return reverse_iterator(begin());
    }

    const_iterator begin() const noexcept {
        return data();
    }

    const_iterator end() const noexcept {
        return data() + size();
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    bool empty() const {
        return size() == 0;
    }

    void reserve(size_t capacity) {
        get_control();
        if (capacity <= vector::capacity()) {
            return;
        }
        if (is_small) {
            auto *new_big = static_cast<big_object *> (operator new(
                    sizeof(big_object) + capacity * sizeof(T)));
            new_big->size = 1;
            new_big->capacity = capacity;
            new_big->users = 1;
            try {
                new(&new_big[0]) T(small);
            } catch (std::exception const &e) {
                operator delete(new_big);
                throw;
            }
            small.~T();
            big = new_big;
        } else {
            auto *new_big = static_cast<big_object *> (operator new(
                    sizeof(big_object) + capacity * sizeof(T)));
            new_big->size = size();
            new_big->capacity = capacity;
            new_big->users = 1;
            size_t succ = 0;
            try {
                for (size_t i = 0; i < size(); i++) {
                    new(&new_big->elements[i]) T(big->elements[i]);
                    succ++;
                }
            } catch (std::exception const &e) {
                for (size_t i = 0; i < succ; i++) {
                    new_big->elements[i].~T();
                }
                operator delete(new_big);
                throw;
            }
            for (size_t i = 0; i < size(); i++) {
                big->elements[i].~T();
            }
            operator delete(big);
            big = new_big;
        }
    }

    size_t capacity() const {
        return is_small ? 1 : big ? big->capacity : 0;
    }

    void shrink_to_fit() {
        get_control();
        if (!is_small) {
            reserve(size());
        }
    }

    void resize(size_t size, T const &value) {
        get_control();
        while (vector::size() > size) {
            pop_back();
        }
        while (vector::size() < size) {
            push_back(value);
        }
    }

    void clear() {
        get_control();
        while (size() > 0) {
            pop_back();
        }
    }

    iterator insert(const_iterator pos, T const &value) {
        size_t dist = pos - begin();
        get_control();
        push_back(value);
        for (iterator it = end() - 1, lft = begin() + dist; it > lft; it--) {
            std::swap(*it, *(it - 1));
        }
        return begin() + dist;
    }

    iterator erase(const_iterator first, const_iterator last) {
        if (first == last) {
            return begin() + (first - begin());
        }
        size_t dist1 = first - begin();
        size_t dist2 = last - begin();
        get_control();
        if (is_small) {
            small.~T();
            return begin();
        }
        size_t shift = last - first;
        for (iterator it = begin() + dist2; it != end(); it++) {
            *(it - shift) = *it;
        }
        for (size_t i = 0; i < shift; i++) {
            pop_back();
        }
        return begin() + dist1;
    }

    iterator erase(const_iterator pos) {
        get_control();
        return erase(pos, pos + 1);
    }

    template<typename Type>
    friend void swap(vector<Type> &a, vector<Type> &b);

    template<typename Type>
    friend bool operator==(vector<Type> const &, vector<Type> const &);

    template<typename Type>
    friend bool operator!=(vector<Type> const &, vector<Type> const &);

    template<typename Type>
    friend bool operator<(vector<Type> const &, vector<Type> const &);

    template<typename Type>
    friend bool operator>(vector<Type> const &, vector<Type> const &);

    template<typename Type>
    friend bool operator<=(vector<Type> const &, vector<Type> const &);

    template<typename Type>
    friend bool operator>=(vector<Type> const &, vector<Type> const &);

private:
    struct big_object {
        size_t size;
        size_t capacity;
        size_t users;
        T elements[];
    };
    bool is_small;
    union {
        big_object *big;
        T small;
    };

    void get_control() {
        if (!is_small && big && big->users != 1) {
            big->users--;
            auto *new_big = static_cast<big_object *> (operator new(
                    sizeof(big_object) + capacity() * sizeof(T)));
            new_big->size = size();
            new_big->capacity = capacity();
            new_big->users = 1;
            size_t succ = 0;
            try {
                for (size_t i = 0; i < size(); i++) {
                    new(&new_big->elements[i]) T(big->elements[i]);
                    succ++;
                }
            } catch (std::exception const &e) {
                for (size_t i = 0; i < succ; i++) {
                    new_big->elements[i].~T();
                }
                operator delete(new_big);
                throw;
            }
            big = new_big;
        }
    }
};

template<typename T>
void swap(vector<T> &a, vector<T> &b) {
    if (a.is_small) {
        if (b.is_small) {
            std::swap(a.small, b.small);
        } else {
            auto *t = b.big;
            new(&b.small) T(a.small);
            a.small.~T();
            a.big = t;
        }
    } else {
        if (b.is_small) {
            swap(b, a);
            return;
        } else {
            std::swap(a.big, b.big);
        }
    }
    std::swap(a.is_small, b.is_small);
}

template<typename T>
bool operator==(vector<T> const &a, vector<T> const &b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

template<typename T>
bool operator!=(vector<T> const &a, vector<T> const &b) {
    return !(a == b);
}

template<typename T>
bool operator<(vector<T> const &a, vector<T> const &b) {
    for (size_t i = 0; i < std::min(a.size(), b.size()); i++) {
        if (a[i] != b[i]) {
            return a[i] < b[i];
        }
    }
    return a.size() < b.size();
}

template<typename T>
bool operator>(vector<T> const &a, vector<T> const &b) {
    return b < a;
}

template<typename T>
bool operator<=(vector<T> const &a, vector<T> const &b) {
    return !(b < a);
}

template<typename T>
bool operator>=(vector<T> const &a, vector<T> const &b) {
    return !(a < b);
}


#endif //VECTOR_VECTOR_H
