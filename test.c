#include <stdio.h>
#include <string.h>

#include "register.h"
#include "flags.h"
#include "alu-opcodes.h"
#include "alu.h"

int main() {
    char test_reg[REG_WIDTH+1] = "00101101";

    printf("Testing one complement\nin: \t%s\n", test_reg);
    one_complement(test_reg);
    printf("out: \t%s\n", test_reg);

    printf("Testing two complement\nin: \t%s\n", test_reg);
    two_complement(test_reg);
    printf("out: \t%s\n", test_reg);

    char test_reg_add[REG_WIDTH+1] = "11011011";
    char test_accu[REG_WIDTH+1] = "00000000";
    char test_flags[REG_WIDTH+1] = "00000000";
    printf("Testing ADD\nin\t reg_a:%s\t reg_b:%s\n", test_reg, test_reg_add);
    alu_op_ADD(test_reg, test_reg_add, test_accu, test_flags);
    return 0;
}
