#include <time.h>
#include <stdlib.h>

#include "trans.h"
#include "../general/general.h"

//===================================================================

#define CUR_NSPACE trans->cur_nspace

#define TREE trans->tree

#define NSPACES trans->nspaces

#define NSPACES_NUM trans->nspaces_num

#define ASM_FILE asm_file

//===================================================================

#define L left_son
#define R right_son

//===================================================================

static const char* Register = "dx";

//static const char* Service_register = "fx";

static const char* Argument_register = "ax";

static const char* Return_register = "ex";

//===================================================================

int _trans_struct_ctor(Trans* trans, FILE* asm_file, Tree* tree FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    TRANS_PTR_CHECK(trans);
    
    if (!trans)
    {
        error_report(INV_TRANS_PRT);
        return -1;
    }

    if (!asm_file)
    {
        error_report(INV_FILE_PTR);
        return -1;
    }

    if (!tree)
    {
        error_report(INV_TREE_PTR);
        return -1;
    }

    trans->asm_file = asm_file;
    trans->tree     = tree;

    trans->nspaces_num = 0;
}

//-------------------------------------------------------------------

int _trans_struct_dump(Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    TRANS_PTR_CHECK(trans);

    fprintf(logs_file, "<pre><div class=\"outline\"  "
                                   "style = \"background-color:lightgrey;\" "
                                   "style = \"text-align: center;\">\n");

    fprintf(logs_file, "\n <b> Trans structure dump </b> \n");

    fprintf(logs_file, "\n Assembler FILE pointer: <%p> \n", trans->asm_file);
    fprintf(logs_file, "\n Tree structure pointer: <%p> \n", trans->tree);

    fprintf(logs_file, "\n Namespaces number: %d \n",          trans->nspaces_num);
    fprintf(logs_file, "\n Namespaces array pointer: <%p> \n", trans->nspaces);
    fprintf(logs_file, "\n Current namespace:        <%p> \n", trans->cur_nspace);
    
    for (int counter = 0; counter < trans->nspaces_num; counter++)
    {
        fprintf(logs_file, "\n");
        fprintf(logs_file, "\n <b> Namespace </b><%p> \n",   trans->nspaces[counter]);
        fprintf(logs_file, "\n Variables array <%p>",        trans->nspaces[counter].vars);
        fprintf(logs_file, "\n Vraiables array capcity: %d", trans->nspaces[counter].var_cap);
        fprintf(logs_file, "\n Variables number: %d",        trans->nspaces[counter].bar_num);

        for (int ct = 0; ct < trans->nspaces[counter].var_num; ct++)
        {
            fprintf(logs_file, "\n[%d] Hash: %ld Ram position: %d", trans->nspaces[counter].vars[ct].hash,
                                                                    trans->nspaces[counter].vars[ct].ram_pos);
        }
        fprintf(logs_file, "\n\n");
    }

    fprintf(logs_file, "\n</div></pre>\n");
}

//-------------------------------------------------------------------

int _trans_struct_dtor(Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    TRANS_PTR_CHECK(trans);

    if (trans->nspaces)
    {
        for (int counter = 0; counter < trans->nspaces_num; counter++)
        {
            if (trans->nspaces[counter].vars)
                free(trans->nsapces[counter].vars);
        }

        free(trans->nspaces);
    }

    trans->asm_file    = 0;
    trans->tree        = 0;

    trans->nspaces     = 0;
    trans->nspaces_num = 0;
    trans->nspaces_cap = 0;

    return 0;
}

//-------------------------------------------------------------------

int _add_nspace(Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    TRANS_PTR_CHECK(trans);

    if (!NSPACES_NUM)
    {
        NSPACES = (Nspace*)calloc(sizeof(Npace), 1);
        NULL_CHECK(NSPACES);
    }

    else
    {
        NSPACES = (Nspace*)my_recalloc(nspaces, (size_t)(NSPACES_NUM + 1), 
                                                (size_t) NSPACES_NUM, 
                                                 sizeof (Nspace));
        NULL_CHECK(NSPASEC);
    }

    NSPACES_NUM++;
    CUR_NSPACE = &(NSPASEC[NSPACES_NUM - 1]);

    return 0;
}

//-------------------------------------------------------------------

int _rm_nspace(Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    TRANS_PTR_CHECK(trans);

    if (CUR_NSPACE->vars)
        free(CUR_NSPACE->vars);

    if (NSPACES_NUM == 1)
    {
        free(NSPACES);
        NSPASEC = NULL;
    }

    else
    {
        NSPASEC = (Nspace*)my_recalloc(NSPACES, (size_t)(NSPACES_NUM - 1), 
                                                (size_t) NSPASEC, 
                                                 sizeof (Nspace));
        NULL_CHECK(NSPASEC);
    }

    NSPACES_NUM--;

    if (NSPASEC_NUM)
        CUR_NSPACE = &(NSPASEC[NSPASEC_NUM - 1]);
    else
        CUR_NSPACE = NULL;

    return 0;
}

//-------------------------------------------------------------------

int _add_var_decl(Trans* trans, int64_t var_hash FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    TRANS_PTR_CHECK(trans);

    if (CUR_NSPACE->var_num == CUR_NSPACE->var_cap - 1 || CUR_NSPACE->var_num == 0)
    {
        int ret = var_arr_increase(CUR_NSPACE);
        RETURN_CHECK(ret);
    }

    for (int counter = 0; counter < CUR_NSPACE->var_num; counter++)
    {
        if (var_hash = CUR_NSPACE->vars[counter].hash)
        {
            error_report(VAR_REDECLARE);
            return -1;
        }
    }

    CUR_NSPACE->vars[CUR_NSPACE->var_num].hash    = var_hash;
    CUR_NSPACE->vars[CUR_NSPACE->var_num].ram_pos = CUR_NSPACE->ram_pos;

    CUR_NSPACE->ram_pos++;
    CUR_NSPACE->var_num++;                 //fix !!!

    return 0;
}  

//-------------------------------------------------------------------

int _was_var_decl(Trans* trans, int64_t hash FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    TRANS_PTR_CHECK(trans);

    if (trans->in_func_flag)
    {
        for (int ct = 0; ct < CUR_NSPACE->var_num; ct++)

            if (hash == CUR_NSPACE->vars[ct].hash)
                return 1;
    }

    else
    {
        for (int counter = 0; counter < NSPASEC_NUM; counter++)

            for (int ct = 0; ct < NSPACES[counter].var_num; ct++)

                if (hash == NSPACES[counter].vars[ct].hash)
                    return 1;
    }

    return 0;
}

//-------------------------------------------------------------------

int _get_var_ram_pos(Trans* trans, int64_t hash FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    TRANS_PTR_CHECK(trans);

    if (trans->in_func_flag)
    {
        for (int ct = 0; ct < CUR_NSPACE->var_num; ct++)

            if (hash == CUR_NSPACE->vars[ct].hash)
                return CUR_NSPACE->vars[ct].ram_pos;
    }

    else
    {
        for (int counter = 0; counter < NSPASEC_NUM; counter++)

            for (int ct = 0; ct < NSPACES[counter].var_num; ct++)

                if (hash == NSPASEC[counter].vars[ct].hash)
                    return NSPACES[counter].vars[ct].ram_pos;
    }

    error_report(VAR_UNDECLARED);
    return -1;
}

//-------------------------------------------------------------------

int _var_arr_increase(Nspace* nspace FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();

    if (!nspace->var_number)
    {
        nspace->vars = (Var*)calloc(Start_var_arr_size, sizeof(Var));
        NULL_CHECK(nsapce->vars);

        nspace->var_cap = Start_var_arr_size;
    }

    else
    {
        nspace->vars = (Var*)my_recalloc(nspace->vars, (size_t)(nspace->var_num * 2), 
                                                       (size_t) nspace->var_num, 
                                                        sizeof(Var));
        NULL_CHECK(nspace->vars);

        nspace->var_cap *= 2;
    }

    return 0;
}

//-------------------------------------------------------------------

int _move_memory_place(Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    FILE_PTR_CHECK(asm_file);

    int offset = 0;

    if (trans->in_func_flag)
        offset = trans->cur_nspace->var_num;
    else
        offset = get_sum_var_num(trans);

    fprintf(asm_file, "\n PUSH dx \n");
    fprintf(asm_file, "\n PUSH %d \n", offset);
    fprintf(asm_file, "\n ADD \n");
    fprintf(asm_file, "\n POP dx \n");

    return offset;
}

//-------------------------------------------------------------------

int _move_memory_place_back(int offset FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    FILE_PTR_CHECK(asm_file);

    fprintf(asm_file, "\n PUSH dx \n");
    fprintf(asm_file, "\n PUSH %d \n", offset);
    fprintf(asm_file, "\n SUB \n");
    fprintf(asm_file, "\n POP dx \n");

    return 0;
}

//-------------------------------------------------------------------

int _get_sum_var_num(Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    TRANS_PTR_CHECK(trans);

    int sum_var_num;

    for (int counter = 0; counter < NSPASEC_NUM; counter++)
        sum_var_num += NSPACES[counter].var_num;

    return sum_var_num;
}

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
    RETURN_CHECK(ret);

    return close_file(asm_file);
}

//===================================================================

int _write_asm_preparations(FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    lang_log_report();
    FILE_PTR_CHECK(asm_file);

    fprintf(ASM_FILE, "\n PUSH %d \n", Free_ram_pos);
    fprintf(ASM_FILE, "\n POP  %s \n", Register);

    fprintf(ASM_FILE, "\n CALL: main\n");
    fprintf(ASM_FILE, "\n HLT \n");

    return 0;
}

//===================================================================

int _trans_tree_to_asm(Tree* tree, FILE* asm_file FOR_LOGS(, LOG_PARAMS))
{
    TREE_PTR_CHECK(tree);
    FILE_PTR_CHECK(asm_file);

    int ret = 0;

    write_asm_preparations(asm_file);

    Trans trans = { 0 };
    ret = trans_struct_ctor(&trans, asm_file, tree);
    RETURN_CHECK(ret);

    if (tree->root->R)
    {
        ret = trans_definitions(tree->root->R, &trans);
        RETURN_CHECK(ret);
    }

    fprintf(asm_file, "\n main: \n");

    int ret = trans_entities(tree->root->L, &trans);
    RETURN_CHECK(ret);

    ret = trans_struct_dtor(&trans);
    RETURN_CHECK(ret);

    return 0;
}

//===================================================================

int _trans_entities(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    int ret = add_nspace(trans);
    RETURN_CHECK(ret);

    while (node)
    {
        if (NODE_IS_STAT(node))
        {
            ret = trans_statement(node, trans);
            RETURN_CHECK(ret);
        }

        else if (NODE_IS_LABEL(node))
        {
            ret = trans_label_decl(node, trans);
            RETURN_CHECK(ret);
        }

        node = node->L;
    }

    ret = rm_nspace(trans);
    RETURN_CHECK(ret);

    return 0;
}

//===================================================================

int _trans_definitions(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    int ret = 0;

    while(node)
    {
        ret = trans_func_defn(node->R, trans);
        RETURN_CHECK(ret);

        node = node->L;
    }

    return 0;
}

//-------------------------------------------------------------------

int _trans_func_defn(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    int ret = add_nspace(trans);
    RETURN_CHECK(ret);

    Node* func_nd = node->L;
    Node* stat_nd = node->R;

    if (!node->L || !node->R)
    {
        error_report(INV_TREE);
        return -1;
    }

    fprintf(ASM_FILE, "\n %ld: \n", func_nd->L->data.id_hash);

    int64_t var_hash = func_nd->R->R->data.id_hash;
    ret = add_var_decl(trans, var_hash);
    RETURN_CHECK(ret);

    // int ram_pos = get_var_ram_pos(trans, var_hash);
    // RETURN_CHECK(ram_pos);

    // fprintf(ASM_FILE, "\n PUSH %s \n", Argument_register);
    // fprintf(ASM_FILE, "\n POP [%s + %d] \n", Register, ram_pos);

    trans->in_func_flag = 1;

    ret = trans_compl_stat(stat_nd, trans);
    RETURN_CHECK(ret);

    trans->in_func_flag = 0;

    ret = rm_nspace(trans);
    RETURN_CHECK(ret);

    return 0;
}

//===================================================================

int _trans_statement(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    Node* content = node->R;

    if (NODE_IS_GOTO(content))
    {
        int ret = trans_label_jump(content, trans);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_DECL(content))
    {
        int ret = trans_decl(content, trans);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_ASS(content))
    {
        int ret = trans_ass(content, trans);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_WHILE(content))
    {
        int ret = trans_cycle(content, trans);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_IF(content))
    {
        int ret = trans_cond(content, trans);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_STD_FUNC(content))
    {
        int ret = trans_std_func_call(content, trans);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_CALL(content))
    {
        int ret = trans_func_call(content, trans);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_RET(content))
    {
        int ret = trans_ret(content, trans);
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

int _trans_compl_stat(Node* node, Trans* trans FOR_LOGS(,LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    int ret = 0;

    ret = add_nspace(trans);
    RETURN_CHECK(ret);

    while(node)
    {
        ret = trans_statement(node, trans);
        RETURN_CHECK(ret);

        node = node->L;
    }

    ret = rm_nspace(trans);
    RETURN_CHECK(ret);

    return 0;
}

//-------------------------------------------------------------------

int _trans_label_decl(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    if (!NODE_IS_ID(node->R))
    {
        error_report(INV_TREE);
        return -1;
    }

    fprintf(ASM_FILE, "\n %ld: \n", node->R->data.id_hash);

    return 0;
}

//-------------------------------------------------------------------

int _trans_cond(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    srand((unsigned int)time(NULL));
    int random = rand();

    int ret = trans_exp(node->R, trans);
    RETURN_CHECK(ret);

    Node* true_stat  = node->L->R;
    Node* false_stat = node->L->L;

    fprintf(ASM_FILE, "\n PUSH 0 \n");
    fprintf(ASM_FILE, "\n JAE: FALSE%d \n", random);

    ret = trans_compl_stat(true_stat, trans);
    RETURN_CHECK(ret);

    fprintf(ASM_FILE, "\n JMP: CONDEND%d \n", random);

    fprintf(ASM_FILE, "\n FALSE%d: \n", random);

    ret = trans_compl_stat(false_stat, trans);
    RETURN_CHECK(ret);

    fprintf(ASM_FILE, "\n CONDEND%d: \n", random);
    return 0;
}

//-------------------------------------------------------------------

int _trans_func_call(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    Node* func_nd = node->L;
    if (!func_nd)
    {
        error_report(INV_TREE);
        return -1;
    }

    int ret = trans_exp(func_nd->R->L, trans);
    RETURN_CHECK(ret);

    int offset = move_memory_place(trans);
    RETURN_CHECK(offset);

    // fprintf(ASM_FILE, "\n POP %s \n", Argument_register);
    // fprintf(ASM_FILE, "\n CALL: %ld \n", func_nd->L->data.id_hash);

    fprintf(ASM_FILE, "\n PUSH %s \n", Return_register);

    ret = move_memory_place_back(offset);
    RETURN_CHECK(ret);

    return 0;
}

//-------------------------------------------------------------------

int _trans_ass(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    int ret = trans_exp(node->R, trans);
    RETURN_CHECK(ret);

    int ram_pos = get_var_ram_pos(trans, node->L->data.id_hash);
    RETURN_CHECK(ram_pos);

    fprintf(ASM_FILE, "\n POP [%s + %d] \n", Register, ram_pos);

    return 0;
}

//-------------------------------------------------------------------

int _trans_decl(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    int64_t var_hash = 0;
    int ret          = 0;

    if (NODE_IS_ID(node->L))
    {
        ret = add_var_decl(node->L->data.id_hash, 0);
        RETURN_CHECK(ret);

        var_hash = node->L->data.id_hash;
    }

    else if (NODE_IS_PERM(node->L) && NODE_IS_ID(node->L->R))
    {
        ret = add_var_decl(node->L->R->data.id_hash, 1);
        RETURN_CHECK(ret);

        var_hash = node->L->R->data.id_hash;
    }

    else
    {
        error_report(INV_TREE);
        return -1;
    }

    int ret = trans_exp(node->R, trans);
    RETURN_CHECK(ret);

    int ram_pos = get_var_ram_pos(trans, var_hash);
    RETURN_CHECK(ram_pos);

    fprintf(ASM_FILE, "\n POP [%s + %d] \n", Register, ram_pos);

    return 0;
}

//-------------------------------------------------------------------

int _trans_cycle(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    srand((unsigned int)time(NULL));
    int random = rand();

    fprintf(ASM_FILE, "\n REPEAT%d: \n", random);

    int ret = trans_exp(node->R, trans);
    RETURN_CHECK(ret);

    fprintf(ASM_FILE, "\n PUSH 0 \n");
    fprintf(ASM_FILE, "\n JB: CYCLEEND%d \n", random);

    ret = trans_statement(node->L, trans);
    RETURN_CHECK(ret);
    fprintf(ASM_FILE, "\n JMP: REPEAT%d \n", random);

    fprintf(ASM_FILE, "\n CYCLEEND%d: \n", random);

    return 0;
}

//-------------------------------------------------------------------

int _trans_print(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    int ret = trans_exp(node->R, trans);
    RETURN_CHECK(ret);

    fprintf(ASM_FILE, "\n OUT \n");

    return 0;
}

//-------------------------------------------------------------------

int _trans_scan(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    if (!NODE_IS_ID(node->R))
    {
        error_report(INV_TREE);
        return -1;
    }

    int ram_pos = get_var_ram_pos(trans, node->R->data.id_hash);
    RETURN_CHECK(ram_pos);

    fprintf(ASM_FILE, "\n IN \n");
    fprintf(ASM_FILE, "\n POP [%s + %d] \n", Register, ram_pos);

    return 0;
}

//-------------------------------------------------------------------

int _trans_std_func_call(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    switch(node->data.std_func_code)
    {
        case SCAN:
        {
            int ret = trans_scan(node, trans);
            RETURN_CHECK(ret);
            break;
        }

        case PRINT:
        {
            int ret = trans_print(node, trans);
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

int _trans_label_jump(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    if (!NODE_IS_ID(node->L))
    {
        error_report(INV_TREE);
        return -1;
    }

    fprintf(ASM_FILE, "\n JMP: %ld\n", node->L->data.id_hash);

    return 0;
}

//-------------------------------------------------------------------

int _trans_ret(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    int ret = trans_exp(node->R, trans);
    RETURN_CHECK(ret);

    fprintf(ASM_FILE, "\n POP %s \n", Return_register);
    fprintf(ASM_FILE, "\n RET \n");

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

int _trans_exp(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    int ret = 0;

    if (NODE_IS_CALL(node))
    {
        ret = trans_func_call(node, trans);
        RETURN_CHECK(ret);

        return 0;
    }

    if (node->L)
    {
        ret = trans_exp(node->L, trans);
        RETURN_CHECK(ret);
    }

    if (node->R)
    {
        ret = trans_exp(node->R, trans);
        RETURN_CHECK(ret);
    }

    if (NODE_IS_CONSTANT(node))
    {
        ret = trans_constant(node, trans);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_ID(node))
    {
        ret = trans_variable(node, trans);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_UNAR_OPER(node))
    {
        ret = trans_unar_operand(node, trans);
        RETURN_CHECK(ret);
    }

    else if (NODE_IS_BIN_OPER(node))
    {
        ret = trans_bin_operand(node, trans);
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

int _trans_constant(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    fprintf(ASM_FILE, "\n PUSH %g \n", node->data.constant);

    return 0;
}

//-------------------------------------------------------------------

int _trans_variable(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    printf("\n\n hash is %ld \n\n", node->data.id_hash);

    int ram_pos = get_var_ram_pos(trans, node->data.id_hash);
    RETURN_CHECK(ram_pos);

    fprintf(ASM_FILE, "\n PUSH [%s + %d] \n", Register, ram_pos);

    return 0;
}

//-------------------------------------------------------------------

int _trans_bin_operand(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    switch(node->data.operand_code)
    {
        case ADD:
        {
            fprintf(ASM_FILE, "\n ADD \n");
            break;
        }

        case SUB:
        {
            fprintf(ASM_FILE, "\n SUB \n");
            break;
        }

        case MUL:
        {
            fprintf(ASM_FILE, "\n MUL \n");
            break;
        }

        case DIV:
        {
            fprintf(ASM_FILE, "\n DIV \n");
            break;
        }

        case POW:
        {
            fprintf(ASM_FILE, "\n POW \n");
            break;
        }

        case EQ:
        {
            fprintf(ASM_FILE, "\n EQ \n");
            break;
        }

        case NEQ:
        {
            fprintf(ASM_FILE, "\n NEQ \n");
            break;
        }

        case MORE:
        {
            fprintf(ASM_FILE, "\n MR \n");
            break;
        }

        case LESS:
        {
            fprintf(ASM_FILE, "\n LS \n");
            break;
        }

        case MEQ:
        {
            fprintf(ASM_FILE, "\n MRE \n");
            break;
        }

        case LEQ:
        {
            fprintf(ASM_FILE, "\n LSE \n");
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

int _trans_unar_operand(Node* node, Trans* trans FOR_LOGS(, LOG_PARAMS))
{
    TRANS_START_CHECK(node, trans);

    switch(node->data.operand_code)
    {
        case SIN:
        {
            fprintf(ASM_FILE, "\n SIN \n");
            break;
        }

        case COS:
        {
            fprintf(ASM_FILE, "\n COS \n");
            break;
        }

        case SQRT:
        {
            fprintf(ASM_FILE, "\n PUSH 0.5 \n");
            fprintf(ASM_FILE, "\n POW \n");
            break;
        }

        case TG:
        {
            fprintf(ASM_FILE, "\n TAN \n");
            break;
        }

        case CTG:
        {
            fprintf(ASM_FILE, "\n TAN \n");
            fprintf(ASM_FILE, "\n PUSH -1 \n");
            fprintf(ASM_FILE, "\n POW \n");
            break;
        }

        case LN:
        {
            fprintf(ASM_FILE, "\n LN \n");
            break;
        }

        case ARCSIN:
        {
            fprintf(ASM_FILE, "\n ASIN \n");
            break;
        }

        case ARCCOS:
        {
            fprintf(ASM_FILE, "\n ASIN \n");
            fprintf(ASM_FILE, "\n PUSH -1 \n");
            fprintf(ASM_FILE, "\n MUL \n");
            fprintf(ASM_FILE, "\n PUSH 1.570796 \n");
            fprintf(ASM_FILE, "\n ADD \n");
            break;
        }

        case ARCCTG:
        {
            fprintf(ASM_FILE, "\n ATG \n");
            fprintf(ASM_FILE, "\n PUSH -1 \n");
            fprintf(ASM_FILE, "\n POW \n");
            break;
        }

        case ARCTG:
        {
            fprintf(ASM_FILE, "\n ATG \n");
            break;
        }

        case SQR:
        {
            fprintf(ASM_FILE, "\n PUSH 2 \n");
            fprintf(ASM_FILE, "\n POW \n");
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