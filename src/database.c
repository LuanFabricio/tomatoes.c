#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlite3.h"

#include "system/array/task_array.h"

typedef struct {
	char **items;
	size_t size;
	size_t capacity;
} tables_t;

#define da_append(arr, value)\
do {\
	if (arr->size >= arr->capacity) {\
		if (arr->capacity <= 0) {\
			arr->capacity = 10;\
		}\
		if (arr->items == NULL) {\
			arr->items = malloc(sizeof(*arr->items) * arr->capacity);\
		} else {\
			arr->capacity *= 2;\
			arr->items = realloc(arr->items, sizeof(*arr->items) * arr->capacity);\
		}\
	}\
	arr->items[arr->size] = value;\
	arr->size += 1;\
} while(0)

static sqlite3 *db = NULL;

static int database__fetch_tables_callback(void* table_arr_ptr, int argc, char**argv, char** col_name)
{
	tables_t *table_names = (tables_t*) table_arr_ptr;
	const size_t name_len = strlen(argv[0]);
	char* value = malloc(name_len + 1);
	strcpy(value, argv[0]);
	da_append(table_names, value);
	return 0;
}

static void database__fetch_tables()
{
	if (db == NULL) return;
	char* err_msg;
	const char *query = "SELECT name FROM sqlite_master WHERE type='table';";
	tables_t tables = {0};
	sqlite3_exec(db, query, database__fetch_tables_callback, &tables, &err_msg);

	printf("System tables:\n");
	for (size_t i = 0; i < tables.size; i++) {
		printf("\t%s\n", tables.items[i]);
	}

	bool exist_task_table = false;
	for (size_t i = 0; i < tables.size; i++) {
		exist_task_table |= strcmp(tables.items[i], "tasks") == 0;
	}

	// TODO: Check if all of the app tables exists, if not create it.
	if (!exist_task_table) {
		printf("Creating task table...\n");
		const char* create_task_table = ""
			"create table tasks(name varchar(30), level int, completed bool)";
		if (sqlite3_exec(db, create_task_table, NULL, NULL, &err_msg) == SQLITE_ABORT) {
			printf("ERROR: %s\n", err_msg);
		}
	}
}

static int database__fetch_tasks_callback(void* task_arr_ptr, int argc, char**argv, char** col_name)
{
	task_array_t *task_array = (task_array_t*)task_arr_ptr;

	if (task_array == NULL) return 0;
	const size_t index = task_array ? task_array->size : 0;
	printf("========== ROW %02lu ==========\n", index + 1);
	task_t task = {
		.task_level = atoi(argv[2]),
		.completed = strcmp(argv[3], "true") == 0,
	};


	if (task_array) {
		strncpy(task.content, argv[1], TASK_MAX_CONTENT_LEN);
		task_array_append(task_array, task);
	}
	for (size_t i = 0; i < argc; i++) {
		printf("%s = %s\n", col_name[i], argv[i] ? argv[i] : "NULL");
	}
	return 0;
}

void database_init()
{
	int ret = sqlite3_open("./database.db", &db);
	if (ret) {
		printf("[Error]: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(-1);

	}
	database__fetch_tables();
}

void database_fetch_tasks(task_array_t *task_array)
{
	if (db == NULL) return;

	char *err_msg;
	int res = sqlite3_exec(db, "select * from tasks;", database__fetch_tasks_callback, (void*)task_array, &err_msg);
	if (res != SQLITE_OK) printf("%s\n", err_msg);

	if (task_array == NULL) return;

	for (size_t i = 0; i < task_array->size; i++) {
		const task_t *task = &task_array->items[i];
		printf("============== Task %02lu ==============\n", i);
		printf("\t.content=%s\n", task->content);
		printf("\t.level=%d\n", task->task_level);
		printf("\t.completed=%s\n", task->completed ? "true" : "false");
	}
}

int main_()
{
	database_init();
	printf("Database ptr: %p\n", db);
	database_fetch_tasks(NULL);

	return 0;
}
