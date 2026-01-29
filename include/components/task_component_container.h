#pragma once

#include "raylib.h"

#include "components/array/task_component_array.h"

typedef struct {
	task_component_array_t tasks;

	Vector2 position;
	Vector2 size;
} task_component_container_t;

task_component_container_t task_component_container_create(Vector2 position);
void task_component_container_append(task_component_container_t* container, task_t* task, int font_size);
void task_component_container_draw(task_component_container_t container);
void task_component_container_update_sizes(task_component_container_t *container);
void task_component_container_update_position(task_component_container_t *container, const Vector2 position);
