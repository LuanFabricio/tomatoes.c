#pragma once

#define TASK_MAX_CONTENT_LEN 50

typedef enum {
	TASK_LEVEL_EASY,
	TASK_LEVEL_MEDIUM,
	TASK_LEVEL_HARD
} task_level_e;

typedef struct {
	char content[TASK_MAX_CONTENT_LEN];
	task_level_e task_level;
	bool completed;
} task_t;
