#pragma once

#include <stdbool.h>

#include "raylib.h"

#include "text.h"

typedef struct button_t {
	Vector2 position;
	Vector2 size;
	Vector2 padding;

	Color normal_color;
	Color selected_color;

	bool selected;

	text_t text;

	void (*on_click_callback)(void*);
} button_t;

button_t button_create(Vector2 position, Vector2 padding, text_t text, Color normal_color, Color selected_color);
void button_draw(const button_t* button);
bool button_contain_point(const button_t* button, Vector2 point);
