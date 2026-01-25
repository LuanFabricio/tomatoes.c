
#pragma once

#include "raylib.h"

#include "array/button_array.h"

typedef struct {
	button_array_t buttons;

	Vector2 position;
	Vector2 size;

	Vector2 padding;
} button_component_container_t;

button_component_container_t button_component_container_create(Vector2 position);
void button_component_container_append(button_component_container_t* container, text_t text, Color normal_color, Color selected_color);
void button_component_container_draw(button_component_container_t container);
void button_component_container_update_sizes(button_component_container_t *container);
void button_component_container_update_position(button_component_container_t *container, const Vector2 position);
