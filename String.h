#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <iostream>

class String {
public:
	String();
	String(char c);
	String(const char* str);
	String(const String& other);
	String& operator=(const String& other);
	~String();

	bool operator==(const String& other) const;
	bool operator!=(const String& other) const;
	bool operator==(const char* str) const;
	bool operator!=(const char* str) const;
	bool operator==(char c) const;
	bool operator!=(char c) const;

	String& operator+=(const String& other);
	String& operator+=(const char* str);
	String operator+(const String& other) const;
	String operator+(const char* str) const;
	String& operator+=(char c);
	String operator+(char c) const;

	char operator[](size_t index) const;

	size_t size() const { return _size; }
	bool empty() const { return _size == 0; }
	void clear();
	const char* c_str() const;
	int find(char c) const;
	String substr(size_t start, size_t end) const;

private:
	char* concat(const char* str) const;
	char* concat(char c) const;
	char at(size_t index) const;

private:
	char* data;
	size_t _size;
};

std::ostream& operator<<(std::ostream& os, const String& str);