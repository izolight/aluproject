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
    printf("out dec: \t%d\t out binary: \t%s\n\n", num, reg);
}

void test_two_complement(int num) {
    char buf[REG_WIDTH+1];
    char *reg = dec_to_binary(num, buf);
    printf("in dec: \t%i\tin binary: \t%s\n", num, reg);
    two_complement(reg);
    num = binary_to_dec(reg)-256;
    printf("out dec: \t%i\tout binary: \t%s\n\n", num, reg);
}

void test_LOGIC(int a, int b, void (*f)(char *reg_a, char *reg_b, char *accu, char *flags)) {
    char buff_a[REG_WIDTH+1];
    char buff_b[REG_WIDTH+1];
    char *reg_a = dec_to_binary(a, buff_a);
    char *reg_b = dec_to_binary(b, buff_b);
    char accu[REG_WIDTH+1] = "00000000";
    char flags[REG_WIDTH+1] = "00000000";

    printf("in\t reg_a:%s\t reg_b:%s\t accu:%s\t flags:%s\n", reg_a, reg_b, accu, flags);
    printf("in dec_a: \t%i\tin dec_b: \t%i\n", a, b);
    (*f)(reg_a, reg_b, accu, flags);
    int accu_out = binary_to_dec(accu);
    printf("out\t reg_a:%s\t reg_b:%s\t accu:%s\t flags:%s\n", reg_a, reg_b, accu, flags);
    printf("out accu: \t%i\n\n", accu_out);
}

int main(int argc, char *argv[]) {
    int a = 35;
    int b = 99;
    if (argc == 3) {
        a = atoi(argv[1]);
        b = atoi(argv[2]);
    }

    printf("ALU Testprogram\n");
    printf("Test values are %d and %d\n\n", a, b);

    printf("Testing one complement\n");
    test_one_complement(a);
    test_one_complement(b);

    printf("Testing two complement\n");
    test_two_complement(a);
    test_two_complement(b);

    printf("Testing ADD\n");
    test_LOGIC(a, b, alu_op_ADD);

    printf("Testing OR\n");
    test_LOGIC(a, b, alu_op_OR);

    printf("Testing AND\n");
    test_LOGIC(a, b, alu_op_AND);

    printf("Testing XOR\n");
    test_LOGIC(a, b, alu_op_XOR);

    printf("Testing ASL\n");
    test_LOGIC(a, b, alu_op_ASL);

    printf("Testing ROL\n");
    test_LOGIC(a, b, alu_op_ROL);

    printf("Testing LSR\n");
    test_LOGIC(a, b, alu_op_LSR);

    printf("Testing ADC\n");
    test_LOGIC(a, b, alu_op_ADC);

    printf("Testing ROR\n");
    test_LOGIC(a, b, alu_op_ROR);
    return 0;
}
