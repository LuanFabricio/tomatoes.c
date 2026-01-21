#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "task_array.h"

task_array_t task_array_create(size_t initial_size)
{
	task_array_t tasks = {0};

	tasks.capacity = initial_size;
	tasks.size = 0;
	size_t array_bytes = sizeof(task_t) * tasks.capacity;
	tasks.items = malloc(array_bytes);
	memset(tasks.items, 0, array_bytes);

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

		const size_t bytes_size = sizeof(*arr->items) * arr->capacity;
		if (arr->items == NULL) {
			arr->items = malloc(bytes_size);
		} else {
			task_t *old_buffer = arr->items;
			arr->items = realloc(arr->items, sizeof(*arr->items) * arr->capacity);
		}
	}

	// const size_t content_len = strlen(task.content);
	// arr->items[arr->size].content = malloc(content_len);
	// strcpy(arr->items[arr->size].content, task.content);
	// arr->items[arr->size].task_level = task.task_level;
	// arr->items[arr->size].completed = task.completed;
	memcpy(&arr->items[arr->size], &task, (sizeof(task_t)));
	printf("\t.content=%s\n", task.content);
	printf("\t.level=%d\n", task.task_level);
	printf("\t.completed=%s\n", task.completed ? "true" : "false");

	arr->size += 1;
}

void task_array_free(task_array_t *arr)
{
	arr->size = 0;
	arr->capacity = 0;
	free(arr->items);
}
