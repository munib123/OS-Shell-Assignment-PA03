#include "shell.h"


int execute(char* arglist[], int background) {
    int i = 0;
    int pipe_index = -1;

    while (arglist[i] != NULL) {
        if (strcmp(arglist[i], "|") == 0) {
            pipe_index = i;
            break;
        }
        i++;
    }

    if (pipe_index != -1) {
        arglist[pipe_index] = NULL;
        char** left = arglist;
        char** right = &arglist[pipe_index + 1];

        int fds[2];
        if (pipe(fds) == -1) {
            perror("pipe failed");
            return -1;
        }

        int c1 = fork();
        if (c1 == -1) {
            perror("fork failed");
            return -1;
        }
        if (c1 == 0) {
            dup2(fds[1], STDOUT_FILENO);
            close(fds[0]);
            close(fds[1]);
            execvp(left[0], left);
            perror("Command not found");
            exit(1);
        }

        int c2 = fork();
        if (c2 == -1) {
            perror("fork failed");
            return -1;
        }
        if (c2 == 0) {
            dup2(fds[0], STDIN_FILENO);
            close(fds[1]);
            close(fds[0]);
            execvp(right[0], right);
            perror("Command not found");
            exit(1);
        }

        close(fds[0]);
        close(fds[1]);

        if (!background) {
            waitpid(c1, NULL, 0);
            waitpid(c2, NULL, 0);
        }

        return 0;
    }

    int in = -1, out = -1;

    for (i = 0; arglist[i] != NULL; i++) {
        if (strcmp(arglist[i], "<") == 0) {
            in = open(arglist[i + 1], O_RDONLY);
            if (in == -1) {
                perror("Input file error");
                return -1;
            }
            arglist[i] = NULL;
            break;
        }
        if (strcmp(arglist[i], ">") == 0) {
            out = open(arglist[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out == -1) {
                perror("Output file error");
                return -1;
            }
            arglist[i] = NULL;
            break;
        }
    }

    int pid = fork();
    if (pid == -1) {
        perror("fork failed");
        return -1;
    }
    if (pid == 0) {
        if (in != -1) {
            dup2(in, STDIN_FILENO);
            close(in);
        }
        if (out != -1) {
            dup2(out, STDOUT_FILENO);
            close(out);
        }
        execvp(arglist[0], arglist);
        perror("Command not found");
        exit(1);
    }

    if (!background) {
        waitpid(pid, NULL, 0);
    }
if (background) {
    bg_jobs[bg_count].pid = pid;
    strncpy(bg_jobs[bg_count].cmd, arglist[0], MAX_LEN);
    bg_count++;
}


    return 0;
}