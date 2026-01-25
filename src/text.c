#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#include "text.h"

float text__calc_font_spacing(int font_size)
{
	return font_size / 10.f;
}

text_t text_create(char* content, int font_size, Font font)
{
	text_t text = {
		.font_size = font_size,
		.font_spacing = text__calc_font_spacing(font_size),
	};
	const size_t content_len = TextLength(content);
	memset(text.content, '\0', content_len);
	// strncpy(text.content, content, content_len + 1);
	strncpy(text.content, content, TEXT_MAX_CONTENT_LEN);

	text.size = MeasureTextEx(
		font,
		text.content,
		text.font_size,
		text.font_spacing
	);

	return text;
}

void text_draw(const text_t *text, const Color color)
{
	DrawText(
		text->content,
		text->position.x,
		text->position.y,
		text->font_size,
		color
	);

#ifdef DEBUG
	DrawRectangleLines(
		text->position.x,
		text->position.y,
		text->size.x,
		text->size.y,
		GREEN);
#endif
}

void text_update_font_size(text_t *text, Font font, const int font_size)
{
	text->font_size = font_size;
	text->font_spacing = text__calc_font_spacing(font_size);

	text->size = MeasureTextEx(
		font,
		text->content,
		text->font_size,
		text->font_spacing
	);
}
