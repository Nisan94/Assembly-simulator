#include "analyze.h"

extern char error[N];
extern char err0[], err1[], err2[], err3[], err4[], err5[], err6[], err7[], err8[];
extern char act1[];
extern char act2[];
extern char act3[];
extern char act4[];
extern char act5[];
extern char act6[];
extern char act7[];
extern char act8[];
extern char act9[];
extern char act10[];
extern char act11[];
extern char act12[];
extern char act13[];
extern char act14[];
extern char act15[];
extern char act16[];
extern fst_word first_word;
extern word second_word;
extern word third_word;
extern int code_counter;
extern int DC;                        /*data counter*/
extern int symbol_counter;            /*initialize to 0*/ 
extern int img_counter;
extern int max_dynamic_size;
extern int there_is_error, there_is_extern, there_is_entry;
extern mem_image code_img[MAX];     /*initialize to 0*/
extern mem_image data_img[MAX];
extern symbol_struct * symbol_table;

/*This struct contains the name of one of the possible programs' commands and
a define number*/
typedef struct action_table{
    char name[ACT_TABLE_SIZE];
    int define;         /*in act_table1- define=opcode; in act_table2- define=funct */
} action_table;

action_table act_table1[16], act_table2[16];

/*This function initializes the structs of the first array from action_table struct.
The define number represents the opcode of the commands.*/
void build1 (action_table act_table1[], char act1[], char act2[],char act3[], char act4[], char act5[], char act6[], char act7[], char act8[], char act9[], char act10[], char act11[], char act12[], char act13[], char act14[], char act15[], char act16[])
{
    strcpy(act_table1[0].name, act1); 
    strcpy(act_table1[1].name, act2); 
    strcpy(act_table1[2].name, act3);  
    strcpy(act_table1[3].name, act4); 
    strcpy(act_table1[4].name, act5); 
    strcpy(act_table1[5].name, act6); 
    strcpy(act_table1[6].name, act7);
    strcpy(act_table1[7].name, act8); 
    strcpy(act_table1[8].name, act9); 
    strcpy(act_table1[9].name, act10);  
    strcpy(act_table1[10].name, act11); 
    strcpy(act_table1[11].name, act12); 
    strcpy(act_table1[12].name, act13); 
    strcpy(act_table1[13].name, act14);
    strcpy(act_table1[14].name, act15); 
    strcpy(act_table1[15].name, act16);
    act_table1[0].define= 0;
    act_table1[1].define= 1;
    act_table1[2].define= 2;
    act_table1[3].define= 2;
    act_table1[4].define= 4;
    act_table1[5].define= 5;
    act_table1[6].define= 5;
    act_table1[7].define= 5;
    act_table1[8].define= 5;
    act_table1[9].define= 9;
    act_table1[10].define= 9;
    act_table1[11].define= 9;
    act_table1[12].define= 12;
    act_table1[13].define= 13;
    act_table1[14].define= 14;
    act_table1[15].define= 15;
}

/*This function initializes the structs of the second array from action_table struct.
The define number represents the funct of the commands.*/
void build2 (action_table act_table2[], char act1[], char act2[],char act3[], char act4[], char act5[], char act6[], char act7[], char act8[], char act9[], char act10[], char act11[], char act12[], char act13[], char act14[], char act15[], char act16[])
{
    strcpy(act_table2[0].name, act1); 
    strcpy(act_table2[1].name, act2); 
    strcpy(act_table2[2].name, act3);  
    strcpy(act_table2[3].name, act4); 
    strcpy(act_table2[4].name, act5); 
    strcpy(act_table2[5].name, act6); 
    strcpy(act_table2[6].name, act7);
    strcpy(act_table2[7].name, act8); 
    strcpy(act_table2[8].name, act9); 
    strcpy(act_table2[9].name, act10);  
    strcpy(act_table2[10].name, act11); 
    strcpy(act_table2[11].name, act12); 
    strcpy(act_table2[12].name, act13); 
    strcpy(act_table2[13].name, act14);
    strcpy(act_table2[14].name, act15); 
    strcpy(act_table2[15].name, act16);
    act_table2[0].define= 0;
    act_table2[1].define= 0;
    act_table2[2].define= 1;
    act_table2[3].define= 2;
    act_table2[4].define= 0;
    act_table2[5].define= 1;
    act_table2[6].define= 2;
    act_table2[7].define= 3;
    act_table2[8].define= 4;
    act_table2[9].define= 1;
    act_table2[10].define= 2;
    act_table2[11].define= 3;
    act_table2[12].define= 0;
    act_table2[13].define= 0;
    act_table2[14].define= 0;
    act_table2[15].define= 0;
}

/*First pass according to page 36 of the maman.
Input: the received file contains the commands.
The function initializes the values of the code image with all the commands' "first words",
 and the values of the second/third words if it's possible.
Also, initializes the values of the symbols table,the external symbols table and the data image.
Output: TRUE if it passed with no errors. else, FALSE.*/
int first_pass(FILE * kf)
{
    char kind_of_command[N];
    char command[N];
    int emp;                        /*checking that there is noting at the end of the command*/
    int there_is_label;
    build1 (act_table1, act1, act2, act3, act4, act5, act6, act7, act8, act9, act10, act11, act12, act13, act14, act15, act16);
    build2 (act_table2, act1, act2, act3, act4, act5, act6, act7, act8, act9, act10, act11, act12, act13, act14, act15, act16);
    while (fgets(command,N,kf) != NULL)
    {
        char label[MAX_NAME];
        int num_of_command;
        code_counter ++;
        there_is_label = FALSE;
        strcpy(error,err0);
        command[strlen(command)-1]= '\0';
        if (command[0] == ';')                              /*note line*/
            continue;
        strcpy(kind_of_command, kind(command));             /*gets the name of the command */
        if (kind_of_command[0] == '\0')                     /*empty line*/
            continue;
        if (!strcmp(error,err0))
        {
            num_of_command = num_command(kind_of_command);	/*gets the number that represents the kind of command*/
            if (num_of_command == 17)
            {
                char tempo[MAX_NAME];
                {
                    memcpy(label, kind_of_command, strlen(kind_of_command)-1);
                    label[strlen(kind_of_command)-1]= '\0';
                    there_is_label = TRUE;
                }
                strcpy(tempo, kind(command));             /*get the next word after the label */
                num_of_command = num_command(tempo);
            }
            if (num_of_command == 18 || num_of_command == 19 ) /*data/string*/
            {
                char data[MAX];
                if (there_is_label)     /*label: .data/.string ...*/
                {
                    if (!thereISlabel(label, DC))
                        continue;
                    symbol_table[symbol_counter-1].label_type = 1 ; /*1-data*/
                }
                strcpy(data,first_operand(command));
                if (data[0] != '\0' && !strcmp(error,err0))
                {
                    if (num_of_command == 18)      /*data */
                    {
                        while (strlen(data) != 0 && !strcmp(error,err0) && data_check(data) && !strcmp(error,err0))  /*if got to here- data is a legal number */
                        {
                            data_img[DC].address = DC;
                            data_img[DC++].code_word = atoi(data);
                            strcpy(data,data_operand(command));
                        }
                        if (!strcmp(error,err0))
                        {
                            emp = check_empty(command);
                            if (!emp)
                                printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                        }
                        else
                            printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                        continue;
                    }
                    else if (num_of_command == 19 && data[0] == '\"' && data[strlen(data)-1] == '\"')        /*string */
                    {
                        int i;
                        for (i= 1; i < (strlen(data)-1); i++) /*without " " */
                        {
                            data_img[DC].address = DC;
                            data_img[DC++].code_word = data[i];   /*it's ascii code */
                        }
                        data_img[DC].address = DC;
                        data_img[DC++].code_word = 0;
                        continue;
                    }
                    else
                    {
                        strcpy(error,err3);     /*invalid parameter- a string with no quotation marks.*/
                        there_is_error = TRUE;
                        printf("\nIn line %d there is an error- %s-  a string with no quotation marks.\n", code_counter, error);
                        continue;
                    }
                }
                else
                {
                    printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                    continue;
                }
            }
            if (num_of_command == 20)       /*extern. after extern should always be a new label.*/
            {
                if (there_is_label)
                    printf("\nWARNING- In line %d there is a label at the beginning of an extern sentence.\n", code_counter);
                strcpy(kind_of_command, kind(command));             /*get the next word after the .extern */
                emp = check_empty(command);
                if (strlen(kind_of_command) == 0)
                {
                    strcpy(error, err4);        /*missing parameter*/
                    there_is_error = TRUE;
                    printf ("\nIn line %d there is an error- %s.\n", code_counter, error);
                    continue;
                }
                if (!strcmp(error,err0))    /*if emp==false, the error variable won't be err0*/
                {
                    int exist;
                    int i;
                    exist = FALSE;
                    for (i=0; i< symbol_counter; i++)       /*check if the label is in the table already*/
                    {
                        if (!strcmp(symbol_table[i].label,kind_of_command))
                        {
                            exist = TRUE;
                            if (symbol_table[i].label_type !=2)
                            {
                                there_is_error = TRUE;
                                printf ("\nThe label in line %d is declared as external while in anothe line- is not external.\n",code_counter);
                            }
                            break;
                        }
                    }
                    if (exist == FALSE)
                    {
                        if (legal_label(kind_of_command))
                        {
                            strcpy(symbol_table[symbol_counter].label, kind_of_command); /*add the label to the table. kind of command=the name of the label */
                            symbol_table[symbol_counter].decimalAddress = 0;
                            symbol_table[symbol_counter].label_type = 2;
                            symbol_counter ++;
                            there_is_extern = TRUE;
                        }
                        else 
                        {
                            strcpy(error, err1);
                            there_is_error = TRUE;
                            printf ("\nIn line %d there is an error- %s.\n", code_counter, error);
                            continue;
                        }
                    }
                }
                else
                    printf ("\nIn line %d there is an error- %s", code_counter, error);
                continue;
            }
            if (num_of_command == 21)       /*entry */
            {
                strcpy(kind_of_command, kind(command));
                if (!legal_label(kind_of_command))  /*checks if the label after .entry is legal*/
                {
                    strcpy(error, err1);
                    there_is_error = TRUE;
                    printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                }
                continue; 
            }  
            switch(num_of_command)
            {
                case 1: case 3: case 4:     /*mov, add, sub*/
                {
                    char first[N], second[N];
                    int kind_of_operand1, kind_of_operand2;
                    if (there_is_label)
                    {
                        if (!thereISlabel(label, IC+img_counter))
                            continue;
                        symbol_table[symbol_counter-1].label_type = 4 ;
                    }
                    strcpy(first, first_operand(command));
                    kind_of_operand1= kind_operand(first);
                    strcpy(second, second_operand(command));
                    kind_of_operand2= kind_operand(second);
                    if (!strcmp(error,err0))
                    {
                        emp = check_empty(command);
                        if (emp)
                        {
                            code_img[img_counter].address = IC + img_counter;
                            first_word.opcode = act_table1[num_of_command-1].define;
                            first_word.funct = act_table2[num_of_command-1].define;
                            first_word.ARE = 4;     /*A=1,R=0,E=0*/
                            if (kind_of_operand1 != 2 && kind_of_operand2 != 0 && kind_of_operand2 != 2)
                            {
                                char * p;
                                int value;
                                int L;
                                L = two_operand(first, second, kind_of_operand1, kind_of_operand2);    /*fill the first_word*/
                                p  = (char *)(&first_word);
                                value = bitTOint(p);
                                code_img[img_counter].code_word = value;
                                if (kind_of_operand1 == 0)  /*miun miyadi, we know the second word*/
                                {
                                    img_counter ++ ;
                                    p  = (char *)(&second_word);
                                    value = bitTOint(p);
                                    code_img[img_counter].code_word = value;
                                    img_counter += (L-1);
                                }
                                else
                                    img_counter += L;
                            }
                            else 
                            {
                                strcpy(error, err3);
                                there_is_error = TRUE;
                                printf("\nIn line %d there is an error- Invalid addressing mode.\n", code_counter);
                                continue;
                            } 
                        }
                        else 
                        {
                            printf("\nIn line %d there is an error- %s.\n", code_counter, error);   /*err5 */
                            continue;
                        } 
                    }
                    else
                    {
                        printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                        continue;
                    }
                    break;
                }
                case 2:   /*cmp*/
                {
                    char first[N], second[N];
                    int kind_of_operand1, kind_of_operand2;
                    if (there_is_label)
                    {
                        if (!thereISlabel(label, IC+img_counter))
                            continue;
                        symbol_table[symbol_counter-1].label_type = 4 ;
                    }
                    strcpy(first, first_operand(command));
                    kind_of_operand1= kind_operand(first);
                    strcpy(second, second_operand(command));
                    kind_of_operand2= kind_operand(second);
                    if (!strcmp(error,err0))
                    {
                        emp = check_empty(command);
                        if (emp)
                        {
                            code_img[img_counter].address = IC + img_counter;
                            first_word.opcode = act_table1[1].define;
                            first_word.funct = act_table2[1].define;
                            first_word.ARE = 4;     /*A=1,R=0,E=0*/
                            if (kind_of_operand1 != 2 && kind_of_operand2 != 2)
                            {
                                char * p;
                                int value;
                                int L;
                                L = two_operand(first, second, kind_of_operand1, kind_of_operand2);    /*fill the first_word*/
                                p  = (char *)(&first_word);
                                value = bitTOint(p);
                                code_img[img_counter].code_word = value;
                                if (kind_of_operand1 == 0)  /*miun miyadi, we know the second word*/
                                {
                                    p  = (char *)(&second_word);
                                    value = bitTOint(p);
                                    code_img[img_counter+1].address = IC + img_counter;
                                    code_img[img_counter+1].code_word = value;
                                }
                                if (kind_of_operand2 == 0)
                                {
                                    p  = (char *)(&third_word);
                                    value = bitTOint(p);
                                    code_img[img_counter+L-1].code_word = value;
                                }
                                img_counter += L;
                            }
                            else
                            {
                                strcpy(error, err3);
                                there_is_error = TRUE;
                                printf("\nIn line %d there is an error- Invalid addressing mode.\n", code_counter);
                                continue;
                            } 
                        }
                        else 
                        {
                            printf("\nIn line %d there is an error- %s.\n", code_counter, error);   /*err5 */
                            continue;
                        }
                    } 
                    else
                    {
                        printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                        continue;
                    }
                    break;
                }
                case 5:   /*lea */
                {
                    char first[N], second[N];
                    int kind_of_operand1, kind_of_operand2;
                    if (there_is_label)
                    {
                        if (!thereISlabel(label, IC+img_counter))
                            continue;
                        symbol_table[symbol_counter-1].label_type = 4 ;
                    }
                    strcpy(first, first_operand(command));
                    kind_of_operand1= kind_operand(first);
                    strcpy(second, second_operand(command));
                    kind_of_operand2= kind_operand(second);
                    if (!strcmp(error,err0))
                    {
                        emp = check_empty(command);
                        if (emp)
                        {
                            code_img[img_counter].address = IC + img_counter;
                            first_word.opcode = act_table1[4].define;
                            first_word.funct = act_table2[4].define;
                            first_word.ARE = 4;     /*A=1,R=0,E=0*/
                            if (kind_of_operand1 == 1 && kind_of_operand2 != 0 && kind_of_operand2 != 2)
                            {
                                char * p;
                                int value;
                                int L;
                                L = two_operand(first, second, kind_of_operand1, kind_of_operand2);    /*fill the first_word*/
                                p  = (char *)(&first_word);
                                value = bitTOint(p);
                                code_img[img_counter].code_word = value;
                                img_counter += L ;
                            }
                            else 
                            {
                                strcpy(error, err3);
                                there_is_error = TRUE;
                                printf("\nIn line %d there is an error- Invalid addressing mode.\n", code_counter);
                                continue;
                            } 
                        }
                        else 
                        {
                            printf("\nIn line %d there is an error- %s.\n", code_counter, error);   /*err5 */
                            continue;
                        }
                    } 
                    else
                    {
                        printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                        continue;
                    }
                    break;
                }
                case 6: case 7: case 8: case 9: case 13: case 14:     /*clr, not, inc, dec, red, prn*/
                {
                    char second[N];
                    int kind_of_operand2;
                    int index;
                    if (there_is_label)
                    {
                        if (!thereISlabel(label, IC+img_counter))
                            continue;
                        symbol_table[symbol_counter-1].label_type = 4 ;
                    }
                    strcpy(second, first_operand(command));
                    kind_of_operand2= kind_operand(second);
                    if (!strcmp(error,err0))
                    {
                        emp = check_empty(command);
                        if (emp)
                        {
                            index = img_counter;
                            code_img[index].address = IC + index;
                            first_word.opcode = act_table1[num_of_command-1].define;
                            first_word.funct = act_table2[num_of_command-1].define;
                            first_word.ARE = 4;     /*A=1,R=0,E=0*/
                            first_word.reg_source = 0;
                            first_word.type_source = 0;
                            if (kind_of_operand2 != 2) /*for all*/
                            {
                                char * p;
                                int value;
                                int L;
                                if (num_of_command != 14) /*if num_of_command is not 14 -> will do the "one_operand" only if kind2 is not 0*/
                                {
                                    if (kind_of_operand2 != 0)
                                    {
                                        L = one_operand(second, kind_of_operand2);    /*fill the first_word*/ 
                                        p  = (char *)(&first_word);
                                        value = bitTOint(p);
                                        code_img[img_counter].code_word = value;
                                        img_counter += L;
                                    }
                                    else 
                                    {
                                        strcpy(error, err3);
                                        there_is_error = TRUE;
                                        printf("\nIn line %d there is an error- Invalid addressing mode.\n", code_counter);
                                        continue;
                                    }
                                }
                                else 
                                {
                                    L = one_operand(second, kind_of_operand2);    /*fill the code_word*/
                                    p  = (char *)(&first_word);
                                    value = bitTOint(p);
                                    code_img[img_counter].code_word = value;
                                    if (kind_of_operand2 == 0)  /*miun miyadi, we know the second word*/
                                    {
                                        img_counter ++ ;
                                        code_img[img_counter].address = IC + img_counter;
                                        p  = (char *)(&second_word);
                                        value = bitTOint(p);
                                        code_img[img_counter].code_word = value;
                                        img_counter ++;
                                    }
                                    else
                                        img_counter += L;
                                }
                            } 
                            else 
                            {
                                strcpy(error, err3);
                                there_is_error = TRUE;
                                printf("\nIn line %d there is an error- Invalid addressing mode.\n", code_counter);
                                continue;
                            }
                        }
                        else 
                        {
                            printf("\nIn line %d there is an error- %s.\n", code_counter, error);   /*err5 */
                            continue;
                        }
                    } 
                    else
                    {
                        printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                        continue;
                    }
                    break;
                }
                case 10: case 11: case 12:    /*jmp,bne,jsr */
                {
                    char second[N];
                    int kind_of_operand2;
                    if (there_is_label)
                    {
                        if (!thereISlabel(label, IC+img_counter))
                            continue;
                        symbol_table[symbol_counter-1].label_type = 4 ;
                    }
                    strcpy(second, first_operand(command));
                    kind_of_operand2= kind_operand(second);
                    if (!strcmp(error,err0))
                    {
                        emp = check_empty(command);
                        if (emp)
                        {
                            code_img[img_counter].address = IC + img_counter;
                            first_word.opcode = act_table1[num_of_command-1].define;
                            first_word.funct = act_table2[num_of_command-1].define;
                            first_word.ARE = 4;     /*A=1,R=0,E=0*/
                            first_word.reg_source = 0;
                            first_word.type_source = 0;
                            if (kind_of_operand2 != 0 && kind_of_operand2 != 3)
                            {
                                char * p;
                                int value;
                                int L;
                                L = word_j(kind_of_operand2);    /*fill the first_word*/
                                p  = (char *)(&first_word);
                                value = bitTOint(p);
                                code_img[img_counter].code_word = value;
                                img_counter += L ;
                            }
                            else 
                            {
                                strcpy(error, err3);
                                there_is_error = TRUE;
                                printf("\nIn line %d there is an error- Invalid addressing mode.\n", code_counter);
                                continue;
                            }
                        }
                        else 
                        {
                            printf("\nIn line %d there is an error- %s.\n", code_counter, error);   /*err5 */
                            continue;
                        }
                    } 
                    else
                    {
                        printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                        continue;
                    }
                    break;
                }
                case 15: case 16:   /*rts, stop */
                    emp = check_empty(command);
                    if (emp)
                    {
                        char * p;
                        int value;
                        if (there_is_label)
                        {
                            if (!thereISlabel(label,IC+img_counter))
                                continue;
                            symbol_table[symbol_counter-1].label_type = 4 ;
                        }
                        code_img[img_counter].address = IC + img_counter;
                        first_word.opcode = act_table1[num_of_command-1].define;
                        first_word.funct = act_table2[num_of_command-1].define;
                        first_word.ARE = 4;     /*A=1,R=0,E=0*/
                        first_word.reg_source = 0;
                        first_word.type_source = 0;
                        first_word.reg_target = 0;
                        first_word.type_target = 0;
                        p  = (char *)(&first_word);
                        value = bitTOint(p);
                        code_img[img_counter].code_word = value;
                        img_counter ++ ;
                    }
                    else
                    {
                        printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                        continue;
                    }
                    break;                  
                default:
                    strcpy(error,err2);
                    there_is_error = TRUE;
                    printf("\nIn line %d there is an error- %s.\n", code_counter, error);
                    break;
            }
        }
    }       /*END OF FILE */
    if (there_is_error)
        return FALSE;       /*to the next file*/
    return TRUE;
}
