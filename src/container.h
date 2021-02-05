#pragma once
#include "types.h"
#include "event.h"
#include "container.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>
#include "my_string.h"

#define ARRAY_RESIZE_FACTOR 2
#define ARRAY_MIN_SIZE 5

//Array

template <typename T>
struct Array {
  
	T* data;
	sizet length;
	sizet capacity;

	T& operator[](sizet index);

};

template <typename T> T&
Array<T>::operator[](sizet index) {
	
	ASSERT(index >= 0);
	return data[index];
}


template <typename T> void
array_init(Array<T>* arr, sizet capacity) {

	if (capacity < ARRAY_MIN_SIZE)
		capacity = ARRAY_MIN_SIZE;

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
list_init(List<T>* list) {

	list->head = NULL;
	list->tail = NULL;

}

template <typename T> void
list_add(List<T>* list, const T& item) {

	Member<T>* node = (Member<T>*)calloc(1, sizeof(Member<T>));
	node->data = item;
	node->next = NULL;

	if (!list->head) {
		list->head = node;
		list->tail = node;
	}
	else {
		list->tail->next = node;
		list->tail = node;
	}
}

// Linked list
template <typename T> void
list_insert(List<T>* list, const T& item, sizet pos) {

	Member<T>* node = list->head;
	for (sizet i = 1; i < pos; ++i) {

		node = node->next;
	}

	
	Member<T>* newnode = (Member<T>*)calloc(1, sizeof(Member<T>));
	newnode->data = item;
	newnode->next = node->next;
	node->next = newnode;

}

template <typename T> void
list_free(List<T>* list) {

	Member<T>* node = list->head;
	Member<T>* toFree;
	while (node) {
		
		toFree = node;
		node = node->next;
		free(toFree);
	}
	list->head = NULL;
	list->tail = NULL;
}


template <typename T> T&
list_at(List<T>* list, sizet pos) {

	Member<T>* node = list->head;
	for (sizet i = 0; i < pos; ++i) {

		node = node->next;
	}

	return node->data;
}

// Hashtable

// needs to be power of 2
#define HASH_TABLE_SIZE 1024

template <typename T>
struct HashTable {

	T* data;
	T& operator[](sizet index);
};


template <typename T> T&
HashTable<T>::operator[](sizet index) {

	return data[index];
}

static i32
hash_function(const String& str) {
    long hash = 5381;
    i32 c;

	for (sizet i = 0; i < str.length; ++i) 
        hash = ((hash << 5) + hash) + (i32)str.data[i];

    return hash;
}


template <typename T> void
hash_table_init(HashTable<T>* table) {

	table->data = (T*)calloc(HASH_TABLE_SIZE, sizeof(T));
}

template <typename T> void
hash_table_put(HashTable<T>* table, const String& key, const T& value) {

	i32 hashValue = hash_function(key);
	i32 hashIndex = hashValue & (HASH_TABLE_SIZE - 1);

	ASSERT_MSG(&table->data[hashIndex], "Hash collision detected");

	table->data[hashIndex] = value;
}

template <typename T> i32
hash_table_index_from_key(HashTable<T>* table, const char* key) {

	i32 hashValue = hash_function(key);
	i32 hashIndex = hashValue & (HASH_TABLE_SIZE - 1);

	return hashIndex;
}

template <typename T> T&
hash_table_get(HashTable<T>* table, const String& key) {

	i32 hashValue = hash_function(key);
	i32 hashIndex = hashValue & (HASH_TABLE_SIZE - 1);

	return table->data[hashIndex];
}

template <typename T> b8
hash_table_value_exists(HashTable<T>* table, const char* key) {

	i32 hashValue = hash_function(key);
	i32 hashIndex = hashValue & (HASH_TABLE_SIZE - 1);

	if (&table->data[hashIndex]) {
		return true;
	}
	return false;
}
