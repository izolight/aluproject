/*
   alu.h

   bho1 2015
   initial
   
   GPL applies
*/

#ifndef _ALU_HEADER_
#define _ALU_HEADER_

#include "alu-opcodes.h"

char getCarryflag(char flags[]);
char getSignflag(char flags[]);
char getZeroflag(char flags[]);
char getOverflowflag(char flags[]);

int zero_test(char flags[]);
void alu(unsigned int alu_opcode, char reg_in_a[], char reg_in_b[], char reg_out_accu[], char flags[]);

void print_alu(char *rega, char *regb, char * accumulator, char flags[]);
void zsflagging(char *flags, char *dummy);
void one_complement(char* reg);
void two_complement(char* reg);
void alu_op_ADD(char *rega, char *regb, char *accumulator, char flags[]);
void alu_op_OR(char *rega, char *regb, char *accumulator, char flags[]);
void alu_op_AND(char *rega, char *regb, char *accumulator, char flags[]);
void alu_op_XOR(char *rega, char *regb, char *accumulator, char flags[]);
void alu_op_ASL(char *rega, char *regb, char *accumulator, char flags[]);
void alu_op_LSR(char *rega, char *regb, char *accumulator, char flags[]);
void alu_op_ROR(char *rega, char *regb, char *accumulator, char flags[]);
void alu_op_ROL(char *rega, char *regb, char *accumulator, char flags[]);
void alu_op_ADC(char *rega, char *regb, char *accumulator, char flags[]);
#endif /* _ALU_H_ */
