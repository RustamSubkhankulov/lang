#pragma once

#include "../tree/tree.h"
#include "../lang_common/lang_common.h"
#include "../syntax/syntax.h"
#include "trans_config.h"

//=========================================================

#define TRANS_START_CHECK(node, file) {                     \
                                                            \
    do                                                      \
    {                                                       \
        lang_log_report();                                  \
        NODE_PTR_CHECK(node);                               \
        FILE_PTR_CHECK(file);                               \
        printf("\n\n node <%p>\n\n", node);                 \
                                                            \
    } while(0);                                             \
}


//=========================================================

#define NODE_IS_STAT(node)      (node->data_type == KEY_NODE && node->data.key_node_code == STATEMENT_ND)

#define NODE_IS_GOTO(node)      (node->data_type == KEY_NODE && node->data.key_node_code == GOTO_ND)

#define NODE_IS_LABEL(node)     (node->data_type == KEY_NODE && node->data.key_node_code == LABEL_ND)

#define NODE_IS_IF(node)        (node->data_type == KEY_NODE && node->data.key_node_code == IF_ND)

#define NODE_IS_ASS(node)       (node->data_type == KEY_NODE && node->data.key_node_code == ASS_ND)

#define NODE_IS_DECL(node)      (node->data_type == KEY_NODE && node->data.key_node_code == DECL_ND)

#define NODE_IS_WHILE(node)     (node->data_type == KEY_NODE && node->data.key_node_code == WHILE_ND)

#define NODE_IS_CALL(node)      (node->data_type == KEY_NODE && node->data.key_node_code == CALL_ND)

#define NODE_IS_RET(node)       (node->data_type == KEY_NODE && node->data.key_node_code == RET_ND)

#define NODE_IS_CONSTANT(node)  (node->data_type == CONSTANT)

#define NODE_IS_ID(node)        (node->data_type == IDENTIFICATOR)

#define NODE_IS_UNAR_OPER(node) (node->data_type == UNAR_OPERAND)

#define NODE_IS_BIN_OPER(node)  (node->data_type == BIN_OPERAND)

#define NODE_IS_STD_FUNC(node)  (node->data_type == STD_FUNC_NODE)

#define NODE_IS_KEY_NODE(node)  (node->data_type == KEY_NODE)

//=========================================================

int _translation_execute(Tree* tree, const char* asm_filename FOR_LOGS(, LOG_PARAMS));

int _trans_tree_to_asm  (Tree* tree, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_compl_stat   (Node* node, FILE* asm_file FOR_LOGS(,LOG_PARAMS));

int _trans_statement    (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_label_decl   (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_cond         (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_ass          (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_decl         (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_cycle        (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_func_call    (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_std_func_call(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_label_jump   (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_ret          (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_exp          (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_unary_oper   (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_binary_oper  (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _node_is_cmp_sign   (Node* node FOR_LOGS(, LOG_PARAMS));

int _trans_print        (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_scan         (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_constant     (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_variable     (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_bin_operand  (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_unar_operand (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_definitions  (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _trans_func_defn    (Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS));

int _write_asm_preparations(FILE* asm_file FOR_LOGS(, LOG_PARAMS));

//===================================================================

#define write_asm_preparations(asm_file) \
       _write_asm_preparations(asm_file FOR_LOGS(, LOG_ARGS))

#define trans_definitions(node, asm_file) \
       _trans_definitions(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_func_defn(node, asm_file) \
       _trans_func_defn(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_constant(node, asm_file) \
       _trans_constant(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_compl_stat(node, asm_file) \
       _trans_compl_stat(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_variable(node, asm_file) \
       _trans_variable(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_unar_operand(node, asm_file) \
       _trans_unar_operand(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_bin_operand(node, asm_file) \
       _trans_bin_operand(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_print(node, asm_file) \
       _trans_print(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_scan(node, asm_file) \
       _trans_scan(node, asm_file FOR_LOGS(, LOG_ARGS))

#define translation_execute(tree, filename) \
       _translation_execute(tree, filename FOR_LOGS(, LOG_ARGS))

#define node_is_cmp_sign(node) \
       _node_is_cmp_sign(node FOR_LOGS(, LOG_ARGS))

#define trans_tree_to_asm(tree, asm_file) \
       _trans_tree_to_asm(tree, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_statement(node, asm_file) \
       _trans_statement(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_label_decl(node, asm_file) \
       _trans_label_decl(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_cond(node, asm_file) \
       _trans_cond(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_ass(node, asm_file) \
       _trans_ass(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_decl(node, asm_file) \
       _trans_decl(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_cycle(node, asm_file) \
       _trans_cycle(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_std_func_call(node, asm_file) \
       _trans_std_func_call(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_func_call(node, asm_file) \
       _trans_func_call(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_label_jump(node, asm_file) \
       _trans_label_jump(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_ret(node, asm_file) \
       _trans_ret(node, asm_file FOR_LOGS(, LOG_ARGS))

#define trans_exp(node, asm_file) \
       _trans_exp(node, asm_file FOR_LOGS(, LOG_ARGS))






