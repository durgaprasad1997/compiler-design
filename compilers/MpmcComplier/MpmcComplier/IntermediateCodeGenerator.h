
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_NONSTDC_NO_DEPRECATE

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<math.h>
#include<string.h>
#include"GlobalStructures.h"



char* seperator(char** stringp, const char* delim)
{
	char* start = *stringp;
	char* p;

	p = (start != NULL) ? strpbrk(start, delim) : NULL;

	if (p == NULL)
	{
		*stringp = NULL;
	}
	else
	{
		*p = '\0';
		*stringp = p + 1;
	}

	return start;
}


void segregateOnCommand(char *token,char *line)
{
	if (!strcmp(token, "JMP"))
	{
		char *token1 = seperator(&line, " ");
		char *token2 = seperator(&line, " ");
		updateJmpCode(token2);
	}
	else if (!strcmp(token, "PRINT"))
	{
		char *token1 = seperator(&line, " ");
		char *token2 = seperator(&line, " ");
		updatePrintCode(token2);
	}
	else if (!strcmp(token, "READ"))
	{
		char *token1 = seperator(&line, " ");
		char *token2 = seperator(&line, " ");
		
		updateReadCode(token2);
	}
	else if (!strcmp(token, "DATA"))
	{
		//printf("%s  %d", line, strlen(line));
		char *token = seperator(&line, " ");
		char *token2 = seperator(&line, " ");
	

		if (strlen(token2) == 1)
		{
			addToSymbolTable(token2[0],1,-1);
		}
		else
		{
			addToSymbolTable(token2[0],token2[2]-'0',-1);
		}
	}
	else if (!strcmp(token, "MOV"))
	{
		char *token1 = seperator(&line, " ");
		char *token2 = seperator(&line, " ");
		char *token3 = seperator(&line, " ");

		if (strlen(token2) == 1||(token2[1]=='['))
		{
			updateMoveFirstM(token2, token3);
		}
		else
		{
			updateMoveFirstR(token2, token3);
		}


	}
	else if (!strcmp(token, "CONST"))
	{
		char *token1 = seperator(&line, " ");
		char *token2 = seperator(&line, " ");
		char *token3= seperator(&line, " ");
		char *token4 = seperator(&line, " ");
		addToSymbolTable(token2[0], 0,token4[0]-'0');
	}
	else if (!strcmp(token, "ADD"))
	{
		char *token1 = seperator(&line, " ");
		char *token2 = seperator(&line, " ");
		char *token3 = seperator(&line, " ");
		char *token4 = seperator(&line, " ");

		updateAddCode(token2, token3, token4);
		
	}
	else if (!strcmp(token, "SUB"))
	{
		char *token1 = seperator(&line, " ");
		char *token2 = seperator(&line, " ");
		char *token3 = seperator(&line, " ");
		char *token4 = seperator(&line, " ");
		updateSubCode(token2, token3, token4);
	}
	else if (!strcmp(token, "MUL"))
	{
		char *token1 = seperator(&line, " ");
		char *token2 = seperator(&line, " ");
		char *token3 = seperator(&line, " ");
		char *token4 = seperator(&line, " ");

		updateMulCode(token2, token3, token4);
	}
	else if (!strcmp(token, "IF"))
	{
		char *token1 = seperator(&line, " ");
		char *token2 = seperator(&line, " ");
		char *token3 = seperator(&line, " ");
		char *token4 = seperator(&line, " ");
		char *token5 = seperator(&line, " ");
		
		updateifCode(token2, token3, token4, token5);
	}
	else if (!strcmp(token, "ELSE"))
	{
		
		updateelseCode();
	}
	else if (!strcmp(token, "ENDIF"))
	{
		

		updateendifCode();
	}
	else if (token[strlen(token) - 1]==':'&&strcmp(token,"START:"))
	{
		token[strlen(token) - 1] = '\0';
		updateLabel(token);
	}
	
}

void generateIntermediate(char *filename)
{
	//init all strutures
	initializeIntermediate();
	initializeLabelTable();
	initializeSymbolTable();
	initializeOpCodes();
	initializeRegister();
	stack = (int *)malloc(100 * sizeof(int));


	char *line = (char *)malloc(100 * sizeof(char));
	FILE *fp = fopen(filename, "r");

	while (fgets(line, 1000, fp) != NULL)
	//while ((fscanf(fp, "%[^\n]s",line))!=-1)
	{
		line[strlen(line)-1] = '\0';
		//printf("%s", line);

		char *temp = (char *)malloc(100 * sizeof(char));
		char *fullline = (char *)malloc(100 * sizeof(char));
		strcpy(temp, line);
		strcpy(fullline, line);
		char *token = seperator(&temp, " ");

		segregateOnCommand(token,fullline);

	}


}

void analyseOutput()
{
	int * memory = (int *)malloc(100*sizeof(int));

	int i;
	for (i = 0; i <= symboltablesize; i++)
	{
		if (symboltables[i]->defaults != -1)
		{
			memory[symboltables[i]->address] = symboltables[i]->defaults;
			
		}
	}

	for (i = 1; i <= intermediatesize; i++)
	{
		
		if (intermediatecode[i]->opcode == 1)
		{

			memory[intermediatecode[i]->parameters[0]] = registercodes[intermediatecode[i]->parameters[1]]->value;
			
		}
		else if (intermediatecode[i]->opcode == 2)
		{
			registercodes[intermediatecode[i]->parameters[0]]->value = memory[intermediatecode[i]->parameters[1]];
		}
		else if (intermediatecode[i]->opcode == 3)
		{
			registercodes[intermediatecode[i]->parameters[0]]->value = getValueInRegister(intermediatecode[i]->parameters[1]) + getValueInRegister(intermediatecode[i]->parameters[2]);

			
		}
		else if (intermediatecode[i]->opcode == 4)
		{
			registercodes[intermediatecode[i]->parameters[0]]->value = getValueInRegister(intermediatecode[i]->parameters[1]) -getValueInRegister(intermediatecode[i]->parameters[2]);

			
		}
		else if (intermediatecode[i]->opcode == 5)
		{
			registercodes[intermediatecode[i]->parameters[0]]->value = getValueInRegister(intermediatecode[i]->parameters[1])*  getValueInRegister(intermediatecode[i]->parameters[2]);


		}
		else if (intermediatecode[i]->opcode == 6)
		{
			i = intermediatecode[i]->parameters[0]-1;
		}
		else if (intermediatecode[i]->opcode == 7)
		{

			int x1 = getValueInRegister(intermediatecode[i]->parameters[0]);
			int x2 = getValueInRegister(intermediatecode[i]->parameters[2]);

			int c = (intermediatecode[i]->parameters[1]);
			int status = 0;
			if (c == 8)
			{
				if (x1 == x2)
					status = 1;
			}
			else if (c == 9)
			{
				if (x1 < x2)
					status = 1;
			}
			else if (c == 10)
			{
				if (x1 > x2)
					status = 1;
			}
			else if (c == 11)
			{
				if (x1<= x2)
					status = 1;
			}
			else if (c >= 12)
			{
				if (x1 == x2)
					status = 1;
			}
			
			if (status == 0)
			{
				i = (intermediatecode[i]->parameters[3]);
			}

		}
		
		else if (intermediatecode[i]->opcode == 13)
		{
			
			int x = intermediatecode[i]->parameters[0];
			int y = intermediatecode[i]->parameters[1];
			int j = 0;
			if (y == 0)
			{
				for (j = 0; j < registercodesize; j++)
				{

					if (x == registercodes[j]->index)
					{
						printf("%d\n", registercodes[j]->value);
						break;
					}
				}
			}
			else if (y == 1)
			{
				
				printf("%d\n", memory[x]);
			}
			
		}
		else if (intermediatecode[i]->opcode == 14)
		{
			
			int x = intermediatecode[i]->parameters[0];
			int y=0;
			printf("enter operand\n");
			scanf("%d", &y);
		int j = 0;
			for ( j= 0; j < registercodesize; j++)
			{
				if (x == registercodes[j]->index)
				{
					
					registercodes[j]->value = y;
					break;
				}
			}
		}
		else if (intermediatecode[i]->opcode == 15)
		{

		}
	}
}

void analyseFile()
{
	char *filename = (char *)malloc(100 * sizeof(char));
	scanf("%s", filename);
	FILE *fp = fopen(filename, "r");
	char *c = (char *)malloc(100 * sizeof(char));
	intermediatesize = 1;


	while (fgets(c, 1000, fp) != NULL)
	{
		intermediatecode[intermediatesize] = (struct intermediate *)malloc(sizeof(struct intermediate));
		intermediatecode[intermediatesize]->parameters = (int *)malloc(10 * sizeof(int));
		char *temp = (char *)malloc(100 * sizeof(char));
		strcpy(temp, c);

		int y = atoi(seperator(&temp, " "));
		int x = atoi(seperator(&temp, " "));
		if (x==1)
		{
			intermediatecode[intermediatesize]->opcode = x;
			intermediatecode[intermediatesize]->parameters[0] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parameters[1] = atoi(seperator(&temp, " "));
			
			intermediatecode[intermediatesize]->parametersize = 2;
			intermediatecode[intermediatesize]->inNo = intermediatesize;
		}
		else if (x == 2)
		{
			intermediatecode[intermediatesize]->opcode = x;
			intermediatecode[intermediatesize]->parameters[0] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parameters[1] = atoi(seperator(&temp, " "));
			
			intermediatecode[intermediatesize]->parametersize = 2;
			intermediatecode[intermediatesize]->inNo = intermediatesize;
		}
		else if (x == 3)
		{
			intermediatecode[intermediatesize]->opcode = x;
			intermediatecode[intermediatesize]->parameters[0] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parameters[1] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parameters[2] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parametersize = 3;
			intermediatecode[intermediatesize]->inNo = intermediatesize;
		}
		else if (x == 4)
		{
			intermediatecode[intermediatesize]->opcode = x;
			intermediatecode[intermediatesize]->parameters[0] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parameters[1] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parameters[2] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parametersize = 3;
			intermediatecode[intermediatesize]->inNo = intermediatesize;
		}
		else if (x == 5)
		{
			intermediatecode[intermediatesize]->opcode = x;
			intermediatecode[intermediatesize]->parameters[0] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parameters[1] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parameters[2] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parametersize = 3;
			intermediatecode[intermediatesize]->inNo = intermediatesize;
		}
		else if (x == 6)
		{
			intermediatecode[intermediatesize]->opcode = x;
			intermediatecode[intermediatesize]->parameters[0] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parametersize = 1;
			intermediatecode[intermediatesize]->inNo = intermediatesize;
		}
		else if (x == 7)
		{
			intermediatecode[intermediatesize]->opcode = x;
			intermediatecode[intermediatesize]->parameters[0] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parameters[1] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parameters[2] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parameters[3] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parametersize = 4;
			intermediatecode[intermediatesize]->inNo = intermediatesize;
		}
		
		else if (x == 13)
		{
			intermediatecode[intermediatesize]->opcode = x;
			intermediatecode[intermediatesize]->parameters[0] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parameters[1] = atoi(seperator(&temp, " "));

			intermediatecode[intermediatesize]->parametersize = 2;
			intermediatecode[intermediatesize]->inNo = intermediatesize;
		}
		else if (x == 14)
		{
			intermediatecode[intermediatesize]->opcode = x;
			intermediatecode[intermediatesize]->parameters[0] = atoi(seperator(&temp, " "));
			intermediatecode[intermediatesize]->parametersize = 1;
			intermediatecode[intermediatesize]->inNo = intermediatesize;
		}
		else if (x == 15)
		{

		}

		intermediatesize++;
		
	}
	intermediatesize--;
	analyseOutput();


}