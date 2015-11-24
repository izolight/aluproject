#include <stdio.h>
#include <string.h>

#include "register.h"
#include "flags.h"
#include "alu-opcodes.h"
#include "alu.h"

int main() {
    char test_reg[REG_WIDTH+1] = "00101101";
    char one_comp_reg[REG_WIDTH+1] = "00101101";
    one_complement(one_comp_reg);
    printf("Testing one complement\nin: \t%s\nout: \t%s\n", test_reg, one_comp_reg);

    return 0;
}
