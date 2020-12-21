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

	arr->data = (T*)calloc(capacity, sizeof(T));
	arr->capacity = capacity;
	arr->length = 0;
}

template <typename T> void
array_free(Array<T>* arr) {

	free(arr->data);
}

template <typename T> void
array_reset(Array<T>* arr) {

	arr->length = 0;
}

template <typename T> void
array_expand(Array<T>* arr) {

	arr->capacity *= 2;
	T* old = arr->data;
	//arr->data = new T[arr->capacity];
	arr->data = (T*)calloc(arr->capacity, sizeof(T));
	memcpy(arr->data, old, arr->length * sizeof(T));
	free(old);
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

template <typename T>
struct Member {
	
	T data;
	Member<T>* next;
};

template <typename T>
struct List {
	Member<T>* head;
	Member<T>* tail;
};

template <typename T> void
list_init(List<T>* list, sizet size) {

	ASSERT(size > 0);
	Member<T>* node = (Member<T>*)malloc(sizeof(Member<T>));
	list->head = node;
	list->tail = node;

	for (sizet i = 0; i < size; ++i) {
		node = (Member<T>*)malloc(sizeof(Member<T>));
		list->tail->next = node;
		list->tail = node;
	}

	list->tail->next = NULL;
	list->tail = list->head;
}


template <typename T> void
list_add(List<T>* list, const T& item) {

	if (!list->tail)
		list->tail = (Member<T>*)malloc(sizeof(Member<T>));
		
	list->tail->data = item;
	list->tail = list->tail->next;
}
