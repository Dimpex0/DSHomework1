#pragma once
#include <stdexcept>

template<typename T>
class Vector {
public:
	Vector();
	Vector(size_t _size);
	Vector(const Vector<T>& other);
	Vector<T>& operator=(const Vector<T>& other);
	~Vector();

	T& operator[](size_t index) { return at(index); };
	const T& operator[](size_t index) const { return at(index); };

	T& at(size_t index);
	const T& at(size_t index) const;

	void push_back(const T& el);
	void pop_back();
	bool empty() const { return _size == 0; }
	size_t size() const { return _size; }
	void print() const;
	T& back();
	const T& back() const;

private:
	void resize();
	void copyFrom(const Vector<T>& other);

private:
	T* array = nullptr;
	size_t _size = 0;
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
	this->capacity = capacity;
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
inline T& Vector<T>::at(size_t index)
{
	if (index >= _size) {
		throw std::out_of_range("Index out of range");
	}
	return array[index];
}

template<typename T>
inline const T& Vector<T>::at(size_t index) const
{
	if (index >= _size) {
		throw std::out_of_range("Index out of range");
	}
	return array[index];
}

template<typename T>
inline void Vector<T>::push_back(const T& el)
{
	if (_size == capacity) {
		resize();
	}

	array[_size++] = el;
}

template<typename T>
inline void Vector<T>::pop_back()
{
	_size--;
}

template<typename T>
inline void Vector<T>::print() const
{
	for (size_t i = 0; i < _size; i++) {
		std::cout << array[i] << " ";
	}

	std::cout << std::endl;
}

template<typename T>
inline T& Vector<T>::back()
{
	return array[_size - 1];
}

template<typename T>
inline const T& Vector<T>::back() const
{
	return array[_size - 1];
}

template<typename T>
inline void Vector<T>::resize()
{
	size_t newCapacity = capacity == 0 ? 4 : capacity * growthFactor;
	T* newArray = new T[newCapacity]{};
	for (size_t i = 0; i < _size; i++) {
		newArray[i] = array[i];
	}
	delete[] array;
	array = newArray;
	capacity = newCapacity;
}

template<typename T>
inline void Vector<T>::copyFrom(const Vector<T>& other)
{
	T* newArray = new T[other._size]{};
	for (size_t i = 0; i < other._size; i++) {
		newArray[i] = other.array[i];
	}
	delete[] array;
	array = newArray;
	_size = other._size;
	capacity = _size;
}