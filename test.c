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
    return 0;
}
