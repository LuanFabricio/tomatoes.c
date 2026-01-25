#pragma once

#include "raylib.h"

#include "system/task.h"

typedef struct {
	task_t task;

	int font_size;
	Vector2 text_size;

	Rectangle task_rect;
	Rectangle text_rect;

	bool selected;
} task_component_t;

task_component_t task_component_create(task_t task, int font_size, Vector2 position);
void task_component_draw(task_component_t component);
bool task_component_contain_point(const task_component_t* component, Vector2 point);
