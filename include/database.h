#pragma once

#include "system/array/task_array.h"

void database_init();
void database_fetch_tasks(task_array_t *task_array);
void database_update_task(task_t task);
