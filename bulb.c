/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bulb.h"

static char buffer[256];

char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 256, stdin);
  char* cpy = (char*) malloc(strlen(buffer) + 1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

void add_history(char* unused) {}

int main(int argc, char **argv) 
{
    char *program;
    bulbEnv *env = bulbNewEnv(NULL);
    bulbLoadModule(env, bulbSTD);
    if (argc == 1) {  
        printf("BULB v%s\n", BULB_VERSION);
        for (;;) {
            program = readline("> ");
            add_history(program);
            bulbObj *ast = bulbParse(program);
            bulbPrintAst(bulbEvalSequence(ast, env));
            printf("\n");
            free(program);
        }
    } else {
        if (strcmp(argv[1], "-h") == 0) {   
            printf("BULB v%s\n", BULB_VERSION);
            printf("usage: bulb [file]\tload specified file\n"
                   "   or: bulb\t        open the repl\n");
        } else {
            FILE *f = fopen(argv[1], "r");
            if(f == NULL)
            {
                printf("Exception: failed to open file.\n");
                exit(1);
            }
            fseek(f, 0L, SEEK_END);
            long size = ftell(f);
            fseek(f, 0L, SEEK_SET);
            program = (char*) malloc(size);
            size = fread(program, sizeof(char), size, f);
            bulbObj *ast = bulbParse(program);
            bulbEvalSequence(ast, env);
            printf("\n");
            fclose(f);
            free(program);
        }
    }
    return 0;
}
