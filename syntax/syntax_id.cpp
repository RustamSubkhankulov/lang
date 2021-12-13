#include <stdlib.h>
#include <string.h>

#include "../general/general.h"
#include "../trans/trans.h"
#include "syntax_id.h"

//===================================================================

int In_func_flag = 0;
 
int In_defn_flag = 0;  

//===================================================================

static Var* Variables_decl = 0;

static int Variables_decl_counter = 0;

static int Variables_decl_capacity = 0;

//-------------------------------------------------------------------

static int64_t* Labels_decl = 0;

static int Labels_decl_counter = 0;

static int Labels_decl_capacity = 0;

//-------------------------------------------------------------------

static int Ram_pos = 0;

//===================================================================

int _in_func_flag_on( FOR_LOGS(LOG_PARAMS))
{
    lang_log_report();

    In_func_flag = 1;
    return 0;
}

//-------------------------------------------------------------------

int _in_func_flag_off( FOR_LOGS(LOG_PARAMS))
{
    lang_log_report();

    In_func_flag = 0;
    return 0;
}

//-------------------------------------------------------------------

int _in_defn_flag_on( FOR_LOGS(LOG_PARAMS))
{
    lang_log_report();

    In_defn_flag = 1;
    return 0;
}

//-------------------------------------------------------------------

int _in_defn_flag_off( FOR_LOGS(LOG_PARAMS))
{
    lang_log_report();

    In_defn_flag = 0;
    return 0;
}

//===================================================================

int _free_label_declarations( FOR_LOGS(LOG_PARAMS))
{
    if (Labels_decl)
    {
        free(Labels_decl);
    }

    return 0;
}

//-------------------------------------------------------------------

int _free_variables_declarations( FOR_LOGS(LOG_PARAMS))
{
    printf("\n\n number %d cap %d\n\n", Variables_decl_counter, Variables_decl_capacity);
    
    printf("\n\n variables decl are freed \n\n");

    if (Variables_decl)
    {
        free(Variables_decl);
        Variables_decl = NULL;
    }

    Variables_decl_capacity = 0;
    Variables_decl_counter  = 0;

    return 0;
}

//-------------------------------------------------------------------

int _was_label_declared(int64_t id_hash FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();

    for (int counter = 0; counter < Labels_decl_counter; counter++)
    {
        if (id_hash == Labels_decl[counter])
            return 1;
    }

    return 0;
}

//-------------------------------------------------------------------

int _add_label_decl(int64_t id_hash FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();

    if (!Labels_decl_capacity)
    {
        Labels_decl = (int64_t*)calloc(Start_label_declarations_number, sizeof(int64_t));
        ALLOCATION_CHECK(Labels_decl);

        Labels_decl_capacity = Start_label_declarations_number;
    }

    if (Labels_decl_counter == Labels_decl_capacity - 1)
    {
        Labels_decl = (int64_t*)my_recalloc(Labels_decl, (size_t)(Labels_decl_capacity * 2), 
                                                         (size_t) Labels_decl_capacity, 
                                                          sizeof (int64_t));
        ALLOCATION_CHECK(Labels_decl);

        Labels_decl_capacity *= 2;
    }

    Labels_decl[Labels_decl_counter++] = id_hash;

    return 0;
}

//===================================================================

int _was_var_declared(int64_t id_hash FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();

    for (int counter = 0; counter < Variables_decl_counter; counter++)

        if (id_hash == Variables_decl[Variables_decl_counter - 1 - counter].hash)
            return 1;

    return 0;
}

//-------------------------------------------------------------------

int _get_var_pos(int64_t var_hash FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();

    fflush(logs_file);
    printf("\n\n hash %ld \n\n", var_hash);

    for (int counter = 0; counter < Variables_decl_counter; counter++)
    
        if (var_hash == Variables_decl[Variables_decl_counter - 1 - counter].hash)
            return Variables_decl[counter].ram_pos;

    error_report(VAR_UNDECLARED);
    return -1;
}

//-------------------------------------------------------------------

int _add_var_decl(int64_t id_hash, int is_perm FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();

    if (!Variables_decl_capacity)
    {
        Variables_decl = (Var*)calloc(Start_variables_declarations_number, sizeof(Var));
        ALLOCATION_CHECK(Variables_decl);

        Variables_decl_capacity = Start_variables_declarations_number;
        Ram_pos = 0;
    }

    if (Variables_decl_counter == Variables_decl_capacity - 1)
    {
        Variables_decl = (Var*)my_recalloc(Variables_decl, (size_t)(Variables_decl_capacity * 2), 
                                                           (size_t) Variables_decl_capacity, 
                                                            sizeof (Var));
        ALLOCATION_CHECK(Variables_decl);

        Variables_decl_capacity *= 2;
        printf("\n\n increasing variables array counter %d cap %d\n\n", Variables_decl_counter, Variables_decl_capacity);
    }

    printf("\n\n before add counter %d capacity %d \n\n", Variables_decl_counter, Variables_decl_capacity);

    Variables_decl[Variables_decl_counter].hash     = id_hash;
    Variables_decl[Variables_decl_counter].ram_pos  = Ram_pos++;
    Variables_decl[Variables_decl_counter].is_perm  = is_perm;

    Variables_decl_counter++;

    printf("\n\n after add counter %d capacity %d \n\n", Variables_decl_counter, Variables_decl_capacity);

    return 0;
}

//-------------------------------------------------------------------

int _is_perm_var(int64_t var_hash FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();

    int index = -1;

    for ( int counter = 0; counter < Variables_decl_counter; counter++)
    
        if (var_hash == Variables_decl[Variables_decl_counter - 1 - counter].hash)
        { 
            index = counter;
            break;
        }
        
    if (index == -1)
    {
        error_report(VAR_UNDECLARED);
        return -1;
    }

    return Variables_decl[index].is_perm;
}

//-------------------------------------------------------------------

int _get_variables_number(FOR_LOGS(LOG_PARAMS))
{
    lang_log_report();

    return Variables_decl_counter;
}

//-------------------------------------------------------------------

int _remove_variables_declarations(int var_number FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();

    if (var_number < 0 || var_number > Variables_decl_counter)
    {
        error_report(INV_VAR_NUMBER);
        return -1;
    }

    memset(Variables_decl + var_number, 0, sizeof(int64_t) * (size_t)(Variables_decl_counter - var_number));
    Variables_decl_counter = var_number;
    Ram_pos -= (Variables_decl_counter - var_number);

    if (Variables_decl_counter < (Variables_decl_capacity/4) && Variables_decl_counter > Start_variables_declarations_number * 2)
    {
        Variables_decl = (Var*)my_recalloc(Variables_decl, (size_t)(Variables_decl_capacity/2), 
                                                           (size_t)(Variables_decl_capacity), 
                                                            sizeof(Var));
        ALLOCATION_CHECK(Variables_decl);

        Variables_decl_capacity /= 2;
    }

    return 0;
}

//===================================================================

int _labels_log_dump( FOR_LOGS(LOG_PARAMS))
{
    lang_log_report();

    fprintf(logs_file, "\n<pre>\n");

    fprintf(logs_file, "<div class=\"outline\"  "
                                   "style = \"background-color:lightgrey;\" "
                                   "style = \"text-align: center;\">");

    fprintf(logs_file, "\n<b>Labels capacity: %d Labels counter: %d </b>\n\n", 
                                   Labels_decl_capacity, Labels_decl_counter);
    
    for (int counter = 0; counter < Labels_decl_counter; counter++)
    {
        fprintf(logs_file, "\n Index: [%d] Id_hash: %ld\n", counter, Labels_decl[counter]);

    }

    fprintf(logs_file, "\n</div></pre>\n");
    return 0;
}

//-------------------------------------------------------------------

int _variables_log_dump( FOR_LOGS(LOG_PARAMS))
{
    lang_log_report();

    fprintf(logs_file, "\n<pre>\n");

    fprintf(logs_file, "<div class=\"outline\"  "
                                   "style = \"background-color:lightgrey;\" "
                                   "style = \"text-align: center;\">");

    fprintf(logs_file, "\n<b>Variables capacity: %d Variables counter: %d </b>\n\n", 
                                   Variables_decl_capacity, Variables_decl_counter);
    
    for (int counter = 0; counter < Variables_decl_counter; counter++)
    {
        fprintf(logs_file, "\n Index: [%d] Id_hash: %ld Ram pos: %d Is permanent %d\n", counter, Variables_decl[counter].hash, 
                                                                                                 Variables_decl[counter].ram_pos,
                                                                                                 Variables_decl[counter].is_perm);
    }

    fprintf(logs_file, "\n</div></pre>\n");
    return 0;
}




