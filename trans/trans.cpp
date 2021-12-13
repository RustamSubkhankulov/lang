#include <time.h>
#include <stdlib.h>

#include "trans.h"
#include "../general/general.h"
#include "../syntax/syntax_id.h"

#define L left_son
#define R right_son

//===================================================================

static const char* Register = "dx";

//static const char* Service_register = "fx";

static const char* Argument_register = "ax";

static const char* Return_register = "ex";

//===================================================================

int _translation_execute(Tree* tree, const char* asm_filename FOR_LOGS(, LOG_PARAMS))
{
    TREE_PTR_CHECK(tree);
    if (!asm_filename)
    {
        error_report(INV_FILE_NAME);
        return -1;
    }

    FILE* asm_file = open_file(asm_filename, "wb");
    if (!asm_file)
        return -1;
    
    int ret = trans_tree_to_asm(tree, asm_file);
    if (ret == -1)
        return -1;

    free_variables_declarations();

    return close_file(asm_file);
}

//===================================================================

int _write_asm_preparations(FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    FILE_PTR_CHECK(asm_file);

    fprintf(asm_file, "\n PUSH %d \n", Free_ram_pos);
    fprintf(asm_file, "\n POP  %s \n", Register);

    fprintf(asm_file, "\n CALL: main\n");
    fprintf(asm_file, "\n HLT \n");

    return 0;
}

//===================================================================

int _trans_tree_to_asm(Tree* tree, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TREE_PTR_CHECK(tree);
    FILE_PTR_CHECK(asm_file);

    int ret = 0;

    write_asm_preparations(asm_file);

    if (tree->root->R)
    {
        ret = trans_definitions(tree->root->R, asm_file);
        RETURN_CHECK(ret);
    }

    fprintf(asm_file, "\n main: \n");

    Node* entity = tree->root->L;
    while(entity)
    {
        if (NODE_IS_STAT(entity))
        {
            ret = trans_statement(entity, asm_file);
            if (ret == -1)
                return -1;
        }

        if (NODE_IS_LABEL(entity))
        {
            ret = trans_label_decl(entity, asm_file);
            if (ret == -1)
                return -1;
        }

        entity = entity->L;
    }

    fprintf(asm_file, "\n RET \n");

    return 0;
}

//===================================================================

int _trans_definitions(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    int ret = 0;

    while(node)
    {
        ret = trans_func_defn(node->R, asm_file);
        RETURN_CHECK(ret);

        node = node->L;
    }

    return 0;
}

//-------------------------------------------------------------------

int _trans_func_defn(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    int var_number = get_variables_number();

    Node* func_nd = node->L;
    Node* stat_nd = node->R;

    if (!node->L || !node->R)
    {
        error_report(INV_TREE);
        return -1;
    }

    fprintf(asm_file, "\n %ld: \n", func_nd->L->data.id_hash);

    int64_t var_hash = func_nd->R->R->data.id_hash;
    add_var_decl(var_hash);
    int ram_pos = get_var_pos(var_hash);

    fprintf(asm_file, "\n PUSH %s \n", Argument_register);
    fprintf(asm_file, "\n POP [%s + %d] \n", Register, ram_pos);

    int ret = 0;

    ret = trans_compl_stat(stat_nd, asm_file);
    RETURN_CHECK(ret);

    remove_variables_declarations(var_number);
    in_func_flag_off();

    return 0;
}

//===================================================================

int _trans_statement(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    Node* content = node->R;

    if (NODE_IS_GOTO(content))
    {
        int ret = trans_label_jump(content, asm_file);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_DECL(content))
    {
        int ret = trans_decl(content, asm_file);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_ASS(content))
    {
        int ret = trans_ass(content, asm_file);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_WHILE(content))
    {
        int ret = trans_cycle(content, asm_file);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_IF(content))
    {
        int ret = trans_cond(content, asm_file);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_STD_FUNC(content))
    {
        int ret = trans_std_func_call(content, asm_file);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_CALL(content))
    {
        int ret = trans_func_call(content, asm_file);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_RET(content))
    {
        int ret = trans_ret(content, asm_file);
        RETURN_CHECK(ret);
    }

    else
    {
        error_report(INV_TREE);
        return -1;
    }

    return 0;
}

//-------------------------------------------------------------------

int _trans_compl_stat(Node* node, FILE* asm_file FOR_LOGS(,LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    int ret = 0;

    while(node)
    {
        ret = trans_statement(node, asm_file);
        RETURN_CHECK(ret);

        node = node->L;
    }

    return 0;
}

//-------------------------------------------------------------------

int _trans_label_decl(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    if (!NODE_IS_ID(node->R))
    {
        error_report(INV_TREE);
        return -1;
    }

    fprintf(asm_file, "\n %ld: \n", node->R->data.id_hash);

    return 0;
}

//-------------------------------------------------------------------

int _trans_cond(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    srand((unsigned int)time(NULL));
    int random = rand();

    int ret = trans_exp(node->R, asm_file);
    RETURN_CHECK(ret);

    Node* true_stat  = node->L->R;
    Node* false_stat = node->L->L;

    fprintf(asm_file, "\n PUSH 0 \n");
    fprintf(asm_file, "\n JAE: FALSE%d \n", random);

    ret = trans_compl_stat(true_stat, asm_file);
    RETURN_CHECK(ret);

    fprintf(asm_file, "\n JMP: CONDEND%d \n", random);

    fprintf(asm_file, "\n FALSE%d: \n", random);

    ret = trans_compl_stat(false_stat, asm_file);
    RETURN_CHECK(ret);

    fprintf(asm_file, "\n CONDEND%d: \n", random);
    return 0;
}

//-------------------------------------------------------------------

int _trans_func_call(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    Node* func_nd = node->L;
    if (!func_nd)
    {
        error_report(INV_TREE);
        return -1;
    }

    int ret = trans_exp(func_nd->R->L, asm_file);
    RETURN_CHECK(ret);

    fprintf(asm_file, "\n POP %s \n", Argument_register);
    fprintf(asm_file, "\n CALL: %ld \n", func_nd->L->data.id_hash);

    fprintf(asm_file, "\n PUSH %s \n", Return_register);

    return 0;
}

//-------------------------------------------------------------------

int _trans_ass(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    int ret = trans_exp(node->R, asm_file);
    RETURN_CHECK(ret);

    int ram_pos = get_var_pos(node->L->data.id_hash);
    RETURN_CHECK(ram_pos);

    fprintf(asm_file, "\n POP [%s + %d] \n", Register, ram_pos);

    return 0;
}

//-------------------------------------------------------------------

int _trans_decl(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    if (NODE_IS_ID(node->R))
    {
        add_var_decl(node->R->data.id_hash);
        return 0;
    }

    else if (NODE_IS_ASS(node->R))
    {
        add_var_decl(node->R->L->data.id_hash);
        
        int ret = trans_ass(node->R, asm_file);
        RETURN_CHECK(ret);
    }

    else
    {
        error_report(INV_TREE);
        return -1;
    }

    return 0;
}

//-------------------------------------------------------------------

int _trans_cycle(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    srand((unsigned int)time(NULL));
    int random = rand();

    fprintf(asm_file, "\n REPEAT%d: \n", random);

    int ret = trans_exp(node->R, asm_file);
    RETURN_CHECK(ret);

    fprintf(asm_file, "\n PUSH 0 \n");
    fprintf(asm_file, "\n JB: CYCLEEND%d \n", random);

    ret = trans_statement(node->L, asm_file);
    RETURN_CHECK(ret);
    fprintf(asm_file, "\n JMP: REPEAT%d \n", random);

    fprintf(asm_file, "\n CYCLEEND%d: \n", random);

    return 0;
}

//-------------------------------------------------------------------

int _trans_print(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    int ret = trans_exp(node->R, asm_file);
    RETURN_CHECK(ret);

    fprintf(asm_file, "\n OUT \n");

    return 0;
}

//-------------------------------------------------------------------

int _trans_scan(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    if (!NODE_IS_ID(node->R))
    {
        error_report(INV_TREE);
        return -1;
    }

    int ram_pos = get_var_pos(node->R->data.id_hash);
    RETURN_CHECK(ram_pos);

    fprintf(asm_file, "\n IN \n");
    fprintf(asm_file, "\n POP [%s + %d] \n", Register, ram_pos);

    return 0;
}

//-------------------------------------------------------------------

int _trans_std_func_call(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    switch(node->data.std_func_code)
    {
        case SCAN:
        {
            int ret = trans_scan(node, asm_file);
            RETURN_CHECK(ret);
            break;
        }

        case PRINT:
        {
            int ret = trans_print(node, asm_file);
            RETURN_CHECK(ret);
            break;
        }

        default:
        {
            error_report(INV_STD_FUNC_CODE);
            return -1;
        }
    }

    return 0;
}

//-------------------------------------------------------------------

int _trans_label_jump(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    if (!NODE_IS_ID(node->L))
    {
        error_report(INV_TREE);
        return -1;
    }

    fprintf(asm_file, "\n JMP: %ld\n", node->L->data.id_hash);

    return 0;
}

//-------------------------------------------------------------------

int _trans_ret(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    int ret = trans_exp(node->R, asm_file);
    RETURN_CHECK(ret);

    fprintf(asm_file, "\n POP %s \n", Return_register);
    fprintf(asm_file, "\n RET \n");

    return 0;
}

//===================================================================

int _node_is_cmp_sign(Node* node FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();

    if (node->data_type != KEY_NODE)
        return 0;

    for (int counter = 0; counter < Std_cmp_signs_number; counter++)
    {
        if (node->data.std_func_code == Std_cmp_signs[counter].code)
            return 1;
    }

    return 0;
}

//===================================================================

int _trans_exp(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    int ret = 0;

    if (NODE_IS_CALL(node))
    {
        ret = trans_func_call(node, asm_file);
        RETURN_CHECK(ret);

        return 0;
    }

    if (node->L)
    {
        ret = trans_exp(node->L, asm_file);
        RETURN_CHECK(ret);
    }

    if (node->R)
    {
        ret = trans_exp(node->R, asm_file);
        RETURN_CHECK(ret);
    }

    if (NODE_IS_CONSTANT(node))
    {
        ret = trans_constant(node, asm_file);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_ID(node))
    {
        ret = trans_variable(node, asm_file);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_UNAR_OPER(node))
    {
        ret = trans_unar_operand(node, asm_file);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_BIN_OPER(node))
    {
        ret = trans_bin_operand(node, asm_file);
        RETURN_CHECK(ret);
    }

    else
    {
        error_report(INV_NODE_DATA_TYPE);
        return -1;
    }

    return 0;
}

//===================================================================

int _trans_constant(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    fprintf(asm_file, "\n PUSH %g \n", node->data.constant);

    return 0;
}

//-------------------------------------------------------------------

int _trans_variable(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    printf("\n\n hash is %ld \n\n", node->data.id_hash);

    int ram_pos = get_var_pos(node->data.id_hash);
    RETURN_CHECK(ram_pos);

    fprintf(asm_file, "\n PUSH [%s + %d] \n", Register, ram_pos);

    return 0;
}

//-------------------------------------------------------------------

int _trans_bin_operand(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    switch(node->data.operand_code)
    {
        case ADD:
        {
            fprintf(asm_file, "\n ADD \n");
            break;
        }

        case SUB:
        {
            fprintf(asm_file, "\n SUB \n");
            break;
        }

        case MUL:
        {
            fprintf(asm_file, "\n MUL \n");
            break;
        }

        case DIV:
        {
            fprintf(asm_file, "\n DIV \n");
            break;
        }

        case POW:
        {
            fprintf(asm_file, "\n POW \n");
            break;
        }

        case EQ:
        {
            fprintf(asm_file, "\n EQ \n");
            break;
        }

        case NEQ:
        {
            fprintf(asm_file, "\n NEQ \n");
            break;
        }

        case MORE:
        {
            fprintf(asm_file, "\n MR \n");
            break;
        }

        case LESS:
        {
            fprintf(asm_file, "\n LS \n");
            break;
        }

        case MEQ:
        {
            fprintf(asm_file, "\n MRE \n");
            break;
        }

        case LEQ:
        {
            fprintf(asm_file, "\n LSE \n");
            break;
        }

        default:
        {
            error_report(NODE_INV_OPER_CODE);
            break;
        }
    }

    return 0;
}

//-------------------------------------------------------------------

int _trans_unar_operand(Node* node, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, asm_file);

    switch(node->data.operand_code)
    {
        case SIN:
        {
            fprintf(asm_file, "\n SIN \n");
            break;
        }

        case COS:
        {
            fprintf(asm_file, "\n COS \n");
            break;
        }

        case SQRT:
        {
            fprintf(asm_file, "\n PUSH 0.5 \n");
            fprintf(asm_file, "\n POW \n");
            break;
        }

        case TG:
        {
            fprintf(asm_file, "\n TAN \n");
            break;
        }

        case CTG:
        {
            fprintf(asm_file, "\n TAN \n");
            fprintf(asm_file, "\n PUSH -1 \n");
            fprintf(asm_file, "\n POW \n");
            break;
        }

        case LN:
        {
            fprintf(asm_file, "\n LN \n");
            break;
        }

        case ARCSIN:
        {
            fprintf(asm_file, "\n ASIN \n");
            break;
        }

        case ARCCOS:
        {
            fprintf(asm_file, "\n ASIN \n");
            fprintf(asm_file, "\n PUSH -1 \n");
            fprintf(asm_file, "\n MUL \n");
            fprintf(asm_file, "\n PUSH 1.570796 \n");
            fprintf(asm_file, "\n ADD \n");
            break;
        }

        case ARCCTG:
        {
            fprintf(asm_file, "\n ATG \n");
            fprintf(asm_file, "\n PUSH -1 \n");
            fprintf(asm_file, "\n POW \n");
            break;
        }

        case ARCTG:
        {
            fprintf(asm_file, "\n ATG \n");
            break;
        }

        case SQR:
        {
            fprintf(asm_file, "\n PUSH 2 \n");
            fprintf(asm_file, "\n POW \n");
            break;
        }

        default:
        {
            error_report(NODE_INV_OPER_CODE);
            return -1;
        }
    }

    return 0;
}
//===================================================================

#undef L
#undef R