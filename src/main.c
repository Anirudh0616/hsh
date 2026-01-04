#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

#define HSH_RL_BUFSIZE 1024;

#define HSH_TOK_BUFSIZE 64
#define HSH_TOK_DELIM " \t\r\n\a" // this is a set. not a sequence. i.e. for any of these characters, tokenize

char **hsh_split_line(char *line){
    int bufsize = HSH_TOK_BUFSIZE, pos = 0;
    char **tokens = malloc(sizeof(char*) * bufsize);
    char *token;

    if(!tokens){
        fprintf(stderr, "hsh: allocation error\n");
        exit(EXIT_FAILURE);
    }
    // Start tokenizing a new stirng (Since it is not Null)
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


int hsh_launch(char ** args){
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

int hsh_cd(char ** args);
int hsh_help(char **args);
int hsh_exit(char **args);
int hsh_echo(char **args);


char *builtin_str[] = {"cd", "help", "exit", "echo"};

int (*builtin_func[]) (char ** ) = {&hsh_cd, &hsh_help, &hsh_exit, &hsh_echo};

int hsh_num_builtins(){
    return sizeof(builtin_str) / sizeof(char*);
}

// Builtin Functions Implementations
int hsh_cd(char **args){
    if(args[1] == NULL){
        fprintf(stderr, "hsh: expected argument for \"cd\"\n");
    } else {
        if(chdir(args[1]) != 0) {
            perror("hsh");
        }
    }
    return 1;
}
int hsh_echo(char **args){
    if(args[1] == NULL){
        fprintf(stderr, "hsh: expected argument for \"echo\"\n");
    } else {
        int i = 1;
        while(args[i]){
            printf("%s ",args[i]);
            i++;
        }
    }
    return 1;
}

int hsh_help(char **args){
    int i;
    if(args[1] != NULL){
        printf("Why Would you give args for this.\n");
    }
    printf("\n");
    printf("Anirudh's hsh\n");
    printf("type out some program names or arguments and hit backspace, or was it enter?\n");
    printf("Builtin functions:\n");

    for(int i = 0; i<hsh_num_builtins(); i++){
        printf("    %s\n", builtin_str[i]);
    }
    printf("And many more, maybe.\n");

    printf("Use the man command ig, for info on other programs");
    printf("\n");
    return 1;
}

int hsh_exit(char **args){
    if(args[1] != NULL){
        printf("Who gives args for an exit command?\n");
    }
    return 0;
}


int hsh_execute(char ** args){
    if(args[0] == NULL){
        return 1;
    }

    for(int i = 0; i < hsh_num_builtins();i++){
        if(strcmp(args[0], builtin_str[i]) == 0){
            return (*builtin_func[i])(args);
        }
    }
    return hsh_launch(args);
}

void hsh_loop(void){
    char* line;
    char ** args;
    int status = 1;

    do{
        printf("\n> ");
        line = hsh_read_line();
        args = hsh_split_line(line);
        status = hsh_execute(args);

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
