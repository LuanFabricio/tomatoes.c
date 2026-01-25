#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "button.h"

typedef struct {
	button_t *items;
	size_t size;
	size_t capacity;
} button_array_t;

button_array_t button_array_create(size_t initial_size);
void button_array_append(button_array_t* arr, button_t button);
