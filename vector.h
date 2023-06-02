//
// Created by nikit on 05.05.2023.
//

#ifndef VECTORCHIK_VECTOR_H
#define VECTORCHIK_VECTOR_H

#include <iostream>
#include <sstream>
#include <vector>
#include "raw_memory.h"

namespace bmstu {
    template<typename Type>
    class vector {
    public:
        struct iterator {
            using iterator_category = std::random_access_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = Type;
            using pointer = Type *;
            using reference = Type &;
            iterator(pointer ptr) : m_ptr(ptr) {}
            reference operator*() const {return *m_ptr;}
            pointer operator->() {return m_ptr;}
            iterator &operator++() {
                m_ptr ++;
                return *this;
            }
            iterator &operator--() {
                m_ptr --;
                return *this;
            }
            iterator &operator=(const iterator &other) {
                m_ptr = other.m_ptr;
                return *this;
            }
            iterator operator++(int) {
                iterator tmp = *this;
                ++(*this);
                return tmp;
            }
            iterator operator--(int) {
                iterator tmp = *this;
                --(*this);
                return tmp;
            }
            friend bool operator==(const iterator &a, const iterator &b) {
                return a.m_ptr == b.m_ptr;
            }
            friend bool operator!=(const iterator &a, const iterator &b) {
                return a.m_ptr != b.m_ptr;
            }
            friend ptrdiff_t operator-(const iterator &a, const iterator &b) {
                return (a.m_ptr - b.m_ptr);
            }
            iterator &operator+(size_t n) noexcept {
                m_ptr = m_ptr + n;
                return *this;
            }
            iterator &operator-(size_t n) noexcept {
                m_ptr = m_ptr - n;
                return *this;
            }

        private:
            pointer m_ptr;
        };
        using const_iterator = const iterator;
        vector() noexcept = default;
        vector(size_t size, const Type &value = Type{}): size_(size), capacity_(size), data_(size) {
            for (int i=0; i<size; i++){
                data_[i] = value;
            }
        }
        vector(const vector<Type> &other) : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {  //конструктор копирования элементов вектора
        }
        vector(vector<Type> &&other) : data_(other.data_), size_(other.size_), capacity_(other.capacity_) { //конструктор копирования ссылок на элементы вектора (или перемещения)
        }
        vector(std::initializer_list<Type> ilist) : data_(ilist.size()){
            size_ = ilist.size();
            capacity_ = ilist.size();
            std::copy(ilist.begin(), ilist.end(), data_.get_address());
        }
        void clear() noexcept {
            size_ = 0;
        }
        vector &operator=(const vector<Type> &other) {
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
        }
        vector &operator=(vector<Type> &&other) {
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
        }
        iterator begin() noexcept {
            return iterator(data_.get_address());
        }
        iterator end() noexcept {
            return iterator(data_.get_address() + size_);
        }
        const_iterator begin() const noexcept {
            return iterator(data_.get_address());
        }
        const_iterator end() const noexcept {
            return iterator(data_.get_address() + size_);
        }
        const_iterator cbegin() const noexcept {
            return iterator(data_.get_address());
        }
        const_iterator cend() const noexcept {
            end();
        }
        typename iterator::reference operator[](size_t index) noexcept {
            if (index <= size_){
                return data_[index];
            } else {throw std::out_of_range("Index is out of range");}
        }
        typename const_iterator::reference operator[](size_t index) const noexcept {
            if (index <= size_){
                return data_[index];
            } else {throw std::out_of_range("Index is out of range");}
        }
        typename iterator::reference at(size_t index) {
            if (index <= size_){
                return data_[index];
            } else {throw std::out_of_range("Index is out of range");}
        }
        typename const_iterator::reference at(size_t index) const {
            if (index <= size_){
                return data_[index];
            } else {throw std::out_of_range("Index is out of range");}
        }
        size_t size() const noexcept {return size_;}
        size_t capacity() const noexcept {return capacity_;}
        bool empty() const noexcept {
            return size_ == 0;
        }
        void swap(vector &other) noexcept {
            data_.swap(other.data_);
            size_t tmpcapacity = capacity_;
            capacity_ = other.capacity_;
            other.capacity_ = tmpcapacity;
            size_t tmpsize = size_;
            size_ = other.size_;
            other.size_ = tmpsize;
        }
        friend void swap(vector<Type> &lhs, vector<Type> &rhs) {
            lhs.swap(rhs);
        }
        void resize(size_t new_size) {
            if (size_ == new_size){
                return;
            }
            if (size_ > new_size){
                size_ = new_size;
                return;
            }
            if (capacity_ >= new_size){
                size_ = new_size;
                return;
            }
            if (data_.get_address() != nullptr){
                raw_memory<Type> tmp(new_size);
                std::move(data_.get_address(), data_.get_address() + size_, tmp.get_address());
                data_.~raw_memory();
                data_ = tmp;
                capacity_ = new_size;
                size_ = new_size;
            }
        }
        void reserve(size_t new_capacity) {
            if (new_capacity > capacity_){
                raw_memory<Type> tmp(new_capacity);
                std::copy(data_.get_address(), data_.get_address() + size_, tmp.get_address());
                data_ = tmp;
                capacity_ = new_capacity;
            }
        }
        iterator insert(const_iterator pos, Type &&value) {
            emplace(pos, value);
        }
        iterator insert(const_iterator pos, const Type &value) {
            emplace(pos, value);
        }
        void push_back(const Type &value) {
            if (capacity_ >= size_ + 1){
                data_[size_] = value;
                size_ ++;
            }
            else {
                this->reserve(capacity_ * 2);
                capacity_ = capacity_ * 2;
                data_[size_] = value;
                size_ ++;
            }
        }
        void pop_back() noexcept {
            size_ --;
        }
        template<typename ... Args>
        Type &emplace_back(Args &&... args) { //умеет переделывать rvalue в lvalue в отличие от пушбека (определять тип( как с парой))
            if (size_ == capacity_){
                reserve(2 * capacity_);
            }
            new (&data_[size_]) Type(std::forward<Args>(args)...); //закинули все элементы (куда, какого типа, что)
            size_ ++;
        }
        template<typename ... Args>
        iterator emplace(const_iterator pos, Args &&... args) {
            const size_t dist = pos - this->begin();
            if ((dist > size_) || (dist < 0)){
                throw std::out_of_range("Position is out of range");
            } else{
                if (dist == capacity_){
                    emplace_back(Type(std::forward<Args>(args)...));
                } else {
                    if (size_ == capacity_){
                        reserve(2 * capacity_);
                    }
                    std::move_backward(this->begin() + dist, end(), end() + 1); //сдвинули все
                    iterator itr = &data_[dist];
                    *itr = std::move(Type(std::forward<Args>(args)...)); //нужно получить элемент по индексу dist и туда переместить аргумент
                    size_ ++;
                    return itr;
                }
            }
        }
        iterator erase(const_iterator pos) {
            size_t n = pos - begin();
            Type* tmp_ptr = new Type[capacity_];
            auto first = begin();
            while(first != end()) {
                if (first!=pos) {
                    *tmp_ptr = std::move(*first);
                    ++tmp_ptr;
                }
                ++first;
            }
            std::copy(tmp_ptr - size_ + 1, tmp_ptr + 1, data_.get_address());  //чудо техники
            --size_;
            return begin() + n;
        }

        void push_back(Type &&value) {
            emplace_back(std::forward<Type>(value));
        }
        friend bool operator==(const vector<Type> &l, const vector<Type> &r) {
            if (l.size_ == r.size_){
                for (int i=0; i<r.size_; i++){
                    if (l.data_[i] != r.data_[i]){
                        return false;
                    }
                }
                return true;
            }
            return false;
        }
        friend bool operator!=(const vector<Type> &l, const vector<Type> &r) {
            return !(l == r);
        }
        friend bool operator<(const vector<Type> &l, const vector<Type> &r) {
            if (l != r){
                return !lexicographical_compare_(l, r);
            } return false;
        }
        friend bool operator>(const vector<Type> &l, const vector<Type> &r) {
            if (l != r){
                return lexicographical_compare_(l, r);
            } return false;
        }
        friend bool operator<=(const vector<Type> &l, const vector<Type> &r) {
            return !lexicographical_compare_(l, r) || l == r;
        }
        friend bool operator>=(const vector<Type> &l, const vector<Type> &r) {
            return lexicographical_compare_(l, r) || l == r;
        }
        friend std::ostream &operator<<(std::ostream &os, const vector<Type> &other) {
            for (size_t i=0; i<other.size_; i++){
                os << other.data_[i] << " ";
            }
            return os;
        }
    private:
        static bool lexicographical_compare_(const vector<Type> &l, const vector<Type> &r) {
            size_t index = 0;
            if ((l.data_.get_address() == nullptr) && (r.data_.get_address() == nullptr)){ //НЕ РАБОТАЕТ АРП ЫРАВПРЫРПГЫЖП
                return false;
            }
            while (index < std::min(l.size_, r.size_)){
                if (l.data_[index] < r.data_[index]){
                    return false;
                }
                if (l.data_[index] > r.data_[index]){
                    return true;
                }
                index ++;
            }
            if (index == l.size_ && index < r.size_){
                return false;
            }
            if (index == r.size_ && index < l.size_){
                return true;
            }
        }
        size_t size_ = 0;
        size_t capacity_ = 0;
        raw_memory<Type> data_;
    };
}

#endif //VECTORCHIK_VECTOR_H
