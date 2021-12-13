#pragma once

#include "syntax.h"
#include "../logs/errors_and_logs.h"

//===================================================================

const int Func_declarations_arr_start_size = 10;

//===================================================================

int _was_func_declared   (int64_t id_hash FOR_LOGS(, LOG_PARAMS));

int _add_func_decleration(int64_t id_hash FOR_LOGS(, LOG_PARAMS));

int _free_func_declarations( FOR_LOGS(LOG_PARAMS));

int _func_declarations_log_dump( FOR_LOGS(LOG_PARAMS));

//===================================================================

#define func_declarations_log_dump() \
       _func_declarations_log_dump( FOR_LOGS (LOG_ARGS))

#define free_func_declarations() \
       _free_func_declarations( FOR_LOGS( LOG_ARGS))

#define was_func_declared(id_hash) \
       _was_func_declared(id_hash FOR_LOGS(, LOG_ARGS))

#define add_func_decleration(id_hash) \
       _add_func_decleration(id_hash FOR_LOGS(, LOG_ARGS))
