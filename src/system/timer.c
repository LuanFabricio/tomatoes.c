#include <stdio.h>
#include <stddef.h>

#include "raylib.h"

#include "system/timer.h"

timer_pomodoro_t timer_create(seconds_t focus_timer, seconds_t rest_timer, Sound *alarm)
{
	timer_pomodoro_t timer = {
		.total_elapsed = 0.f,
		.focus_timer = focus_timer,
		.focus_elapsed = 0.f,
		.rest_timer = rest_timer,
		.rest_elapsed = 0.f,
		.on_focus = true,
		.on_pause = false,
		.alarm = alarm,
	};

	return timer;
}

void timer_increment(timer_pomodoro_t *timer, seconds_t delta_time)
{
	timer->total_elapsed += delta_time;
	if (timer->on_pause || IsSoundPlaying(*timer->alarm)) return;

	if (timer->on_focus) {
		timer->focus_elapsed += delta_time;
		if (timer->focus_elapsed >= timer->focus_timer) {
			timer_set_focus(timer, false, true);
		}
	} else {
		timer->rest_elapsed += delta_time;
		if (timer->rest_elapsed >= timer->rest_timer) {
			timer_set_focus(timer, true, true);
		}
	}
}

void timer_set_focus(timer_pomodoro_t *timer, bool focus, bool play_sound)
{
	timer->on_focus = focus;

	timer->focus_elapsed = 0.f;
	timer->rest_elapsed = 0.f;

	if (timer->alarm && play_sound) {
		PlaySound(*timer->alarm);
	}
}

Vector2 timer_get_digits(const timer_pomodoro_t *timer)
{
	seconds_t elapsed;
	if (timer->on_focus) {
		elapsed = timer->focus_elapsed;
	} else {
		elapsed = timer->rest_elapsed;
	}

	int minutes, seconds;
	minutes = (int)elapsed / 60;
	seconds = (int)elapsed % 60;

	Vector2 digits = {minutes, seconds};
	return digits;
}

void timer_get_digits_string(const timer_pomodoro_t *timer, char* buffer, const size_t buffer_len)
{
	Vector2 digits = timer_get_digits(timer);
	memset(buffer, '\0', buffer_len);
	snprintf(
		buffer,
		buffer_len,
		"%02d:%02d",
		(int)digits.x,
		(int)digits.y);
}
