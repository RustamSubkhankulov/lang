#pragma once

#include "../lang_common/lang_config.h"
#include "../lang_common/lang_common.h"
#include "../logs/errors_and_logs.h"
#include "../text_processing/text_processing.h"
#include "../tree/tree.h"

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

Node* _get_var_id          (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_func_id         (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_var_id_decl     (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

Node* _get_func_id_decl    (Tokens* tokens FOR_LOGS(, LOG_PARAMS));

//-------------------------------------------------------------------

// #define node_afterbuild_check(node) 
//        _node_afterbuild_check(node FOR_LOGS(, LOG_ARGS))

#define get_ret(tokens) \
       _get_ret(tokens FOR_LOGS(, LOG_ARGS))

#define get_var_id(tokens) \
       _get_var_id(tokens FOR_LOGS(, LOG_ARGS))

#define get_func_id(tokens) \
       _get_func_id(tokens FOR_LOGS(, LOG_ARGS))

#define get_var_id_decl(tokens) \
       _get_var_id_decl(tokens FOR_LOGS(, LOG_ARGS))

#define get_func_id_decl(tokens) \
       _get_func_id_decl(tokens FOR_LOGS(, LOG_ARGS))

#define get_label_id(tokens) \
       _get_label_id(tokens FOR_LOGS(, LOG_ARGS))

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


