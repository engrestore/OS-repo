/*
 * mini_shell.c
 *
 * A small command-line process launcher (a "mini shell") that demonstrates
 * basic UNIX/Linux process management: fork(), execvp(), and waitpid().
 *
 * CECS 326 - PA#1
 * Edgar Restor
 * Version 1.0.0
 * 
 * Version change note:
 * 1.0.0: Initial code
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
 
#define MAX_LINE 1024   // max characters per input line
#define MAX_ARGS 64     // max whitespace-separated tokens

int main(void) { 
    char line[MAX_LINE];
    char *argv[MAX_ARGS];

    while (1) {
        printf("osh> ");
        fflush(stdout);
        // fgets keeps the trailing newline, so strip it
        if (fgets(line, sizeof(line), stdin) == NULL) {
            // EOF (e.g. Ctrl-D) — exit the shell loop cleanly
            printf("\n");
            break;
        }
     
        // Strip the trailing newline that fgets() keeps
        line[strcspn(line, "\n")] = '\0';
 
        // Tokenize on whitespace into argv[], NULL-terminated for execvp()
        int argc = 0;
        char *token = strtok(line, " \t");
        while (token != NULL && argc < MAX_ARGS - 1) {
            argv[argc++] = token;
            token = strtok(NULL, " \t");
        }
        argv[argc] = NULL;
 
        // Blank line (nothing tokenized) — just redisplay the prompt
        if (argc == 0) {
            continue;
        }
 
        // Built-in: exit — terminate without forking
        if (strcmp(argv[0], "exit") == 0) {
            break;
        }
 
        pid_t pid = fork();
 
        if (pid < 0) {
            // fork() failed
            perror("[parent] fork failed");
            continue;
        }
 
        if (pid == 0) {
            // child process
            execvp(argv[0], argv);
 
            // execvp() only returns if it failed
            fprintf(stderr, "[parent] %s: %s\n", argv[0], strerror(errno));
            _exit(errno == ENOENT ? 127 : 126);
        }
 
        // parent process
        printf("[parent] started child process %d\n", pid);
 
        int status;
        if (waitpid(pid, &status, 0) < 0) {
            perror("[parent] waitpid failed");
            continue;
        }
 
        if (WIFEXITED(status)) {
            printf("[parent] child %d exited with status %d\n", pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("[parent] child %d terminated by signal %d\n", pid, WTERMSIG(status));
        }
    }

    return 0;
}
