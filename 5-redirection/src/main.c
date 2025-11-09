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
    char** arglist;

    rl_bind_key('\t', rl_complete);

    while (1) {

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

        arglist = tokenize(cmdline);
        if (arglist != NULL) {
            if (!handle_builtin(arglist)) {
                execute(arglist); 
            }

            for (int i = 0; arglist[i] != NULL; i++) {
                free(arglist[i]);
            }
            free(arglist);
        }

        free(cmdline);
    }

    printf("\nShell exited.\n");
    return 0;
}