#include "button_component_container.h"
#include "../array/button_array.h"
#include <raylib.h>

button_component_container_t button_component_container_create(Vector2 position)
{
	button_component_container_t container = {0};

	container.padding = (Vector2){32, 0};
	container.position = position;
	container.buttons = button_array_create(1);
	return container;
}

void button_component_container_append(button_component_container_t* container, text_t text, Color normal_color, Color selected_color)
{
	Vector2 position = {
		.x = container->position.x + container->size.x,
        	.y = container->position.y,
	};
	position = Vector2Add(position, Vector2Scale(container->padding, container->buttons.size));
	const Vector2 text_padding = {32, 32};

	button_t button = button_create(position, text_padding, text, normal_color, selected_color);

	button_array_append(&container->buttons, button);
	Vector2 container_padding = {0};
	if (container->buttons.size > 1) {
		container_padding = container->padding;
	}
	const float button_height = container_padding.y + button.size.y;
	if (container->size.y < button_height) {
		container->size.y = button_height;
	}

	container->size.x += button.size.x + container_padding.x;
}

void button_component_container_draw(button_component_container_t container)
{
	for (size_t i = 0; i < container.buttons.size; i++) {
		button_draw(&container.buttons.items[i]);
	}
}

void button_component_container_update_sizes(button_component_container_t *container)
{
	button_array_t *buttons = &container->buttons;
	for (size_t i = 0; i < container->buttons.size; ++i) { button_t *item = &buttons->items[i];
		item->size.y = container->size.y;

		const float padding = (item->size.y - item->text.size.y) / 2.f;
		item->text.position.y = item->position.y + padding;
	}
}

void button_component_container_update_position(button_component_container_t *container, const Vector2 position)
{
	container->position = position;

	Vector2 button_position = container->position;
	for(size_t i = 0; i < container->buttons.size; ++i) {
		button_t *item = &container->buttons.items[i];
		item->position = button_position;

		const float padding = (item->size.x - item->text.size.x) / 2.f;
		item->text.position.x = item->position.x + padding;
		item->text.position.y = item->position.y + item->padding.y / 2.f;

		button_position = Vector2Add(button_position, container->padding);
		button_position.x += item->size.x;
	}
}
