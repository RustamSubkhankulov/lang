#include <stdlib.h>

#include "../general/general.h"
#include "func_definitions.h"

//===================================================================

static int64_t* Func_declarations = NULL;

static int Func_declarations_number = 0;

static int Func_declarations_arr_capacity = 0;

//===================================================================

int _func_declarations_log_dump( FOR_LOGS(LOG_PARAMS))
{
        lang_log_report();

    fprintf(logs_file, "\n<pre>\n");

    fprintf(logs_file, "<div class=\"outline\"  "
                                   "style = \"background-color:lightgrey;\" "
                                   "style = \"text-align: center;\">");

    fprintf(logs_file, "\n<b>Func declarations array capacity: %d Func declarations counter: %d </b>\n\n", 
                                   Func_declarations_arr_capacity, Func_declarations_number);
    
    for (int counter = 0; counter < Func_declarations_number; counter++)
    {
        fprintf(logs_file, "\n Index: [%d] Id_hash: %ld\n", counter, Func_declarations[counter]);

    }

    fprintf(logs_file, "\n</div></pre>\n");
    return 0;
}

//-------------------------------------------------------------------

int _free_func_declarations( FOR_LOGS (LOG_PARAMS))
{
    if (!Func_declarations)
    {
        free(Func_declarations);
    }

    return 0;
}

//-------------------------------------------------------------------

int _was_func_declared(int64_t id_hash FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();

    for (int counter = 0; counter < Func_declarations_number; counter++)
    {
        if (id_hash == Func_declarations[counter])
            return 1;
    }

    return 0;
}

//-------------------------------------------------------------------

int _add_func_decleration(int64_t id_hash FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();

    if (!Func_declarations_arr_capacity)
    {
        Func_declarations = (int64_t*)calloc(sizeof(int64_t), Func_declarations_arr_start_size);
        if (!Func_declarations)
        {
            error_report(CANNOT_ALLOCATE_MEM);
            return -1;
        }
        Func_declarations_arr_capacity = Func_declarations_arr_start_size;
    }

    if (Func_declarations_number == Func_declarations_arr_capacity - 1)
    {
        Func_declarations = (int64_t*)my_recalloc((void*)Func_declarations, 
                                                 (size_t)Func_declarations_arr_capacity * 2, 
                                                 (size_t)Func_declarations_arr_capacity, 
                                                  sizeof(int64_t));
        if (!Func_declarations)
        {
            error_report(CANNOT_ALLOCATE_MEM);
            return -1;
        }

        Func_declarations_arr_capacity *= 2;
    }

    Func_declarations[Func_declarations_number++] = id_hash;
    return 0;
}