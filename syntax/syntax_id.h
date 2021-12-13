#pragma once

#include "syntax.h"
#include "../logs/errors_and_logs.h"

//===================================================================

struct Var
{
    int64_t hash;
    int ram_pos;
    int is_perm;
};

//===================================================================

const int Start_label_declarations_number     = 10;

const int Start_variables_declarations_number = 5;

//===================================================================

int _was_label_declared(int64_t id_hash  FOR_LOGS(, LOG_PARAMS));

int _is_perm_var       (int64_t var_hash FOR_LOGS(, LOG_PARAMS));

int _add_label_decl    (int64_t id_hash  FOR_LOGS(, LOG_PARAMS));

int _was_var_declared  (int64_t id_hash  FOR_LOGS(, LOG_PARAMS));

int _add_var_decl      (int64_t id_hash, int is_perm FOR_LOGS(, LOG_PARAMS));

int _get_variables_number(FOR_LOGS(LOG_PARAMS));

int _remove_variables_declarations(int var_number FOR_LOGS(, LOG_PARAMS));

int _free_label_declarations( FOR_LOGS(LOG_PARAMS));

int _free_variables_declarations( FOR_LOGS(LOG_PARAMS));

int _get_var_pos(int64_t var_hash FOR_LOGS(,LOG_PARAMS));

//-------------------------------------------------------------------

int _labels_log_dump( FOR_LOGS(LOG_PARAMS));

int _variables_log_dump( FOR_LOGS(LOG_PARAMS));

//-------------------------------------------------------------------

int _in_func_flag_on( FOR_LOGS(LOG_PARAMS));

int _in_func_flag_off( FOR_LOGS(LOG_PARAMS));

int _in_defn_flag_on( FOR_LOGS(LOG_PARAMS));

int _in_defn_flag_off( FOR_LOGS(LOG_PARAMS));

//===================================================================

#define is_perm_var(hash) \
       _is_perm_var(hash FOR_LOGS(, LOG_ARGS))

#define in_func_flag_on() \
       _in_func_flag_on( FOR_LOGS(LOG_ARGS))

#define in_func_flag_off() \
       _in_func_flag_off( FOR_LOGS(LOG_ARGS))

#define in_defn_flag_on() \
       _in_defn_flag_on( FOR_LOGS(LOG_ARGS))

#define in_defn_flag_off() \
       _in_defn_flag_off( FOR_LOGS(LOG_ARGS))

//-------------------------------------------------------------------

#define labels_log_dump() \
       _labels_log_dump( FOR_LOGS( LOG_ARGS))

#define variables_log_dump() \
       _variables_log_dump( FOR_LOGS(LOG_ARGS))

//-------------------------------------------------------------------

#define free_label_declarations() \
       _free_label_declarations( FOR_LOGS( LOG_ARGS))

#define free_variables_declarations() \
       _free_variables_declarations( FOR_LOGS( LOG_ARGS))

//-------------------------------------------------------------------

#define was_label_declared(id_hash) \
       _was_label_declared(id_hash FOR_LOGS(, LOG_ARGS))

#define add_label_decl(id_hash) \
       _add_label_decl(id_hash FOR_LOGS(, LOG_ARGS))

//-------------------------------------------------------------------

#define was_var_declared(id_hash) \
       _was_var_declared(id_hash FOR_LOGS(, LOG_ARGS))

#define add_var_decl(id_hash, is_perm) \
       _add_var_decl(id_hash, is_perm FOR_LOGS(, LOG_ARGS))

#define get_variables_number() \
       _get_variables_number( FOR_LOGS(LOG_ARGS))

#define remove_variables_declarations(var_number) \
       _remove_variables_declarations(var_number FOR_LOGS(, LOG_ARGS))

#define get_var_pos(hash) \
       _get_var_pos(hash FOR_LOGS(, LOG_ARGS))
