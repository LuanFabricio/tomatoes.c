#include <stdlib.h>
#include <string.h>

#include "task_array.h"

task_array_t task_array_create(size_t initial_size)
{
	task_array_t tasks = {0};

	tasks.capacity = initial_size;
	tasks.size = 0;
	tasks.items = malloc(sizeof(task_t) * tasks.capacity);

	return tasks;
}

void task_array_append(task_array_t* arr, task_t task)
{
	if (arr->size >= arr->capacity) {
		if (arr->capacity == 0) {
			arr->capacity = 0xff;
		} else {
			arr->capacity *= 2;
		}
		task_t *old_buffer = arr->items;
		arr->items = realloc(arr->items, sizeof(*arr->items) * arr->capacity);
	}

	arr->items[arr->size] = task;
	size_t text_len = strlen(task.text.content);
	memcpy(arr->items[arr->size].text.content, task.text.content, text_len);

	arr->size += 1;
}
