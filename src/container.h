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

	T& operator[](sizet index);

};

template <typename T>
T& Array<T>::operator[](sizet index) {
	
	ASSERT(index >= 0);
	return data[index];
}


template <typename T> void
array_init(Array<T>* arr, sizet capacity) {

	arr->data = new T[capacity];
	arr->capacity = capacity;
	arr->length = 0;
}

template <typename T> void
array_free(Array<T>* arr) {

	delete[] arr->data;
}

template <typename T> void
array_reset(Array<T>* arr) {

	arr->length = 0;
}

template <typename T> void
array_expand(Array<T>* arr) {

	arr->capacity *= 2;
	T* old = arr->data;
	arr->data = new T[arr->capacity];
	memcpy(arr->data, old, arr->length * sizeof(T));
	delete[] old;
}

template <typename T> void
array_push(Array<T>* arr, const T& item) {

	if (arr->length >= arr->capacity) {
		array_expand(arr);
	}

	arr->data[arr->length] = item;
	arr->length += 1;
}

template <typename T> T&
array_pop(Array<T>* arr) {

	arr->length -= 1;
	return arr->data[arr->length + 1];
}

template <typename T> void 
array_insert(Array<T>* arr, const T& item, sizet pos) {

	ASSERT(pos <= arr->length && pos >= 0);

	if (arr->length >= arr->capacity) {
		array_expand(arr);
	}

	memcpy(arr->data + (pos + 1), arr->data + pos, (arr->length - pos) * sizeof(T));
	arr->data[pos] = item;

	arr->length += 1;
}

template <typename T> void 
array_erase(Array<T>* arr, sizet pos) {


	ASSERT(arr->length >= 1);

	memcpy(arr->data + pos, arr->data + (pos + 1), (arr->length - pos) * sizeof(T));
	arr->length -= 1;
}

// void
// print_array(Array<int> arr) {
// 	for (sizet i = 0; i < arr.length; ++i) {
// 		printf("%i, ", arr[i]);
// 	}
// 	printf("\n");
// }
