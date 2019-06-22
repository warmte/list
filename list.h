#ifndef LIST_LIST_H
#define LIST_LIST_H

#include <iterator>
#include <vector>
#include <cassert>

template <typename T>
class list {
private:
    struct node_base {
        node_base* next_, *prev_;
        node_base(node_base* p = nullptr, node_base* n = nullptr) : next_(n), prev_(p) {}
        virtual ~node_base() = default;
    };

    struct node : node_base {
        T value;
        node(T v, node_base* p = nullptr, node_base* n= nullptr) : node_base(p, n), value(v) {}
    };

    node_base fake_;

public:
    list() noexcept { fake_.next_ = fake_.prev_ = &fake_; }

    list(list const& other) : list() {
        const_iterator cur = other.begin();
        while (cur != other.end()) {
            push_back(*cur);
            cur++;
        }
    }

    list& operator=(list const& other) {
        list<T> buf(other);
        swap(buf, *this);
        return *this;
    }

    ~list() {
        clear();
    }

private:
    template <typename U>
    struct bdr_list_iterator
            : public std::iterator<std::bidirectional_iterator_tag, U> {
    public:
        friend class list<T>;
        bdr_list_iterator() = default;
        bdr_list_iterator(bdr_list_iterator<T> const& other) : ptr(other.ptr) {}
        bdr_list_iterator& operator++() {
            ptr = ptr->next_;
            return *this;
        }
        bdr_list_iterator operator++(int) {
            bdr_list_iterator<U> buf(*this);
            ++(*this);
            return buf;
        }
        bdr_list_iterator& operator--() {
            ptr = ptr->prev_;
            return *this;
        }
        bdr_list_iterator operator--(int) {
            bdr_list_iterator<U> buf(*this);
            --(*this);
            return buf;
        }
        U& operator*() const {
            return static_cast<node*>(ptr)->value;
        }

        U* operator->() const {
            return &static_cast<node*>(ptr)->value;
        }

        template <typename V>
        bool operator==(bdr_list_iterator<V> const& other) const {
            return ptr == other.ptr;
        }
        template <typename V>
        bool operator!=(bdr_list_iterator<V> const& other) const {
            return ptr != other.ptr;
        }

    private:
        bdr_list_iterator(node_base* p) : ptr(p) {}
        node_base* ptr;
    };
public:
    typedef bdr_list_iterator<T> iterator;
    typedef bdr_list_iterator<T const> const_iterator;
    typedef std::reverse_iterator<bdr_list_iterator<T>> reverse_iterator;
    typedef std::reverse_iterator<bdr_list_iterator<T const>> const_reverse_iterator;


    iterator begin() {
        return iterator(fake_.next_);
    }
    const_iterator begin() const {
        return iterator(fake_.next_);

    }
    iterator end() {
        return iterator(&fake_);
    }
    const_iterator end() const {
        return const_iterator(const_cast<node_base*>(&fake_));
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    void push_back(T const& val) {
        insert(end(), val);
    }

    void pop_back() {
        erase(--end());
    }

    T& back() {
        return *(--end());
    }
    T const& back() const {
        return *(--end());
    }

    void push_front(T const& val) {
        insert(begin(), val);
    }
    void pop_front() {
        erase(begin());
    }

    T& front() {
        return *begin();
    }
    T const& front() const {
        return *begin();
    }

    bool empty() const {
        return &fake_ ==fake_.prev_;
    }

    void clear() {
        node_base* cur = fake_.next_;
        while (cur != &fake_) {
            node_base* to_del = cur;
            cur = cur->next_;
            delete to_del;
        }
        fake_.next_ = fake_.prev_ = &fake_;
    }

    iterator insert(const_iterator pos, T const& value) {
        auto p1 = pos;
        auto cur = new node(value, p1.ptr->prev_, p1.ptr);
        p1.ptr->prev_ = cur;
        cur->next_->prev_ = cur;
        cur->prev_->next_ = cur;
        return cur;
    }

    iterator erase(const_iterator pos) {
        pos.ptr->prev_->next_ = pos.ptr->next_;
        pos.ptr->next_->prev_ = pos.ptr->prev_;
        iterator res(pos.ptr->next_);
        delete pos.ptr;
        return res;
    }

    void splice(const_iterator pos,
                list<T>& other,
                const_iterator first,
                const_iterator last) {
        node_base* cur = first.ptr->prev_;

        pos.ptr->prev_->next_ = first.ptr;
        first.ptr->prev_ = pos.ptr->prev_;
        last.ptr->prev_->next_ = pos.ptr;
        pos.ptr->prev_ = last.ptr->prev_;
        last.ptr->prev_ = cur;
        cur->next_ = last.ptr;
    }

    friend void swap(list &a, list &b) {
        auto ah = a.fake_.prev_, at = a.fake_.next_, bh = b.fake_.prev_, bt = b.fake_.next_;

        ah->next_ = at->prev_ = &b.fake_;
        bh->next_ = bt->prev_ = &a.fake_;
        std::swap(a.fake_, b.fake_);
    }
};

#endif //LIST_LIST_H
