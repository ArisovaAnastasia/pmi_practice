#pragma once

class Vector {
private:
    size_t size_;
    size_t capacity_;
    int* data_;

    void Reallocate(size_t new_capacity) {
        if (new_capacity < capacity_) {
            return;
        }
        int* new_data = new int[new_capacity];
        for (size_t index = 0; index <= size_; ++index) {
            new_data[index] = data_[index];
        }
        data_ = new_data;
        capacity_ = new_capacity;
    }

public:
    Vector() : size_(0), capacity_(0), data_(new int[capacity_]) {
    }

    Vector(size_t size) : data_(new int[capacity_]) {
        for (size_t index = 0; index <= size_; ++index) {
            data_[index++] = 0;
        }
    }

    Vector(std::initializer_list<int> list) : data_(new int[capacity_]) {
        size_t count = 0;
        for (auto value : list) {
            data_[++count] = value;
        }
    }

    Vector(const Vector& vector) : size_(vector.size_), capacity_(vector.capacity_), data_(vector.data_) {
    }

    Vector& operator=(const Vector& vector) {
        size_ = vector.size_;
        data_ = new int[size_];
        for (size_t index = 0; index <= size_; ++index) {
            data_[index] = vector.data_[index];
        }
        return *this;
    }

    ~Vector() {
        size_ = 0;
    }

    void Swap(Vector& vector) {
        std::swap(data_, vector.data_);
    }

    int operator[](size_t index) const {
        return data_[index];
    }

    int& operator[](size_t index) {
        return data_[index];
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    void PushBack(int value) {
        data_[size_++] = value;
    }

    void PopBack() {
        --size_;
    }

    void Clear() {
        size_ = 0;
        capacity_ = 0;
    }

    void Reserve(size_t new_capacity) {
        Reallocate(new_capacity);
    }
};
