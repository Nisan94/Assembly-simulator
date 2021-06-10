/*Author: Nisan Ben Abu
This program simulates an assembler with 16 command options.
Input: An assembly file with commands.
Output: If all the command in the received file are correct, the program will create 3 files: 
1. .ob file contains the translation of the commands to binary, presented in hex, next to their addresses in the memory.
2. .ext file contains the names of the external symbols and the all addreses they appears in, in the received file.
3. .ent file contains the names of the internal symbols and the addresses they declared at.
If some of the commands in the received file are not correct- ERROR message will apear correspondingly
 and the above files will not be exported.*/


#include "assembler.h"

int DC;                        /*data counter*/
int symbol_counter;            /*initialize to 0*/ 
int img_counter;
int ext_counter;
int max_dynamic_size;
int ext_dynamic_size;
int ICF;                     /*final value of IC+img_counter */
int there_is_error, there_is_extern, there_is_entry;
mem_image code_img[MAX];     /*initialize to 0*/
mem_image data_img[MAX];
symbol_struct * symbol_table;
ext_struct * ext_table;


/*Program starts her.*/
int main (int argc, char * argv[])
{
    int i;
    FILE * ob;
    FILE * ent;
    FILE * ext;
    there_is_entry = FALSE;
    there_is_extern = FALSE;
    max_dynamic_size =START_SIZE_OF_SYMBOL_TABLE;
    symbol_table = (symbol_struct *)malloc(START_SIZE_OF_SYMBOL_TABLE*sizeof(symbol_struct));
    if (!symbol_table)
    {
        printf("Memory allocation for the symbol tables- FAILED\n");
        exit(0);
    }
    symbol_counter = 0;
    ext_dynamic_size = START_SIZE_OF_SYMBOL_TABLE;
    ext_table = (ext_struct *)malloc(START_SIZE_OF_SYMBOL_TABLE*sizeof(ext_struct));
    if (!ext_table)
    {
        printf("Memory allocation for the ext tables- FAILED\n");
        exit(0);
    }
    ext_counter = 0;
    for (i=1; i <= argc; i++)
    {
        FILE * kf;
        char arg_value[MAX_NAME];
        there_is_error = FALSE;
        strcpy(arg_value, argv[i]);
        kf = fopen(strcat(argv[i],".as"), "r");
        if (!kf)        /*if (kf == NULL) */
        {
            printf("\nCannot open file \"%s\"- ERROR.\n", argv[i]);
            continue;
        }
        if (first_pass(kf))     /*starts the first pass*/
        {
            ICF = IC + img_counter;
            for (i=0; i< symbol_counter; i++)
            {
                if (symbol_table[i].label_type == 1 || symbol_table[i].label_type == 5)
                    symbol_table[i].decimalAddress += ICF;
            }
            rewind(kf);         /*go back to the beginning of the file*/
            img_counter = 0;
            if (second_pass(kf))
            {
                ob = fopen(strcat(arg_value,".ob"), "a");
                if (!ob)
                {
                    printf("\nCannot open file \"%s\"- ERROR.\n", argv[i]);
                    exit(0);
                }
                fprintf(ob,"%7d %d\n",ICF-100,DC);
                for(i=0; i < (ICF-100); i++)
                {
                    fprintf(ob,"%07d %06x\n",code_img[i].address,code_img[i].code_word);
                }
                for(i=0; i < DC; i++)
                {
                    data_img[i].address += ICF;
                    fprintf(ob,"%07d %06x\n",data_img[i].address,data_img[i].code_word);
                }
                if (there_is_entry)
                {
                    strncpy(arg_value, arg_value ,strlen(arg_value)-3);
                    arg_value[strlen(arg_value)-3] = '\0';
                    ent = fopen(strcat(arg_value,".ent"), "a");
                    if (!ent)
                    {
                        printf("\nCannot open file \"%s\"- ERROR.\n", argv[i]);
                        exit(0);
                    }
                    for (i=0; i < symbol_counter; i++)
                    {
                        if (symbol_table[i].label_type == 3 || symbol_table[i].label_type == 5 || symbol_table[i].label_type == 8)
                            fprintf(ent, "%s %07d\n", symbol_table[i].label,symbol_table[i].decimalAddress);
                    }
                }
                if (there_is_extern)
                {
                    strncpy(arg_value, arg_value ,strlen(arg_value)-4);
                    arg_value[strlen(arg_value)-4] = '\0';
                    ext = fopen(strcat(arg_value,".ext"), "a");
                    if (!ext)
                    {
                        printf("\nCannot open file \"%s\"- ERROR.\n", argv[i]);
                        exit(0);
                    }
                    for (i=0; i < ext_counter; i++)
                    {
                        fprintf(ext, "%s %07d\n", ext_table[i].label, ext_table[i].codeAddress);
                    }
                }
            }
            else
            {
                printf("Second run failed. EXIT.\n");
                return 1;
            }
        }
        else
        {
            printf("First run failed. EXIT.\n");
            return 1;
        }
    }
    free(symbol_table);
    free(ext_table);
    return 1;
}
