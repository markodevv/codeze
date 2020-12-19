#pragma once
#include "types.h"
#include "event.h"
#include "container.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>

#define ARRAY_RESIZE_FACTOR 2

template <typename T>
struct Array {
  
	T* data;
	sizet length;
	sizet capacity;

	void init(sizet capacity);
	void reset();
	void free_data();
	void push(T item);
	void expand();
	T& pop();
	void insert(T item, sizet pos);
	void erase(sizet pos);

	T& operator[](sizet index);

};

  
template <typename T>
void Array<T>::init(sizet capacity) {

	data = new T[capacity];
	this->capacity = capacity;
	this->length = 0;
}

template <typename T>
void Array<T>::free_data() {

	delete[] data;
}

template <typename T>
void Array<T>::reset() {

	length = 0;
}

template <typename T>
T& Array<T>::operator[](sizet index) {
	
	ASSERT(index >= 0);
	return data[index];
}

template <typename T>
void Array<T>::expand() {

	capacity *= 2;
	T* old = data;
	data = new T[capacity];
	memcpy(data, old, length * sizeof(T));
	delete[] old;
}

template <typename T>
void Array<T>::push(T item) {

	if (length >= capacity) {
		expand();
	}

	data[length] = item;
	length += 1;
}

template <typename T>
T& Array<T>::pop() {

	length -= 1;
	return data[length + 1];
}

template <typename T>
void Array<T>::insert(T item, sizet pos) {

	ASSERT(pos <= length && pos >= 0);

	if (length >= capacity) {
		expand();
	}

	memcpy(data + (pos + 1), data + pos, (length - pos) * sizeof(T));
	data[pos] = item;

	length += 1;
}

template <typename T>
void Array<T>::erase(sizet pos) {

	ASSERT(length >= 1);

	memcpy(data + pos, data + (pos + 1), (length - pos) * sizeof(T));
	length -= 1;
}



