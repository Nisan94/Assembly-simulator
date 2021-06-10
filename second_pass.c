#include "analyze.h"

extern char error[N];
extern char err0[], err1[], err2[], err3[], err4[], err5[], err6[], err7[], err8[];
extern word second_word;
extern word third_word;
int code_counter;
extern int symbol_counter;            /*initialize to 0*/ 
extern int img_counter;
extern int there_is_error, there_is_entry;
extern mem_image code_img[MAX];     /*initialize to 0*/
extern symbol_struct * symbol_table; 


/*Second pass according to page 37 of the maman.
Input: the received file contains the commands.
The function completes the initialization of the code image with the values of the second/third words (if there are).
Also, updates the type of the entry symbols in the symbols table.
Output: TRUE if it passed with no errors. else, FALSE.*/
int second_pass(FILE * kf)
{
    char kind_of_command[N];
    char command[N];
    code_counter = 0;
    while (fgets(command,N,kf) != NULL)
    {
        int num_of_command;
        code_counter ++;
        strcpy(error,err0);
        command[strlen(command)-1]= '\0';
        if (command[0] == ';')                              /*note line*/
            continue;
        strcpy(kind_of_command, kind(command));             /*gets the name of the command */
        if (kind_of_command[0] == '\0')                     /*empty line*/
            continue;
        num_of_command = num_command(kind_of_command);	/*gets the number that represents the kind of command*/
        if (num_of_command == 17)
        {
            strcpy(kind_of_command, kind(command));             /*get the next word after the label */
            num_of_command = num_command(kind_of_command);
        }
        if (num_of_command == 18 || num_of_command == 19 || num_of_command == 20)
            continue;   /*read the next line */
        if (num_of_command == 21)   /*entry */
        {
            int i;
            int exist;
            strcpy(kind_of_command, kind(command));
            if (strcmp(error,err0))
            {
                there_is_error = TRUE;
                printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                continue;
            }
            if (kind_of_command == '\0')
            {
                strcpy(error,err4);
                there_is_error = TRUE;
                printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                continue;
            }
            exist = FALSE;
            for (i = 0; i<symbol_counter; i++)
            {
                if (!strcmp(symbol_table[i].label,kind_of_command)) /*the label is in the table */
                {
                    symbol_table[i].label_type += 4;
                    there_is_entry = TRUE;
                    exist = TRUE;
                    break;
                }
            }
            if (exist == FALSE)
            {
                strcpy(error, err1);
                there_is_error = TRUE;
                printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                continue;
            }
            continue;
        }
        switch(num_of_command)
        {
            case 1: case 3: case 4:     /*mov, add, sub*/
            {
                char first[N], second[N];
                int kind_of_operand1, kind_of_operand2, exist;
                exist = FALSE;
                strcpy(first, first_operand(command));
                kind_of_operand1= kind_operand(first);
                strcpy(second, second_operand(command));
                kind_of_operand2= kind_operand(second);
                if (kind_of_operand1 == 1)
                {
                    char * p;
                    int value;
                    int i;
                    for (i = 0; i<symbol_counter; i++)
                    {
                        if (!strcmp(symbol_table[i].label,first)) /*the label is in the table */
                        {
                            exist = TRUE;
                            second_word.address = symbol_table[i].decimalAddress;
                            if(symbol_table[i].label_type == 2)
                            {
                                second_word.ARE = 1;    /*E=1*/
                                addToExtTable (symbol_table[i].label, IC + img_counter +1);
                            }
                            else 
                                second_word.ARE = 2;    /*R=1*/
                            p  = (char *)(&second_word);
                            value = bitTOint(p);
                            img_counter++;
                            code_img[img_counter].code_word = value;
                            img_counter++;
                            break;
                        }
                    }
                    if (exist == FALSE)
                    {
                        strcpy(error, err1);
                        there_is_error = TRUE;
                        printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                        continue;
                    } 
                }
                if (kind_of_operand1 == 0)
                    img_counter += 2;
                if (kind_of_operand1 == 3)
                {
                    img_counter ++;
                }
                if (kind_of_operand2 == 1)
                {
                    char * p;
                    int value;
                    int i;
                    exist = FALSE;
                    for (i = 0; i<symbol_counter; i++)
                    {
                        if (!strcmp(symbol_table[i].label,second)) /*the label is in the table */
                        {
                            exist = TRUE;
                            third_word.address = symbol_table[i].decimalAddress;
                            if(symbol_table[i].label_type == 2)
                            {
                                third_word.ARE = 1;    /*E=1*/
                                addToExtTable (symbol_table[i].label, IC + img_counter);
                            }
                            else 
                                third_word.ARE = 2;    /*R=1*/
                            p  = (char *)(&third_word);
                            value = bitTOint(p);
                            code_img[img_counter].code_word = value;
                            img_counter++;
                            break;
                        }
                    }
                    if (exist == FALSE)
                    {
                        strcpy(error, err1);
                        there_is_error = TRUE;
                        printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                        continue;
                    }
                }
                break;
            }
            case 2:   /*cmp*/
            {
                char first[N], second[N];
                int kind_of_operand1, kind_of_operand2, exist;
                int i;
                exist = FALSE;
                strcpy(first, first_operand(command));
                kind_of_operand1= kind_operand(first);
                strcpy(second, second_operand(command));
                kind_of_operand2= kind_operand(second);
                if (kind_of_operand1 == 1)
                {
                    char * p;
                    int value;
                    for (i = 0; i<symbol_counter; i++)
                    {
                        if (!strcmp(symbol_table[i].label,first)) /*the label is in the table */
                        {
                            exist = TRUE;
                            second_word.address = symbol_table[i].decimalAddress;
                            if(symbol_table[i].label_type == 2)
                            {
                                second_word.ARE = 1;
                                addToExtTable (symbol_table[i].label, IC + img_counter +1);
                            }
                            else 
                                second_word.ARE = 2;    /*R=1*/
                            p  = (char *)(&second_word);
                            value = bitTOint(p);
                            img_counter++;
                            code_img[img_counter].code_word = value;
                            img_counter++;
                            break;
                        }
                    }
                    if (exist == FALSE)
                    {
                        strcpy(error, err1);
                        there_is_error = TRUE;
                        printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                        continue;
                    }
                }
                if (kind_of_operand1 == 0)
                    img_counter += 2;
                if (kind_of_operand1 == 3)
                    img_counter ++;
                if (kind_of_operand2 == 1)
                {
                    char * p;
                    int value;
                    int i;
                    exist = FALSE;
                    for (i = 0; i<symbol_counter; i++)
                    {
                        if (!strcmp(symbol_table[i].label,second)) /*the label is in the table */
                        {
                            exist = TRUE;
                            third_word.address = symbol_table[i].decimalAddress;
                            if(symbol_table[i].label_type == 2)
                            {
                                third_word.ARE = 1;    /*E=1*/
                                addToExtTable (symbol_table[i].label, IC + img_counter);
                            }
                            else 
                                third_word.ARE = 2;    /*R=1*/
                            p  = (char *)(&third_word);
                            value = bitTOint(p);
                            code_img[img_counter].code_word = value;
                            img_counter++;
                            break;
                        }
                    }
                    if (exist == FALSE)
                    {
                        strcpy(error, err1);
                        there_is_error = TRUE;
                        printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                        continue;
                    }
                }
                if (kind_of_operand2 == 0)
                    img_counter++;
                break;
            }
            case 5:   /*lea */
            {
                char * p;
                int value;
                int i;
                char first[N], second[N];
                int kind_of_operand2, exist;
                exist = FALSE;
                strcpy(first, first_operand(command));
                strcpy(second, second_operand(command));
                kind_of_operand2= kind_operand(second);
                for (i = 0; i<symbol_counter; i++)
                {
                    if (!strcmp(symbol_table[i].label,first)) /*the label is in the table */
                    {
                        exist = TRUE;
                        second_word.address = symbol_table[i].decimalAddress;
                        if(symbol_table[i].label_type == 2)
                        {
                            second_word.ARE = 1;    /*E=1*/
                            addToExtTable (symbol_table[i].label, IC + img_counter +1);
                        }
                        else 
                            second_word.ARE = 2;    /*R=1*/
                        p  = (char *)(&second_word);
                        value = bitTOint(p);
                        img_counter++;
                        code_img[img_counter].code_word = value;
                        img_counter++;
                        break;
                    }
                }
                if (exist == FALSE)
                {
                    strcpy(error, err1);
                    there_is_error = TRUE;
                    printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                    continue;
                }
                if (kind_of_operand2 == 1)
                {
                    char * p;
                    int value;
                    exist = FALSE;
                    for (i = 0; i<symbol_counter; i++)
                    {
                        if (!strcmp(symbol_table[i].label,second)) /*the label is in the table */
                        {
                            exist = TRUE;
                            third_word.address = symbol_table[i].decimalAddress;
                            if(symbol_table[i].label_type == 2)
                            {
                                third_word.ARE = 1;    /*E=1*/
                                addToExtTable (symbol_table[i].label, IC + img_counter);
                            }
                            else 
                                third_word.ARE = 2;    /*R=1*/
                            p  = (char *)(&third_word);
                            value = bitTOint(p);
                            code_img[img_counter].code_word = value;
                            img_counter ++;
                            break;
                        }
                    }
                    if (exist == FALSE)
                    {
                        strcpy(error, err1);
                        there_is_error = TRUE;
                        printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                        continue;
                    }
                }
                break;
            }
            case 6: case 7: case 8: case 9: case 13: case 14:     /*clr, not, inc, dec, red, prn*/
            {
                char * p;
                int value;
                char second[N];
                int kind_of_operand2, exist;
                exist = FALSE;
                strcpy(second, first_operand(command));
                kind_of_operand2= kind_operand(second);
                if (kind_of_operand2 == 1)
                {
                    int i;
                    for (i = 0; i<symbol_counter; i++)
                    {
                        if (!strcmp(symbol_table[i].label,second)) /*the label is in the table */
                        {
                            exist = TRUE;
                            second_word.address = symbol_table[i].decimalAddress;
                            if(symbol_table[i].label_type == 2)
                            {
                                second_word.ARE = 1;    /*E=1*/
                                addToExtTable (symbol_table[i].label, IC + img_counter +1);
                            }
                            else 
                                second_word.ARE = 2;    /*R=1*/
                            p  = (char *)(&second_word);
                            value = bitTOint(p);
                            img_counter++;
                            code_img[img_counter].code_word = value;
                            img_counter++;
                            break;
                        }
                    }
                    if (exist == FALSE)
                    {
                        strcpy(error, err1);
                        there_is_error = TRUE;
                        printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                        continue;
                    }
                    continue;
                }
                if (kind_of_operand2 == 0)
                    img_counter += 2;
                else
                {
                    img_counter++;
                }
                break;
            }
            case 10: case 11: case 12:    /*jmp,bne,jsr */
            {
                char * p;
                int value;
                int i;
                char second[N];
                int kind_of_operand2, exist;
                exist = FALSE;
                strcpy(second, first_operand(command));
                kind_of_operand2= kind_operand(second);
                if (kind_of_operand2 == 1)
                {
                    for (i = 0; i<symbol_counter; i++)
                    {
                        if (!strcmp(symbol_table[i].label,second)) /*the label is in the table */
                        {
                            exist = TRUE;
                            second_word.address = symbol_table[i].decimalAddress;
                            if(symbol_table[i].label_type == 2)
                            {
                                second_word.ARE = 1;    /*E=1*/
                                addToExtTable (symbol_table[i].label, IC + img_counter +1);
                            }
                            else 
                                second_word.ARE = 2;    /*R=1*/
                            p  = (char *)(&second_word);
                            value = bitTOint(p);
                            img_counter++;  /*go next to the decimal address of the label */
                            code_img[img_counter].code_word = value;
                            img_counter++;
                            break;
                        }
                    }
                    if (exist == FALSE)
                    {
                        strcpy(error, err1);
                        there_is_error = TRUE;
                        printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                        continue;
                    }
                }
                if (kind_of_operand2 == 2)
                {
                    for (i = 0; i<symbol_counter; i++)
                    {
                        if (!strcmp(symbol_table[i].label,second)) /*the label is in the table */
                        {
                            exist = TRUE;
                            if(symbol_table[i].label_type == 2)     /*cannot be extern */
                            {
                                there_is_error = TRUE;
                                printf("\nIn line %d the label is external and cannot be used in this addressing method.\n", code_counter);
                                continue;
                            }                                        
                            else 
                                second_word.ARE = 4;    /*A=1*/
                            second_word.address = symbol_table[i].decimalAddress - (IC + img_counter);
                            p  = (char *)(&second_word);
                            value = bitTOint(p);
                            img_counter++;  /*go next to the decimal address of the label */
                            code_img[img_counter].code_word = value;
                            img_counter++;
                            break;
                        }
                    }
                    if (exist == FALSE)
                    {
                        strcpy(error, err1);
                        there_is_error = TRUE;
                        printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                        continue;
                    }
                }
                break;
            }
        }
    }
    if (there_is_error)
        return FALSE;       /*to the next file*/
    return TRUE; 
}
