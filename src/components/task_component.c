#include "assert.h"

#include "raylib.h"

#include "components/task_component.h"

static const Color TASK_LEVEL_EASY_DEFAULT_COLOR = GREEN;
static const Color TASK_LEVEL_MEDIUM_DEFAULT_COLOR = YELLOW;
static const Color TASK_LEVEL_HARD_DEFAULT_COLOR = RED;

static Color task_component__get_level_default_color(task_level_e level)
{
	Color color = BLACK;
	switch (level) {
		case TASK_LEVEL_EASY:
			color = TASK_LEVEL_EASY_DEFAULT_COLOR;
			break;
		case TASK_LEVEL_MEDIUM:
			color = TASK_LEVEL_MEDIUM_DEFAULT_COLOR;
			break;
		case TASK_LEVEL_HARD:
			color = TASK_LEVEL_HARD_DEFAULT_COLOR;
			break;
		default:
			assert(false);
			break;
	}

	return color;
}

task_component_t task_component_create(task_t *task, int font_size, Vector2 position)
{
	const Font default_font = GetFontDefault();
	const float font_spacing = font_size / 10.f;
	const Vector2 text_size = MeasureTextEx(
		default_font,
		task->content,
		font_size,
		font_spacing);

	const Vector2 default_padding = {32, 32};
	Rectangle task_rect = {
		.x = position.x,
		.y = position.y,
		.width = text_size.x + default_padding.x,
		.height = text_size.y + default_padding.y,
	};

	const Rectangle text_rect = {
		.x = task_rect.x + (task_rect.width  - text_size.x) / 2.f,
		.y = task_rect.y + (task_rect.height - text_size.y) / 2.f,
		.width = text_size.x,
		.height = text_size.y,
	};

	task_component_t component = {
		.task = task,
		.font_size = font_size,
		.text_size = text_size,
		.task_rect = task_rect,
		.text_rect = text_rect,
	};

	return component;
}

void task_component_draw(task_component_t component)
{
	Color rect_color = task_component__get_level_default_color(component.task->task_level);
	Color text_color = BLACK;

	float color_brightness = 0.f;
	if (component.selected) {
		color_brightness += 0.25f;
	}
	if (component.task->completed) {
		color_brightness += -0.5f;
	}

	if (color_brightness != 0.f) {
		rect_color = ColorBrightness(rect_color, color_brightness);
		text_color = ColorBrightness(text_color, color_brightness);
	}

	DrawRectangleRec(component.task_rect, rect_color);

	DrawText(
		component.task->content,
		component.text_rect.x,
		component.text_rect.y,
		component.font_size,
		text_color
	);
}

bool task_component_contain_point(const task_component_t* component, Vector2 point)
{
	return CheckCollisionPointRec(point, component->task_rect);
}
