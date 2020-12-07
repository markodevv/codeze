#pragma once
#include "types.h"

#define ARRAY_RESIZE_FACTOR 2

enum {
	_CAPACITY,
	_LENGTH,
	_STRIDE,
	_FIELDS
};

#define ARRAY_LENGTH(arr) array_field_get(arr, _LENGTH)
#define ARRAY_STRIDE(arr) array_field_get(arr, _STRIDE)
#define ARRAY_CAPACITY(arr) array_field_get(arr, _CAPACITY)

void* array_create(sizet capacity, sizet stride);
sizet array_field_get(void* arr, sizet field);
void array_field_set(void* arr, sizet field, sizet value);
void array_release(void *arr);
void array_reset(void *arr);
void* array_resize(void *arr);
void* array_push(void *arr, void *xptr);
void array_pop(void *arr, void *dest);
void* array_insert(void *arr, sizet pos, void* item);
void array_erase(void *arr, sizet pos);

