/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */  
#include "bulb.h"

char* readline(char* prompt) {
    fputs(prompt, stdout);
    unsigned len = 256, i = 0, bc = 0;
    bool n = false, comment = false, string = false;
    char *buffer = (char*) malloc(len), c, prev = '\0';
    do {
        c = getc(stdin);
        if (comment || ((c == '\n' || c == ' ') && i == 0)) n = true;
        else if (!comment && c != '\n' && c != ' ' && n) n = false;
        if (c == '(') bc++;
        else if (c == ')') bc--;
        else if (c == '\b' && i > 0) i--;
        else if (c == '\"' && prev != '\\') string = !string;
        else if (!string && c == ';') comment = true;
        else if (comment && c == '\n') comment = false;
        buffer[i] = c;
        prev = c;
        i++;
        if (i >= len) {
            len *= 2;
            buffer = (char*) realloc(buffer, len);
        }
    } while (bc > 0 || c != '\n' || n || string || comment);
    buffer[i] = '\0';
    buffer = (char*) realloc(buffer, i + 1);
    return buffer;
}

void *bulbNewInstance(void *envp)
{
    bulbEnv *env = *((bulbEnv**) envp);
    for (;;) { 
        char *program = readline("> ");
        bulbObj *ast = bulbParse(program);
        bulbPrintAst(bulbEvalSequence(ast, env));
        printf("\n");
        free(program);
    }
    return NULL;
}

int main(int argc, char **argv) 
{
    char *program;

    /* initalizes the base types and internals*/
    BULB_NIL_TAG = bulbLoadType(BULB_NIL);
    BULB_BOOL_TAG = bulbLoadType(BULB_BOOL);
    BULB_INT_TAG = bulbLoadType(BULB_INT);
    BULB_FLOAT_TAG = bulbLoadType(BULB_FLOAT);
    BULB_SYMBOL_TAG = bulbLoadType(BULB_SYMBOL);
    BULB_STRING_TAG = bulbLoadType(BULB_STRING);
    BULB_PROCEDURE_TAG = bulbLoadType(BULB_PROCEDURE);
    BULB_PRIMITIVE_TAG = bulbLoadType(BULB_PRIMITIVE);
    BULB_CONS_TAG = bulbLoadType(BULB_CONS);
    bulbObjPool->type = BULB_CONS_TAG;
    bulbNil->type = BULB_NIL_TAG;
    bulbTrue->type = BULB_BOOL_TAG;
    bulbFalse->type = BULB_BOOL_TAG;
    bulbEnv *env = bulbNewEnv(NULL);
    
    /* loads the STD */
    bulbLoadModule(env, bulbSTD);
    
    if (argc == 1) {  
        printf("BULB v%s\n", BULB_VERSION);
        for (;;) {
            pthread_t instance;
            pthread_create(&instance, NULL, bulbNewInstance, &env);
            pthread_join(instance, NULL);
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
            program[size - 1] = '\0';
            bulbObj *ast = bulbParse(program);
            bulbEvalSequence(ast, env);
            printf("\n");
            fclose(f);
            free(program);
        }
    }
    return 0;
}
