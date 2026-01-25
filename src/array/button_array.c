#include <stdlib.h>
#include <string.h>

#include "array/button_array.h"

button_array_t button_array_create(size_t initial_size)
{
	button_array_t buttons = {0};

	buttons.capacity = initial_size;
	buttons.size = 0;
	buttons.items = malloc(sizeof(button_t) * buttons.capacity);

	return buttons;
}

void button_array_append(button_array_t* arr, button_t button)
{
	if (arr->size >= arr->capacity) {
		if (arr->capacity == 0) {
			arr->capacity = 0xff;
		} else {
			arr->capacity *= 2;
		}
		arr->items = realloc(arr->items, sizeof(*arr->items) * arr->capacity);
	}

	arr->items[arr->size] = button;
	size_t text_len = strlen(button.text.content);
	memcpy(arr->items[arr->size].text.content, button.text.content, text_len);

	arr->size += 1;
}
