#pragma once

#include "text.h"

typedef enum {
	TASK_LEVEL_EASY,
	TASK_LEVEL_MEDIUM,
	TASK_LEVEL_HARD
} task_level_e;

typedef struct {
	text_t text;
	task_level_e task_level;

	Vector2 position;
	Vector2 size;
	Vector2 padding;
} task_t;

task_t task_create(text_t text, task_level_e task_level, Vector2 position, Vector2 padding);
void task_draw(const task_t *task);
void task_update_size(task_t* task);
