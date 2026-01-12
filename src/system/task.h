#pragma once

typedef enum {
	TASK_LEVEL_EASY,
	TASK_LEVEL_MEDIUM,
	TASK_LEVEL_HARD
} task_level_e;

typedef struct {
	char* content;
	task_level_e task_level;
	bool completed;
} task_t;
