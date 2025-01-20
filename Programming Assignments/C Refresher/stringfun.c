#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
void reverse_string(char *, int);
void word_print(char *, int);


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int charCount = 0;
    char *input = user_str;
    char *output = buff;

    while (*input != '\0') {
        if (*input != ' ' && *input != '\t') {
            *output = *input;
            output++;
            charCount++;
        } else {
            if (output > buff && *(output - 1) != ' ') {
                *output = ' ';
                output++;
                charCount++;
            }
        }

        if (charCount > len) {
            return -1;
        }

        input++;
    }

    while (charCount < len) {
        *output = '.';
        output++;
        charCount++;
    }

    return output - buff;
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

//Uses a counter and flag to count number of words in the command line argument. Uses the flag to indicate whether we are at a word or a whitespace
int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT
    int wordCount = 0;
    int flag = 0;

    for (int i = 0; i < str_len; i++) {
        if (*(buff + i) != ' ' && *(buff + i) != '.') {
            if (!flag) {
                flag = 1;
                wordCount++;
            }
        } else {
            flag = 0;
        }
    }

    return wordCount;

}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
//Uses a temp variable to reverse the string that works inward from the original string
void reverse_string(char *buff, int len) {
    char *start = buff;
    char *end = buff + len - 1;
    char temp;

    while (start < end) {
        temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;
    }

    printf("Reversed String: ");
    for (int i = 0; i < len; i++) {
        putchar(*(buff+i));
    }
    putchar('\n');
}

//Prints a list of each word with the amount of characters that they each have and then the total number of words
void word_print(char *buff, int len) {
    printf("Word Print\n----------\n");
    char *start = NULL;
    int wordLength = 0;
    int sideNumber = 1;
    int wordCount = 0;

    for (int i = 0; i <= len; i++) {
        if (*(buff + i) != ' ' && *(buff + i) != '\0' && *(buff + i) != '.') {
            if (start == NULL) {
                start = buff + i;
            }

            wordLength++;
        } else {
            if (start != NULL) {
                printf("%d. ", sideNumber);
                
                for (int j = 0; j < wordLength; j++) {
                    putchar(*(start + j));
                }

                printf("(%d)\n", wordLength);
                sideNumber++;
                wordCount++;
                start = NULL;
                wordLength = 0;
            }
        }
    }

    printf("\nNumber of words returned: %d\n", wordCount);
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      This is safe since it first checks if there are at least 2 arguements and the second check checks if argv does not start with the "-" character, meaning the user did not provide the correct starting character for the comamnds
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      To make sure that an argument is provided
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) {
        printf("Memory allocation failed\n");
        exit(99);
    }


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    //Handles which option user chooses
    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options

        case 'r':
            reverse_string(buff, user_str_len);
            break;
        
        case 'w':
            word_print(buff, user_str_len);
            break;

        case 'x':
            printf("Not implemented!\n");
            exit(1);

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
//          If we want to use functions with different buffer lengths, passing the pointer and the length is good since it will not rely on the hard coded 50 bvtes. It also future-proofs our code in the future in case we want to change the size of the buffer
//          