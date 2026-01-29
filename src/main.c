#include <stddef.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "button.h"
#include "components/button_component_container.h"
#include "database.h"
#include "raylib.h"

#include "./system/task.h"
#include "./system/timer.h"
#include "./components/task_component.h"
#include "./components/task_component_container.h"
#include "./text.h"
#include "system/array/task_array.h"

#define DIGITS_BUFFER_LEN 10

static Font font;
static Sound alarm;

static timer_pomodoro_t timer;
static task_component_container_t task_container;
static button_component_container_t button_component_container;

static Vector2 digits_position = { 0, 100 };

static int screen_width = 1280;
static int screen_height = 760;

static char digits_buffer[DIGITS_BUFFER_LEN];

static task_component_t task_component;


static const int default_font_size = 32;

static void button_next_on_click_callback(void *button_raw)
{
	button_t *button = (button_t*) button_raw;
	printf("Button %p clicked\n", button);
	timer_set_focus(&timer, !timer.on_focus, false);
}

static void button_pause_on_click_callback(void* button_raw)
{
	button_t *button = (button_t*) button_raw;
	printf("Button %p clicked\n", button);
	timer.on_pause = !timer.on_pause;
}

void update_positions()
{
	task_component_container_update_position(&task_container, (Vector2){screen_width-task_container.size.x-30, 120});
	button_component_container_update_position(
			&button_component_container,
			(Vector2){
			(screen_width - button_component_container.size.x)/ 2.f,
			digits_position.y + 120
			});
}

void setup(task_array_t tasks)
{
	InitWindow(screen_width, screen_height, "Pomodoro");
	InitAudioDevice();

	font = GetFontDefault();
	alarm = LoadSound("clock-alarm-8761.mp3");
	timer = timer_create(60, 30, &alarm);

	task_t task = {
		.content = "test",
		.task_level = TASK_LEVEL_EASY,
		.completed = false,
	};
	task_component = task_component_create(task, 32, (Vector2){screen_width/2.f, screen_height/2.f});
;
	task_container = task_component_container_create((Vector2){64, 64});
	for (size_t i = 0; i < tasks.size; i++) {
		task_component_container_append(
			&task_container,
			tasks.items[i],
			32
		);
	}
	/*
	task_component_container_append(
		&task_container,
		(task_t){.content = "task easy (1)", .task_level = TASK_LEVEL_EASY},
		32);
	task_component_container_append(
		&task_container,
		(task_t){.content = "task medium (2)", .task_level = TASK_LEVEL_MEDIUM},
		32);
	task_component_container_append(
		&task_container,
		(task_t){.content = "task hard (3)", .task_level = TASK_LEVEL_HARD},
		32);
	task_component_container_append(
		&task_container,
		(task_t){.content = "Ir para academia", .task_level = TASK_LEVEL_HARD},
		32);
	*/

	task_component_container_update_sizes(&task_container);
	task_component_container_update_position(&task_container, (Vector2){960-task_container.size.x-30, 120});

	button_component_container = button_component_container_create((Vector2){342, 42});
	button_component_container_append(
		&button_component_container,
		text_create("Next", default_font_size, font),
		BLUE,
		RED
	);
	button_component_container.buttons.items[0].on_click_callback = &button_next_on_click_callback;

	button_component_container_append(
		&button_component_container,
		text_create("Pause", default_font_size, font),
		BLUE,
		RED
	);
	button_component_container.buttons.items[1].on_click_callback = &button_pause_on_click_callback;

	update_positions();

	task_container.tasks.items[0].task.completed = true;
}

void draw_loop()
{
	const int font_size = 64;
	const Color timer_color = timer.on_focus ? RED : GREEN;
	timer_get_digits_string(&timer, digits_buffer, DIGITS_BUFFER_LEN);

	const int digits_buffer_width = MeasureText(digits_buffer, font_size);
	digits_position.x = (screen_width - digits_buffer_width) / 2.f;
	digits_position.y = 100;


	BeginDrawing();

	ClearBackground(BLACK);

	task_component_container_draw(task_container);
	task_component_draw(task_component);

	DrawText(digits_buffer, digits_position.x, digits_position.y, font_size, timer_color);

	button_component_container_draw(button_component_container);

	EndDrawing();
}

void update_loop()
{
	seconds_t frame_time = GetFrameTime();
	timer_increment(&timer, frame_time);

	bool window_changed = screen_width != GetScreenWidth()
		|| screen_height != GetScreenHeight();
	screen_width = GetScreenWidth();
	screen_height = GetScreenHeight();



	const Vector2 mouse = GetMousePosition();
	const bool mouse_clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

	for (size_t i = 0; i < button_component_container.buttons.size; i++) {
		button_t* button = &button_component_container.buttons.items[i];

		button->selected = button_contain_point(button, mouse);
		const bool is_button_clicked = button->selected && mouse_clicked;
		if (is_button_clicked && button->on_click_callback) {
			button->on_click_callback(button);
		}
	}

	for (size_t i = 0; i < task_container.tasks.size; i ++) {
		task_component_t *component = &task_container.tasks.items[i];

		component->selected = task_component_contain_point(component, mouse);
		const bool is_task_clicked = component->selected && mouse_clicked;
		if (is_task_clicked) {
			component->task.completed = !component->task.completed;
		}
	}


	if (window_changed) {
		update_positions();
	}
}

int main(void)
{

	database_init();
	task_array_t task_array = {0};
	database_fetch_tasks(&task_array);
	setup(task_array);


	while (!WindowShouldClose()) {
		draw_loop();
		update_loop();
	}

	for (size_t i = 0; i < task_array.size; i++) {
		database_update_task(task_array.items[i]);
	}
	task_array_free(&task_array);

	return 0;
}
