#include "raylib.h"
#include "components/task_component_container.h"
#include "components/array/task_component_array.h"
#include "components/task_component.h"

task_component_container_t task_component_container_create(Vector2 position)
{
	task_component_container_t container = {0};

	container.position = position;
	container.tasks = task_component_array_create(1);
	return container;
}

void task_component_container_append(task_component_container_t* container, task_t task, int font_size)
{
	const Vector2 position = {
		.x = container->position.x,
        	.y = container->position.y + container->size.y,
	};

	task_component_t render = task_component_create(task, font_size, position);

	task_component_array_append(&container->tasks, render);
	if (container->size.x < render.task_rect.width) {
		container->size.x = render.task_rect.width;
	}

	container->size.y += render.task_rect.height;
}

void task_component_container_draw(task_component_container_t container)
{
	for (size_t i = 0; i < container.tasks.size; i++) {
		task_component_draw(container.tasks.items[i]);
	}


	DrawRectangleLines(
		container.position.x,
		container.position.y,
		container.size.x,
		container.size.y,
		WHITE);
}

void task_component_container_update_sizes(task_component_container_t *container)
{
	float max_container_width = 0;
	float container_height = 0;

	task_component_array_t *tasks = &container->tasks;
	for (size_t i = 0; i < container->tasks.size; ++i) {
		task_component_t *item = &tasks->items[i];

		if (item->task_rect.width > max_container_width) {
			max_container_width = item->task_rect.width;
		}
		container_height += item->task_rect.height;
	}

	printf("Container size y: %.06f\nItem max size y: %.06f\n", container->size.y, container_height);
	printf("Container size x: %.06f\nItem max size x: %.06f\n", container->size.x, max_container_width);
	container->size.y = container_height;
	container->size.x = max_container_width;

	for (size_t i = 0; i < container->tasks.size; ++i) {
		task_component_t *item = &tasks->items[i];
		item->task_rect.width = container->size.x;

		const float padding = (item->task_rect.width - item->task_rect.width) / 2.f;
		item->text_rect.x = item->text_rect.x + padding;
	}
}

void task_component_container_update_position(task_component_container_t *container, const Vector2 position)
{
	container->position = position;

	Vector2 task_position = container->position;
	for(size_t i = 0; i < container->tasks.size; ++i) {
		task_component_t *item = &container->tasks.items[i];
		item->task_rect.x = task_position.x;
		item->task_rect.y = task_position.y;

		const float padding = (item->task_rect.width - item->text_rect.width) / 2.f;
		item->text_rect.x = item->task_rect.x + padding;
		item->text_rect.y = item->task_rect.y + 16.f;

		task_position.y += item->task_rect.height;
	}
}
