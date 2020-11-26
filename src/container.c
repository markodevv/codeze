#include "container.h"
#include "debug.h"

#include <stdlib.h>
#include <string.h>

void*
array_create(sizet capacity, sizet stride) {

	sizet headerSize = _FIELDS * sizeof(sizet);
	sizet arrSize = capacity * stride;

	sizet* arr = (sizet*)malloc(arrSize + headerSize);
	ASSERT_MSG(arr, "array malloc failed");
	arr[_CAPACITY] = capacity;
	arr[_LENGTH] = 0;
	arr[_STRIDE] = stride;

	return (void*)(arr + _FIELDS);
	
}



sizet
array_field_get(void* arr, sizet field) {
	
	return ((sizet*)(arr) - _FIELDS)[field];

}

void
array_field_set(void* arr, sizet field, sizet value) {

	((sizet*)(arr) - _FIELDS)[field] = value;

}

void
array_destroy(void *arr) {

    free(arr - _FIELDS * sizeof(sizet));
}

void*
array_resize(void *arr) {

	void *temp = array_create(ARRAY_RESIZE_FACTOR * ARRAY_CAPACITY(arr),
							  ARRAY_STRIDE(arr));
    array_field_set(temp, _LENGTH, ARRAY_LENGTH(arr));
    memcpy(temp, arr, ARRAY_LENGTH(arr) * ARRAY_STRIDE(arr));

    array_destroy(arr);

	return temp;
}

void*
array_push(void *arr, void *xptr) {

	if (ARRAY_LENGTH(arr) >= ARRAY_CAPACITY(arr)) {
		arr = array_resize(arr);
	}

    memcpy(arr + ARRAY_LENGTH(arr) * ARRAY_STRIDE(arr), xptr, ARRAY_STRIDE(arr));
    array_field_set(arr, _LENGTH, ARRAY_LENGTH(arr) + 1);
    return arr;
}

// Removes the last element in the array, but copies it to `*dest` first.
void
array_pop(void *arr, void *dest) {

    memcpy(dest, arr + (ARRAY_LENGTH(arr) - 1) * ARRAY_STRIDE(arr), ARRAY_STRIDE(arr));
    array_field_set(arr, _LENGTH, ARRAY_LENGTH(arr) - 1); // Decrement length.
}
