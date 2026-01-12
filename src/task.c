#include <assert.h>

#include "raylib.h"
#include "raymath.h"

#include "task.h"
#include "text.h"

const static Color TASK_LEVEL_EASY_COLOR = GREEN;
const static Color TASK_LEVEL_MEDIUM_COLOR = YELLOW;
const static Color TASK_LEVEL_HARD_COLOR = RED;

task_t task_create(text_t text, task_level_e task_level, Vector2 position, Vector2 padding)
{
	task_t task = {
		.text = text,
		.task_level = task_level,
		.position = position,
		.size = Vector2Add(text.size, padding),
		.padding = padding,
	};
	// task.text.position = Vector2Add(position, Vector2Scale(task.size, 0.5f));
	task.text.position = Vector2Add(position, Vector2Scale(padding, 0.5f));

	return task;
}

void task_draw(const task_t *task)
{
	Color color;
	switch (task->task_level) {
		case TASK_LEVEL_EASY: {
					      color = TASK_LEVEL_EASY_COLOR;
				      } break;
		case TASK_LEVEL_MEDIUM: {
					      color = TASK_LEVEL_MEDIUM_COLOR;
				      } break;
		case TASK_LEVEL_HARD: {
					      color = TASK_LEVEL_HARD_COLOR;
				      } break;
		default:
				      assert(false);
				      break;
	}

	DrawRectangleV(task->position, task->size, color);
	text_draw(&task->text, BLACK);
}

void task_update_size(task_t* task)
{
	task->size = Vector2Add(task->text.size, task->padding);
}
