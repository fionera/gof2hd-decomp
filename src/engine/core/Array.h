#ifndef GALAXYONFIRE2_ARRAY_H
#define GALAXYONFIRE2_ARRAY_H
#include <cstring>
#include <new>

template<class T>
class Array {
public:
    // @0  element count. `size_`/`count` are the same 4-byte uint32 field; the
    // alias lets raw-offset (+0x0) call sites read it as a named member.
    union {
        unsigned int size_;
        unsigned int count;
    };

    // @4  backing store (grown with realloc); raw-offset (+0x4) call sites read this pointer.
    // `data_`/`wantedListData` are the same 4-byte pointer field; the alias lets raw-offset (+0x4)
    // call sites (e.g. Status::getWanted()->data, indexing the Wanted list) read it as a named member.
    union {
        T *data_;
        T *wantedListData;
    };

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
    void shrink_to_fit() {
    }
};

template<class T>
void ArrayAdd(T item, Array<T> &a) {
    a.capacity_ = a.size_ + 1;
    a.data_ = static_cast<T *>(realloc(a.data_, a.capacity_ * sizeof(T)));
    a.data_[a.size_] = item;
    a.size_ = a.capacity_;
}

template<class T>
void ArrayAdd(const T *src, unsigned int count, Array<T> &a) {
    a.capacity_ = a.size_ + count;
    a.data_ = static_cast<T *>(realloc(a.data_, a.capacity_ * sizeof(T)));
    memcpy(a.data_ + a.size_, src, count * sizeof(T));
    a.size_ = a.capacity_;
}

template<class T>
void ArraySet(const T *src, unsigned int count, Array<T> &a) {
    T *p;
    if (a.capacity_ == count) {
        p = a.data_;
    } else {
        a.capacity_ = count ? count : 1;
        p = static_cast<T *>(realloc(a.data_, a.capacity_ * sizeof(T)));
        a.data_ = p;
    }
    memcpy(p, src, count * sizeof(T));
    a.size_ = count;
}

template<class T>
void ArraySet(const Array<T> &src, Array<T> &a) {
    ArraySet(src.data_, src.size_, a);
}

template<class T>
void ArraySetLength(unsigned int n, Array<T> &a) { a.resize(n); }

template<class T>
void ArrayRemoveAll(Array<T> &a) { a.clear(); }

template<class T>
void ArrayRemove(T item, Array<T> &a) {
    unsigned int write = 0;
    for (unsigned int read = 0; read < a.size_; ++read) {
        T cur = a.data_[read];
        if (cur != item)
            a.data_[write++] = cur;
    }
    a.size_ = write;
    unsigned int cap = write ? write : 1;
    a.capacity_ = cap;
    a.data_ = static_cast<T *>(realloc(a.data_, cap * sizeof(T)));
}

template<class T>
void ArrayRelease(Array<T> &a) {
    if (a.data_) ::operator delete[](a.data_);
    a.data_ = nullptr;
}

template<class T>
void ArrayReleaseClasses(Array<T> &a) {
    for (unsigned int i = 0; i < a.capacity_; ++i) {
        if (a.data_[i]) delete a.data_[i];
        a.data_[i] = nullptr;
    }
    if (a.data_) ::operator delete[](a.data_);
    a.data_ = nullptr;
}

template<class T>
void ArrayAddCached(T item, Array<T> &a) {
    if (a.size_ >= a.capacity_) {
        unsigned int oldCap = a.capacity_;
        a.data_ = static_cast<T *>(realloc(a.data_, static_cast<size_t>(oldCap) * 2 * sizeof(T)));
        memset(reinterpret_cast<char *>(a.data_) + static_cast<size_t>(oldCap) * sizeof(T),
               0, static_cast<size_t>(oldCap) * sizeof(T));
        a.capacity_ = oldCap * 2;
    }
    a.data_[a.size_] = item;
    a.size_ = a.size_ + 1;
}

#endif // GALAXYONFIRE2_ARRAY_H
