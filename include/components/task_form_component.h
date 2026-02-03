#pragma once

#include "raylib.h"

#include "system/task.h"

typedef enum {
	TASK_FORM_NONE,
	TASK_FORM_INPUT,
	TASK_FORM_TASK_LEVEL,
} task_form_input_e;

typedef struct {
	task_t task;

	task_form_input_e input_state;

	Rectangle form_rect;

	Rectangle input_rect;
	Rectangle task_level_rect;
	Rectangle task_level_options_rect[TASK_LEVEL_LEN];
	Rectangle completed_rect;

	bool show_form;
	bool show_task_level_selector;
} task_form_t;

void task_form_component_handle_input(task_form_t *form);
void task_form_component_draw(const task_form_t *form);
void task_form_component_setup(task_form_t *form, int screen_width, int screen_height);
