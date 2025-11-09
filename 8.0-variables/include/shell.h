#ifndef SHELL_H
#define SHELL_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_LEN 512
#define MAXARGS 10
#define ARGLEN 30
#define PROMPT "PUCIT> "
#define HISTORY_SIZE 20
#define MAX_JOBS 50
typedef struct {
    int pid;             
    char cmd[MAX_LEN];   
} job_t;
typedef struct shell_var {
    char *name;
    char *value;
    struct shell_var *next;
} shell_var_t;
extern job_t bg_jobs[MAX_JOBS];
extern int bg_count;
extern char* history[HISTORY_SIZE];
extern int history_count;
extern shell_var_t *var_list;
void set_variable(const char *name, const char *value);
char* get_variable(const char *name);
void print_variables();
void expand_variables(char **args);
char* read_cmd(char* prompt, FILE* fp);
char** tokenize(char* cmdline);
int execute(char* arglist[], int background);
int handle_builtin(char** arglist);
void handle_if_structure();
#endif