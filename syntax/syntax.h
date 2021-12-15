#pragma once

#include "../lang_common/lang_config.h"
#include "../lang_common/lang_common.h"
#include "../logs/errors_and_logs.h"
#include "../text_processing/text_processing.h"
#include "../tree/tree.h"

//===================================================================

const int Var_names_start_num   = 5;

const int Label_names_start_num = 5; 

const int Func_names_start_num  = 5;

//===================================================================

#define SYNT_ERROR(error, tokens) {                                 \
                                                                    \
    do                                                              \
    {                                                               \
        error_report(error);                                        \
        tokens_dump(tokens, logs_file);                             \
                                                                    \
    } while(0);                                                     \
}

//-------------------------------------------------------------------

#define NAMES_PTR_CHECK(names) {                                    \
                                                                    \
    do                                                              \
    {                                                               \
        if (!names)                                                 \
        {                                                           \
            error_report(INV_NAMES_STRUCT_PTR);                     \
            return NULL;                                            \
        }                                                           \
                                                                    \
    } while(0);                                                     \
}

//-------------------------------------------------------------------

#define VAR_CLASTER_PTR_CHECK(cluster) {                            \
                                                                    \
    do                                                              \
    {                                                               \
        if(!cluster)                                                \
        {                                                           \
            error_report(INV_VAR_CLASTER_PTR);                      \
            return -1;                                              \
        }                                                           \
    } while(0);                                                     \
}

//-------------------------------------------------------------------

#define LABEL_CLASTER_PTR_CHECK(cluster) {                          \
                                                                    \
    do                                                              \
    {                                                               \
        if(!cluster)                                                \
        {                                                           \
            error_report(INV_LABEL_CLASTER_PTR);                    \
            return -1;                                              \
        }                                                           \
    } while(0);                                                     \
}

//-------------------------------------------------------------------

#define FUNC_CLASTER_PTR_CHECK(cluster) {                           \
                                                                    \
    do                                                              \
    {                                                               \
        if(!cluster)                                                \
        {                                                           \
            error_report(INV_FUNC_CLASTER_PTR);                     \
            return -1;                                              \
        }                                                           \
    } while(0);                                                     \
}

//===================================================================

struct Var_name
{
    int64_t id_hash;
    int is_perm;
};

struct Var_space
{
    Var_name* var_names;
    int var_names_num;
    int var_names_cap;
};

typedef int64_t Label_name;

typedef int64_t Func_name;

//-------------------------------------------------------------------

struct Var_cluster
{
    Var_space* var_spaces;
    Var_space* cur_var_space;
    int var_spaces_num;
};

struct Label_cluster
{
    Label_name* label_names;
    int label_names_num;
    int label_names_cap;
};

struct Func_cluster
{
    Func_name* func_names;
    int func_names_num;
    int func_names_cap;
};

//-------------------------------------------------------------------

struct Names
{
    Var_cluster   var_cluster;
    Label_cluster label_cluster;
    Func_cluster  func_cluster;
};

//===================================================================

#define TOKEN_IS_STD_FUNC(token)  (token->type == STD_FUNC)

#define TOKEN_IS_DEFN(token)      (token->type == KEY_WORD && token->data.key_word_code == DEFN)

#define TOKEN_IS_RET(token)       (token->type == KEY_WORD && token->data.key_word_code == RET)

#define TOKEN_IS_LABEL(token)     (token->type == KEY_WORD && token->data.key_word_code == LABEL)

#define TOKEN_IS_SOL(token)       (token->type == KEY_WORD && token->data.key_word_code == SOL)

#define TOKEN_IS_IF(token)        (token->type == KEY_WORD && token->data.key_word_code == IF)

#define TOKEN_IS_ELSE(token)      (token->type == KEY_WORD && token->data.key_word_code == ELSE)

#define TOKEN_IS_DECL(token)      (token->type == KEY_WORD && token->data.key_word_code == DECL)

#define TOKEN_IS_WHILE(token)     (token->type == KEY_WORD && token->data.key_word_code == WHILE)

#define TOKEN_IS_DO(token)        (token->type == KEY_WORD && token->data.key_word_code == DO)

#define TOKEN_IS_ASS(token)       (token->type == KEY_WORD && token->data.key_word_code == ASS)

#define TOKEN_IS_GOTO(token)      (token->type == KEY_WORD && token->data.key_word_code == GOTO)

#define TOKEN_IS_ID(token)        (token->type == ID)

#define TOKEN_IS_FBR_OPEN(token)  (token->type == KEY_WORD && token->data.key_word_code == FBR_OPEN)

#define TOKEN_IS_CMP_SIGN(token)  (token->type == CMP_SIGN)

#define TOKEN_IS_CALC_FUNC(token) (token->type == CALC_FUNC)

#define TOKEN_IS_CONSTANT(token)  (token->type == CONST)

#define TOKEN_IS_PERM(token)      (token->type == KEY_WORD && token->data.key_word_code == PERM)

#define TOKEN_IS_ADD(token)       (token->type == OPER && token->data.operand == ADD)

#define TOKEN_IS_SUB(token)       (token->type == OPER && token->data.operand == SUB)

#define TOKEN_IS_MUL(token)       (token->type == OPER && token->data.operand == MUL)

#define TOKEN_IS_DIV(token)       (token->type == OPER && token->data.operand == DIV)

#define TOKEN_IS_POW(token)       (token->type == OPER && token->data.operand == POW)

#define TOKEN_IS_BR_OPEN(token)   (token->type == KEY_WORD && token->data.key_word_code == BR_OPEN) 

#define TOKEN_IS_NEG(token)       (token->type == KEY_WORD && token->data.key_word_code == NEG)

//===================================================================

#define REQUIRE_EOL                                                 \
                                                                    \
    do                                                              \
    {                                                               \
        if (CUR_TOKEN->type != KEY_WORD                             \
        ||  CUR_TOKEN->data.key_word_code != EOL)                   \
        {                                                           \
            error_report(MISSING_EOL);                              \
            tokens_dump(tokens, logs_file);                         \
            return NULL;                                            \
        }                                                           \
        else                                                        \
            tokens->counter++;                                      \
                                                                    \
    } while (0);                                                        

//-------------------------------------------------------------------

#define REQUIRE_SOL                                                 \
                                                                    \
    do                                                              \
    {                                                               \
        if (CUR_TOKEN->type != KEY_WORD                             \
        ||  CUR_TOKEN->datnode_inita.key_word_code != SOL)                   \
        {                                                           \
            error_report(MISSING_SOL);                              \
            tokens_dump(tokens, logs_file);                         \
            return NULL;                                            \
        }                                                           \
        else                                                        \
            tokens->counter++;                                      \
                                                                    \
    } while (0);  

//-------------------------------------------------------------------

#define REQUIRE_KEY_WORD(key_code) {                                \
                                                                    \
    do                                                              \
    {                                                               \
        if (CUR_TOKEN->type != KEY_WORD                             \
        || CUR_TOKEN->data.key_word_code != key_code)               \
        {                                                           \
            error_report(REQUIRE_KEY_WORD);                         \
            tokens_dump(tokens, logs_file);                         \
            return NULL;                                            \
        }                                                           \
        else                                                        \
            tokens->counter++;                                      \
                                                                    \
    } while(0);                                                     \
}

//===================================================================

#define SYNTAX_READ_FUNC_START(tokens) {                            \
                                                                    \
       lang_log_report();                                           \
       TOKENS_PTR_CHECK(tokens);                                    \
}

//===================================================================

#define TOKENS_PTR_CHECK(tokens) {                                  \
                                                                    \
    do                                                              \
    {                                                               \
        if (!tokens)                                                \
        {                                                           \
            error_report(INV_TOKENS_STRUCT_PTR);                    \
            return NULL;                                            \
        }                                                           \
    } while(0);                                                     \
}

//===================================================================

#define NULL_CHECK(ret) {                                           \
                                                                    \
    do                                                              \
    {                                                               \
        if (!ret)                                                   \
            return NULL;                                            \
                                                                    \
    } while(0);                                                     \
}

//===================================================================

int _init_names_struct (Names* names FOR_LOGS(, LOG_PARAMS));

int _names_struct_dump (Names* names FOR_LOGS(, LOG_PARAMS));

int _kill_names_struct (Names* names FOR_LOGS(, LOG_PARAMS));

int _init_var_cluster  (Var_cluster*   var_cluster   FOR_LOGS(, LOG_PARAMS));

int _kill_var_cluster  (Var_cluster*   var_cluster   FOR_LOGS(, LOG_PARAMS));

int _init_label_cluster(Label_cluster* label_cluster FOR_LOGS(, LOG_PARAMS));

int _kill_label_cluster(Label_cluster* label_cluster FOR_LOGS(, LOG_PARAMS));

int _init_func_cluster (Func_cluster*  func_cluster  FOR_LOGS(, LOG_PARAMS));

int _kill_func_cluster (Func_cluster*  func_cluster  FOR_LOGS(, LOG_PARAMS));

//-------------------------------------------------------------------

int _add_func_defn  (int64_t hash, Func_cluster* func_cluster FOR_LOGS(, LOG_PARAMS));

int _func_is_defined(int64_t hash, Func_cluster* func_cluster FOR_LOGS(, LOG_PARAMS));

int _func_defn_arr_increase       (Func_cluster* func_cluster FOR_LOGS(, LOG_PARAMS));

//-------------------------------------------------------------------

int _add_var_declare (int64_t hash, int is_perm, Var_cluster* var_cluster FOR_LOGS(, LOG_PARAMS));

int _var_is_declared (int64_t hash, Var_cluster* var_cluster FOR_LOGS(, LOG_PARAMS));

int _var_is_permanent(int64_t hash, Var_cluster* var_cluster FOR_LOGS(, LOG_PARAMS));

int _var_decl_arr_increase         (Var_cluster* var_cluster FOR_LOGS(, LOG_PARAMS));

int _add_var_space                 (Var_cluster* var_cluster FOR_LOGS(, LOG_PARAMS));

int _rm_var_space                  (Var_cluster* var_cluster FOR_LOGS(, LOG_PARAMS));

//-------------------------------------------------------------------

int _add_label_defn  (int64_t hash, Label_cluster* label_cluster FOR_LOGS(, LOG_PARAMS));

int _label_is_defined(int64_t hash, Label_cluster* label_cluster FOR_LOGS(, LOG_PARAMS));

int _label_defn_arr_increase       (Label_cluster* label_cluster FOR_LOGS(, LOG_PARAMS));

//===================================================================

// int node_afterbuild_check  (Node* node FOR_LOGS(, LOG_PARAMS));

int _build_a_tree          (Tree* tree, Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_g               (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_definitions     (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_entity          (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_defn            (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_label_decl      (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_statement       (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_compl_statement (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_instruction     (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_cond            (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_ass             (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_decl            (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_cycle           (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_func_call       (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_func_parameters (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_std_func_call   (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_label_jump      (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_ret             (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_exp             (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_e               (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_t               (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_m               (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_p               (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_id              (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_var_id          (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_func_id         (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_var_id_decl     (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_func_id_decl    (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _get_label_id_decl   (Tokens* tokens, Names* names FOR_LOGS(, LOG_PARAMS));

Node* _build_func_defn_constr(FOR_LOGS(LOG_PARAMS));

//-------------------------------------------------------------------

// #define node_afterbuild_check(node) 
//        _node_afterbuild_check(node FOR_LOGS(, LOG_ARGS))

#define build_func_defn_constr() \
       _build_func_defn_constr(FOR_LOGS(LOG_ARGS))

#define get_ret(tokens, names) \
       _get_ret(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_id(tokens, names) \
       _get_id(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_var_id(tokens, names) \
       _get_var_id(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_func_id(tokens, names) \
       _get_func_id(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_var_id_decl(tokens, names) \
       _get_var_id_decl(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_func_id_decl(tokens, names) \
       _get_func_id_decl(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_label_id_decl(tokens, names) \
       _get_label_id_decl(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_func_parameters(tokens, names) \
       _get_func_parameters(tokens, names FOR_LOGS(, LOG_ARGS))

#define build_a_tree(tree, tokens) \
       _build_a_tree(tree, tokens FOR_LOGS(, LOG_ARGS))

#define get_g(tokens, names) \
       _get_g(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_definitions(tokens, names) \
       _get_definitions(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_entity(tokens, names) \
       _get_entity(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_defn(tokens, names) \
       _get_defn(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_label_decl(tokens, names) \
       _get_label_decl(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_label_jump(tokens, names) \
       _get_label_jump(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_statement(tokens, names) \
       _get_statement(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_compl_statement(tokens, names) \
       _get_compl_statement(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_instruction(tokens, names) \
       _get_instruction(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_cond(tokens, names) \
       _get_cond(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_ass(tokens, names) \
       _get_ass(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_decl(tokens, names) \
       _get_decl(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_cycle(tokens, names) \
       _get_cycle(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_func_call(tokens, names) \
       _get_func_call(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_std_func_call(tokens, names) \
       _get_std_func_call(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_exp(tokens, names) \
       _get_exp(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_e(tokens, names) \
       _get_e(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_t(tokens, names) \
       _get_t(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_m(tokens, names) \
       _get_m(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_p(tokens, names) \
       _get_p(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_func_id(tokens, names) \
       _get_func_id(tokens, names FOR_LOGS(, LOG_ARGS))

#define get_var_id(tokens, names) \
       _get_var_id(tokens, names FOR_LOGS(, LOG_ARGS))

//===================================================================

#define init_names_struct(names) \
       _init_names_struct(names FOR_LOGS(, LOGS_ARGS))

#define names_struct_dump(names) \
       _names_struct_dump(names FOR_LOGS(, LOGS_ARGS))

#define kill_names_struct(names) \
       _kill_names_struct(names FOR_LOGS(, LOGS_ARGS))

#define init_var_cluster(cluster) \
       _init_var_cluster(cluster FOR_LOGS(, LOG_ARGS))

#define init_func_cluster(cluster) \
       _init_func_cluster(cluster FOR_LOGS(, LOG_ARGS))

#define init_label_cluster(cluster) \
       _init_label_cluster(cluster FOR_LOGS(, LOG_ARGS))

#define kill_var_cluster(cluster) \
       _kill_var_cluster(cluster FOR_LOGS(, LOG_ARGS))

#define kill_func_cluster(cluster) \
       _kill_func_cluster(cluster FOR_LOGS(, LOG_ARGS))

#define kill_label_cluster(cluster) \
       _kill_label_cluster(cluster FOR_LOGS(, LOG_ARGS))  

//------------------------------------------------------------------- 

#define add_func_defn(hash, cluster) \
       _add_func_defn(hash, cluster FOR_LOGS(, LOG_ARGS))

#define func_is_defined(hash, cluster) \
       _func_is_defined(hash, cluster FOR_LOGS(, LOGS_ARGS))

#define func_defn_arr_increase(cluster) \
       _func_defn_arr_increase(cluster FOR_LOGS(, LOGS_ARGS))

//-------------------------------------------------------------------

#define add_label_defn(hash, cluster) \
       _add_label_defn(hash, cluster FOR_LOGS(, LOG_ARGS))

#define label_is_defined(hash, cluster) \
       _label_is_defined(hash, cluster FOR_LOGS(, LOGS_ARGS))

#define label_defn_arr_increase(cluster) \
       _label_defn_arr_increase(cluster FOR_LOGS(, LOGS_ARGS))

//-------------------------------------------------------------------

#define add_var_declare(hash, is_perm, cluster) \
       _add_var_declare(hash, is_perm, cluster FOR_LOGS(, LOG_ARGS))

#define var_is_declared(hash, cluster) \
       _var_is_declared(hash, cluster FOR_LOGS(, LOGS_ARGS))

#define var_is_permanent(hash, cluster) \
       _var_is_permanent(hash, cluster FOR_LOGS(, LOGS_ARGS))

#define var_decl_arr_increase(cluster) \
       _var_decl_arr_increase(cluster FOR_LOGS(, LOGS_ARGS))

#define add_var_space(cluster) \
       _add_var_space(cluster FOR_LOGS(, LOGS_ARGS))

#define rm_var_space(cluster) \
       _rm_var_space(cluster FOR_LOGS(, LOGS_ARGS))

