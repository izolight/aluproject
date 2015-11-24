#include <stdio.h>
#include <string.h>

#include "register.h"
#include "flags.h"
#include "alu-opcodes.h"
#include "alu.h"

int main() {
    char initial_reg[REG_WIDTH+1] = "00101101";

    char test_reg_1[REG_WIDTH+1] = "00101101";
    one_complement(test_reg_1);
    printf("Testing one complement\nin: \t%s\nout: \t%s\n", initial_reg, test_reg_1);

    char test_reg_2[REG_WIDTH+1] = "00101101";
    two_complement(test_reg_2);
    printf("Testing two complement\nin: \t%s\nout: \t%s\n", initial_reg, test_reg_2);

    return 0;
}
