#pragma once

#include <raylib.h>
#include <stdbool.h>
#include <string.h>

typedef float seconds_t;

typedef struct {
	seconds_t total_elapsed;

	seconds_t focus_elapsed;
	seconds_t focus_timer;

	seconds_t rest_elapsed;
	seconds_t rest_timer;

	bool on_pause;
	bool on_focus;

	Sound *alarm;
} timer_pomodoro_t;

timer_pomodoro_t timer_create(seconds_t focus_timer, seconds_t rest_timer, Sound *alarm);
void timer_increment(timer_pomodoro_t *timer, seconds_t delta_time);
void timer_set_focus(timer_pomodoro_t *timer, bool focus, bool play_sound);
Vector2 timer_get_digits(const timer_pomodoro_t *timer);
void timer_get_digits_string(const timer_pomodoro_t *timer, char* buffer, const size_t buffer_len);
