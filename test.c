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

int main(int argc, char *argv[]) {
    int base_a = 35;
    int base_b = 99;
    if (argc == 3) {
        base_a = atoi(argv[1]);
        base_b = atoi(argv[2]);
    }
    printf("ALU Testprogram\n");
    char buffer[REG_WIDTH+1];


    printf("Testing one complement\n");

    int a = base_a;
    char *reg_a = dec_to_binary(a, buffer);
    printf("in dec: \t%i\tin binary: \t%s\n", a, reg_a);
    one_complement(reg_a);
    a = binary_to_dec(reg_a);
    printf("out dec: \t%i\tout binary: \t%s\n", a, reg_a);
    
    int b = base_b;
    char *reg_b = dec_to_binary(b, buffer);
    printf("in dec: \t%i\tin binary: \t%s\n", b, reg_b);
    one_complement(reg_b);
    b = binary_to_dec(reg_b);
    printf("out dec: \t%i\tout binary: \t%s\n", b, reg_b);


    printf("Testing two complement\n");

    a = base_a;
    reg_a = dec_to_binary(a, buffer),
    printf("in dec: \t%i\tin binary: \t%s\n", a, reg_a);
    two_complement(reg_a);
    a = binary_to_dec(reg_a)-256;
    printf("out dec: \t%i\tout binary: \t%s\n", a, reg_a);

    b = base_b;
    reg_b = dec_to_binary(b, buffer),
    printf("in dec: \t%i\tin binary: \t%s\n", b, reg_b);
    two_complement(reg_b);
    b = binary_to_dec(reg_b)-256;
    printf("out dec: \t%i\tout binary: \t%s\n", b, reg_b);

    char *test_reg = reg_a;
    char test_reg_add[REG_WIDTH+1] = "11000011";
    char test_accu[REG_WIDTH+1] = "00000000";
    char test_flags[REG_WIDTH+1] = "00000000";
    printf("Testing ADD\n");
    printf("in\t reg_a:%s\t reg_b:%s\t accu:%s\t flags:%s\n", test_reg, test_reg_add, test_accu, test_flags);
    alu_op_ADD(test_reg, test_reg_add, test_accu, test_flags);
    printf("out\t reg_a:%s\t reg_b:%s\t accu:%s\t flags:%s\n", test_reg, test_reg_add, test_accu, test_flags);

    return 0;
}
