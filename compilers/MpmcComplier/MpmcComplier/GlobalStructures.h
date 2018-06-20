
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_NONSTDC_NO_DEPRECATE

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<math.h>
#include<string.h>



void push(int *a, int *size, int val)
{
	int t = *size;
	a[++t] = val;
	*size = t;
}
int pop(int *a, int *size)
{
	int t = *size;
	int result = a[t--];
	*size = t;
	return result;
}
int *stack;
int stacktop = -1;


struct opCode
{
	char *instruction;
	int opcode;
}**opcodes;
int opcodesize = -1;

struct registerCodes
{
	char *registername;
	int index;
	int value;
}**registercodes;
int registercodesize = -1;

struct symbolTable
{
	char variable;
	int address;
	int size;
	int defaults = -1;
}**symboltables;
int symboltablesize=-1;

struct labelTable
{
	char* blockname;
	int address;
}**labeltables;
int labeltablesize = -1;


struct intermediate
{
	int inNo;
	int opcode;
	int *parameters;
	int parametersize;
}**intermediatecode;
int intermediatesize = -1;




void initializeIntermediate()
{
	intermediatecode = (struct intermediate**)malloc(50 * sizeof(struct intermediate *));
	intermediatesize++;
}

void initializeLabelTable()
{
	labeltables = (struct labelTable**)malloc(20* sizeof(struct labelTable *));

	int i;
	for (i = 0; i < 20; i++)
	{
		labeltables[i] = (struct labelTable*)malloc(20 * sizeof(struct labelTable ));
		labeltables[i]->blockname = (char *)malloc(20 * sizeof(char));

	}
}

void initializeSymbolTable()
{
	
		symboltables = (struct symbolTable **)malloc(20 * sizeof(struct symbolTable*));
		symboltablesize = -1;
	
}

void initializeOpCodes()
{
	opcodes = (struct opCode **)malloc(20 * sizeof(struct opCode*));

	int i;
	for (i = 1; i < 20; i++)
	{
		opcodes[i] = (struct opCode *)malloc(20 * sizeof(struct opCode));
		opcodes[i]->instruction = (char *)malloc(100 * sizeof(char));
	}

	strcpy(opcodes[1]->instruction, "MOV");
	opcodes[1]->opcode = 1;
	strcpy(opcodes[2]->instruction, "MOV");
	opcodes[2]->opcode = 2;
	strcpy(opcodes[3]->instruction, "ADD");
	opcodes[3]->opcode = 3;
	strcpy(opcodes[4]->instruction, "SUB");
	opcodes[4]->opcode = 4;
	strcpy(opcodes[5]->instruction, "MUL");
	opcodes[5]->opcode = 5;
	strcpy(opcodes[6]->instruction, "JMP");
	opcodes[6]->opcode = 6;
	strcpy(opcodes[7]->instruction, "IF");
	opcodes[7]->opcode = 7;
	strcpy(opcodes[8]->instruction, "EQ");
	opcodes[8]->opcode = 8;
	strcpy(opcodes[9]->instruction, "LT");
	opcodes[9]->opcode = 9;
	strcpy(opcodes[10]->instruction, "GT");
	opcodes[10]->opcode = 10;
	strcpy(opcodes[11]->instruction, "LTEQ");
	opcodes[11]->opcode = 11;
	strcpy(opcodes[12]->instruction, "GTEQ");
	opcodes[12]->opcode = 12;
	strcpy(opcodes[13]->instruction, "PRINT");
	opcodes[13]->opcode = 13;
	strcpy(opcodes[14]->instruction, "READ");
	opcodes[14]->opcode = 14;
	strcpy(opcodes[15]->instruction, "ENDIF");
	opcodes[15]->opcode = 15;

	opcodesize = 15;
}

void initializeRegister()
{
	registercodes = (struct registerCodes **)malloc(20 * sizeof(struct registerCodes *));

	int i;
	for (i = 0; i < 8; i++)
	{
		registercodes[i] = (struct registerCodes *)malloc(20 * sizeof(struct registerCodes));
		registercodes[i]->registername = (char *)malloc(100 * sizeof(char));
	}
	registercodes[0]->index = 0;
	strcpy(registercodes[0]->registername, "AX");
	registercodes[1]->index = 1;
	strcpy(registercodes[1]->registername, "BX");
	registercodes[2]->index = 2;
	strcpy(registercodes[2]->registername, "CX");
	registercodes[3]->index = 3;
	strcpy(registercodes[3]->registername, "DX");
	registercodes[4]->index = 4;
	strcpy(registercodes[4]->registername, "EX");
	registercodes[5]->index = 5;
	strcpy(registercodes[5]->registername, "FX");
	registercodes[6]->index = 6;
	strcpy(registercodes[6]->registername, "GX");
	registercodes[7]->index = 0;
	strcpy(registercodes[7]->registername, "HX");

	registercodesize = 7;
}

int getOpcode(char *x)
{
	if (!strcmp(x, "EQ"))
		return 8;
	if (!strcmp(x, "LT"))
		return 9;
	if (!strcmp(x, "GT"))
		return 10;
	if (!strcmp(x, "LTEQ"))
		return 11;
	if (!strcmp(x, "GTEQ"))
		return 12;
}

void addToSymbolTable(char v, int size,int value)
{
	++symboltablesize;
	symboltables[symboltablesize] = (struct symbolTable *)malloc(20 * sizeof(struct symbolTable));
	int previoussize = 1;
	int address = 0;

	symboltables[symboltablesize]->defaults = -1;
	if (symboltablesize > 0)
	{
		previoussize = symboltables[symboltablesize - 1]->size;
		address = symboltables[symboltablesize - 1]->address;
		if (symboltables[symboltablesize - 1]->size == 0)
			previoussize++;

		symboltables[symboltablesize]->variable = v;
		symboltables[symboltablesize]->address = address + previoussize;
		symboltables[symboltablesize]->size = size;
	}
	else
	{
		symboltables[symboltablesize]->variable = v;
		symboltables[symboltablesize]->address = address;
		symboltables[symboltablesize]->size = size;
	}
	if (size == 0)
	{
		symboltables[symboltablesize]->defaults = value;
	}
	
}


int getValueInRegister(int x)
{
	return registercodes[x]->value;
}
int getAddressOfMemory(char c)
{
	int i = 0;
	for (i = 0; i <= symboltablesize; i++)
	{
		if (symboltables[i]->variable == c)
			return symboltables[i]->address;
	}
	return 0;
}


int getAddressFromSymbol(char v)
{
	int i = 0;
	for (i = 0; i <= symboltablesize; i++)
	{
		if (!strcmp(symboltables[i]->variable+"",v+""))
			return symboltables[i]->address;
	}

	return 1000;
}

int getCommandIndex(char *t)
{
	int i;
	for (i = 1; i <= registercodesize; i++)
	{
		if (!(strcmp(t, registercodes[i]->registername)))
			return i;
	}
	return 0;
}

void updateReadCode(char *token2)
{
	//printf("%s  %d", token2, strlen(token2));
	++intermediatesize;
	intermediatecode[intermediatesize] = (struct intermediate *)malloc(sizeof(struct intermediate));
	intermediatecode[intermediatesize]->inNo = intermediatesize;
	intermediatecode[intermediatesize]->opcode = 14;
	intermediatecode[intermediatesize]->parametersize = 1;
	
	intermediatecode[intermediatesize]->parameters = (int *)malloc(sizeof(int));
	intermediatecode[intermediatesize]->parameters[0] =getCommandIndex(token2) ;
	//printf("  %d   ", intermediatecode[intermediatesize]->parameters[0]);
	

}

void updatePrintCode(char *token2)
{
	++intermediatesize;
	intermediatecode[intermediatesize] = (struct intermediate *)malloc(sizeof(struct intermediate));
	intermediatecode[intermediatesize]->inNo = intermediatesize;
	intermediatecode[intermediatesize]->opcode = 13;
	intermediatecode[intermediatesize]->parametersize = 2;

	intermediatecode[intermediatesize]->parameters = (int *)malloc(sizeof(int));
	if (strlen(token2) == 2)
	{
		intermediatecode[intermediatesize]->parameters[0] = getCommandIndex(token2);
		intermediatecode[intermediatesize]->parameters[1] = 0;
	}
	else
	{
		if (strlen(token2) != 1)
		{
			intermediatecode[intermediatesize]->parameters[1] = 1;
			intermediatecode[intermediatesize]->parameters[0] = getAddressOfMemory(token2[0]) + (token2[2] - '0');
			//printf("%d\n\n", getAddressOfMemory(token2[0]) + (token2[2] - '0'));
		}
		else
		{
			intermediatecode[intermediatesize]->parameters[1] = 1;
			printf("%d", getAddressOfMemory(token2[0]));
			intermediatecode[intermediatesize]->parameters[0] = getAddressOfMemory(token2[0]) ;
		}

	}
	
}

void updateMoveFirstM(char *token2, char *token3)
{
	++intermediatesize;
	intermediatecode[intermediatesize] = (struct intermediate *)malloc(sizeof(struct intermediate));
	intermediatecode[intermediatesize]->inNo = intermediatesize;
	intermediatecode[intermediatesize]->opcode = 1;
	intermediatecode[intermediatesize]->parametersize = 2;

	intermediatecode[intermediatesize]->parameters = (int *)malloc(sizeof(int));

	if (strlen(token2) == 1)
	{
		intermediatecode[intermediatesize]->parameters[0] = getAddressFromSymbol(token2[0]);
		
	}
	else
	{
		//printf("   %d    ", getAddressFromSymbol(token2[0]) + token2[2]-'0');
	
		intermediatecode[intermediatesize]->parameters[0] = getAddressFromSymbol(token2[0])+(token2[2]-'0');

		//printf("    %d   ", getAddressFromSymbol(token2[0]) + token2[2] - '0');
	
	}

	intermediatecode[intermediatesize]->parameters[1] = getCommandIndex(token3);
}

void updateMoveFirstR(char *token2, char *token3)
{
	++intermediatesize;
	intermediatecode[intermediatesize] = (struct intermediate *)malloc(sizeof(struct intermediate));
	intermediatecode[intermediatesize]->inNo = intermediatesize;
	intermediatecode[intermediatesize]->opcode = 2;
	intermediatecode[intermediatesize]->parametersize = 2;

	intermediatecode[intermediatesize]->parameters = (int *)malloc(sizeof(int));
	intermediatecode[intermediatesize]->parameters[0] = getCommandIndex(token2);
	
	if (strlen(token3) == 1)
	{
		intermediatecode[intermediatesize]->parameters[1] = getAddressFromSymbol(token3[0]);
		
	}
	else
	{
		intermediatecode[intermediatesize]->parameters[1] = getAddressFromSymbol(token3[0]) +(token3[2]-'0');
		
	}
	
}

void updateAddCode(char *token2, char *token3, char *token4)
{
	++intermediatesize;
	intermediatecode[intermediatesize] = (struct intermediate *)malloc(sizeof(struct intermediate));
	intermediatecode[intermediatesize]->inNo = intermediatesize;
	intermediatecode[intermediatesize]->opcode = 3;
	intermediatecode[intermediatesize]->parametersize = 3;

	intermediatecode[intermediatesize]->parameters = (int *)malloc(sizeof(int));
	intermediatecode[intermediatesize]->parameters[0] = getCommandIndex(token2);
	intermediatecode[intermediatesize]->parameters[1] = getCommandIndex(token3);
	intermediatecode[intermediatesize]->parameters[2] = getCommandIndex(token4);
}

void updateSubCode(char *token2, char *token3, char *token4)
{
	++intermediatesize;
	intermediatecode[intermediatesize] = (struct intermediate *)malloc(sizeof(struct intermediate));
	intermediatecode[intermediatesize]->inNo = intermediatesize;
	intermediatecode[intermediatesize]->opcode = 4;
	intermediatecode[intermediatesize]->parametersize = 3;

	intermediatecode[intermediatesize]->parameters = (int *)malloc(sizeof(int));
	intermediatecode[intermediatesize]->parameters[0] = getCommandIndex(token2);
	intermediatecode[intermediatesize]->parameters[1] = getCommandIndex(token3);
	intermediatecode[intermediatesize]->parameters[2] = getCommandIndex(token4);
}

void updateMulCode(char *token2, char *token3, char *token4)
{
	++intermediatesize;
	intermediatecode[intermediatesize] = (struct intermediate *)malloc(sizeof(struct intermediate));
	intermediatecode[intermediatesize]->inNo = intermediatesize;
	intermediatecode[intermediatesize]->opcode = 5;
	intermediatecode[intermediatesize]->parametersize = 3;

	intermediatecode[intermediatesize]->parameters = (int *)malloc(sizeof(int));
	intermediatecode[intermediatesize]->parameters[0] = getCommandIndex(token2);
	intermediatecode[intermediatesize]->parameters[1] = getCommandIndex(token3);
	intermediatecode[intermediatesize]->parameters[2] = getCommandIndex(token4);
}


void updateLabel(char *token)
{
	++labeltablesize;
	strcpy(labeltables[labeltablesize]->blockname, token);
	labeltables[labeltablesize]->address = intermediatesize + 1;
}

int getAddressStack(char *token)
{
	int i = labeltablesize;
	for (i = labeltablesize; i >= 0; i--)
	{
		if (!strcmp(labeltables[i]->blockname, token))
			return labeltables[i]->address;
	}
	return 0;

}

void updateJmpCode(char *token)
{
	++intermediatesize;
	intermediatecode[intermediatesize] = (struct intermediate *)malloc(sizeof(struct intermediate));
	intermediatecode[intermediatesize]->inNo = intermediatesize;
	intermediatecode[intermediatesize]->opcode = 6;
	intermediatecode[intermediatesize]->parametersize = 1;

	intermediatecode[intermediatesize]->parameters = (int *)malloc(sizeof(int));
	intermediatecode[intermediatesize]->parameters[0] = getAddressStack(token);
	
}

void updateifCode(char *token2, char *token3, char *token4, char *token5)
{
	++intermediatesize;
	intermediatecode[intermediatesize] = (struct intermediate *)malloc(sizeof(struct intermediate));
	intermediatecode[intermediatesize]->inNo = intermediatesize;
	intermediatecode[intermediatesize]->opcode = 7;
	intermediatecode[intermediatesize]->parametersize = 4;

	intermediatecode[intermediatesize]->parameters = (int *)malloc(sizeof(int));
	intermediatecode[intermediatesize]->parameters[0] = getCommandIndex(token2);
	intermediatecode[intermediatesize]->parameters[1] = getOpcode(token3);
	intermediatecode[intermediatesize]->parameters[2] = getCommandIndex(token4);
	intermediatecode[intermediatesize]->parameters[3] = -1000;

	push(stack, &stacktop, intermediatesize);
}


void updateelseCode()
{
	++intermediatesize;
	intermediatecode[intermediatesize] = (struct intermediate *)malloc(sizeof(struct intermediate));
	int x = pop(stack, &stacktop);
	intermediatecode[x]->parameters[3] = intermediatesize;

	++labeltablesize;
	strcpy(labeltables[labeltablesize]->blockname, "ELSE");
	labeltables[labeltablesize]->address = intermediatesize;

	intermediatecode[intermediatesize]->inNo = intermediatesize;
	intermediatecode[intermediatesize]->opcode = 6;
	intermediatecode[intermediatesize]->parametersize = 1;

	intermediatecode[intermediatesize]->parameters = (int *)malloc(sizeof(int));
	intermediatecode[intermediatesize]->parameters[0] = -1000;
	
	push(stack, &stacktop, intermediatesize);
}
void updateendifCode()
{
	++intermediatesize;
	intermediatecode[intermediatesize] = (struct intermediate *)malloc(sizeof(struct intermediate));
	intermediatecode[intermediatesize]->inNo = intermediatesize;
	intermediatecode[intermediatesize]->opcode = 0;
	intermediatecode[intermediatesize]->parametersize = 0;



	int i=0;
	for (i = stacktop; i >= 0; i--)
		printf("%d", stack[i]);
	int x = pop(stack, &stacktop);
	printf("%d ", x);


	intermediatecode[x]->parameters[0] = intermediatesize;
}