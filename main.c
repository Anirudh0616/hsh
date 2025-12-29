#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#define HSH_RL_BUFSIZE 1024;

#define HSH_TOK_BUFSIZE 64
#define HSH_TOK_DELIM " \t\r\n\a"

char **hsh_split_line(char *line){
    int bufsize = HSH_TOK_BUFSIZE, pos = 0;
    char **tokens = malloc(sizeof(char*) * bufsize);
    char *token;

    if(!tokens){
        fprintf(stderr, "hsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, HSH_TOK_DELIM);
    while(token != NULL){
        tokens[pos++] = token;

        if(pos >= bufsize){
            bufsize += HSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if(!tokens){
                fprintf(stderr, "hsh: allocation error\n");
                exit(EXIT_FAILURE);
            }

        }
        token = strtok(NULL, HSH_TOK_DELIM);
        // NULL -> Continue tokenizing the same string, from where it left off
    }

    tokens[pos] = NULL;
    return tokens;
}


char *hsh_read_line(void){
    char *line = NULL;
    ssize_t bufsize = 0;
    // getline gets the bufsize for us
    
    if (getline(&line, &bufsize, stdin) == -1){
        if(feof(stdin)){
            exit(EXIT_SUCCESS);
        } else {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}


int hsh_execute(char ** args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0) {
        // child process
        if(execvp(args[0], args) == -1){
            perror("hsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0 ){
        // error forking 
        perror("hsh");
    } else {
        // Parent Process 
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

void hsh_loop(void){
    char* line;
    char ** args;
    int status = 1;

    do{
        printf("\n> ");
        line = hsh_read_line();
        args = hsh_split_line(line);
        status = hsh_execute(args)

        free(line);
        free(args);
    } while(status);
}

int main(int argc, char **argv){
    
    // Init the config files

    // Run the shell interactive loop 

    hsh_loop();

    // Perform Cleanup and terminate


    return 0;
}
