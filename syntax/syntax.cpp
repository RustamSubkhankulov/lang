#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "syntax.h"
#include "../general/general.h"
#include "func_definitions.h"
//#include "syntax_id.h"

//===================================================================

#define CUR_TOKEN tokens->token_array[tokens->counter]

#define N_TOKEN tokens->token_array[tokens->counter + 1]

#define N_N_TOKEN tokens->token_array[tokens->counter + 2]

#define PR_TOKEN tokens->token_array[tokens->counter - 1]

//--------------------------------------------------------------------

#define L left_son
#define R right_son

#define NL node->left_son

#define NR node->right_son

#define NLL node->left_son->left_son

#define NLR node->left_son->right_son

#define NRR node->right_son->right_son

#define NRL node->right_son->left_son

//===================================================================

// int node_afterbuild_check(Node* node FOR_LOGS(, LOG_PARAMS))
// {
//     NODE_PTR_CHECK(node);

//     return 0;
// }

//-------------------------------------------------------------------

int _init_names_struct(Names* names FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();

    if (!names)
    {
        error_report(INV_NAMES_STRUCT_PTR);
        return -1;
    }

    int ret = 0;

    ret = init_var_cluster(&(names->var_cluster));
    RETURN_CHECK(ret);

    ret = init_label_cluster(&(names->label_cluster));
    RETURN_CHECK(ret);

    ret = init_func_cluster(&(names->func_cluster));
    RETURN_CHECK(ret);

    return 0;
}

//-------------------------------------------------------------------

int _kill_names_struct(Names* names FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();

    if (!names)
    {
        error_report(INV_NAMES_STRUCT_PTR);
        return -1;
    }

    int ret = 0;

    ret = kill_var_cluster(names->var_cluster);
    RETURN_CHECK(ret);

    ret = kill_label_cluster(names->label_cluster);
    RETURN_CHECK(ret);

    ret = kill_func_cluster(names->label_cluster);
    RETURN_CHECK(ret);

    return 0;
}

//-------------------------------------------------------------------

int _init_var_cluster  (Var_cluster*   var_cluster   FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    VAR_CLASTER_PTR_CHECK(var_cluster);

    var_cluster->var_spaces = (Var_space*)calloc(sizeof(Var_space), sizeof(char));
    NULL_CHECK(var_cluster->var_spaces);

    var_cluster->var_spaces_num = 1;
    var_cluster->cur_var_space  = var_cluster->var_spaces;

    cur_var_space->var_names = (Var_name*)calloc(Var_names_start_num, sizeof(Var_name));
    NULL_CHECK(cur_var_space->var_names);

    cur_var_space->var_names_cap = Var_names_start_num;

    return 0;
}

//-------------------------------------------------------------------

int _kill_var_cluster  (Var_cluster*   var_cluster   FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    VAR_CLASTER_PTR_CHECK(var_cluster);

    for (int counter = 0; counter < var_cluster->var_spaces_num, counter++)
    {
        if (var_cluster->var_spaces[counter].var_names)
            free(var_cluster->var_spaces[counter].var_names);
    }

    if (var_cluster->var_spaces)
        free(var_cluster->var_spaces);

    var_cluster->var_spaces     = NULL;
    var_cluster->cur_var_space  = NULL;
    var_cluster->var_spaces_num = 0;

    return 0;
}

//-------------------------------------------------------------------

int _init_label_cluster(Label_cluster* label_cluster FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    LABEL_CLASTER_PTR_CHECK(label_cluster);

    label_cluster->label_names = (Label_name*)calloc(Label_names_start_num, sizeof(Label_name));
    NULL_CHECK(label_cluster->label_names);

    label_cluster->label_names_cap = Label_names_start_num;

    return 0;
}

//-------------------------------------------------------------------

int _kill_label_cluster(Label_cluster* label_cluster FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    LABEL_CLASTER_PTR_CHECK(Label_cluster);

    if (label_cluster->label_names)
        free(label_cluster->label_names);

    label_cluster->label_names = NULL;
    label_cluster->label_names_cap = 0;
    label_cluster->label_names_num = 0;

    return 0;
}

//-------------------------------------------------------------------

int _init_func_cluster (Func_cluster*  func_cluster  FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    FUNC_CLASTER_PTR_CHECK(func_cluster);

    func_cluster->func_names = (Func_name*)calloc(Func_names_start_num, sizeof(Func_name));
    NULL_CHECK(func_cluster->func_names);

    func_cluster->func_names_cap = Func_names_start_num;

    return 0;
}

//-------------------------------------------------------------------

int _kill_func_cluster (Func_cluster*  func_cluster  FOR_LOGS(, LOG_PARAMS))
{
    if (func_cluster->func_names)
        free(func_cluster->func_names);

    func_cluster->func_names = NULL;
    func_cluster->func_names_cap = 0;
    func_cluster->func_names_num = 0;

    return 0;
}

//-------------------------------------------------------------------

int _add_func_defn   (int64_t hash, Func_cluster*  func_cluster  FOR_LOGS(, LOG_PARAMS))
[
    lang_log_report();
    FUNC_CLASTER_PTR_CHECK(func_cluster);

    if (func_cluster->func_names_num = func_cluster->func_names_cap - 1)
    {
        int ret = func_defn_arr_increase(func_cluster);
        RETURN_CHECK(ret);
    }

    func_cluster->func_names[func_cluster->func_names_num++] = hash;
    return 0;
]

//-------------------------------------------------------------------

int _func_is_defined (int64_t hash, Func_cluster*  func_cluster  FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    FUNC_CLASTER_PTR_CHECK(func_cluster);

    for (int counter = 0; counter < func_cluster->func_names_num; counter++)
    {
        if (hash == func_cluster->func_names[counter])
            return 1;
    }

    return 0;
}


//-------------------------------------------------------------------

int _func_defn_arr_increase(Func_cluster* func_cluster FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    FUNC_CLASTER_PTR_CHECK(func_cluster);

    func_cluster->func_names = (Func_name*)my_recalloc(func_cluster->func_names, 
                                              (size_t)(func_cluster->func_names_cap * 2), 
                                               size_t (func_cluster->func_names_cap), 
                                               sizeof(Func_name));
    NULL_CHECK(func_cluster->func_names);

    func_cluster->func_names_cap *= 2;

    return 0;
}

//-------------------------------------------------------------------

int _add_var_declare(int64_t hash, int is_perm, Var_cluster*   var_cluster   FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    VAR_CLASTER_PTR_CHECK(var_cluster);

    if (var_cluster->cur_var_space->var_names_num 
    ==  var_cluster->cur_var_space->var_names_cap - 1)
    {
        int ret = var_decl_arr_increase(var_cluster);
        RETURN_CHECK(ret);
    }

    var_names     = var_cluster->cur_var_space->var_names;
    var_names_num = var_cluster->cur_var_space->var_names_num;

    var_names[var_names_num].hash    = hash;
    var_names[var_names_num].is_perm = is_perm;

    var_cluster->cur_var_space->var_names_num++;

    return 0;
}

//-------------------------------------------------------------------

int _var_is_declared (int64_t hash, Var_cluster*   var_cluster   FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    VAR_CLASTER_PTR_CHECK(var_cluster);

    for (int counter = 0; counter < var_cluster->var_spaces_num; counter++)

        for (int ct = 0; ct < var_cluster->var_spaces[counter].var_names_num)
        {
            if (hash = var_cluster->var_spaces[counter].vars[ct].hash)
                return 1;
        }

    return 0;
}

//------------------------------------------------------------------

int _var_is_permanent(int64_t hash, Var_cluster* var_cluster FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    VAR_CLASTER_PTR_CHECK(var_cluster);

    for (int counter = 0; counter < var_cluster->var_spaces_num; counter++)

        for (int ct = 0; ct < var_cluster->var_spaces[counter].var_names_num)
        {
            if (hash = var_cluster->var_spaces[counter].vars[ct].hash)
                return var_cluster->var_spaces[counter].vars[ct].is_perm;
        }

    error_report(VAR_UNDECLARED);
    return -1;
}

//-------------------------------------------------------------------

int _var_decl_arr_increase(Var_cluster* var_cluster FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    VAR_CLASTER_PTR_CHECK(var_cluster);

    int var_names_cap   = var_cluster->cur_var_space->var_names_cap;
    Var_name* var_names = var_cluster->cur_var_space->var_names;

    var_cluster->cur_var_space->var_names = (Var_name*)my_recalloc(var_names, 
                                                          (size_t)(var_names_cap * 2), 
                                                          (size_t) var_names_cap, 
                                                           sizeof (Var_name));
    NULL_CHECK(var_cluster->cur_var_space->var_names);

    var_cluster->cur_var_space->var_names_cap *= 2;

    return 0;
}

//-------------------------------------------------------------------

int _add_var_space(Var_cluster* var_cluster FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    VAR_CLASTER_PTR_CHECK(var_cluster);

    int var_spaces_num = var_cluster->var_spaces_num;

    if (!var_spaces_num)
    {
        var_cluster->var_spaces = (Var_space*)calloc(sizeof(Var_space), sizeof(char));
        NULL_CHECK(var_cluster->var_spaces);
        
        var_cluster->var_spaces_num = 1;
        var_cluster->cur_var_space = var_spaces;
    }

    else
    {
        var_cluster->var_spaces = (Var_space*)my_recalloc(var_cluster->var_spaces, 
                                                              (size_t)(var_spaces_num + 1), 
                                                              (size_t) var_spaces_num, 
                                                               sizeof (Var_space));
        NULL_CHECK(var_cluster->var_spaces);

        var_cluster->var_spaces_num++;
        var_cluster->cur_var_space = &(var_cluster->var_spaces[var_cluster->var_spaces_num - 1]);
    }
    var_cluster->cur_var_space->var_names = (Var_name*)calloc(Var_names_start_num, sizeof(Var_name));
    NULL_CHECK(var_cluster->cur_var_space->var_names);

    var_cluster->cur_var_space->var_names_cap = Var_names_start_num;

    return 0;
}

//-------------------------------------------------------------------

int _rm_var_space(Var_cluster* var_cluster FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    VAR_CLASTER_PTR_CHECK(var_cluster);

    if (var_cluster->cur_var_space->var_names)
        free(var_cluster->cur_var_space->var_names);

    int var_spaces_num = var_cluster->var_spaces_num;

    if (var_spaces_num == 1)
    {
        free(var_cluster->var_spaces);

        var_cluster->var_spaces_num = 0;
        var_cluster->var_spaces     = NULL;
        var_cluster->cur_var_space  = NULL;
    }

    else
    {
        var_cluster->var_spaces = (Var_sapce*)my_recalloc(var_cluster->var_spaces, 
                                                              (size_t)(var_spaces_num - 1), 
                                                              (size_t) var_spaces_num, 
                                                               sizeof (Var_space));
        NULL_CHECK(var_cluster->var_spaces);

        var_cluster->var_spaces_num--;
        var_cluster->cur_var_space = &(var_cluster->var_spaces[var_cluster->var_spaces_num - 1]);
    }

    return 0;
}

//-------------------------------------------------------------------

int _add_label_defn  (int64_t hash, Label_cluster* label_cluster FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    LABEL_CLASTER_PTR_CHECK(label_cluster);

    if (label_cluster->label_names_num == Label_cluster->label_names_cap - 1)
    {
        int ret = label_defn_arr_increase(label_cluster);
        RETURN_CHECK(ret);
    }

    label_cluster->label_names[label_cluster->label_names_num++] = hash;
    return 0;
}

//-------------------------------------------------------------------

int _label_is_defined(int64_t hash, Label_cluster* label_cluster FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    LABEL_CLASTER_PTR_CHECK(label_cluster);

    for (int counter = 0; counter < label_cluster->label_names_num; counter++)
    {
        if (hash = label_cluster->label_names[counter])
            return 1;
    }

    return 0;
}

//-------------------------------------------------------------------

int _label_defn_arr_increase(Label_cluster* label_cluster FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    LABEL_CLASTER_PTR_CHECK(label_cluster);

    label_cluster->label_names = (Label_name*)my_recalloc(label_cluster->label_names, 
                                                 (size_t)(label_cluster->label_names_cap * 2), 
                                                 (size_t) label_cluster->label_names_cap, 
                                                  sizeof(Label_name));
    NULL_CHECK(label_cluster->label_names);

    label_cluster->label_names_cap *= 2;

    return 0;
}

//--------------------------------------------------------------------

int _names_struct_dump(Names* names FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    NAMES_PTR_CHECK(names);

    fprintf(logs_file, "<pre><div class=\"outline\"  "
                                   "style = \"background-color:lightgrey;\" "
                                   "style = \"text-align: center;\">\n");

    fprintf(logs_file, "\n\n <b> Names structure dump </b> \n");

    fprintf(logs_file, "Address: <%p>\n", names);

    Var_cluster* var_cluster = names->var_cluster;
    fprintf(logs_file, "\n <b> Var cluster </b> <%p> \n", names->var_cluster);
    fprintf(logs_file, "Var_spaces: <%p> Current var_space: <%p> Var spaces number: %d\n", var_cluster->var_spaces, 
                                                                                           var_cluster->cur_var_space, 
                                                                                           var_cluster->var_spaces_num);

    for (int counter = 0; counter < var_cluster->var_spaces_num; counter++)
    {
        fprintf(logs_file, "\n Var space [%d] <%p> : Number of names: %d Capacity: %d\n", counter, var_cluster->var_spaces + counter, 
                                                                                                   var_cluster->var_spaces[counter].var_names_num, 
                                                                                                   var_cluster->var_spaces[counter].var_names_cap);
        for (int ct = 0; ct < var_cluster->var_spaces[counter].var_names_num; ct++)
            fprintf(logs_file, "\n [%d] Name hash %ld Ram position: %d \n", ct, var_cluster->var_spaces[counter].var_names[ct].hash, 
                                                                                var_cluster->var_spaces[counter].var_names[ct].ram_pos);
    }

    Label_cluster* label_cluster = names->label_cluster;

    fprintf(logs_file, "\n <b> Labels cluster: </b> <%p> \n". label_cluster);
    fprintf(logs_file, " Number of labels: %d Labels array capacity: %d \n", label_cluster->label_names_num, 
                                                                             label_cluster->label_names_cap);

    for (int counter = 0; counter < label_cluster->label_names_num; counter++)
    {
        fprintf(logs_file, "\n [%d] Label name hash: %ld \n", counter, label_cluster->label_names[counter]);
    }

    Func_cluster* func_cluster = names->func_cluster;

    fprintf(logs_file, "\n <b> Func cluster: </b> <%p> \n". func_cluster);
    fprintf(logs_file, " Number of functions: %d Func array capacity: %d \n", func_cluster->func_names_num, 
                                                                              func_cluster->func_names_cap);

    for (int counter = 0; counter < func_cluster->func_names_num; counter++)
    {
        fprintf(logs_file, "\n [%d] Func name hash: %ld \n", counter, func_cluster->func_names[counter]);
    }

    return 0;
}

//===================================================================

int _build_a_tree(Tree* tree, Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    TREE_PTR_CHECK(tree);
    TOKENS_STRUCT_PTR_CHECK(tokens);

    Names names = { 0 };
    int ret = init_names_struct(&names);
    RETURN_CHECK(ret);

    Node* root = get_g(tokens, &names);
    if (!root)
        return -1;

    tree->root = root;

    ret = kill_names_struct(&names);
    RETURN_CHECK(ret);

    return 0;
}

//-------------------------------------------------------------------

Node* _get_g(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* node = (Node*)node_allocate_memory();
    NULL_CHECK(node);

    NODE_INIT_KEY_NODE(node, HEAD_ND);
    node->parent = No_parent;

    if (TOKEN_IS_DEFN(CUR_TOKEN))
    {
        node->right_son = get_definitions(tokens, names);
        CONNECT_RIGHT(node, node->right_son);
    }

    REQUIRE_KEY_WORD(START);

    Node* entity = get_entity(tokens, names);
    NULL_CHECK(entity);

    CONNECT_LEFT(node, entity);

    while (TOKEN_IS_SOL(CUR_TOKEN))
    {
        Node* new_entity = get_entity(tokens, names);
        NULL_CHECK(new_entity);

        CONNECT_LEFT(entity, new_entity);
        entity = new_entity;
    }
    
    REQUIRE_KEY_WORD(END);

    if (tokens->counter != tokens->amount)
    {
        error_report(TOKENS_AFTER_END);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    return node;
}

//-------------------------------------------------------------------

Node* _get_definitions(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS))
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* defn = get_defn(tokens, names);
    NULL_CHECK(defn);

    Node* first_defn = defn;

    while (TOKEN_IS_DEFN(CUR_TOKEN))
    {
        Node* next_defn = get_defn(tokens, names);
        NULL_CHECK(next_defn);

        CONNECT_LEFT(defn, next_defn);
        defn = next_defn;
    }

    return first_defn;
}

//-------------------------------------------------------------------

Node* _get_entity(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS))
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* node = NULL;

    if (TOKEN_IS_LABEL(N_TOKEN))
    {
        node = get_label_decl(tokens, names);
        NULL_CHECK(node);
    }

    else
    {
        node = get_statement(tokens, names);
        NULL_CHECK(node);
    }

    return node;
}

//-------------------------------------------------------------------

Node* _build_func_defn_constr(FOR_LOGS(LOG_PARAMS))
{
    lang_log_report();
    
    Node* func_defn_node = (Node*)node_allocate_memory();
    NULL_CHECK(func_defn_node);
    NODE_INIT_KEY_NODE(func_defn_node, FUNC_DEFN_ND);

    ADD_RIGHT(func_defn_node);
    NODE_INIT_KEY_NODE(func_defn_node->R, DEFN_ND);

    ADD_LEFT(func_defn_node->R);
    NODE_INIT_KEY_NODE(func_defn_node->R->L, FUNC_ND);

    return func_defn_node;
;}

//-------------------------------------------------------------------

Node* _get_defn(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(DEFN);

    Node* func_defn_nd = build_func_defn_constr();
    NULL_CHECK(func_defn_nd);

    int var_number = get_variables_number();
    if (var_number == -1) return NULL;

    Node* func_name = get_func_id_decl(tokens, names);
    NULL_CHECK(func_name);
    CONNECT_LEFT(func_defn_nd->R->L, func_name);

    Node* param_node = get_func_parameters(tokens, names);
    NULL_CHECK(param_node);
    CONNECT_RIGHT(func_defn_nd->R->L, param_node);

    Node* statement = get_compl_statement(tokens, names);
    NULL_CHECK(statement);
    CONNECT_RIGHT(func_defn_nd->R, statement);

    remove_variables_declarations(var_number);

    return func_defn_nd;
}

//-------------------------------------------------------------------

Node* _get_func_parameters(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS))
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(BR_OPEN);

    Node* parameter_name = get_var_id_decl(tokens, names);

    Node* param_node = (Node*)node_allocate_memory();
    NODE_INIT_KEY_NODE(param_node, PARAMETER_ND);

    CONNECT_RIGHT(param_node, parameter_name);

    REQUIRE_KEY_WORD(BR_CLOSE);

    return param_node;
}

//-------------------------------------------------------------------

Node* _get_label_decl(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(SOL);
    REQUIRE_KEY_WORD(LABEL);

    Node* label_name = get_label_id_decl(tokens, names);
    NULL_CHECK(label_name);

    Node* label_nd = (Node*)node_allocate_memory();
    NULL_CHECK(label_nd);

    CONNECT_RIGHT(label_nd, label_name);

    REQUIRE_KEY_WORD(EOL);

    return label_nd;
}

//-------------------------------------------------------------------

Node* _get_statement(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(SOL);

    Node* node = (Node*)node_allocate_memory();
    NODE_INIT_KEY_NODE(node, STATEMENT_ND);

    if (TOKEN_IS_IF(CUR_TOKEN))
    {
        Node* son = get_cond(tokens, names);
        NULL_CHECK(son);
        CONNECT_RIGHT(node, son);
    }

    else if (TOKEN_IS_DECL(CUR_TOKEN))
    {
        Node* son = get_decl(tokens, names);
        NULL_CHECK(son);
        CONNECT_RIGHT(node, son);
    }

    else if (TOKEN_IS_WHILE(CUR_TOKEN))
    {
        Node* son = get_cycle(tokens, names);
        NULL_CHECK(son);
        CONNECT_RIGHT(node, son);
    }

    else if (TOKEN_IS_ASS(N_TOKEN))
    {
        Node* son = get_ass(tokens, names);
        NULL_CHECK(son);
        CONNECT_RIGHT(node, son);
    }

    else if (TOKEN_IS_RET(CUR_TOKEN))
    {
        Node* son = get_ret(tokens, names);
        NULL_CHECK(son);
        CONNECT_RIGHT(node, son);
    }

    else if (TOKEN_IS_GOTO(CUR_TOKEN))
    {
        Node* son = get_label_jump(tokens, names);
        NULL_CHECK(son);
        CONNECT_RIGHT(node, son);
    }

    else if (TOKEN_IS_STD_FUNC(CUR_TOKEN))
    {
        Node* son = get_std_func_call(tokens, names);
        NULL_CHECK(son);
        CONNECT_RIGHT(node, son);
    }

    else if (TOKEN_IS_ID(CUR_TOKEN) && TOKEN_IS_BR_OPEN(N_TOKEN))
    {
        Node* son = get_func_call(tokens, names);
        NULL_CHECK(son);
        CONNECT_RIGHT(node, son);
    }

    else
    {
        error_report(MISSING_STATEMENT);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    REQUIRE_KEY_WORD(EOL);
    return node;
}

//-------------------------------------------------------------------

Node* _get_compl_statement(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(FBR_OPEN);

    int var_number = get_variables_number();
    if (var_number == -1) return NULL;

    Node* statement = get_statement(tokens, names);
    NULL_CHECK(statement);

    Node* first_stat = statement;

    while (TOKEN_IS_SOL(CUR_TOKEN))
    {
        Node* new_statement = get_statement(tokens, names);
        NULL_CHECK(new_statement);

        CONNECT_LEFT(statement, new_statement);
        statement = new_statement;
    }

    REQUIRE_KEY_WORD(FBR_CLOSE);

    remove_variables_declarations(var_number);

    return first_stat;
}

//--------------------------FF-----------------------------------------

Node* _get_ret(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS))
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(RET);

    Node* node = (Node*)node_allocate_memory();
    NULL_CHECK(node);
    NODE_INIT_KEY_NODE(node, RET_ND);

    Node* exp = get_exp(tokens, names);
    NULL_CHECK(exp);

    CONNECT_RIGHT(node, exp);

    return node;
}
//-------------------------------------------------------------------

Node* _get_instruction(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* node = 0;

    if (TOKEN_IS_FBR_OPEN(CUR_TOKEN))
    {
        node = get_compl_statement(tokens, names);
        NULL_CHECK(node);
    }

    else
    {
        node = get_statement(tokens, names);
        NULL_CHECK(node);
    }

    return node;
}

//-------------------------------------------------------------------

Node* _get_cond(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(IF);
    REQUIRE_KEY_WORD(BR_OPEN);

    Node* node = (Node*)node_allocate_memory();
    NULL_CHECK(node);

    NODE_INIT_KEY_NODE(node, IF_ND);

    Node* exp = get_exp(tokens, names);
    NULL_CHECK(exp);
    CONNECT_RIGHT(node, exp);

    REQUIRE_KEY_WORD(BR_CLOSE);
    REQUIRE_KEY_WORD(DO);

    ADD_LEFT(node);
    NODE_INIT_KEY_NODE(NL, DECISION_ND);

    Node* ttrue = get_instruction(tokens, names);
    NULL_CHECK(ttrue);
    CONNECT_RIGHT(NL, ttrue);

    if (TOKEN_IS_ELSE(CUR_TOKEN))
    {
        tokens->counter++;

        Node* ffalse = get_instruction(tokens, names);
        NULL_CHECK(ffalse);
        CONNECT_LEFT(NL, ffalse);
    }

    return node;
}

//-------------------------------------------------------------------

Node* _get_ass(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* node = (Node*)node_allocate_memory();
    NODE_INIT_KEY_NODE(node, ASS_ND);

    if (!TOKEN_IS_ID(CUR_TOKEN))
    {
        error_report(INV_LEFT_VALUE);
        tokens_dump(tokens, logs_file);
        return NULL;
    }
    else
    {
        if (!was_var_declared(CUR_TOKEN->data.id_hash))
        {
            error_report(VAR_UNDECLARED);
            tokens_dump(tokens, logs_file);
            return NULL;
        }
        else if (is_perm_var(CUR_TOKEN->data.id_hash))
        {
            error_report(ASS_TO_PERM);
            tokens_dump(tokens, logs_file);
            return NULL;
        }

        ADD_LEFT(node);
        NODE_INIT_IDENTIFICATOR(NL, CUR_TOKEN->data.id_hash);
        tokens->counter++;
    }

    REQUIRE_KEY_WORD(ASS);

    Node* exp = get_exp(tokens, names);
    NULL_CHECK(exp);
    CONNECT_RIGHT(node, exp);

    return node;
}

//-------------------------------------------------------------------

Node* _get_decl(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(DECL);

    Node* var = get_var_id_decl(tokens, names);
    NULL_CHECK(var);

    REQUIRE_KEY_WORD(ASS);

    Node* decl = (Node*)node_allocate_memory();
    NODE_INIT_KEY_NODE(decl, DECL_ND);
    NULL_CHECK(decl);

    Node* ass = (Node*)node_allocate_memory();
    NODE_INIT_KEY_NODE(ass, ASS_ND);
    NULL_CHECK(ass);

    CONNECT_RIGHT(decl, ass);

    Node* exp = get_exp(tokens, names);
    NULL_CHECK(exp);

    CONNECT_RIGHT(ass, exp);
    CONNECT_LEFT(ass, var);

    return decl;

    // int is_permanent = 0;

    // if (TOKEN_IS_PERM(CUR_TOKEN))
    // {
    //     is_permanent
    // }

    // Node* decl_nd = (Node*)node_allocate_memory();
    // NODE_INIT_KEY_NODE(decl_nd, DECL_ND);

    // if (!TOKEN_IS_ID(CUR_TOKEN))
    // {
    //     error_report(MISSING_ID);
    //     tokens_dump(tokens, logs_file);
    //     return NULL;
    // }

    // if (was_var_declared(CUR_TOKEN->data.id_hash))
    // {
    //     error_report(VAR_REDECLARE);
    //     tokens_dump(tokens, logs_file);
    //     return NULL;
    // }
    // else
    //     add_var_decl(CUR_TOKEN->data.id_hash, is_permanent);

    // if (TOKEN_IS_ASS(N_TOKEN))
    // {
    //     tokens->counter++;
    //     REQUIRE_KEY_WORD(ASS_ND);

    //     Node* ass = (Node*)node_allocate_memory();
    //     NULL_CHECK(ass);

    //     NODE_INIT_KEY_NODE(ass, ASS_ND);

    //     ADD_LEFT(ass);
    //     NODE_INIT_IDENTIFICATOR(ass->L, PR_TOKEN->data.id_hash);
    //     tokens->counter++;
    
    //     Node* exp = get_exp(tokens);
    //     NULL_CHECK(exp);

    //     CONNECT_RIGHT(ass, exp);
    //     CONNECT_RIGHT(node, ass);
    // }

    // else
    // {
    //     ADD_RIGHT(node);
    //     NODE_INIT_IDENTIFICATOR(NR, CUR_TOKEN->data.id_hash);
    //     tokens->counter++;
    // }

    // return node;
}

//-------------------------------------------------------------------

Node* _get_cycle(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(WHILE);
    REQUIRE_KEY_WORD(BR_OPEN);

    Node* node = (Node*)node_allocate_memory();
    NODE_INIT_KEY_NODE(node, WHILE_ND);

    Node* exp = get_exp(tokens, names);
    NULL_CHECK(exp);
    CONNECT_RIGHT(node, exp);

    REQUIRE_KEY_WORD(BR_CLOSE);
    REQUIRE_KEY_WORD(DO);

    Node* instr = get_instruction(tokens, names);
    NULL_CHECK(instr);
    CONNECT_LEFT(node, instr);

    return node;
}

//-------------------------------------------------------------------

Node* _get_std_func_call(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS))
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* node = (Node*)node_allocate_memory();
    int func_code = CUR_TOKEN->data.std_func_code;

    NODE_INIT_STD_FUNC_NODE(node, func_code);
    tokens->counter++;

    REQUIRE_KEY_WORD(BR_OPEN);

    if (func_code != SCAN)
    {
        Node* exp = get_exp(tokens, names);
        NULL_CHECK(exp);
        CONNECT_RIGHT(node, exp);

        REQUIRE_KEY_WORD(BR_CLOSE);
        return node;
    }

    Node* arg = get_var_id(tokens, names);
    NULL_CHECK(arg);
    CONNECT_RIGHT(node, arg);

    REQUIRE_KEY_WORD(BR_CLOSE);

    return node;
}

//-------------------------------------------------------------------

Node* _get_func_call(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* node = (Node*)node_allocate_memory();
    NODE_INIT_KEY_NODE(node, CALL_ND);
    Node* call = node;

    ADD_LEFT(node);
    node = NL;

    NODE_INIT_KEY_NODE(node, FUNC_ND);

    Node* func_name = get_func_id(tokens, names);
    NULL_CHECK(func_name);
    CONNECT_LEFT(node, func_name);

    REQUIRE_KEY_WORD(BR_OPEN);

    ADD_RIGHT(node);
    NODE_INIT_KEY_NODE(NR, PARAMETER_ND);

    Node* exp = get_exp(tokens, names);
    NULL_CHECK(exp);
    CONNECT_LEFT(NR, exp);

    REQUIRE_KEY_WORD(BR_CLOSE);

    return call;
}

//-------------------------------------------------------------------

Node* _get_label_jump(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(GOTO);

    Node* node = (Node*)node_allocate_memory();
    NODE_INIT_KEY_NODE(node, GOTO_ND);

    Node* label_name = get_id(tokens, names);
    NULL_CHECK(label_name);
    CONNECT_LEFT(node, label_name);

    return node;
}

//-------------------------------------------------------------------

Node* _get_exp(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* exp = get_e(tokens, names);
    NULL_CHECK(exp);

    if (TOKEN_IS_CMP_SIGN(CUR_TOKEN))
    {
        Node* cmp = (Node*)node_allocate_memory();
        NULL_CHECK(cmp);

        NODE_INIT_BIN_OPERAND(cmp, CUR_TOKEN->data.cmp_sign_code);
        tokens->counter++;

        CONNECT_LEFT(cmp, exp);

        Node* exp_second = get_e(tokens, names);
        NULL_CHECK(exp_second);

        CONNECT_RIGHT(cmp, exp_second);
        return cmp;
    }

    return exp;
}

//-------------------------------------------------------------------

Node* _get_e(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* t = get_t(tokens, names);
    NULL_CHECK(t);

    while (TOKEN_IS_ADD(CUR_TOKEN) || TOKEN_IS_SUB(CUR_TOKEN))
    {
        Node* e = (Node*)node_allocate_memory();
        NULL_CHECK(e);

        NODE_INIT_BIN_OPERAND(e, (int)(CUR_TOKEN->data.operand));
        tokens->counter++;

        CONNECT_LEFT(e, t);

        Node* t2 = get_t(tokens, names);
        NULL_CHECK(t2);

        CONNECT_RIGHT(e, t2);
        t = e;
    }

    return t;
}

//int _get_cmp_sign(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

//-------------------------------------------------------------------

Node* _get_t(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* m = get_m(tokens, names);
    NULL_CHECK(m);

    while (TOKEN_IS_MUL(CUR_TOKEN) || TOKEN_IS_DIV(CUR_TOKEN))
    {
        Node* t = (Node*)node_allocate_memory();
        NULL_CHECK(t);

        NODE_INIT_BIN_OPERAND(t, (int)(CUR_TOKEN->data.operand));
        tokens->counter++;

        CONNECT_LEFT(t, m);

        Node* m2 = get_m(tokens, names);
        NULL_CHECK(m2);

        CONNECT_RIGHT(t, m2);
        m = t;
    }

    return m;
}

//-------------------------------------------------------------------

Node* _get_m(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* p = get_p(tokens, names);
    NULL_CHECK(p);

    if (TOKEN_IS_POW(CUR_TOKEN))
    {
        Node* m = (Node*)node_allocate_memory();
        NULL_CHECK(m);

        NODE_INIT_BIN_OPERAND(m, CUR_TOKEN->data.cmp_sign_code);
        tokens->counter++;

        CONNECT_LEFT(m, p);

        Node* p2 = get_p(tokens, names);
        NULL_CHECK(p2);

        CONNECT_RIGHT(m, p2);
        return m;
    }

    return p;
}

//-------------------------------------------------------------------

Node* _get_p(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    int is_negative = 0;
    if (TOKEN_IS_NEG(CUR_TOKEN))
    {
        is_negative = 1;
        tokens->counter++;
    }

    Node* node = NULL;

    if (TOKEN_IS_BR_OPEN(CUR_TOKEN))
    {
        tokens->counter++;
        node = get_exp(tokens, names);
        NULL_CHECK(node);

        REQUIRE_KEY_WORD(BR_CLOSE);
    }

    else if (TOKEN_IS_ID(CUR_TOKEN) && !TOKEN_IS_BR_OPEN(N_TOKEN))
    {
        node = get_var_id(tokens, names);
        NULL_CHECK(node);
    }

    else if (TOKEN_IS_CONSTANT(CUR_TOKEN))
    {
        node = (Node*)node_allocate_memory();
        NODE_INIT_CONSTANT(node, CUR_TOKEN->data.constant);
        tokens->counter++;
    }

    else if (TOKEN_IS_CALC_FUNC(CUR_TOKEN))
    {
        node = (Node*)node_allocate_memory();
        NODE_INIT_UNAR_OPERAND(node, CUR_TOKEN->data.calc_func_code);
        tokens->counter++;
        REQUIRE_KEY_WORD(BR_OPEN);

        node->left_son = get_exp(tokens, names);
        CONNECT_LEFT(node, node->left_son);
        NULL_CHECK(node->left_son);

        REQUIRE_KEY_WORD(BR_CLOSE);
    }

    else
    {
        node = get_func_call(tokens, names);
        NULL_CHECK(node);
    }

    if (is_negative)
    {
        Node* multiply = (Node*)node_allocate_memory();
        NODE_INIT_BIN_OPERAND(multiply, MUL);

        ADD_LEFT(multiply);
        NODE_INIT_CONSTANT(multiply->left_son, -1);
        CONNECT_RIGHT(multiply, node);

        return multiply;
    }

    return node;
}

//===================================================================

Node* _get_var_id(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS))
{
    SYNTAX_READ_FUNC_START(tokens);

    if (!TOKEN_IS_ID(CUR_TOKEN))
    {
        error_report(MISSING_ID);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    else if (!was_var_declared(CUR_TOKEN->data.id_hash))
    {
        error_report(VAR_UNDECLARED);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    Node* node = (Node*)node_allocate_memory();
    NULL_CHECK(node);

    NODE_INIT_IDENTIFICATOR(node, CUR_TOKEN->data.id_hash);
    tokens->counter++;

    return node;
}

//-------------------------------------------------------------------

Node* _get_func_id(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS))
{
    SYNTAX_READ_FUNC_START(tokens);

    if (!TOKEN_IS_ID(CUR_TOKEN))
    {
        error_report(MISSING_ID);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    else if (!was_func_declared(CUR_TOKEN->data.id_hash))
    {
        error_report(FUNC_UNDECLARED);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    Node* node = (Node*)node_allocate_memory();
    NULL_CHECK(node);

    NODE_INIT_IDENTIFICATOR(node, CUR_TOKEN->data.id_hash);
    tokens->counter++;

    return node;
}

//-------------------------------------------------------------------

Node* _get_var_id_decl(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS))
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* node = NULL;
    Node* decl = NULL;
    int is_permanent = 0;

    if (TOKEN_IS_PERM(CUR_TOKEN))
    {
        node = (Node*)node_allocate_memory();
        NULL_CHECK(node);

        NODE_INIT_KEY_NODE(node, PERM_ND);
        tokens->counter++;
        
        ADD_RIGHT(node);
        NULL_CHECK(node->R);

        decl = node;
        node = node->R;
        is_permanent = 1;
    }

    else
    {
        node = (Node*)node_allocate_memory();
        NULL_CHECK(node);
    }

    if (!TOKEN_IS_ID(CUR_TOKEN))
    {
        error_report(MISSING_ID);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    else if (was_var_declared(CUR_TOKEN->data.id_hash))
    {
        error_report(VAR_REDECLARE);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    else 
        add_var_decl(CUR_TOKEN->data.id_hash, is_permanent);

    NODE_INIT_IDENTIFICATOR(node, CUR_TOKEN->data.id_hash);
    tokens->counter++;

    return (is_permanent)? decl: node;
}

//-------------------------------------------------------------------

Node* _get_func_id_decl(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS))
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* node = NULL;

    if (!TOKEN_IS_ID(CUR_TOKEN))
    {
        error_report(MISSING_ID);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    else if (was_func_declared(CUR_TOKEN->data.id_hash))
    {
        error_report(FUNC_REDECL);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    else
        add_func_decleration(CUR_TOKEN->data.id_hash);

    node = (Node*)node_allocate_memory();
    NULL_CHECK(node);

    NODE_INIT_IDENTIFICATOR(node, CUR_TOKEN->data.id_hash);
    tokens->counter++;

    return node;      
} 

//-------------------------------------------------------------------

Node* _get_label_id_decl(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS))
{
    SYNTAX_READ_FUNC_START(tokens);

    if (!TOKEN_IS_ID(CUR_TOKEN))
    {
        error_report(MISSING_ID);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    else if (was_label_declared(CUR_TOKEN->data.id_hash))
    {
        error_report(LABEL_REDECL);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    else
        add_label_decl(CUR_TOKEN->data.id_hash);

    Node* node = (Node*)node_allocate_memory();
    NULL_CHECK(node);

    NODE_INIT_IDENTIFICATOR(node, CUR_TOKEN->data.id_hash);
    tokens->counter++;

    return node;
}

//===================================================================

Node* _get_id(Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS))
{
    SYNTAX_READ_FUNC_START(tokens);

    if (!TOKEN_IS_ID(CUR_TOKEN))
    {
        error_report(MISSING_ID);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    Node* id = (Node*)node_allocate_memory();
    NULL_CHECK(id);

    NODE_INIT_IDENTIFICATOR(id, CUR_TOKEN->data.id_hash);
    tokens->counter++;

    return id;
}

//===================================================================

#undef N_TOKEN
#undef CUR_TOKEN
#undef N_N_TOKEN
#undef PR_TOKEN

//-------------------------------------------------------------------

#undef NL
#undef NR
#undef NLL
#undef NRR
#undef NLR
#undef NRL

//===================================================================
