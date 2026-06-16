#ifndef GALAXYONFIRE2_ARRAY_H
#define GALAXYONFIRE2_ARRAY_H
#include <cstring>
#include <new>

template<class T>
class Array {
public:
    unsigned int size_; // @0  element count
    T *data_; // @4  backing store (grown with realloc)
    unsigned int capacity_; // @8  allocated element count

    Array() {
        T *p = static_cast<T *>(::operator new[](sizeof(T)));
        *p = T();
        size_ = 0;
        data_ = p;
        capacity_ = 1;
    }

    ~Array() {
        if (data_) ::operator delete[](data_);
        data_ = nullptr;
    }

    // The engine treats elements as trivially relocatable (realloc/memclr). A deep copy keeps the
    // by-value / const& uses (e.g. Array<Matrix>) well-defined.
    Array(const Array &o) {
        capacity_ = o.capacity_ ? o.capacity_ : 1;
        data_ = static_cast<T *>(::operator new[](capacity_ * sizeof(T)));
        size_ = o.size_;
        for (unsigned int i = 0; i < size_; ++i) data_[i] = o.data_[i];
    }

    Array &operator=(const Array &o) {
        if (this != &o) {
            if (data_) ::operator delete[](data_);
            capacity_ = o.capacity_ ? o.capacity_ : 1;
            data_ = static_cast<T *>(::operator new[](capacity_ * sizeof(T)));
            size_ = o.size_;
            for (unsigned int i = 0; i < size_; ++i) data_[i] = o.data_[i];
        }
        return *this;
    }

    unsigned int size() const { return size_; }
    bool empty() const { return size_ == 0; }
    T *data() { return data_; }
    const T *data() const { return data_; }
    T &operator[](unsigned int i) { return data_[i]; }
    const T &operator[](unsigned int i) const { return data_[i]; }
    T *begin() { return data_; }
    T *end() { return data_ + size_; }
    const T *begin() const { return data_; }
    const T *end() const { return data_ + size_; }
    T &front() { return data_[0]; }
    T &back() { return data_[size_ - 1]; }
    T &at(unsigned int i) { return data_[i]; }

    // push_back == ArrayAdd: grow by exactly one (exact-fit realloc), then append.
    void push_back(T item) {
        capacity_ = size_ + 1;
        data_ = static_cast<T *>(realloc(data_, capacity_ * sizeof(T)));
        data_[size_] = item;
        size_ = capacity_;
    }

    // resize == ArraySetLength: (re)allocate only when capacity differs, zero the buffer, set size.
    void resize(unsigned int n) {
        T *p;
        unsigned int c;
        if (capacity_ == n) {
            p = data_;
            c = n;
        } else {
            c = n ? n : 1;
            capacity_ = c;
            p = static_cast<T *>(realloc(data_, c * sizeof(T)));
            c = capacity_;
            data_ = p;
        }
        memset(p, 0, c * sizeof(T));
        size_ = n;
    }

    // clear == ArrayRemoveAll: shrink to a 1-element zeroed buffer.
    void clear() {
        capacity_ = 1;
        size_ = 0;
        data_ = static_cast<T *>(realloc(data_, sizeof(T)));
        memset(data_, 0, capacity_ * sizeof(T));
    }

    void pop_back() { if (size_) --size_; }

    void erase(T *pos) {
        for (T *q = pos; q + 1 != end(); ++q) *q = *(q + 1);
        if (size_) --size_;
    }

    void insert(T *pos, T item) {
        unsigned int idx = static_cast<unsigned int>(pos - data_);
        push_back(item);
        for (unsigned int i = size_ - 1; i > idx; --i) data_[i] = data_[i - 1];
        data_[idx] = item;
    }

    // Range insert (used for bulk byte loads). Only the append-at-end() case occurs in the tree.
    template<class It>
    void insert(T *pos, It first, It last) {
        unsigned int idx = static_cast<unsigned int>(pos - data_);
        for (It it = first; it != last; ++it) {
            insert(data_ + idx, *it);
            ++idx;
        }
    }

    // The engine's Array has no spare capacity to release; clear() already shrinks to one slot.
    void shrink_to_fit() {}
};

// The engine's container free-functions (out-of-line symbols in the original: _Z8ArrayAdd...,
// _Z14ArraySetLength..., _Z14ArrayRemoveAll...). Bodies match the binary so call sites match too.
template<class T>
void ArrayAdd(T item, Array<T> &a) {
    a.capacity_ = a.size_ + 1;
    a.data_ = static_cast<T *>(realloc(a.data_, a.capacity_ * sizeof(T)));
    a.data_[a.size_] = item;
    a.size_ = a.capacity_;
}

template<class T>
void ArraySetLength(unsigned int n, Array<T> &a) { a.resize(n); }

template<class T>
void ArrayRemoveAll(Array<T> &a) { a.clear(); }

#endif // GALAXYONFIRE2_ARRAY_H
