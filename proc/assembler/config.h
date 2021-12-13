#pragma once

#include <stdint.h>

#include "../stack/stack_config.h"

//=========================================================

//PROCESSOR CONFIG

#define PROC_NUM_OF_CMND_LINE_ARGS 5

#define PROC_LOGS

#define REGISTER_SIZE 16

#define RAM_SIZE 2048 

#define FRAME_LENGTH 15

#define FRAME_WIDTH 41

#define VIDEO_MEMORY_ADDRESS 1024

#define VIDEO_SIZE FRAME_LENGTH * FRAME_WIDTH

#define PRECISION 1e-3

const float Sleep_duration = 0.5;

//=========================================================

//ASSEMBLER

#define ASM_NUM_OF_CMND_LINE_ARGS 5

#define ASM_LOGS

#define LISTING

#define ASM_MAX_CODE_SIZE 2048 

#define UD_JUMPS_NUMBER 10

#define STRING_JUMPS_NUMBER 10

#define OPERATION_NAME_BUF_SIZE 10

//LABELS

#define LABEL_NAME_SIZE 30

#define START_LABELS_NUMBER 10

//=========================================================

//DISASSEMBLER OPTIONS

#define DISASM_NUM_OF_CMND_LINE_ARGS 4

#define DISASM_DEBUG

#define DISASM_LOGS

#define DISASM_LISTING

//=========================================================

//TEXT_PROCESSING

#define TEXT_LOGS
