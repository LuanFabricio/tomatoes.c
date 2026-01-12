#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "../task.h"

typedef struct {
	task_t *items;
	size_t size;
	size_t capacity;
} task_array_t;

task_array_t task_array_create(size_t initial_size);
void task_array_append(task_array_t* arr, task_t task);
