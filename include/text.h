#pragma once

#include "raylib.h"

#define TEXT_MAX_CONTENT_LEN 0xff

typedef struct {
	char content[TEXT_MAX_CONTENT_LEN];

	Vector2 position;
	Vector2 size;

	int font_size;
	float font_spacing;
} text_t;

// TODO: Move text create to use an text_t buffer instead of create from scratch
text_t text_create(char* content, int font_size, Font font);

void text_draw(const text_t *text, const Color color);

void text_update_font_size(text_t *text, Font font, const int font_size);
