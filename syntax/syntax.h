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
    } while(0);                                                    
}

//-------------------------------------------------------------------

#define VAR_CLASTER_PTR_CHECK(claster) {                            \
                                                                    \
    do                                                              \
    {                                                               \
        if(!claster)                                                \
        {                                                           \
            error_report(INV_VAR_CLASTER_PTR);                      \
            return -1;                                              \
        }                                                           \
    } while(0);                                                     \
}

//-------------------------------------------------------------------

#define LABEL_CLASTER_PTR_CHECK(claster) {                          \
                                                                    \
    do                                                              \
    {                                                               \
        if(!claster)                                                \
        {                                                           \
            error_report(INV_LABEL_CLASTER_PTR);                    \
            return -1;                                              \
        }                                                           \
    } while(0);                                                     \
}

//-------------------------------------------------------------------

#define FUNC_CLASTER_PTR_CHECK(claster) {                           \
                                                                    \
    do                                                              \
    {                                                               \
        if(!claster)                                                \
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

typedef Label_name int64_t;

typedef Func_name int64_t;

//-------------------------------------------------------------------

struct Var_claster
{
    Var_space* var_spaces;
    Var_space* cur_var_space;
    int var_spaces_num;
};

struct Label_claster
{
    Label_name* label_names;
    int label_names_num;
    int label_names_cap;
};

struct Func_claster
{
    Func_name* func_names;
    int func_names_num;
    int func_names_cap;
};

//-------------------------------------------------------------------

struct Names
{
    Var_claster   var_claster;
    Label_claster label_claster;
    Func_claster  func_claster;
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

int _kill_names_struct (Names* names FOR_LOGS(, LOG_PARAMS));

int _init_var_claster  (Var_claster*   var_claster   FOR_LOGS(, LOG_PARAMS));

int _kill_var_claster  (Var_claster*   var_claster   FOR_LOGS(, LOG_PARAMS));

int _init_label_claster(Label_claster* label_claster FOR_LOGS(, LOG_PARAMS));

int _kill_label_claster(Label_claster* label_claster FOR_LOGS(, LOG_PARAMS));

int _init_func_claster (Func_claster*  func_claster  FOR_LOGS(, LOG_PARAMS));

int _kill_func_claster (Func_claster*  func_claster  FOR_LOGS(, LOG_PARAMS));

//-------------------------------------------------------------------

int _add_func_defn  (int64_t hash, Func_claster* func_claster FOR_LOGS(, LOG_PARAMS));

int _func_is_defined(int64_t hash, Func_claster* func_claster FOR_LOGS(, LOG_PARAMS));

int _func_defn_arr_increase       (Func_claster* func_claster FOR_LOGS(, LOG_PARAMS));

//-------------------------------------------------------------------

int _add_var_decl    (int64_t hash, int is_perm, Var_claster* var_claster FOR_LOGS(, LOG_PARAMS));

int _var_is_declared (int64_t hash, Var_claster* var_claster FOR_LOGS(, LOG_PARAMS));

int _var_is_permanent(int64_t hash, Var_claster* var_claster FOR_LOGS(, LOG_PARAMS));

int _var_decl_arr_increase         (Var_cluster* var_cluster FOR_LOGS(, LOG_PARAMS));

int _add_var_space                 (Var_claster* var_claster FOR_LOGS(, LOG_PARAMS));

int _rm_var_space                  (Var_claster* var_claster FOR_LOGS(, LOG_PARAMS));

//-------------------------------------------------------------------

int _add_label_defn  (int64_t hash, Label_claster* label_claster FOR_LOGS(, LOG_PARAMS));

int _label_is_defined(int64_t hash, Label_claster* label_claster FOR_LOGS(, LOG_PARAMS));

int _label_defn_arr_increase       (Label_claster* label_claster FOR_LOGS(, LOG_PARAMS));

//===================================================================

// int node_afterbuild_check  (Node* node FOR_LOGS(, LOG_PARAMS));

int _build_a_tree          (Tree* tree, Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_g               (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_definitions     (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_entity          (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_defn            (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_label_decl      (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_statement       (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_compl_statement (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_instruction     (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_cond            (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_ass             (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_decl            (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_cycle           (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_func_call       (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_func_parameters (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_std_func_call   (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_label_jump      (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_ret             (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_exp             (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_e               (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_t               (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_m               (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_p               (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_id              (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_var_id          (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_func_id         (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_var_id_decl     (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_func_id_decl    (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_label_id_decl   (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _build_func_defn_constr(FOR_LOGS(LOG_PARAMS));

//-------------------------------------------------------------------

// #define node_afterbuild_check(node) 
//        _node_afterbuild_check(node FOR_LOGS(, LOG_ARGS))

#define build_func_defn_constr() \
       _build_func_defn_constr(FOR_LOGS(LOG_ARGS))

#define get_ret(tokens) \
       _get_ret(tokens FOR_LOGS(, LOG_ARGS))

#define get_id(tokens) \
       _get_id(tokens FOR_LOGS(, LOG_ARGS))

#define get_var_id(tokens) \
       _get_var_id(tokens FOR_LOGS(, LOG_ARGS))

#define get_func_id(tokens) \
       _get_func_id(tokens FOR_LOGS(, LOG_ARGS))

#define get_var_id_decl(tokens) \
       _get_var_id_decl(tokens FOR_LOGS(, LOG_ARGS))

#define get_func_id_decl(tokens) \
       _get_func_id_decl(tokens FOR_LOGS(, LOG_ARGS))

#define get_label_id_decl(tokens) \
       _get_label_id_decl(tokens FOR_LOGS(, LOG_ARGS))

#define get_func_parameters(tokens) \
       _get_func_parameters(tokens FOR_LOGS(, LOG_ARGS))

#define build_a_tree(tree, tokens) \
       _build_a_tree(tree, tokens FOR_LOGS(, LOG_ARGS))

#define get_g(tokens) \
       _get_g(tokens FOR_LOGS(, LOG_ARGS))

#define get_definitions(tokens) \
       _get_definitions(tokens FOR_LOGS(, LOG_ARGS))

#define get_entity(tokens) \
       _get_entity(tokens FOR_LOGS(, LOG_ARGS))

#define get_defn(tokens) \
       _get_defn(tokens FOR_LOGS(, LOG_ARGS))

#define get_label_decl(tokens) \
       _get_label_decl(tokens FOR_LOGS(, LOG_ARGS))

#define get_label_jump(tokens) \
       _get_label_jump(tokens FOR_LOGS(, LOG_ARGS))

#define get_statement(tokens) \
       _get_statement(tokens FOR_LOGS(, LOG_ARGS))

#define get_compl_statement(tokens) \
       _get_compl_statement(tokens FOR_LOGS(, LOG_ARGS))

#define get_instruction(tokens) \
       _get_instruction(tokens FOR_LOGS(, LOG_ARGS))

#define get_cond(tokens) \
       _get_cond(tokens FOR_LOGS(, LOG_ARGS))

#define get_ass(tokens) \
       _get_ass(tokens FOR_LOGS(, LOG_ARGS))

#define get_decl(tokens) \
       _get_decl(tokens FOR_LOGS(, LOG_ARGS))

#define get_cycle(tokens) \
       _get_cycle(tokens FOR_LOGS(, LOG_ARGS))

#define get_func_call(tokens) \
       _get_func_call(tokens FOR_LOGS(, LOG_ARGS))

#define get_std_func_call(tokens) \
       _get_std_func_call(tokens FOR_LOGS(, LOG_ARGS))

#define get_exp(tokens) \
       _get_exp(tokens FOR_LOGS(, LOG_ARGS))

#define get_e(tokens) \
       _get_e(tokens FOR_LOGS(, LOG_ARGS))

#define get_t(tokens) \
       _get_t(tokens FOR_LOGS(, LOG_ARGS))

#define get_m(tokens) \
       _get_m(tokens FOR_LOGS(, LOG_ARGS))

#define get_p(tokens) \
       _get_p(tokens FOR_LOGS(, LOG_ARGS))

#define get_func_id(tokens) \
       _get_func_id(tokens FOR_LOGS(, LOG_ARGS))

#define get_var_id(tokens) \
       _get_var_id(tokens FOR_LOGS(, LOG_ARGS))

//===================================================================

#define init_names_struct(names) \
       _init_names_struct(names FOR_LOGS(, LOGS_ARGS))

#define kill_names_struct(names) \
       _kill_names_struct(names FOR_LOGS(, LOGS_ARGS))

#define init_var_claster(claster) \
       _init_var_claster(claster FOR_LOGS(, LOG_ARGS))

#define init_func_claster(claster) \
       _init_func_claster(claster FOR_LOGS(, LOG_ARGS))

#define init_label_claster(claster) \
       _init_label_claster(claster FOR_LOGS(, LOG_ARGS))

#define kill_var_claster(claster) \
       _kill_var_claster(claster FOR_LOGS(, LOG_ARGS))

#define kill_func_claster(claster) \
       _kill_func_claster(claster FOR_LOGS(, LOG_ARGS))

#define kill_label_claster(claster) \
       _kill_label_claster(claster FOR_LOGS(, LOG_ARGS))  

//------------------------------------------------------------------- 

#define add_func_defn(hash, claster) \
       _add_func_defn(hash, claster FOR_LOGS(, LOG_ARGS))

#define func_is_defined(hash, claster) \
       _func_is_defined(hash, claster FOR_LOGS(, LOGS_ARGS))

#define func_defn_arr_increase(claster) \
       _func_defn_arr_increase(claster FOR_LOGS(, LOGS_ARGS))

//-------------------------------------------------------------------

#define add_label_defn(hash, claster) \
       _add_label_defn(hash, claster FOR_LOGS(, LOG_ARGS))

#define label_is_defined(hash, claster) \
       _label_is_defined(hash, claster FOR_LOGS(, LOGS_ARGS))

#define label_defn_arr_increase(claster) \
       _label_defn_arr_increase(claster FOR_LOGS(, LOGS_ARGS))

//-------------------------------------------------------------------

#define add_var_decl(hash, is_perm, claster) \
       _add_var_decl(hash, is_perm, claster FOR_LOGS(, LOG_ARGS))

#define var_is_declared(hash, claster) \
       _var_is_declared(hash, claster FOR_LOGS(, LOGS_ARGS))

#define var_is_permanent(hash, claster) \
       _var_is_permanent(hash, claster FOR_LOGS(, LOGS_ARGS))

#define var_decl_arr_increase(claster) \
       _var_decl_arr_increase(claster FOR_LOGS(, LOGS_ARGS))

#define add_var_space(claster) \
       _add_var_space(claster FOR_LOGS(, LOGS_ARGS))

#define rm_var_space(claster) \
       _rm_var_space(claster FOR_LOGS(, LOGS_ARGS))

