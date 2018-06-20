#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_NONSTDC_NO_DEPRECATE

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<math.h>
#include"IntermediateCodeGenerator.h"

int main()
{

	printf("enter file name\n");
	char *filename = (char *)malloc(100 * sizeof(char));
	scanf("%s", filename);

	generateIntermediate(filename);

	while (1)
	{
		printf("MENU\n\n");
		printf("1:display intermediate code\n");
		printf("2:display symbol table\n");
		printf("3:run intermediate\n");
		printf("4:create intermediate code\n");
		printf("5:enter your opcode file\n");
		printf("100:exit\n");
		int ch;
		FILE *fp;
		char *outputfile;
		scanf("%d", &ch);
		int i;
		switch (ch)
		{
		case 1:
			printf("\n\nintermediate table is\n");

			for (i = 1; i <= intermediatesize; i++)
			{
				printf("%d   %d", intermediatecode[i]->inNo, intermediatecode[i]->opcode);
				int j;
				for (j = 0; j < intermediatecode[i]->parametersize; j++)
				{
					printf("   %d ", intermediatecode[i]->parameters[j]);
				}
				printf("\n");
			}

			break;
		case 2:
			for (i = 0; i <= symboltablesize; i++)
				printf("%c  %d  %d\n", symboltables[i]->variable, symboltables[i]->address, symboltables[i]->size);
			break;
		case 3:

			analyseOutput();
			break;
		case 4:
			printf("enter filename to get intermediate language\n");
			outputfile = (char *)malloc(100 * sizeof(char));
			scanf("%s", outputfile);

			fp = fopen(outputfile, "w");

			for (i = 1; i <= intermediatesize; i++)
			{
				fprintf(fp, "%d %d", intermediatecode[i]->inNo, intermediatecode[i]->opcode);
				int j;
				for (j = 0; j < intermediatecode[i]->parametersize; j++)
				{
					fprintf(fp, " %d", intermediatecode[i]->parameters[j]);
				}
				fprintf(fp, "\n");
			}
			fclose(fp);
			break;
		case 5:
			analyseFile();
			break;
		case 100:
			return 0;


		}


	}
	


	
	

	return 0;
}