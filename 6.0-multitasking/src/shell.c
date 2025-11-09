#include "shell.h"

char* history[HISTORY_SIZE];
int history_count = 0;
job_t bg_jobs[MAX_JOBS];
int bg_count = 0;

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


    return 0; 
}