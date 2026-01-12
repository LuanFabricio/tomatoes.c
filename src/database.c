#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlite3.h"

#include "array/task_array.h"

static sqlite3 *db = NULL;

void database_init()
{
	int ret = sqlite3_open("./database.db", &db);
	if (ret) {
		printf("[Error]: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(-1);
	}
}

void database_fetch_tasks()
{

}

int main_()
{
	database_init();
	printf("Database ptr: %p\n", db);

	return 0;
}
