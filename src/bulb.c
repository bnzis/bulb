/*
 *  Bulb - the Lisp Interpreter
 *  Copyright (C) 2018-2019 bnzis (bonzisoft@protonmail.com)
 *  Copyright (C) 2012-2016, Yann Collet (xxhash)
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bulb.h"
#ifdef _WIN32

static char buffer[256];

/* Fake readline function */
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 256, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

/* Fake add_history function */
void add_history(char* unused) {}

#else
#include<readline/readline.h>
#include<readline/history.h>
#endif

int main(int argc, char **argv) 
{
    char *program;
    env_t *env = malloc(sizeof(env_t));
    env->upper_level = NULL;
    env->local = malloc(sizeof(hashmap_t));
    env->local->data = malloc(sizeof(obj_t*) * HMAP_ROWS);
    obj_t module = atom("core", strlen("core"));
    load_module(&module, env);
    if (argc == 1) {  
        printf("BULB v0.0.2\n");
        for (;;) {
            program = readline("> ");
            add_history(program);
            obj_t *tree = parse(program);
            print_ast(eval_sequence(tree, env));
            printf("\n");
            free(program);
        }
    } else {
        if (strcmp(argv[1], "-h") == 0) {   
            printf("BULB v0.0.2\n");
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
            program = malloc(size);
            size = fread(program, sizeof(char), size, f);
            obj_t *tree = parse(program);
            print_ast(eval_sequence(tree, env));
            printf("\n");
            fclose(f);
            free(program);
        }
    }
    return 0;
}
