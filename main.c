#include<stdio.h>
#include<stdlib.h>

#define HSH_RL_BUFSIZE 1024;

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
