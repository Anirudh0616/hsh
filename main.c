#include<stdio.h>
#include<stdlib.h>

#define HSH_RL_BUFSIZE 1024;

char *hsh_read_line(void){
    int bufsize = HSH_RL_BUFSIZE;
    char * buff = malloc(sizeof(char) * bufsize);
    int c = 0;
    int pos = 0;

    if(!buff){
        printf("hsh: Allocation Error\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        c = getchar();

        if(c == EOF || c == '\n'){
            buff[pos] = '\0';
            return buff;
        } else {
            buff[pos++] = c;
        }
        
        if(pos >= bufsize){
            bufsize += HSH_RL_BUFSIZE;
            buff = realloc(buff, bufsize);
            if(!buff){
                printf("hsh: Allocation Error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    
    // return buff;
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
