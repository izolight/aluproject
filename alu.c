/* 
   alu.c 
   - 21.11.05/BHO1
   bho1 29.12.2006
   bho1 6.12.2007
   bho1 30.11.2007 - clean up
   bho1 24.11.2009 - assembler instruction
   bho1 3.12.2009 - replaced adder with full_adder
   bho1 20.7.2011 - rewrite: minimize global vars, ALU-operations are modeled with fct taking in/out register as parameter
   bho1 6.11.2011 - rewrite flags: adding flags as functional parameter. Now alu is truly a function
   bho1 26.11.2012 - remove bit declaration from op_alu_asl and op_alu_ror as they are unused (this may change later)
   bho1 20.9.2014 cleaned
   bho1 7.11.2015 whole new setup with funktion table and alu op header comment
   
   GPL applies

   -->> YOUR FULL NAME HERE  <<--
*/

#include <stdio.h>
#include <string.h>

#include "alu.h"
#include "alu-opcodes.h"
#include "register.h"
#include "flags.h"
int const max_mue_memory = 100;

char mue_memory[100]= "100 Byte - this memory is at your disposal"; /*mue-memory */
char* m = mue_memory;

unsigned int c = 0; 	/* carry bit address    */
unsigned int s = 1;	    /* sum bit address      */
unsigned int c_in = 2;	/* carry in bit address */



/*
  testet ob alle bits im akkumulator auf null gesetzt sind und setzt zero flag entsprechend.
*/
void apply_zero_flag(char accumulator[], char flags[]){
  int i;
  for(i=0;accumulator[i]!='\0'; i++){
    if(accumulator[i]!='0'){		
      flags[ZFLAG] = '0';
      return;
    }
  }
  flags[ZFLAG] = '1';
}


void  apply_overflow_flag(char rega[], char regb[], char regc[], char flags[]){
  //Overflowflag
  if( ( rega[0] == '1' &&
	regb[0] == '1' &&
	regc[0] == '0' )  ||
      ( rega[0] == '0' &&
	regb[0] == '0' &&
	regc[0] == '1' ) ){
    flags[OFLAG] = '1';
  } else {
    flags[OFLAG] = '0';
  }
}

void zsflagging(char* flags,char *acc){
  //Zeroflag
  apply_zero_flag(acc, flags);
  
  //Signflag
  flags[SFLAG] = acc[0];
}


/*
  clear mue_memory
*/
void mu_memory_reset(){
  int i;
  
  for(i=0;i<max_mue_memory;i++)
    m[i] = '0';
}


/*
  Invertieren der Character Bits im Register reg
  one_complement(char reg[]) --> NOT(reg)
*/
void one_complement(char reg[]){
    int i;
    for (i = 0; i < REG_WIDTH; i++) {
        if (reg[i] == '0')
            reg[i] = '1';
        else if (reg[i] == '1')
            reg[i] = '0';
    }
}

/*
  Das zweier-Komplement des Registers reg wird in reg geschrieben
  reg := K2(reg)
*/
void two_complement(char reg[]){
    one_complement(reg);
    int i = REG_WIDTH-1;
    while(reg[i] != '0' && i >= 0) {
        reg[i] = '0';
        i--;
    }
    if (i >= 0)
        reg[i] = '1';
}

/*
  Die Werte in Register rega und Register regb werden addiert, das
  Resultat wird in Register accumulator geschrieben. Die Flags cflag, 
  oflag, zflag und sflag werden entsprechend gesetzt
  
  accumulator := rega + regb
*/
void alu_op_ADD(char rega[], char regb[], char accumulator[], char flags[]){
    int i;
    char carry = '0';
    for (i = REG_WIDTH-1; i >= 0; i--) {
        if (rega[i] == '0' && regb[i] == '0') {
            if (carry == '0') {
                accumulator[i] = '0';
            } else {
                accumulator[i] = '1';
            }
            carry = '0';
        } else if ((rega[i] == '0' && regb[i] == '1') || (rega[i] == '1' && regb[i] == '0')) {
            if (carry == '0') {
                accumulator[i] = '1';
            } else {
                accumulator[i] = '0';
                carry = '1';
            }
        } else if (rega[i] == '1' && regb[i] == '1') {
            if (carry == '0') {
                accumulator[i] = '0';
                carry = '1';
            } else {
                accumulator[i] = '1';
            }
        }
    }
    apply_overflow_flag(rega, regb, accumulator, flags);
    zsflagging(flags, accumulator);
    flags[CFLAG] = carry;
}

//    #######  ######## 
//   ##     ## ##     ##
//   ##     ## ##     ##
//   ##     ## ######## 
//   ##     ## ##   ##  
//   ##     ## ##    ## 
//    #######  ##     ##

/*
  op:      OR
  ex:      25 OR af -> fe and flags setting
  flags:
  *----*-
  NVBDIZC
  short:   or with accumulator
  form1:   A:=A or {adr}
  form2:   A <- (A) V M
  form3:
  *       A = A | M
  *       P.N = A.7
  *       P.Z = (A==0) ? 1:0
  *

  */

void alu_op_OR(char rega[], char regb[], char accumulator[], char flags[]){
    int i;
    for (i = 0; i < REG_WIDTH; i++) {
        if (rega[i] == '1' || regb[i] == '1') {
            accumulator[i] = '1';
        } else {
            accumulator[i] = '0';
        }
    }
    apply_overflow_flag(rega, regb, accumulator, flags);
    zsflagging(flags, accumulator);
}


//      ###     ######  ##      
//     ## ##   ##    ## ##      
//    ##   ##  ##       ##      
//   ##     ##  ######  ##      
//   #########       ## ##      
//   ##     ## ##    ## ##      
//   ##     ##  ######  ########

//  ASL          ASL Shift Left One Bit (Memory or Accumulator)           ASL
//                   +-+-+-+-+-+-+-+-+
//  Operation:  C <- |7|6|5|4|3|2|1|0| <- 0
//                   +-+-+-+-+-+-+-+-+                    N Z C I D V
//                                                        / / / _ _ _
//
//  op:      ASL
//  ex:      25 ASL af -> fe and flags setting
//  flags:   *----**
//  NVBDIZC
//  short:   arithmetic shift left
//  form1:   {adr}:={adr}*2
//  form2:   C <- A7, A <- (A) << 1
//  form3:
//  *       P.C = B.7
//  *       B = (B << 1) & $FE
//  *       P.N = B.7
//  *       P.Z = (B==0) ? 1:0
//  *
//
//

void alu_op_ASL(char accu[], char regb[], char regouta[], char flags[]){
  // your code here
}


//      ###    ##    ## ######## 
//     ## ##   ###   ## ##     ##
//    ##   ##  ####  ## ##     ##
//   ##     ## ## ## ## ##     ##
//   ######### ##  #### ##     ##
//   ##     ## ##   ### ##     ##
//   ##     ## ##    ## ######## 
//
//  op:      AND
//  ex:      25 AND af -> fe and flags setting
//  flags:   *----*-
//  NVBDIZC
//  short:   and (with accumulator)
//  form1:   A:=A\&{adr}
//  form2:   A <- (A) /\ M
//  form3:
//  *       A = A & M
//  *       P.N = A.7
//  *       P.Z = (A==0) ? 1:0
//  *
//


void alu_op_AND(char rega[], char regb[], char accumulator[], char flags[]){
    int i;
    for (i = 0; i < REG_WIDTH; i++) {
        if (rega[i] == '1' && regb[i] == '1') {
            accumulator[i] = '1';
        } else {
            accumulator[i] = '0';
        }        
    }
    apply_overflow_flag(rega, regb, accumulator, flags);
    zsflagging(flags, accumulator);
}



//   ########   #######  ##      
//   ##     ## ##     ## ##      
//   ########  ##     ## ##      
//   ##   ##   ##     ## ##      
//   ##    ##  ##     ## ##      
//   ##     ##  #######  ########

//  ROL          ROL Rotate one bit left (memory or accumulator)          ROL
//
//               +------------------------------+
//               |         M or A               |
//               |   +-+-+-+-+-+-+-+-+    +-+   |
//  Operation:   +-< |7|6|5|4|3|2|1|0| <- |C| <-+         N Z C I D V
//                   +-+-+-+-+-+-+-+-+    +-+             / / / _ _ _
//
//
//  op:      ROL
//  ex:      25 ROL af -> fe and flags setting
//  flags:   *----**
//  NVBDIZC
//  short:   rotate left
//  form1:   {adr}:={adr}*2+C
//  form2:   C <- A7 & A <- A << 1 + C
//  form3:
//  *       t = B.7
//  *       B = (B << 1) & $FE
//  *       B = B | P.C
//  *       P.C = t
//  *       P.Z = (B==0) ? 1:0
//  *       P.N = B.7
//  *  
//

void alu_op_ROL(char accu[], char regb[], char regc[], char flags[]){
}


/*
  op:      EOR
  ex:      25 EOR af -> fe and flags setting
  flags:   *----*-
  NVBDIZC
  short:   exclusive or (with accumulator)
  form1:   A:=A exor {adr}
  form2:   A <- (A) \-/ M
  form3:
  *       A = A ^ M
  *       P.N = A.7
  *       P.Z = (A==0) ? 1:0
  *

  */

void alu_op_XOR(char rega[], char regb[], char accumulator[], char flags[]){
    int i;
    for (i = 0; i < REG_WIDTH; i++) {
        if ((rega[i] == '1' && regb[i] == '1') || (rega[i] == '0' && regb[i] == '0')) {
            accumulator[i] = '0';
        } else { 
            accumulator[i] = '1';
        }
    }
    apply_overflow_flag(rega, regb, accumulator, flags);
    zsflagging(flags, accumulator);
}



//  LSR          LSR Shift right one bit (memory or accumulator)          LSR
//
//                   +-+-+-+-+-+-+-+-+
//  Operation:  0 -> |7|6|5|4|3|2|1|0| -> C               N Z C I D V
//                   +-+-+-+-+-+-+-+-+                    0 / / _ _ _
//
//
//  op:      LSR
//  ex:      25 LSR af -> fe and flags setting
//  flags:   *----**
//  NVBDIZC
//  short:   logical shift right
//  form1:   {adr}:={adr}/2
//  form2:   C <- A0, A <- (A) >> 1
//  form3:
//  *       P.N = 0
//  *       P.C = B.0
//  *       B = (B >> 1) & $7F
//  *       P.Z = (B==0) ? 1:0
//  *
//
void alu_op_LSR(char accu[], char regb[], char regc[], char flags[]){
  // your code here
}

//      ###    ########   ###### 
//     ## ##   ##     ## ##    ##
//    ##   ##  ##     ## ##      
//   ##     ## ##     ## ##      
//   ######### ##     ## ##      
//   ##     ## ##     ## ##    ##
//   ##     ## ########   ###### 

/*
  op:      ADC
  ex:      25 ADC af -> fe and flags setting
  flags:   **---**
  NVBDIZC
  short:   add with carry
  form1:   A:=A+{adr}
  form2:   A <- (A) + M + C
  form3:
  *       t = A + M + P.C
  *       P.V = (A.7!=t.7) ? 1:0
  *       P.N = A.7
  *       P.Z = (t==0) ? 1:0
  *       IF (P.D)
  *         t = bcd(A) + bcd(M) + P.C
  *         P.C = (t>99) ? 1:0
  *       ELSE
  *         P.C = (t>255) ? 1:0
  *       A = t & 0xFF
  *

  */

void alu_op_ADC(char rega[], char regb[], char accumulator[], char flags[]){
  // your code here
}


//   ########   #######  ######## 
//   ##     ## ##     ## ##     ##
//   ##     ## ##     ## ##     ##
//   ########  ##     ## ######## 
//   ##   ##   ##     ## ##   ##  
//   ##    ##  ##     ## ##    ## 
//   ##     ##  #######  ##     ##



//  ROR          ROR Rotate one bit right (memory or accumulator)         ROR
//
//               +------------------------------+
//               |                              |
//               |   +-+    +-+-+-+-+-+-+-+-+   |
//  Operation:   +-> |C| -> |7|6|5|4|3|2|1|0| >-+         N Z C I D V
//                   +-+    +-+-+-+-+-+-+-+-+             / / / _ _ _
//
//
//  op:      ROR
//  ex:      25 ROR af -> fe and flags setting
//  flags:   *----**
//  NVBDIZC
//  short:   rotate right
//  form1:   {adr}:={adr}/2+C*128
//  form2:   C<-A0 & A<- (A7=C + A>>1)
//  form3:
//  *       t = B.0
//  *       B = (B >> 1) & $7F
//  *       B = B | ((P.C) ? $80:$00)
//  *       P.C = t
//  *       P.Z = (B==0) ? 1:0
//  *       P.N = B.7
//  *
//
void alu_op_ROR(char accu[], char regb[], char regouta[], char flags[]){
  // your code here
}



/*
  op:      DEC
  flags:   *----*-
           NVBDIZC
  short:   decrement
  form1:   {adr}:={adr}-1
  form2:   M <- (M) - 1
  form3:
  *       M = (M - 1) & $FF
  *       P.N = M.7
  *       P.Z = (M==0) ? 1:0
  *

  */

void alu_op_DEC(char rega[], char regb[], char accu[], char flags[]){
  // your code here
}

void alu_op_NEG(char rega[], char regb[], char accumulator[], char flags[]){
  // empty, ignore
}

void alu_op_NOT(char rega[], char regb[], char accumulator[], char flags[]){
  // empty, ignore
}
void alu_op_SUB(char rega[], char regb[], char accumulator[], char flags[]){
  // your code here
}

//    ######  ########   ###### 
//   ##    ## ##     ## ##    ##
//   ##       ##     ## ##      
//    ######  ########  ##      
//         ## ##     ## ##      
//   ##    ## ##     ## ##    ##
//    ######  ########   ###### 

/*
  op:      SBC
  ex:      25 SBC af -> fe and flags setting
  flags:   **---**
  NVBDIZC
  short:   subtract with carry
  form1:   A:=A-{adr}
  form2:   A <- (A) - M - ~C
  form3:
  *       IF (P.D)
  *         t = bcd(A) - bcd(M) - !P.C
  *         P.V = (t>99 OR t<0) ? 1:0
  *       ELSE
  *         t = A - M - !P.C
  *         P.V = (t>127 OR t<-128) ? 1:0
  *       P.C = (t>=0) ? 1:0
  *       P.N = t.7
  *       P.Z = (t==0) ? 1:0
  *       A = t & 0xFF
  *

  accumulator =
  a - b - !c  =
  a - b - !c + 256 =
  a - b - (1-c) + 256 =
  a + (255 - b) + c =
  a + !b + c
  accumulator := rega - regb = rega + NOT(regb) +carryflag
*/

void alu_op_SBC(char rega[], char regb[], char accumulator[], char flags[]){
  // your code here
}

//   #### ##    ##  ###### 
//    ##  ###   ## ##    ##
//    ##  ####  ## ##      
//    ##  ## ## ## ##      
//    ##  ##  #### ##      
//    ##  ##   ### ##    ##
//   #### ##    ##  ###### 

//  op:      INC
//  ex:      25 INC af -> fe and flags setting
//  flags:   *----*-
//  NVBDIZC
//  short:   increment
//  form1:   {adr}:={adr}+1
//  form2:   M <- (M) + 1
//  form3:
//  *       M = (M + 1) & $FF
//  *       P.N = M.7
//  *       P.Z = (M==0) ? 1:0
//  *
//
void alu_op_INC(char accu[], char regb[], char regc[], char flags[]){
  // your code here
}


//   ########  ########  ######  ######## ########
//   ##     ## ##       ##    ## ##          ##   
//   ##     ## ##       ##       ##          ##   
//   ########  ######    ######  ######      ##   
//   ##   ##   ##             ## ##          ##   
//   ##    ##  ##       ##    ## ##          ##   
//   ##     ## ########  ######  ########    ##   

void alu_op_RESET(char rega[], char regb[], char accumulator[], char flags[]){
  int i;
  /* clear rega, regb, accumulator, flags */
  for(i=0; i<REG_WIDTH; i++){
    rega[i] = '0';
    regb[i] = '0';
    accumulator[i] = '0';
    flags[i] = '0';
  }
}


//    ######  ##     ## ######## 
//   ##    ## ###   ### ##     ##
//   ##       #### #### ##     ##
//   ##       ## ### ## ######## 
//   ##       ##     ## ##       
//   ##    ## ##     ## ##       
//    ######  ##     ## ##       

/*
  op:      CMP
  ex:      25 CMP af -> fe and flags setting
  flags:   *----**
  NVBDIZC
  short:   compare (with accumulator)
  form1:   A-{adr}
  form2:   (A - M) -> NZC
  form3:
  *       t = A - M
  *       P.N = t.7
  *       P.C = (A>=M) ? 1:0
  *       P.Z = (t==0) ? 1:0
  *

  */
void alu_op_CMP(char rega[], char regb[], char accumulator[], char flags[]){
  char dummy[] ="00000000";
  char overflow = flags[OFLAG];
  int i;
  alu_op_SUB(rega, regb, dummy, flags);
  flags[OFLAG] = overflow;
  for(i=0;i<8;i++){
    if(regb[i]=='1'){ return;}
  }
 flags[CFLAG]='1';
}

/*

  Procedural approach to ALU with side-effect:
  Needed register are already alocated and may be modified
  mainly a switchboard
  
  alu_fct(int opcode, char reg_in_a[], char reg_in_b[], char reg_out_accu[], char flags[])

*/
void (*alu_table[17])(char [], char[], char[],char []) = { 
   alu_op_ADD,
   alu_op_ADC,
   alu_op_SUB,
   alu_op_SBC,
   alu_op_AND,
   alu_op_OR,
   alu_op_XOR,
   alu_op_NEG,
   alu_op_NOT,
   alu_op_ASL,
   alu_op_LSR,
   alu_op_ROL,
   alu_op_ROR,
   alu_op_CMP,
   alu_op_INC,
   alu_op_DEC,
  alu_op_RESET
};




  
void alu(unsigned int alu_opcode, char reg_in_a[], char reg_in_b[], char reg_out_accu[], char flags[]){
  char dummyflags[9] = "00000000";  
  mu_memory_reset();
  alu_table[alu_opcode](reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
}
