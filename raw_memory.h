#include <iostream>

template<typename T>
class raw_memory {
public:
    raw_memory();
    explicit raw_memory(size_t cap) {
        capacity_ = cap;
        buffer_ = new T[capacity_];
    }
    raw_memory(const raw_memory &other){
        if (other.buffer_ == nullptr) {
            buffer_ = nullptr;
            capacity_ = other.capacity_;
        }
        else {
            buffer_ = new T[other.capacity_];
            capacity_ = other.capacity_;
            std::copy(other.buffer_, other.buffer_ + capacity_, buffer_);
        }
    }
    raw_memory(raw_memory &&other) {
        capacity_ = other.capacity_;
        buffer_ = other.buffer_;
        other.capacity_ = 0;
        other.buffer_ = nullptr;
    }
    raw_memory &operator=(raw_memory &&other) noexcept {
        if (other.buffer_ == nullptr){
            auto tmp = buffer_;
            buffer_ = other.buffer_;
            delete[] tmp;
            capacity_ = other.capacity_;
            return *this;
        }
        delete[] buffer_;
        buffer_ = other.buffer_;
        capacity_ = other.capacity_;
        other.buffer_ = nullptr;
        other.capacity_ = 0;
        return *this;
    }

    raw_memory &operator=(const raw_memory &other){
        if (other.buffer_ == nullptr){
            delete[] buffer_;
            buffer_ = other.buffer_;
            capacity_ = other.capacity_;
            return *this;
        }
        auto tmp = buffer_;
        buffer_ = nullptr;
        buffer_ = new T[other.capacity_];
        capacity_ = other.capacity_;
        std::copy(other.buffer_, other.buffer_ + capacity_, buffer_);
        delete[] tmp;
        return *this;
    }

    T &operator[](size_t index) noexcept {
        if ((index <= capacity_) && (buffer_ != nullptr)){
            return buffer_[index];
        }
        else {throw std::logic_error("index must not be > size");}
    }
    const T &operator[](size_t index) const noexcept {
        if ((index <= capacity_) && (buffer_ != nullptr)){
            return buffer_[index];
        }
        else {throw std::logic_error("index must not be > size");}
    }
    ~raw_memory() {
        delete[] buffer_;
        capacity_ = 0;
    }
    void swap(raw_memory &other) noexcept {
        auto tmpbuffer = buffer_;
        size_t tmpcapacity = capacity_;
        buffer_ = other.buffer_;
        capacity_ = other.capacity_;
        other.buffer_ = tmpbuffer;
        other.capacity_ = tmpcapacity;
    }
    size_t capacity() {
        return capacity_;
    }
    T* get_address() const {
        return buffer_;
    }
private:
    size_t capacity_ = 0;
    T *buffer_ = nullptr;
};

