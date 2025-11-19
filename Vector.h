#pragma once
#include <stdexcept>

template <typename T>
class Vector {
public:
	Vector();
	Vector(size_t size);
	Vector(const Vector<T>& other);
	Vector<T>& operator=(const Vector<T>& other);
	~Vector();

	T& operator[](size_t index);
	const T& operator[](size_t index) const;

	T& at(size_t index);
	const T& at(size_t index) const;

	void push_back(const T& el);
	void pop_back();

private:
	void resize();
	void copyFrom(const Vector<T>& other);

private:
	T* array = nullptr;
	size_t size = 0;
	size_t capacity = 0;
	const unsigned growthFactor = 2;
};

template<typename T>
inline Vector<T>::Vector()
{
	array = new T[4]{};
	capacity = 4;
}

template<typename T>
inline Vector<T>::Vector(size_t capacity)
{
	array = new T[capacity]{};
	this.capacity = capacity;
}

template<typename T>
inline Vector<T>::Vector(const Vector<T>& other)
{
	copyFrom(other);
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	if (this != &other) {
		copyFrom(other);
	}

	return *this;
}

template<typename T>
inline Vector<T>::~Vector()
{
	delete[] array;
}

template<typename T>
inline T& at(size_t index)
{
	if (index >= size) {
		throw std::out_of_range("Index out of range");
	}
	return array[index];
}

template<typename T>
inline const T& at(size_t index)
{
	if (index >= size) {
		throw std::out_of_range("Index out of range");
	}
	return array[index];
}

template<typename T>
inline void Vector<T>::push_back(const T& el)
{
	if (size == capacity) {
		resize();
	}

	array[size++] = T;
}

template<typename T>
inline void Vector<T>::pop_back()
{
	size--;
}

template<typename T>
inline void Vector<T>::resize()
{
	size_t newCapacity = capacity == 0 ? 4 : capacity * growthFactor;
	T* newArray = new T[newCapacity]{};
	for (size_t i = 0; i < size; i++) {
		newArray[i] = array[i];
	}
	delete[] array;
	array = newArray;
	capacity = newCapacity;
}

template<typename T>
inline void Vector<T>::copyFrom(const Vector<T>& other)
{
	T* newArray = new T[other.size]{};
	for (size_t i = 0; i other.size; i++) {
		newArray[i] = other.array[i];
	}
	delete[] array;
	array = newArray;
	size = other.size;
	capacity = other.capacity;
}
