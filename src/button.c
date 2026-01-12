#include <stddef.h>

#include "raylib.h"

#include "button.h"
#include "text.h"

button_t button_create(Vector2 position, Vector2 padding, text_t text, Color normal_color, Color selected_color)
{
	button_t button = {
		.position = position,
		.size = Vector2Add(text.size, padding),
		.normal_color = normal_color,
		.selected_color = selected_color,
		.text = text,
		.padding = padding,
		.on_click_callback = NULL,
	};

	button.text.position = Vector2Add(button.position, Vector2Scale(padding, 0.5f));

	return button;
}

void button_draw(const button_t* button)
{
	Color color = !button->selected ? button->normal_color : button->selected_color;
	DrawRectangleV(button->position, button->size, color);
	text_draw(&button->text, WHITE);
}

bool button_contain_point(const button_t* button, Vector2 point)
{
	Rectangle rec = {
		.x = button->position.x,
		.y = button->position.y,
		.width = button->size.x,
		.height = button->size.y,
	};

	return CheckCollisionPointRec(point, rec);
}
