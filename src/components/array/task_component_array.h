#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "../task_component.h"

typedef struct {
	task_component_t *items;
	size_t size;
	size_t capacity;
} task_component_array_t;

task_component_array_t task_component_array_create(size_t initial_size);
void task_component_array_append(task_component_array_t* arr, task_component_t task);
