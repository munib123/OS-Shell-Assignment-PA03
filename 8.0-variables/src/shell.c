#include "shell.h"
shell_var_t *var_list = NULL;


char* history[HISTORY_SIZE];
int history_count = 0;
job_t bg_jobs[MAX_JOBS];
int bg_count = 0;

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void set_variable(const char *name, const char *value) {
    shell_var_t *curr = var_list;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            free(curr->value);
            curr->value = strdup(value);
            return;
        }
        curr = curr->next;
    }
    shell_var_t *new_var = malloc(sizeof(shell_var_t));
    new_var->name = strdup(name);
    new_var->value = strdup(value);
    new_var->next = var_list;
    var_list = new_var;
}


char* get_variable(const char *name) {
    shell_var_t *curr = var_list;
    while (curr) {
        if (strcmp(curr->name, name) == 0) return curr->value;
        curr = curr->next;
    }
    return NULL;
}

void expand_variables(char **args) {
    for (int i = 0; args[i]; i++) {
        if (args[i][0] == '$') {
            char *val = get_variable(args[i] + 1); 
            if (val) {
                free(args[i]);
                args[i] = strdup(val);
            } else {
                free(args[i]);
                args[i] = strdup(""); 
            }
        }
    }
}


void print_variables() {
    shell_var_t *curr = var_list;
    while (curr) {
        printf("%s=%s\n", curr->name, curr->value);
        curr = curr->next;
    }
}


char* read_cmd(char* prompt, FILE* fp) {
    printf("%s", prompt);
    char* cmdline = (char*) malloc(sizeof(char) * MAX_LEN);
    int c, pos = 0;

    while ((c = getc(fp)) != EOF) {
        if (c == '\n') break;
        cmdline[pos++] = c;
    }

    if (c == EOF && pos == 0) {
        free(cmdline);
        return NULL; 
    }
    
    cmdline[pos] = '\0';
    return cmdline;
}


char** tokenize(char* cmdline) {
    char** arglist = malloc(sizeof(char*) * (MAXARGS + 1));
    int argnum = 0;
    char* p = cmdline;

    while (*p != '\0' && argnum < MAXARGS) {
        while (*p == ' ' || *p == '\t') p++;

        if (*p == '\0') break;

        if (*p == '<' || *p == '>' || *p == '|') {
            arglist[argnum] = malloc(2);
            arglist[argnum][0] = *p;
            arglist[argnum][1] = '\0';
            argnum++;
            p++;
            continue;
        }

        char* start = p;
        int len = 0;
        while (*p != '\0' && *p != ' ' && *p != '\t' && *p != '<' && *p != '>' && *p != '|') {
            p++; len++;
        }

        arglist[argnum] = malloc(len + 1);
        strncpy(arglist[argnum], start, len);
        arglist[argnum][len] = '\0';
        argnum++;
    }

    arglist[argnum] = NULL;
    return arglist;
}



int handle_builtin(char** arglist) {
    if (arglist[0] == NULL) return 0;

    // exit
    if (strcmp(arglist[0], "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    }

    // cd
    if (strcmp(arglist[0], "cd") == 0) {
        if (arglist[1] == NULL) {
            fprintf(stderr, "cd: missing argument\n");
        } else {
            if (chdir(arglist[1]) != 0) {
                perror("cd failed");
            }
        }
        return 1;
    }

    // help
    if (strcmp(arglist[0], "help") == 0) {
        printf("Built-in commands:\n");
        printf("  cd <directory>  : Change directory.\n");
        printf("  exit            : Exit the shell.\n");
        printf("  help            : Show help.\n");
        printf("  jobs            : Show job placeholder.\n");
        return 1;
    }

    // jobs
	if (strcmp(arglist[0], "jobs") == 0) {
	    for (int i = 0; i < bg_count; i++) {
	        printf("[%d] PID: %d  Command: %s\n", i+1, bg_jobs[i].pid, bg_jobs[i].cmd);
	    }
	    return 1;
	}


	// history
    if (strcmp(arglist[0], "history") == 0) {
        for (int i = 0; i < history_count; i++) {
        printf("%d  %s\n", i+1, history[i]);
    }
    return 1;
}

	if (strcmp(arglist[0], "set") == 0) {
	    print_variables();
	    return 1;
	}

    return 0; 
}


void handle_if_structure(char *first_line) {
    char *if_cmd = strdup(first_line + 3); 
    char line[256];

    char *then_block[50];
    char *else_block[50];
    int then_count = 0, else_count = 0;
    int in_else = 0;

    
    while (1) {
        printf("> ");  
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin))
            break;

        
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "then") == 0) continue;
        if (strcmp(line, "else") == 0) { in_else = 1; continue; }
        if (strcmp(line, "fi") == 0) break;

        if (!in_else)
            then_block[then_count++] = strdup(line);
        else
            else_block[else_count++] = strdup(line);
    }

    
    char **if_args = tokenize(if_cmd);

    int status;
    pid_t pid = fork();
    if (pid == 0) {
        execvp(if_args[0], if_args);
        exit(1);
    }
    waitpid(pid, &status, 0);

    int exit_code = WEXITSTATUS(status);

   
    char **chosen_block = (exit_code == 0) ? then_block : else_block;
    int chosen_count = (exit_code == 0) ? then_count : else_count;

    for (int i = 0; i < chosen_count; i++) {
      	char **args = tokenize(chosen_block[i]);
	execute(args, 0);
	for (int j = 0; args[j] != NULL; j++)
	    free(args[j]);
	free(args); 
    }
}