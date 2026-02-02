#include <stdio.h>
#include <string.h>

#include "raylib.h"

#include "components/task_form_component.h"
#include "system/task.h"

static void task_form__get_task_level_text(char *buffer, size_t buffer_size, task_level_e level)
{
	switch (level) {
		case TASK_LEVEL_EASY:
			strncpy(buffer, "Easy", buffer_size);
			break;
		case TASK_LEVEL_MEDIUM:
			strncpy(buffer, "Medium", buffer_size);
			break;
		case TASK_LEVEL_HARD:
			strncpy(buffer, "Hard", buffer_size);
			break;
		default: break;
	}

}

static void task_form__component_handle_mouse(task_form_t *form)
{
	const Vector2 mouse = GetMousePosition();
	if (form->input_state == TASK_FORM_TASK_LEVEL) {
		for (size_t i = 0; i < TASK_LEVEL_LEN; i++) {
			if (CheckCollisionPointRec(mouse, form->task_level_options_rect[i])) {
				form->task.task_level = i;
				form->input_state = TASK_FORM_NONE;
				return;
			}
		}
	}

	form->input_state = TASK_FORM_NONE;
	if (CheckCollisionPointRec(mouse, form->input_rect)) {
		form->input_state = TASK_FORM_INPUT;
	} else if (CheckCollisionPointRec(mouse, form->task_level_rect)) {
		form->input_state = TASK_FORM_TASK_LEVEL;
	} else if (CheckCollisionPointRec(mouse, form->completed_rect)) {
		form->task.completed = !form->task.completed;
	}
}

static void task_form__component_handle_keyboard(task_form_t *form, int key_pressed)
{
	if (form->input_state == TASK_FORM_INPUT) {
		const size_t name_len = strlen(form->task.content);
		if (name_len >= TASK_MAX_CONTENT_LEN) return;

		if (key_pressed >= KEY_APOSTROPHE && key_pressed <= KEY_GRAVE) {
			const bool is_numeric = key_pressed >= '0' && key_pressed <= '9';
			const bool should_lower_case = IsKeyUp(KEY_LEFT_SHIFT)
				&& IsKeyUp(KEY_RIGHT_SHIFT)
				&& !is_numeric;
			if (should_lower_case) {
				key_pressed += 'a' - 'A';
			}
			form->task.content[name_len] = (char)key_pressed;
		}
	}
}

void task_form_component_handle_input(task_form_t *form)
{
	const int key_pressed = GetKeyPressed();
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		task_form__component_handle_mouse(form);
	} else if(key_pressed != 0) {
		task_form__component_handle_keyboard(form, key_pressed);
	}
}

void task_form_component_draw(const task_form_t *form)
{
	if (!form->show_form) return;

	DrawRectangleRec(form->form_rect, GREEN);

	const int text_padding_x = 10;
	const int font_size = 32;
	DrawRectangleRec(form->input_rect, WHITE);
	DrawText(
		form->task.content,
		form->input_rect.x + text_padding_x,
		form->input_rect.y,
		font_size,
		BLACK
	);

	DrawRectangleRec(form->completed_rect, form->task.completed ? BLACK : WHITE );

	DrawRectangleRec(form->task_level_rect, WHITE);
	char task_level_text[20] = "";
	const size_t task_level_text_size = sizeof(task_level_text);
	task_form__get_task_level_text(
		task_level_text,
		task_level_text_size,
		form->task.task_level);

	DrawText(
		task_level_text,
		form->task_level_rect.x + text_padding_x,
		form->task_level_rect.y,
		font_size,
		BLACK);

	if (form->input_state == TASK_FORM_TASK_LEVEL) {
		for (size_t i = 0; i < TASK_LEVEL_LEN; i++) {
			Color background = WHITE;
			if (i == form->task.task_level) {
				background = BLUE;
			}
			DrawRectangleRec(form->task_level_options_rect[i], background);
			task_form__get_task_level_text(
				task_level_text,
				task_level_text_size,
				i);
			DrawText(
				task_level_text,
				form->task_level_options_rect[i].x + text_padding_x,
				form->task_level_options_rect[i].y,
				font_size,
				BLACK);
		}
	}
}
