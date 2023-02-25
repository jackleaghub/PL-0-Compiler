/*
	vm.c

  Dennis Shelton, Agustin Rodriguez and Jack Lea

  This code functions as execution for assembly-like instructions created from two other files.  
  A stack and a registry are used to keep track of everything.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#define REG_FILE_SIZE 10
#define MAX_STACK_LENGTH 100

void print_execution(int line, char *opname, instruction IR, int PC, int BP, int SP, int *stack, int *RF)
{
  
	int i;
	// print out instruction and registers
	printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t\t", line, opname, IR.r, IR.l, IR.m, PC, SP, BP);
	
	// print register file
	for (i = 0; i < REG_FILE_SIZE; i++)
		printf("%d ", RF[i]);
	printf("\n");
	
	// print stack
	printf("stack:\t");
	for (i = MAX_STACK_LENGTH - 1; i >= SP; i--)
		printf("%d ", stack[i]);
	printf("\n");
}

// this is the function for going down l levels
int base(int L, int BP, int *stack)
{
	int ctr = L;
	int rtn = BP;
	while (ctr > 0)
	{
		rtn = stack[rtn];
		ctr--;
	}
	return rtn;
}

// this executes EVERYTHING. We will need to create the stack, free it,
// and call print_execution() whenever necessary.
void execute_program(instruction *code, int printFlag) 
{
	// Declare & initialize default values of Base Pointer, Stack Pointer,
  // Program Counter, and Instruction Register.
  int j;
  int BP = MAX_STACK_LENGTH - 1;
  int SP = BP + 1;
  int PC = 0;
  int line;
  instruction IR;
  char opcode[4];
  
  // Halt flag for case 11
  int halt = 0;
  
  int *RF = malloc(REG_FILE_SIZE * sizeof(int));
  for(int i = 0; i < REG_FILE_SIZE; i++)
    RF[i] = 0;
  
  int *stack = malloc(sizeof(int) * MAX_STACK_LENGTH);
  for(int i = 0; i < MAX_STACK_LENGTH; i++)
    stack[i] = 0; // good stack :)

  if (printFlag)
	{
		printf("\t\t\t\t\tPC\tSP\tBP\n");
		printf("Initial values:\t\t\t\t%d\t%d\t%d\n", PC, SP, BP);
	}

  while(halt < 1) {
    strcpy(opcode, "---");
    IR.m = code[PC].m;
    IR.l = code[PC].l;
    IR.r = code[PC].r;
    IR.opcode = code[PC].opcode;
    line = PC;
    PC++;
    
    switch(IR.opcode) {
      case 1: // LIT
        strcpy(opcode, "LIT");
        
        RF[IR.r] = IR.m; // literally exactly how it's written
        break;
      
      case 2: // RET
        strcpy(opcode, "RET");
        // Set up AR
        SP = BP + 1;
        PC = stack[BP - 2];
        BP = stack[BP - 1];
        
        break;

      // turns out 3 and 4 are just word for word from the HW 1 instructions
      case 3: // LOD
        strcpy(opcode, "LOD");
        
        j = base(IR.l, BP, stack) - RF[IR.m];
        if (j < 0 || j >= MAX_STACK_LENGTH) {
          printf("Virtual Machine Error: Out of Bounds Access Error\n");
          halt++;
        }
        else {
          RF[IR.r] = stack[j]; // changes are here in both 3 and 4 -D
        }
        break;
      
      case 4: // STO
        strcpy(opcode, "STO");
        
        j = base(IR.l, BP, stack) - RF[IR.m];
        if (j < 0 || j >= MAX_STACK_LENGTH) {
          printf("Virtual Machine Error: Out of Bounds Access Error\n");
          halt++;
        }
        else {
          stack[j] = RF[IR.r]; // possibly check inside of stack[]
        }
        break;
      
      case 5: // CAL
        strcpy(opcode, "CAL");
        // creates a new lexographical level and makes new AR & PC at m
        
        stack[SP - 1] = base(IR.l, BP, stack); // remember, stack goes from r to left
        stack[SP - 2] = BP;
        stack[SP - 3] = PC;
    
        BP = SP - 1; // Maybe double check this part later.
        PC = IR.m;
        break;

      // Fairly confident cases 6 - 23 + default are good as written.
      case 6: // INC
        strcpy(opcode, "INC");
        SP -= IR.m;
        if(SP < 0) {
          printf("Virtual Machine Error: Stack Overflow Error\n");
          halt++;
        }
        
        break;
      
      case 7: // JMP
        strcpy(opcode, "JMP");
        // go to instruction M
        PC = IR.m;
        break;
      
      case 8: // JPC
        strcpy(opcode, "JPC");
        
        if(RF[IR.r] == 0)
          PC = IR.m;
        break;
      
      case 9: // WRT
        strcpy(opcode, "WRT");
        
        printf("Write Value: %d\n", RF[IR.r]);
        break;
      
      case 10: // RED
        strcpy(opcode, "RED");
        printf("Please Enter a Value:\n");
        int temp;
        
        scanf("%d", &temp);
        RF[IR.r] = temp;
        break;
      
      case 11: // HAL
        strcpy(opcode, "HLT");
        
        halt++; // halt flag
        break;
      
      case 12: // NEG
        strcpy(opcode, "NEG");
    
        RF[IR.r] = -1 * RF[IR.r];
        break;
      
      // Cases 13 - 23 + default are likely good as written.
      case 13: // ADD
        strcpy(opcode, "ADD");
        
        RF[IR.r] = RF[IR.l] + RF[IR.m];
        break;
      
      case 14: // SUB
        strcpy(opcode, "SUB");
        
        RF[IR.r] = RF[IR.l] - RF[IR.m];
        break;
      
      case 15: // MUL
        strcpy(opcode, "MUL");
        
        RF[IR.r] = RF[IR.l] * RF[IR.m];
        break;
      
      case 16: // DIV
        strcpy(opcode, "DIV");
        
        RF[IR.r] = RF[IR.l] / RF[IR.m];
        break;
      
      case 17: // MOD
        strcpy(opcode, "MOD");
        
        RF[IR.r] = RF[IR.l] % RF[IR.m];
        break;
      
      case 18: // EQL
        strcpy(opcode, "EQL");
        
        if (RF[IR.l] == RF[IR.m])
          RF[IR.r] = 1;
        else
          RF[IR.r] = 0;
        break;
      
      case 19: // NEQ
        strcpy(opcode, "NEQ");
        
        if (RF[IR.l] == RF[IR.m])
          RF[IR.r] = 0;
        else
          RF[IR.r] = 1;
        break;
      
      case 20: // LSS
        strcpy(opcode, "LSS");
        
        if (RF[IR.l] < RF[IR.m])
          RF[IR.r] = 1;
        else
          RF[IR.r] = 0;
        break;
      
      case 21: // LEQ
        strcpy(opcode, "LEQ");
        
        if (RF[IR.l] <= RF[IR.m])
          RF[IR.r] = 1;
        else
          RF[IR.r] = 0;
        break;
      
      case 22: // GTR
        strcpy(opcode, "GTR");
        
        if (RF[IR.l] > RF[IR.m])
          RF[IR.r] = 1;
        else
          RF[IR.r] = 0;
        break;
      
      case 23: // GEQ
        strcpy(opcode, "GEQ");
        
        if (RF[IR.l] >= RF[IR.m])
          RF[IR.r] = 1;
        else
          RF[IR.r] = 0;
        break;
      
      default:
        printf("ERROR: out of bounds OPCODE. Halting...\n"); // is it an out of bounds error if we get a bad opcode?
        halt++;
    }
    if(printFlag) {
        print_execution(line, opcode, IR, PC, BP, SP, stack, RF);
    }
  }
  free(RF);
  free(stack);
}