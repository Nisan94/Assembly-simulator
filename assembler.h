#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX 200
#define START_SIZE_OF_SYMBOL_TABLE 20
#define MAX_NAME 31
#define TRUE 1
#define FALSE 0
#define IC 100

/*First pass 
Input: the received file contains the commands.
The function initializes the values of the code image with all the commands' "first words",
 and the values of the second/third words if it's possible.
Also, initializes the values of the symbols table,the external symbols table and the data image.
Output: TRUE if it passed with no errors. else, FALSE.*/
int first_pass(FILE *);


/*Second pass 
Input: the received file contains the commands.
The function completes the initialization of the code image with the values of the second/third words (if there are).
Also, updates the type of the entry symbols in the symbols table.
Output: TRUE if it passed with no errors. else, FALSE.*/
int second_pass(FILE *);


/*This struct contains the values of all the symbols (labels)- name, address in the memory image and their type.*/
typedef struct symbol_struct { /*DATASTRUCTURE TO STORE LABEL AND ITS ADDRESS*/
    char label[MAX_NAME];
    int decimalAddress;
    int label_type;     /*1-data; 2-external; 3-entry; 4-code; 5-data+entry; 8-code+entry;*/
} symbol_struct;


/*This struct contains the values of the code (command) lines only.
The array code_img is from this struct type, and is allocated first in the memory image (when printed to .ob file).
The array data_img is from this struct type also, and is allocated second (after the code_img lines) in the memory image.*/
typedef struct mem_image{
    int address; 
    unsigned int code_word:24;
} mem_image;


/*This struct contains the values of all the external symbols (labels)- name and the addresses of the commands they apear at.*/
typedef struct ext_struct {
    char label[MAX_NAME];
    int codeAddress;
} ext_struct;

int first_pass(FILE *);
int second_pass(FILE *);
