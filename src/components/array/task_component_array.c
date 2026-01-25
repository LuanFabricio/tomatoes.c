#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "components/array/task_component_array.h"

task_component_array_t task_component_array_create(size_t initial_size)
{
	task_component_array_t tasks = {0};

	tasks.capacity = initial_size;
	tasks.size = 0;
	tasks.items = malloc(sizeof(task_component_t) * tasks.capacity);

	return tasks;
}

void task_component_array_append(task_component_array_t* arr, task_component_t task)
{
	if (arr->size >= arr->capacity) {
		if (arr->capacity == 0) {
			arr->capacity = 0xff;
		} else {
			arr->capacity *= 2;
		}
		arr->items = realloc(arr->items, sizeof(*arr->items) * arr->capacity);
	}

	memcpy(&arr->items[arr->size], &task, sizeof(task_component_t));
	arr->size += 1;
}
