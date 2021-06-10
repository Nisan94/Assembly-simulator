#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define MAX 200
#define MAX_NAME 31
#define START_SIZE_OF_SYMBOL_TABLE 20
#define TRUE 1
#define FALSE 0
#define IC 100
#define N 100
#define ACT_TABLE_SIZE 5

/*The description for the following functions- in analyze.c file.*/
int skip(char[]);
char * kind(char[]);
int num_command(char[]);
char * first_operand(char[]);
char * second_operand(char[]);
int kind_operand (char[]);
int two_operand(char[], char[], int, int);
int one_operand(char[], int);
int word_j(int);
char * itoa (int);
int legal_label(char[]);
int thereISlabel(char[], int);
int check_empty(char[]);
int bitTOint(char *);
int data_check(char[]);
char * data_operand(char[]);
void reverse(char[]);
void addToExtTable (char[], int);


/*This struct defines a first word, following the given structure in page 21 in the maman.*/
typedef struct fst_word {
    unsigned int ARE:3;
    unsigned int funct:5;
    unsigned int reg_target:3;
    unsigned int type_target:2;
    unsigned int reg_source:3;
    unsigned int type_source:2;
    unsigned int opcode:6;
} fst_word;

/*This struct defines a second/third word, following the description of the miun options. */
typedef struct word {
    unsigned int ARE:3;
    unsigned int address:21;
} word;


/*This struct contains the values of the code (command) lines only.
The array code_img is from this struct type, and is allocated first in the memory image (when printed to .ob file).
The array data_img is from this struct type also, and is allocated second (after the code_img lines) in the memory image.*/
typedef struct mem_image{
    int address; 
    unsigned int code_word:24;
} mem_image;


/*This struct contains the values of all the symbols (labels)- name, address in the memory image and their type.*/
typedef struct symbol_struct { /*DATASTRUCTURE TO STORE LABEL AND ITS ADDRESS*/
    char label[MAX_NAME];
    int decimalAddress;
    int label_type;     /*1-data; 2-external; 3-entry; 4-code; 5-data+entry; 8-code+entry;*/
} symbol_struct;


/*This struct contains the values of all the external symbols (labels)- name and the addresses of the commands they apear at.*/
typedef struct ext_struct {
    char label[MAX_NAME];
    int codeAddress;
} ext_struct;
