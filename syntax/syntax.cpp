#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "syntax.h"
#include "../general/general.h"
#include "func_definitions.h"
#include "syntax_id.h"

//===================================================================

#define CUR_TOKEN tokens->token_array[tokens->counter]

#define N_TOKEN tokens->token_array[tokens->counter + 1]

#define N_N_TOKEN tokens->token_array[tokens->counter + 2]

#define PR_TOKEN tokens->token_array[tokens->counter - 1]

//--------------------------------------------------------------------

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

//===================================================================

int _build_a_tree(Tree* tree, Tokens* tokens FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    TREE_PTR_CHECK(tree);
    TOKENS_STRUCT_PTR_CHECK(tokens);

    Node* root = get_g(tokens);
    if (!root)
        return -1;

    free_variables_declarations();
    free_func_declarations();
    free_label_declarations();

    tree->root = root;

    return 0;
}

//-------------------------------------------------------------------

Node* _get_g(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* node = (Node*)node_allocate_memory();
    NULL_CHECK(node);

    NODE_INIT_KEY_NODE(node, HEAD_ND);
    node->parent = No_parent;

    if (TOKEN_IS_DEFN(CUR_TOKEN))
    {
        node->right_son = get_definitions(tokens);
        CONNECT_RIGHT(node, node->right_son);
    }

    REQUIRE_KEY_WORD(START);

    Node* entity = get_entity(tokens);
    NULL_CHECK(entity);

    CONNECT_LEFT(node, entity);

    while (TOKEN_IS_SOL(CUR_TOKEN))
    {
        Node* new_entity = get_entity(tokens);
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

Node* _get_definitions(Tokens* tokens FOR_LOGS(, LOG_PARAMS))
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* defn = get_defn(tokens);
    NULL_CHECK(defn);

    Node* first_defn = defn;

    while (TOKEN_IS_DEFN(CUR_TOKEN))
    {
        Node* next_defn = get_defn(tokens);
        NULL_CHECK(next_defn);

        CONNECT_LEFT(defn, next_defn);
        defn = next_defn;
    }

    return first_defn;
}

//-------------------------------------------------------------------

Node* _get_entity(Tokens* tokens FOR_LOGS(, LOG_PARAMS))
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* node = NULL;

    if (TOKEN_IS_LABEL(N_TOKEN))
    {
        node = get_label_decl(tokens);
        NULL_CHECK(node);
    }

    else
    {
        node = get_statement(tokens);
        NULL_CHECK(node);
    }

    return node;
}

//-------------------------------------------------------------------

// Node* _get_func_id(Tokens* tokens FOR_LOGS(, LOG_PARAMS))
// {
//     SYNTAX_READ_FUNC_START(tokens);

//     if (!TOKEN_IS_ID(CUR_TOKEN))
//     {
//         error_report(MISSING_ID);
//         tokens_dump(tokens, logs_file);
//         return NULL;
//     }

//     int64_t id_hash = CUR_TOKEN->data.id_hash;

//     if (was_func_declared(id_hash))
//     {
//         error_report(FUNC_REDECL);
//         tokens_dump(Tokens, logs_file);
//         return NULL;
//     }

//     else
//     {

//     }
// }

//-------------------------------------------------------------------

Node* _get_defn(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(DEFN);

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
    {
        int id_add = add_func_decleration(CUR_TOKEN->data.id_hash);
        if (id_add == -1)
            return NULL;
    }

    Node* func_define_node = (Node*)node_allocate_memory();
    NULL_CHECK(func_define_node);
    NODE_INIT_KEY_NODE(func_define_node, FUNC_DEFN_ND);

    ADD_RIGHT(func_define_node);
    Node* define_node = func_define_node->right_son;
    
    NODE_INIT_KEY_NODE(define_node, DEFN_ND);
    ADD_LEFT(define_node);

    NODE_INIT_KEY_NODE(define_node->left_son, FUNC_ND);

    Node* func_name = (Node*)node_allocate_memory();
    NODE_INIT_IDENTIFICATOR(func_name, CUR_TOKEN->data.id_hash);
    CONNECT_LEFT(define_node->left_son, func_name);
    tokens->counter++;

    int var_number = get_variables_number();
    if (var_number == -1) return NULL;

    REQUIRE_KEY_WORD(BR_OPEN);

    if (!TOKEN_IS_ID(CUR_TOKEN))
    {
        error_report(MISSING_ID);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    if (was_var_declared(CUR_TOKEN->data.id_hash))
    {
        error_report(VAR_REDECLARE);
        tokens_dump(tokens, logs_file);
        return NULL;
    }
    else
    {
        add_var_decl(CUR_TOKEN->data.id_hash);
    }

    Node* param_node = (Node*)node_allocate_memory();
    NODE_INIT_KEY_NODE(param_node, PARAMETER_ND);
    CONNECT_RIGHT(define_node->left_son, param_node);

    ADD_RIGHT(param_node);
    NODE_INIT_IDENTIFICATOR(param_node->right_son, CUR_TOKEN->data.id_hash);
    tokens->counter++;

    REQUIRE_KEY_WORD(BR_CLOSE);

    Node* statement = get_compl_statement(tokens);
    NULL_CHECK(statement);
    CONNECT_RIGHT(define_node, statement);

    remove_variables_declarations(var_number);

    return func_define_node;
}

//-------------------------------------------------------------------

Node* _get_label_decl(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(SOL);
    REQUIRE_KEY_WORD(LABEL);

    Node* node = NULL;

    if (CUR_TOKEN->type != ID)
    {
        error_report(MISSING_ID);
        tokens_dump(tokens, logs_file);           // цеплять справа 
        return NULL;
    }

    if (was_label_declared(CUR_TOKEN->data.id_hash))
    {
        error_report(LABEL_REDECL);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    else
    {
        add_label_decl(CUR_TOKEN->data.id_hash);

        node = (Node*)node_allocate_memory();
        NODE_INIT_KEY_NODE(node, LABEL_ND);
        ADD_RIGHT(node);

        NODE_INIT_IDENTIFICATOR(NR, CUR_TOKEN->data.id_hash);
        tokens->counter++;
    }

    REQUIRE_KEY_WORD(EOL);

    return node;
}

//-------------------------------------------------------------------

Node* _get_statement(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(SOL);

    Node* node = (Node*)node_allocate_memory();
    NODE_INIT_KEY_NODE(node, STATEMENT_ND);

    if (TOKEN_IS_IF(CUR_TOKEN))
    {
        Node* son = get_cond(tokens);
        NULL_CHECK(son);
        CONNECT_RIGHT(node, son);
    }

    else if (TOKEN_IS_DECL(CUR_TOKEN))
    {
        Node* son = get_decl(tokens);
        NULL_CHECK(son);
        CONNECT_RIGHT(node, son);
    }

    else if (TOKEN_IS_DO(CUR_TOKEN) || TOKEN_IS_WHILE(CUR_TOKEN))
    {
        Node* son = get_cycle(tokens);
        NULL_CHECK(son);
        CONNECT_RIGHT(node, son);
    }

    else if (TOKEN_IS_ASS(N_TOKEN))
    {
        Node* son = get_ass(tokens);
        NULL_CHECK(son);
        CONNECT_RIGHT(node, son);
    }

    else if (TOKEN_IS_RET(CUR_TOKEN))
    {
        Node* son = get_ret(tokens);
        NULL_CHECK(son);
        CONNECT_RIGHT(node, son);
    }

    else if (TOKEN_IS_GOTO(CUR_TOKEN))
    {
        Node* son = get_label_jump(tokens);
        NULL_CHECK(son);
        CONNECT_RIGHT(node, son);
    }

    else if (TOKEN_IS_STD_FUNC(CUR_TOKEN))
    {
        Node* son = get_std_func_call(tokens);
        NULL_CHECK(son);
        CONNECT_RIGHT(node, son);
    }

    else if (TOKEN_IS_ID(CUR_TOKEN))
    {
        Node* son = get_func_call(tokens);
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

Node* _get_compl_statement(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(FBR_OPEN);

    int var_number = get_variables_number();
    if (var_number == -1) return NULL;

    Node* statement = get_statement(tokens);
    NULL_CHECK(statement);

    Node* first_stat = statement;

    while (TOKEN_IS_SOL(CUR_TOKEN))
    {
        Node* new_statement = get_statement(tokens);
        NULL_CHECK(new_statement);

        CONNECT_LEFT(statement, new_statement);
        statement = new_statement;
    }

    REQUIRE_KEY_WORD(FBR_CLOSE);

    remove_variables_declarations(var_number);

    return first_stat;
}

//--------------------------FF-----------------------------------------

Node* _get_ret(Tokens* tokens FOR_LOGS(, LOG_PARAMS))
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(RET);

    Node* node = (Node*)node_allocate_memory();
    NULL_CHECK(node);
    NODE_INIT_KEY_NODE(node, RET_ND);

    Node* exp = get_exp(tokens);
    NULL_CHECK(exp);

    CONNECT_RIGHT(node, exp);

    return node;
}
//-------------------------------------------------------------------

Node* _get_instruction(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* node = 0;

    if (TOKEN_IS_FBR_OPEN(CUR_TOKEN))
    {
        node = get_compl_statement(tokens);
        NULL_CHECK(node);
    }

    else
    {
        node = get_statement(tokens);
        NULL_CHECK(node);
    }

    return node;
}

//-------------------------------------------------------------------

Node* _get_cond(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(IF);
    REQUIRE_KEY_WORD(BR_OPEN);

    Node* node = (Node*)node_allocate_memory();
    NULL_CHECK(node);

    NODE_INIT_KEY_NODE(node, IF_ND);

    Node* exp = get_exp(tokens);
    NULL_CHECK(exp);
    CONNECT_RIGHT(node, exp);

    REQUIRE_KEY_WORD(BR_CLOSE);
    REQUIRE_KEY_WORD(DO);

    ADD_LEFT(node);
    NODE_INIT_KEY_NODE(NL, DECISION_ND);

    Node* ttrue = get_instruction(tokens);
    NULL_CHECK(ttrue);
    CONNECT_RIGHT(NL, ttrue);

    if (TOKEN_IS_ELSE(CUR_TOKEN))
    {
        tokens->counter++;

        Node* ffalse = get_instruction(tokens);
        NULL_CHECK(ffalse);
        CONNECT_LEFT(NL, ffalse);
    }

    return node;
}

//-------------------------------------------------------------------

Node* _get_ass(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
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

        ADD_LEFT(node);
        NODE_INIT_IDENTIFICATOR(NL, CUR_TOKEN->data.id_hash);
        tokens->counter++;
    }

    REQUIRE_KEY_WORD(ASS);

    Node* exp = get_exp(tokens);
    NULL_CHECK(exp);
    CONNECT_RIGHT(node, exp);

    return node;
}

//-------------------------------------------------------------------

Node* _get_decl(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(DECL);

    Node* node = (Node*)node_allocate_memory();
    NODE_INIT_KEY_NODE(node, DECL_ND);

    if (!TOKEN_IS_ID(CUR_TOKEN))
    {
        error_report(MISSING_ID);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    if (was_var_declared(CUR_TOKEN->data.id_hash))
    {
        error_report(VAR_REDECLARE);
        tokens_dump(tokens, logs_file);
        return NULL;
    }
    else
    {
        add_var_decl(CUR_TOKEN->data.id_hash);
    }

    if (TOKEN_IS_ASS(N_TOKEN))
    {
        Node* ass = get_ass(tokens);
        NULL_CHECK(ass);

        CONNECT_RIGHT(node, ass);
    }

    else
    {
        ADD_RIGHT(node);
        NODE_INIT_IDENTIFICATOR(NR, CUR_TOKEN->data.id_hash);
        tokens->counter++;
    }

    return node;
}

//-------------------------------------------------------------------

Node* _get_cycle(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(WHILE);
    REQUIRE_KEY_WORD(BR_OPEN);

    Node* node = (Node*)node_allocate_memory();
    NODE_INIT_KEY_NODE(node, WHILE_ND);

    Node* exp = get_exp(tokens);
    NULL_CHECK(exp);
    CONNECT_RIGHT(node, exp);

    REQUIRE_KEY_WORD(BR_CLOSE);
    REQUIRE_KEY_WORD(DO);

    Node* instr = get_instruction(tokens);
    NULL_CHECK(instr);
    CONNECT_LEFT(node, instr);

    return node;
}

//-------------------------------------------------------------------

Node* _get_std_func_call(Tokens* tokens FOR_LOGS(, LOG_PARAMS))
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* node = (Node*)node_allocate_memory();
    int func_code = CUR_TOKEN->data.std_func_code;

    NODE_INIT_STD_FUNC_NODE(node, func_code);
    tokens->counter++;

    REQUIRE_KEY_WORD(BR_OPEN);

    if (func_code != SCAN)
    {
        Node* exp = get_exp(tokens);
        NULL_CHECK(exp);
        CONNECT_RIGHT(node, exp);

        REQUIRE_KEY_WORD(BR_CLOSE);
        return node;
    }

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

    else
    {
        ADD_RIGHT(node);
        NODE_INIT_IDENTIFICATOR(NR, CUR_TOKEN->data.id_hash);
        tokens->counter++;
    }

    REQUIRE_KEY_WORD(BR_CLOSE);

    return node;
}

//-------------------------------------------------------------------

Node* _get_func_call(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* node = (Node*)node_allocate_memory();
    NODE_INIT_KEY_NODE(node, CALL_ND);
    Node* call = node;

    ADD_LEFT(node);
    node = NL;

    NODE_INIT_KEY_NODE(node, FUNC_ND);
    ADD_LEFT_AND_RIGHT(node);

    if (TOKEN_IS_ID(CUR_TOKEN))
    {
        if (!was_func_declared(CUR_TOKEN->data.id_hash))
        {
            error_report(FUNC_UNDECLARED);
            tokens_dump(tokens, logs_file);
            return NULL;
        }

        NODE_INIT_IDENTIFICATOR(NL, CUR_TOKEN->data.id_hash);
        tokens->counter++;
    }

    else 
    {
        error_report(MISSING_ID);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    REQUIRE_KEY_WORD(BR_OPEN);

    NODE_INIT_KEY_NODE(NR, PARAMETER_ND);

    Node* exp = get_exp(tokens);
    NULL_CHECK(exp);
    CONNECT_LEFT(NR, exp);

    REQUIRE_KEY_WORD(BR_CLOSE);

    return call;
}

//-------------------------------------------------------------------

Node* _get_label_jump(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    REQUIRE_KEY_WORD(GOTO);

    Node* node = (Node*)node_allocate_memory();
    NODE_INIT_KEY_NODE(node, GOTO_ND);

    if (TOKEN_IS_ID(CUR_TOKEN))
    {
        ADD_LEFT(node);
        NODE_INIT_IDENTIFICATOR(NL, CUR_TOKEN->data.id_hash);
        tokens->counter++;
    }
    else
    {
        error_report(MISSING_ID);
        tokens_dump(tokens, logs_file);
        return NULL;
    }

    return node;
}

//-------------------------------------------------------------------

Node* _get_exp(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* exp = get_e(tokens);
    NULL_CHECK(exp);

    if (TOKEN_IS_CMP_SIGN(CUR_TOKEN))
    {
        Node* cmp = (Node*)node_allocate_memory();
        NULL_CHECK(cmp);

        NODE_INIT_BIN_OPERAND(cmp, CUR_TOKEN->data.cmp_sign_code);
        tokens->counter++;

        CONNECT_LEFT(cmp, exp);

        Node* exp_second = get_e(tokens);
        NULL_CHECK(exp_second);

        CONNECT_RIGHT(cmp, exp_second);
        return cmp;
    }

    return exp;
}

//-------------------------------------------------------------------

Node* _get_e(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* t = get_t(tokens);
    NULL_CHECK(t);

    while (TOKEN_IS_ADD(CUR_TOKEN) || TOKEN_IS_SUB(CUR_TOKEN))
    {
        Node* e = (Node*)node_allocate_memory();
        NULL_CHECK(e);

        NODE_INIT_BIN_OPERAND(e, (int)(CUR_TOKEN->data.operand));
        tokens->counter++;

        CONNECT_LEFT(e, t);

        Node* t2 = get_t(tokens);
        NULL_CHECK(t2);

        CONNECT_RIGHT(e, t2);
        t = e;
    }

    return t;
}

//int _get_cmp_sign(Tokens* tokens FOR_LOGS(, LOG_PARAMS));

//-------------------------------------------------------------------

Node* _get_t(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* m = get_m(tokens);
    NULL_CHECK(m);

    while (TOKEN_IS_MUL(CUR_TOKEN) || TOKEN_IS_DIV(CUR_TOKEN))
    {
        Node* t = (Node*)node_allocate_memory();
        NULL_CHECK(t);

        NODE_INIT_BIN_OPERAND(t, (int)(CUR_TOKEN->data.operand));
        tokens->counter++;

        CONNECT_LEFT(t, m);

        Node* m2 = get_m(tokens);
        NULL_CHECK(m2);

        CONNECT_RIGHT(t, m2);
        m = t;
    }

    return m;
}

//-------------------------------------------------------------------

Node* _get_m(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
{
    SYNTAX_READ_FUNC_START(tokens);

    Node* p = get_p(tokens);
    NULL_CHECK(p);

    if (TOKEN_IS_POW(CUR_TOKEN))
    {
        Node* m = (Node*)node_allocate_memory();
        NULL_CHECK(m);

        NODE_INIT_BIN_OPERAND(m, CUR_TOKEN->data.cmp_sign_code);
        tokens->counter++;

        CONNECT_LEFT(m, p);

        Node* p2 = get_p(tokens);
        NULL_CHECK(p2);

        CONNECT_RIGHT(m, p2);
        return m;
    }

    return p;
}

//-------------------------------------------------------------------

Node* _get_p(Tokens* tokens FOR_LOGS(, LOG_PARAMS)) 
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
        node = get_exp(tokens);
        NULL_CHECK(node);

        REQUIRE_KEY_WORD(BR_CLOSE);
    }

    else if (TOKEN_IS_ID(CUR_TOKEN) && !TOKEN_IS_BR_OPEN(N_TOKEN))
    {
        if (!was_var_declared(CUR_TOKEN->data.id_hash))
        {
            error_report(VAR_UNDECLARED);
            tokens_dump(tokens, logs_file);
            return NULL;
        }

        node = (Node*)node_allocate_memory();
        NODE_INIT_IDENTIFICATOR(node, CUR_TOKEN->data.id_hash);
        tokens->counter++;
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

        node->left_son = get_exp(tokens);
        CONNECT_LEFT(node, node->left_son);
        NULL_CHECK(node->left_son);

        REQUIRE_KEY_WORD(BR_CLOSE);
    }

    else
    {
        node = get_func_call(tokens);
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
