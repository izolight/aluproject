#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "register.h"
#include "flags.h"
#include "alu-opcodes.h"
#include "alu.h"

int binary_to_dec(char binary[]) {
    int dec = 0;
    int pow2 = 0;
    int i;
    for (i = REG_WIDTH-1; i >= 0; i--) {
        if (binary[i] == '1') {
            dec += pow(2,pow2);
        }
        pow2++;
    }
    return dec;
}

char *dec_to_binary(int dec, char out[]) {
    int i;
    for (i = REG_WIDTH-1; i >= 0; i--) {
        if (dec % 2 == 0) {
            out[i] = '0';
        } else {
            out[i] = '1';
        }
        dec = dec/2;
    }
    out[REG_WIDTH] = 0;
    return out;
}

void test_one_complement(int num) {
    char buf[REG_WIDTH+1];
    char *reg = dec_to_binary(num, buf);
    printf("in dec: \t%d\tin binary: \t%s\n", num, reg);
    one_complement(reg);
    num = binary_to_dec(reg);
    printf("out dec: \t%d\t out binary: \t%s\n", num, reg);
}

void test_two_complement(int num) {
    char buf[REG_WIDTH+1];
    char *reg = dec_to_binary(num, buf);
    printf("in dec: \t%i\tin binary: \t%s\n", num, reg);
    two_complement(reg);
    num = binary_to_dec(reg)-256;
    printf("out dec: \t%i\tout binary: \t%s\n", num, reg);
}

void test_LOGIC(int a, int b, void (*f)(int)) {
    
}

void test_ADD(int a, int b) {
    char buff_a[REG_WIDTH+1];
    char buff_b[REG_WIDTH+1];
    char *reg_a = dec_to_binary(a, buff_a);
    char *reg_b = dec_to_binary(b, buff_b);
    char accu[REG_WIDTH+1] = "00000000";
    char flags[REG_WIDTH+1] = "00000000";
    
    printf("in\t reg_a:%s\t reg_b:%s\t accu:%s\t flags:%s\n", reg_a, reg_b, accu, flags);
    printf("in dec_a: \t%i\tin dec_b: \t%i\n", a, b);
    alu_op_ADD(reg_a, reg_b, accu, flags);
    int accu_out = binary_to_dec(accu);
    printf("out\t reg_a:%s\t reg_b:%s\t accu:%s\t flags:%s\n", reg_a, reg_b, accu, flags);
    printf("out accu: \t%i\n", accu_out);
}

void test_OR(int a, int b) {
    char buff_a[REG_WIDTH+1];
    char buff_b[REG_WIDTH+1];
    char *reg_a = dec_to_binary(a, buff_a);
    char *reg_b = dec_to_binary(b, buff_b);
    char accu[REG_WIDTH+1] = "00000000";
    char flags[REG_WIDTH+1] = "00000000";

    printf("in\t reg_a:%s\t reg_b:%s\t accu:%s\t flags:%s\n", reg_a, reg_b, accu, flags);
    printf("in dec_a: \t%i\tin dec_b: \t%i\n", a, b);    
    alu_op_OR(reg_a, reg_b, accu, flags);
    int accu_out = binary_to_dec(accu);
    printf("out\t reg_a:%s\t reg_b:%s\t accu:%s\t flags:%s\n", reg_a, reg_b, accu, flags);
    printf("out accu: \t%i\n", accu_out);    
}
  
void test_AND(int a, int b) {
    char buff_a[REG_WIDTH+1];
    char buff_b[REG_WIDTH+1];
    char *reg_a = dec_to_binary(a, buff_a);
    char *reg_b = dec_to_binary(b, buff_b);
    char accu[REG_WIDTH+1] = "00000000";
    char flags[REG_WIDTH+1] = "00000000";

    printf("in\t reg_a:%s\t reg_b:%s\t accu:%s\t flags:%s\n", reg_a, reg_b, accu, flags);
    printf("in dec_a: \t%i\tin dec_b: \t%i\n", a, b);    
    alu_op_AND(reg_a, reg_b, accu, flags);
    int accu_out = binary_to_dec(accu);
    printf("out\t reg_a:%s\t reg_b:%s\t accu:%s\t flags:%s\n", reg_a, reg_b, accu, flags);
    printf("out accu: \t%i\n", accu_out);
}  

void test_XOR(int a, int b) {
    char buff_a[REG_WIDTH+1];
    char buff_b[REG_WIDTH+1];
    char *reg_a = dec_to_binary(a, buff_a);
    char *reg_b = dec_to_binary(b, buff_b);
    char accu[REG_WIDTH+1] = "00000000";
    char flags[REG_WIDTH+1] = "00000000";

    printf("in\t reg_a:%s\t reg_b:%s\t accu:%s\t flags:%s\n", reg_a, reg_b, accu, flags);
    printf("in dec_a: \t%i\tin dec_b: \t%i\n", a, b);    
    alu_op_XOR(reg_a, reg_b, accu, flags);
    int accu_out = binary_to_dec(accu);
    printf("out\t reg_a:%s\t reg_b:%s\t accu:%s\t flags:%s\n", reg_a, reg_b, accu, flags);
    printf("out accu: \t%i\n", accu_out);
}  

int main(int argc, char *argv[]) {
    int base_a = 35;
    int base_b = 99;
    if (argc == 3) {
        base_a = atoi(argv[1]);
        base_b = atoi(argv[2]);
    }

    printf("ALU Testprogram\n");
    printf("Test values are %d and %d\n", base_a, base_b);
    printf("\n");

    printf("Testing one complement\n");
    test_one_complement(base_a);
    test_one_complement(base_b);
    printf("\n");

    printf("Testing two complement\n");
    test_two_complement(base_a);
    test_two_complement(base_b);
    printf("\n");

    printf("Testing ADD\n");
    test_ADD(base_a, base_b);
    printf("\n");

    printf("Testing OR\n");
    test_OR(base_a, base_b);
    printf("\n");

    printf("Testing AND\n");
    test_AND(base_a, base_b);
    printf("\n"),

    printf("Testing XOR\n");
    test_XOR(base_a, base_b);
    printf("\n");

    return 0;
}
