#include <stdio.h>
#include <string.h>

#include "button.h"
#include "components/task_component_container.h"
#include "raylib.h"

#include "components/task_form_component.h"
#include "system/array/task_array.h"
#include "system/task.h"
#include "text.h"
#include "types.h"

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

static void task_form__component_handle_mouse(task_form_t *form, const mouse_t mouse)
{
	if (form->input_state == TASK_FORM_TASK_LEVEL) {
		for (size_t i = 0; i < TASK_LEVEL_LEN; i++) {
			if (CheckCollisionPointRec(mouse.position, form->task_level_options_rect[i])) {
				form->task.task_level = i;
				form->input_state = TASK_FORM_NONE;
				return;
			}
		}
	}

	form->input_state = TASK_FORM_NONE;
	if (CheckCollisionPointRec(mouse.position, form->input_rect)) {
		form->input_state = TASK_FORM_INPUT;
	} else if (CheckCollisionPointRec(mouse.position, form->task_level_rect)) {
		form->input_state = TASK_FORM_TASK_LEVEL;
	} else if (CheckCollisionPointRec(mouse.position, form->completed_rect)) {
		form->task.completed = !form->task.completed;
	}
}

static void task_form__component_handle_keyboard(task_form_t *form, int key_pressed)
{
	if (form->input_state == TASK_FORM_INPUT) {
		const size_t name_len = strlen(form->task.content);
		if (name_len >= TASK_MAX_CONTENT_LEN) return;

		const bool is_letter = (key_pressed >= 'A' && key_pressed <= 'Z')
			|| (key_pressed >= 'a' && key_pressed <= 'z');
		const bool is_number = key_pressed >= '0' && key_pressed <= '9';
		if (is_letter || is_number) {
			const bool should_lower_case = IsKeyUp(KEY_LEFT_SHIFT)
				&& IsKeyUp(KEY_RIGHT_SHIFT)
				&& !is_number;
			if (should_lower_case) {
				key_pressed += 'a' - 'A';
			}

			form->task.content[name_len] = (char)key_pressed;
		}
	}
}

void task_form_component_setup(task_form_t *form, int screen_width, int screen_height)
{
	form->input_state = TASK_FORM_INPUT;
	form->show_form = false;
	form->form_rect = (Rectangle){
		.x = (screen_width - 520) / 2.f,
		.y = (screen_height - 320) / 2.f,
		.width = 520,
		.height = 360,
	};
	form->input_rect = (Rectangle) {
		.x = (screen_width - form->form_rect.width + 20.f) / 2.f,
		.y = (screen_height - 164) / 2.f,
		.width = form->form_rect.width - 20.f,
		.height = 32,
	};

	form->task_level_rect = (Rectangle) {
		.x = (screen_width - form->form_rect.width + 20.f) / 2.f,
		.y = form->input_rect.y + form->input_rect.height + 52.f,
		.width = 128,
		.height = 32,
	};

	form->completed_rect = (Rectangle) {
		.x = (screen_width - form->form_rect.width + 20.f) / 2.f,
		.y = form->task_level_rect.y + form->task_level_rect.height + 52.f,
		.width = 32,
		.height = 32,
	};
	for (size_t i = 0; i < TASK_LEVEL_LEN; i++) {
		form->task_level_options_rect[i] = form->task_level_rect;
		form->task_level_options_rect[i].y += form->task_level_options_rect[i].height * (i+1);
	}

	form->save_btn = button_create(
		(Vector2){0},
		(Vector2){32, 32},
		text_create(
			"Save",
			32,
			GetFontDefault()
		),
		BLACK,
		ColorBrightness(BLACK, 0.5)
	);
	button_update_position(
		&form->save_btn,
		(Vector2){
			form->form_rect.x + form->form_rect.width - form->save_btn.size.x - 32,
			form->form_rect.y + form->form_rect.height - form->save_btn.size.y - 32,
		});
}

void task_form_component__handle_input(task_form_t *form, const mouse_t mouse)
{
	const int key_pressed = GetKeyPressed();
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		task_form__component_handle_mouse(form, mouse);
	} else if(key_pressed != 0) {
		task_form__component_handle_keyboard(form, key_pressed);
	}
}

void task_form_component_update(task_form_t *form, const mouse_t mouse, task_component_container_t *task_container, task_array_t *task_array)
{
	task_form_component__handle_input(form, mouse);

	form->save_btn.selected = button_contain_point(&form->save_btn, mouse.position);
	if (form->save_btn.selected && mouse.left_clicked) {
		form->show_form = false;

		task_array_append(task_array, form->task);
		task_component_container_append(
			task_container,
			&task_array->items[task_array->size - 1],
			32);
		task_component_container_update_sizes(task_container);
		task_component_container_fix_position(task_container);

		memset(&form->task, 0, sizeof(form->task));
	}
}

void task_form_component_draw(const task_form_t *form)
{
	if (!form->show_form) return;

	DrawRectangleRec(form->form_rect, GREEN);

	const int text_padding_x = 10;
	const int font_size = 32;
	DrawRectangleRec(form->input_rect, WHITE);

	DrawText("Name",
		form->input_rect.x,
		form->input_rect.y - font_size,
		font_size,
		BLACK);
	DrawText(
		form->task.content,
		form->input_rect.x + text_padding_x,
		form->input_rect.y,
		font_size,
		BLACK
	);

	DrawText(
		"Completed",
		form->completed_rect.x,
		form->completed_rect.y - font_size,
		font_size,
		BLACK);
	DrawRectangleRec(form->completed_rect, form->task.completed ? BLACK : WHITE );

	DrawRectangleRec(form->task_level_rect, WHITE);
	char task_level_text[20] = "";
	const size_t task_level_text_size = sizeof(task_level_text);
	task_form__get_task_level_text(
		task_level_text,
		task_level_text_size,
		form->task.task_level);

	DrawText(
		"Task level",
		form->task_level_rect.x + text_padding_x,
		form->task_level_rect.y - font_size,
		font_size,
		BLACK);
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

	button_draw(&form->save_btn);
}
