#define _CRT_SECURE_NO_WARNINGS

#include "String.h"
#include <new>
#include <iostream>

String::String()
{
	data = new char[1];
	data[0] = '\0';
	_size = 0;
}

String::String(char c)
{
	data = new char[2];
	data[0] = c;
	data[1] = '\0';
	_size = 1;
}

String::String(const char* str)
{
	if (!str) {
		throw std::invalid_argument("Empty string.");
	}
	_size = std::strlen(str);
	char* newData = new char[_size + 1];
	data = std::strcpy(newData, str);
}

String::String(const String& other)
	: String(other.c_str())
{
}

String& String::operator=(const String& other)
{
	if (this != &other) {
		char* newData = new char[other.size() + 1];
		delete[] data;
		data = std::strcpy(newData, other.c_str());
		_size = other.size();
	}

	return *this;
}

String::~String()
{
	delete[] data;
}

bool String::operator==(const String& other) const
{
	return std::strcmp(data, other.c_str()) == 0;
}

bool String::operator!=(const String& other) const
{
	return !(*this == other);
}

bool String::operator==(const char* str) const
{
	return std::strcmp(data, str) == 0;
}

bool String::operator!=(const char* str) const
{
	return !(*this == str);
}

bool String::operator==(char c) const
{
	return (_size == 1 && (data[0] == c));
}

bool String::operator!=(char c) const
{
	return !(*this == c);
}

String& String::operator+=(const String& other)
{
	char* newData = concat(other.c_str());
	delete[] data;
	data = newData;
	_size += other.size();
	return *this;
}

String& String::operator+=(const char* str)
{
	if (!str) {
		return *this;
	}
	char* newData = concat(str);
	delete[] data;
	data = newData;
	_size += std::strlen(str);
	return *this;
}

String String::operator+(const String& other) const
{
	String newString(*this);
	newString += other;
	return newString;
}

String String::operator+(const char* str) const
{
	String newString(*this);
	newString += str;
	return newString;
}

String& String::operator+=(char c)
{
	char* newData = new char[_size + 2];
	std::strcpy(newData, data);
	newData[_size] = c;
	newData[_size + 1] = '\0';
	delete[] data;
	data = newData;
	_size++;
	return *this;
}

String String::operator+(char c) const
{
	String res(*this);
	res += c;
	return res;
}

char String::operator[](size_t index) const
{
	return at(index);
}

void String::clear()
{
	delete[] data;
	data = new char[1];
	data[0] = '\0';
	_size = 0;
}

const char* String::c_str() const
{
	return data;
}

int String::find(char c) const
{
	for (size_t i = 0; i < _size; i++) {
		if (data[i] == c) {
			return i;
		}
	}
	return -1;
}

String String::substr(size_t start, size_t end) const
{

	if (start >= _size || end > _size) {
		throw std::invalid_argument("Invalid positions.");
	}

	String result;
	for (size_t i = start; i < end; i++) {
		result += data[i];
	}

	return result;
}

char* String::concat(const char* str) const
{
	char* newData = new char[_size + std::strlen(str) + 1];
	std::strcpy(newData, data);
	std::strcpy(newData + _size, str);
	return newData;
}

char* String::concat(char c) const
{
	char* newData = new char[_size + 2];
	std::strcpy(newData, data);
	newData[_size] = c;
	newData[_size + 1] = '\0';
	return newData;
}

char String::at(size_t index) const
{
	if (index >= _size) {
		throw std::out_of_range("Index out of range.");
	}

	return data[index];
}

std::ostream& operator<<(std::ostream& os, const String& str) {
	return os << str.c_str();
}