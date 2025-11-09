#include "shell.h"
#include <readline/readline.h>
#include <readline/history.h>


extern char* history[HISTORY_SIZE];
extern int history_count;

void add_to_history(char* cmd) {
    if (history_count < HISTORY_SIZE) {
        history[history_count] = strdup(cmd);
        history_count++;
    } else {
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++) {
            history[i-1] = history[i];
        }
        history[HISTORY_SIZE-1] = strdup(cmd);
    }
}

int main() {
    char* cmdline;

    rl_bind_key('\t', rl_complete);

    while (1) {

	int status;
    pid_t finished;
    for (int i = 0; i < bg_count; ) {
        finished = waitpid(bg_jobs[i].pid, &status, WNOHANG);
        if (finished > 0) {
            for (int j = i; j < bg_count - 1; j++) {
                bg_jobs[j] = bg_jobs[j + 1];
            }
            bg_count--;
        } else {
            i++;
        }
    }

        cmdline = readline(PROMPT);
        if (cmdline == NULL) { 
            printf("\nExiting shell...\n");
            break;
        }

        if (strlen(cmdline) == 0) { 
            free(cmdline);
            continue;
        }


        if (cmdline[0] == '!') {
            int index = atoi(cmdline + 1);
            if (index > 0 && index <= history_count) {
                free(cmdline);
                cmdline = strdup(history[index - 1]);
                printf("%s%s\n", PROMPT, cmdline);
            } else {
                printf("Invalid history reference.\n");
                free(cmdline);
                continue;
            }
        }

        add_history(cmdline);

        add_to_history(cmdline);

        char* commands[MAXARGS];
int n_cmds = 0;
char* token = strtok(cmdline, ";");
while (token != NULL && n_cmds < MAXARGS) {
    commands[n_cmds++] = token;
    token = strtok(NULL, ";");
}

for (int j = 0; j < n_cmds; j++) {
    char* cmd = commands[j];
    while (*cmd == ' ' || *cmd == '\t') cmd++;
    if (*cmd == '\0') continue;
	int background = 0;
	int len = strlen(cmd);
	if (len > 0 && cmd[len - 1] == '&') {
	    background = 1;
	    cmd[len - 1] = '\0'; 
	    while (len > 1 && (cmd[len - 2] == ' ' || cmd[len - 2] == '\t')) {
	        cmd[len - 2] = '\0';
	        len--;
	    }
	}
	

    char** arglist = tokenize(cmd);
    if (arglist != NULL) {
        if (!handle_builtin(arglist)) {
            execute(arglist, background);
        }

        for (int i = 0; arglist[i] != NULL; i++) {
            free(arglist[i]);
        }
        free(arglist);
    }
}


        free(cmdline);
    }

    printf("\nShell exited.\n");
    return 0;
}