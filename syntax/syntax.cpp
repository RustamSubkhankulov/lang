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


//===================================================================

int _build_a_tree(Tree* tree, Tokens* tokens FOR_LOGS(, LOG_PARAMS))
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

    int check = 0;

    if (TOKEN_IS_DEFN(CUR_TOKEN))
    {
        node->right_son = get_definitions(tokens, names);
        CONNECT_RIGHT(node, node->right_son);

        check = add_var_space(names->var_cluster);
        RET_VALUE_CHECK(check);
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

    check = rm_var_space(names->var_cluster);
    RET_VALUE_CHECK(check);
    
    REQUIRE_KEY_WORD(END);

    if (tokens->counter != tokens->amount)
    {
        SYNT_ERROR(TOKENS_AFTER_END, tokens, names);
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

    int check = rm_var_space(names->var_cluster);
    RET_VALUE_CHECK(check);

    Node* first_defn = defn;

    while (TOKEN_IS_DEFN(CUR_TOKEN))
    {
        check = add_var_space(names->var_cluster);
        RET_VALUE_CHECK(check);

        Node* next_defn = get_defn(tokens, names);
        NULL_CHECK(next_defn);

        CONNECT_LEFT(defn, next_defn);
        defn = next_defn;

        check = rm_var_space(names->var_cluster);
        RET_VALUE_CHECK(check);
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

    Node* func_name = get_func_id_decl(tokens, names);
    NULL_CHECK(func_name);
    CONNECT_LEFT(func_defn_nd->R->L, func_name);

    Node* param_node = get_func_parameters(tokens, names);
    NULL_CHECK(param_node);
    CONNECT_RIGHT(func_defn_nd->R->L, param_node);

    Node* statement = get_compl_statement(tokens, names);
    NULL_CHECK(statement);
    CONNECT_RIGHT(func_defn_nd->R, statement);

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
        SYNT_ERROR(MISSING_STATEMENT, tokens, names);
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

    int ret = add_var_space(names->var_cluster);
    RET_VALUE_CHECK(ret);

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

    ret = rm_var_space(names->var_cluster);
    RET_VALUE_CHECK(ret);

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
        SYNT_ERROR(INV_LEFT_VALUE, tokens, names);
        return NULL;
    }
    else
    {
        if (!var_is_declared(CUR_TOKEN->data.id_hash, names->var_cluster))
        {
            SYNT_ERROR(VAR_UNDECLARED, tokens, names);
            return NULL;
        }
        else if (var_is_permanent(CUR_TOKEN->data.id_hash, names->var_cluster))
        {
            SYNT_ERROR(ASS_TO_PERM, tokens, names);
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
        SYNT_ERROR(MISSING_ID, tokens, names);
        return NULL;
    }

    else if (!var_is_declared(CUR_TOKEN->data.id_hash, names->var_cluster))
    {
        SYNT_ERROR(VAR_UNDECLARED, tokens, names);
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
        SYNT_ERROR(MISSING_ID, tokens, names);
        return NULL;
    }

    else if (!func_is_defined(CUR_TOKEN->data.id_hash, names->func_cluster))
    {
        SYNT_ERROR(FUNC_UNDECLARED, tokens, names);
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
        SYNT_ERROR(MISSING_ID, tokens, names);
        return NULL;
    }

    else if (var_is_declared(CUR_TOKEN->data.id_hash, names->var_cluster))
    {
        SYNT_ERROR(VAR_REDECLARE, tokens, names);
        return NULL;
    }

    else 
        add_var_declare(CUR_TOKEN->data.id_hash, is_permanent, 
                                          names->var_cluster);

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
        SYNT_ERROR(MISSING_ID, tokens, names);
        return NULL;
    }

    else if (func_is_defined(CUR_TOKEN->data.id_hash, names->func_cluster))
    {
        SYNT_ERROR(FUNC_REDECL, tokens, names);
        return NULL;
    }

    else
        add_func_defn(CUR_TOKEN->data.id_hash, names->func_cluster);

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
        SYNT_ERROR(MISSING_ID, tokens, names);
        return NULL;
    }

    else if (label_is_defined(CUR_TOKEN->data.id_hash, names->label_cluster))
    {
        SYNT_ERROR(LABEL_REDECL, tokens, names);
        return NULL;
    }

    else
        add_label_defn(CUR_TOKEN->data.id_hash, names->label_cluster);

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
