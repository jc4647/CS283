#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */


int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX);
    if (cmd_buff->_cmd_buffer == NULL) {
        return ERR_MEMORY;
    }

    cmd_buff->argc = 0;
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }

    return OK;
}


int free_cmd_buff(cmd_buff_t *cmd_buff) {
    free(cmd_buff->_cmd_buffer);
    cmd_buff->_cmd_buffer = NULL;

    return OK;
}


int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX);
    cmd_buff->argc = 0;
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }

    return OK;
}


int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    clear_cmd_buff(cmd_buff);
    strncpy(cmd_buff->_cmd_buffer, cmd_line, SH_CMD_MAX - 1);
    cmd_buff->_cmd_buffer[SH_CMD_MAX - 1] = '\0';

    char *input = cmd_buff->_cmd_buffer;
    int argc = 0;

    while (*input != '\0') {
        while (isspace((unsigned char) * input)) {
            input++;
        }
        if (*input == '\0') {
            break;
        }

        char *start;
        if (*input == '"') { 
            input++; 
            start = input;
            while (*input != '"' && *input != '\0') {
                input++;
            }
            if (*input == '"') {
                *input = '\0'; 
                input++;      
            }
        } else {  
            start = input;
            while (*input != '\0' && !isspace((unsigned char) * input)) {
                input++;
            }
            if (*input != '\0') {
                *input = '\0'; 
                input++;
            }
        }

        cmd_buff->argv[argc++] = start;

        if (argc >= CMD_ARGV_MAX - 1) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
    }

    cmd_buff->argv[argc] = NULL;
    cmd_buff->argc = argc;

    return OK;
}



   
int build_cmd_list(char *cmd_line, command_list_t *clist) {
    int count = 0;
    int rc;
    char *tok = strtok(cmd_line, PIPE_STRING);

    while (tok != NULL) {
        if (count == CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        alloc_cmd_buff(&clist->commands[count]);
        clear_cmd_buff(&clist->commands[count]);
        rc = build_cmd_buff(tok, &clist->commands[count]);
        if (rc == ERR_CMD_OR_ARGS_TOO_BIG) {
            return rc;
        }

        count++;
        tok = strtok(NULL, PIPE_STRING);
    }

    clist->num = count;
    return OK;
}


Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, EXIT_CMD) == 0) {
        return BI_CMD_EXIT;
    } else if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    } else if (strcmp(input, "rc") == 0) {
        return BI_EXECUTED;
    } else {
        return BI_NOT_BI;
    }
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd, Built_In_Cmds command) {
    switch (command) {
        case BI_CMD_EXIT:
            exit(OK);
        case BI_CMD_CD:
            if (cmd->argc > 1) {
                if (chdir(cmd->argv[1]) != 0) {
                    fprintf(stderr, "cd: %s: No such file or directory\n", cmd->argv[1]);
                }
            }
            return BI_EXECUTED;
        default:
            return BI_NOT_BI;
    }
}

int exec_cmd(cmd_buff_t *cmd) {
    pid_t pid = fork();
    int status;


    if (pid == 0) {
        execvp(cmd->argv[0], cmd->argv);
        perror("Execvp failed");
        exit(ERR_EXEC_CMD);
    } else if (pid > 0) {
        if (waitpid(pid, &status, 0) == -1) {
            perror("Waitpid failed");
            return ERR_EXEC_CMD;
        }
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
        return ERR_EXEC_CMD;
    } else {
        perror("Fork failed");
        return ERR_EXEC_CMD;
    }
}

int execute_pipeline(command_list_t *clist) {
    int numCommands = clist->num;
    int pipes[2 * (numCommands - 1)];
    pid_t pids[numCommands];

    for (int i = 0; i < numCommands - 1; i++) {
        if (pipe(pipes + i * 2) < 0) {
            perror("Pipe failed");
            return ERR_EXEC_CMD;
        }
    }

    // Create processes for each command
    for (int i = 0; i < numCommands; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            if (i > 0) {
                dup2(pipes[(i - 1) * 2], STDIN_FILENO);
            }

            if (i < numCommands - 1) {
                dup2(pipes[i * 2 + 1], STDOUT_FILENO);
            }

            for (int j = 0; j < (numCommands - 1) * 2; j++) {
                close(pipes[j]);
            }

            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("Execvp failed");
            exit(ERR_EXEC_CMD);
        } else if (pids[i] < 0) {
            perror("Fork failed");
            return ERR_EXEC_CMD;
        }
    }

    for (int i = 0; i < (numCommands - 1) * 2; i++) {
        close(pipes[i]);
    }

    for (int i = 0; i < numCommands; i++) {
        int status;
        if (waitpid(pids[i], &status, 0) == -1) {
            perror("Waitpid failed");
            return ERR_EXEC_CMD;
        }
        if (WIFEXITED(status) && WEXITSTATUS(status) != OK) {
            return WEXITSTATUS(status);
        }
    }

    return OK;
}




int exec_local_cmd_loop()
{
    char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
    
    if (cmd_buff == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return ERR_MEMORY;
    }

    int rc;
    //cmd_buff_t cmd;
    command_list_t list;
    Built_In_Cmds c;
    
    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        if (strlen(cmd_buff) == 0) {
            printf(CMD_WARN_NO_CMD);
            continue;
        }

        rc = build_cmd_list(cmd_buff, &list);
        switch (rc) {
            case WARN_NO_CMDS:
                printf(CMD_WARN_NO_CMD);
                break;
            case ERR_TOO_MANY_COMMANDS:
                printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
                break;
            case OK:
                if (list.num == 1) {
                    c = match_command(list.commands[0].argv[0]);
                    if (c != BI_NOT_BI) {
                        exec_built_in_cmd(&list.commands[0], c);
                    } else {
                        exec_cmd(&list.commands[0]);
                    }
                } else {
                    execute_pipeline(&list);
                }
                break;
            default:
                break;
        }
        
    }

    free(cmd_buff);
    return OK;
}