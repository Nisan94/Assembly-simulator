#include "analyze.h"

extern mem_image code_img[MAX];
extern mem_image data_img[MAX];
extern symbol_struct * symbol_table;
extern ext_struct * ext_table;
extern int symbol_counter;
extern int code_counter;
extern int img_counter;
extern int ext_counter;
extern int there_is_error;
extern int ext_dynamic_size;
extern int max_dynamic_size;
fst_word first_word;                    /*global*/
word second_word;
word third_word;

/*All kind of errors that might occur*/
char err0[]= "OK";
char err1[]= "Invalid label name";
char err2[]= "Undefined command name";
char err3[]= "Invalid parameter"; 
char err4[]= "Missing parameter";
char err5[]= "Extraneous text after end of command";
char err6[]= "Missing comma";
char err7[]= "Multiple consecutive commas";
char err8[]= "Illegal comma";
char error[N];

char act1[]= "mov";
char act2[]= "cmp";
char act3[]= "add";
char act4[]= "sub";
char act5[]= "lea";
char act6[]= "clr";
char act7[]= "not";
char act8[]= "inc";
char act9[]= "dec";
char act10[]= "jmp";
char act11[]= "bne";
char act12[]= "jsr";
char act13[]= "red";
char act14[]= "prn";
char act15[]= "rts";
char act16[]= "stop";

/*The function receives a string and skips the white spaces.
Output: the index of the first char in the string that is not a white space.*/
int skip(char command[])
{
    int i;
    i=0;
    while ((command[i] == ' ' || command[i]== '\t') && i < strlen(command))       
    {
        i++;
    }
    return i;
}

/*The funtion receives a string and extract the first word (reads the chars till white space/end of the string), 
which suppose to be one of the 16 optional commands.*/
char * kind(char command[])
{
    int j, i;
    char *kind;
    char kindcom[N];
    char temp[N];
    kind= kindcom;
    j=0;
    i= skip(command);         /*stops if reach a character that is not space/tab, or reached the end of the command */
    while (command[i] != ' ' && command[i]  != '\t' && command[i] != '\0')
    {
        kindcom[j] = command[i];            /*copies the first characters in the command till reach a space/tab/the end */
        i++;
        j++;
    }
    kindcom[j] = '\0';
    if (kindcom[strlen(kindcom)-1] == ',')
    {
        strcpy(error,err8);
        there_is_error = TRUE;
    }
    strcpy(temp,command+i);
    strcpy(command, temp);
    return kind;
}

/*The function gets a string. If it's one of the 16 optional commands or a label or 
 a directive line- the function will return the number that represents it. 
 Else, it will return 0.*/
int num_command(char kind_of_command[])
{
    if (!strcmp(kind_of_command, "mov"))
        return 1;
    if (!strcmp(kind_of_command, "cmp"))
        return 2;
    if (!strcmp(kind_of_command, "add"))
        return 3;
    if (!strcmp(kind_of_command, "sub"))
        return 4;   
    if (!strcmp(kind_of_command, "lea"))
        return 5; 
    if (!strcmp(kind_of_command, "clr"))
        return 6; 
    if (!strcmp(kind_of_command, "not"))
        return 7;
    if (!strcmp(kind_of_command, "inc"))
        return 8;
    if (!strcmp(kind_of_command, "dec"))
        return 9;
    if (!strcmp(kind_of_command, "jmp"))
        return 10; 
    if (!strcmp(kind_of_command, "bne"))
        return 11;
    if (!strcmp(kind_of_command, "jsr"))
        return 12;
    if (!strcmp(kind_of_command, "red"))
        return 13;  
    if (!strcmp(kind_of_command, "prn"))
        return 14; 
    if (!strcmp(kind_of_command, "rts"))
        return 15;
    if (!strcmp(kind_of_command, "stop"))
        return 16;
    if (kind_of_command[strlen(kind_of_command)-1] == ':')      /*label*/
        return 17;
    if (!strcmp(kind_of_command, ".data"))
        return 18;
    if (!strcmp(kind_of_command, ".string"))
        return 19;
    if (!strcmp(kind_of_command, ".extern"))
        return 20;
    if (!strcmp(kind_of_command, ".entry"))
        return 21;
    return 0;            
}

/*The function gets a string and extract the first word (reads the chars till white space/end of the string), 
which suppose to be the first opernad of the command.
Relevant for a command line that is one of the 16 optional commands. */
char * first_operand(char command[])
{
    if (!strcmp(error,err0))
    {
        int i, j;
        char * first_operand;
        char temp[N];
        char first[N];
        first_operand = first;
        i= skip(command);
        j=0;
        if (i == strlen(command))
        {
            strcpy(error, err4);                    /*missing parameter*/
            there_is_error = TRUE;
            return first_operand;                             /*default value*/
        }
        if (command[i] == ',')
        {
            strcpy(error, err8);                 /*illegal comma*/
            there_is_error = TRUE;
            return first_operand;                             /*default value*/
        }
        if (command[i] == '"')
        {
            first[j] = command[i];
            j++;
            i++;
            while (command[i] != '"' && i <= strlen(command))
            {
                first[j] = command[i];            /*copies the first characters in the command till reach a space/tab/the end */
                i++;
                j++;
            }
            if (command[i] == '"')
            {
                first[j] = '"';
                j++;
            }
        }
        else 
            while (command[i] != ' ' && command[i] != ',' && command[i]  != '\t' && i < strlen(command))
            {
                first[j] = command[i];            /*copies the first characters in the command till reach a space/tab/the end */
                i++;
                j++;
            }
        first[j] = '\0';
        strcpy(temp,command+i);
        strcpy(command, temp);
        return first_operand;
    }
    return command;
}

/*The function gets a string and extract the first word (reads the chars till white space/comma/end of the string), 
which suppose to be the second opernad of the command.
Relevant for a command line that is one of the 16 optional commands. */
char * second_operand(char command[])
{
    if (!strcmp(error,err0))
    {
        int i, j;
        char * second_operand;
        char second[N];
        char temp[N];
        second_operand = second;
        i= skip(command);
        j=0;
        if (command[i] == ',')
        {
            i++;
            while ((command[i] == ' ' || command[i]== '\t') && i < strlen(command))       /*skip as long as this is a white space*/
            {
                i++;
            }
            if (i == strlen(command))
            {
                strcpy(error, err4);                    /*missing parameter  -    "add  r3,  ....    (nothing)"*/
                there_is_error = TRUE;
                return second_operand;                             /*default value*/
            }
            if (command[i] == ',')
            {
                strcpy(error, err7);                 /*multiple consecutive commas  -   "add  r3, , LIST"*/
                there_is_error = TRUE;
                return second_operand;                             /*default value*/
            }
            while (command[i] != ' '  && command[i]  != '\t' && command[i] != '\0' && command[i] != ',' && i < strlen(command))
            {
                second[j] = command[i];            /*copies the first characters in the command till reach a space/tab/the end */
                i++;
                j++;
            }
            if (command[i] == ',')
            {
                strcpy(error, err5);                    /*extra text -    "add  r3, r4 , ...    (nothing)"*/
                there_is_error = TRUE;
                return second_operand;                             /*default value*/
            }
        }
        else if (i == strlen(command))
        {
            strcpy(error, err4);                    /*missing parameter  -    "add  r3,  ....    (nothing)"*/
            there_is_error = TRUE;
            return second_operand;                             /*default value*/
        }
        else    /*reached to a char that is not a comma- missing comma */
        {
            strcpy(error, err6);
            there_is_error = TRUE;
            return second_operand;                             /*default value*/
        }
        second[j] = '\0';
        strcpy(temp,command+i);
        strcpy(command, temp);
        return second_operand;
    }
    return command;
}

/*The function gets a string and extract the first word (reads the chars till white space/comma/end of the string), 
which suppose to be a numerical value.
Relevant for a directive line- .data. */
char * data_operand(char command[])
{
    int i, j;
    char * data_operand;
    char second[N];
    char temp[N];
    data_operand = second;
    i= skip(command);
    j=0;
    if (command[i] == ',')
    {
        i++;
        while ((command[i] == ' ' || command[i]== '\t') && i < strlen(command))       /*skip as long as this is a white space*/
        {
            i++;
        }
        if (i == strlen(command))
        {
            strcpy(error, err5);
            there_is_error = TRUE;
            return error;                             /*default value*/
        }
        if (command[i] == ',')
        {
            strcpy(error, err7);                 /*multiple consecutive commas  -   "add  r3, , LIST"*/
            there_is_error = TRUE;
            return data_operand;                             /*default value*/
        }
        while (command[i] != ' '  && command[i]  != '\t' && command[i] != '\0' && command[i] != ',' && i < strlen(command))
        {
            second[j] = command[i];            /*copies the first characters in the command till reach a space/tab/the end */
            i++;
            j++;
        }
    }
    else if (i != strlen(command))   /*reached to a char that is not a comma- missing comma */
    {
        strcpy(error, err6);
        there_is_error = TRUE;
        return data_operand;                             /*default value*/
    }
    second[j] = '\0';
    strcpy(temp,command+i);
    strcpy(command, temp);
    return data_operand;
}


/*The function receives a string that is an operand (the first or the second) and returns a number
 between 0 to 4 that represents the type of the miun. 4- error.*/
int kind_operand (char operand[])
{
    if (!strcmp(error,err0))
    {
        int i;
        if (operand[0] == 'r' && (operand[1] >= 48 || operand[1] <= 55))
            return 3;
        if (operand[0] == '#')
        { 
            strcpy(operand,operand+1);
            if (data_check(operand))
                return 0;
            else
            {
                strcpy(error,err3);
                there_is_error = TRUE;
                return 4;   
            }
        }
        for (i=0; i < symbol_counter; i++)
            if (symbol_table[i].label == operand)   /*check if the label is in the symbols table*/
            {
                there_is_error = TRUE;
                printf("\nERROR- The symbol in line %d is already exists in the table.\n", code_counter);
                return 4;
            }
        if (legal_label(operand))
            return 1;
        if (operand[0] == '&')
        {
            strcpy(operand,operand+1);
            if (legal_label(operand))
                return 2; 
        }
        strcpy(error,err3);
        there_is_error = TRUE;
        return 4;       /*none of the above */
    }
    return 9;   /*default */
}

/*The function receives 2 strings that are the first and the seconds operands, and 2 numbers that represent their type of miun.
It is initializes the rest fields of the first word and some of the fields of the second word- if it's possible/needed.
Also, initializes the adresses' values in the code image.
Output: L- the number of words that are needed for the current command, in order to encode it in the memory.
Relevant to commands line with two operands.*/
int two_operand(char first_op[], char second_op[], int num1, int num2) /*num1,num2 returns from kind_operand function*/
{
    int L;
    L = 1;
    if (num1 == 3)      /*; source*/
    {
        first_word.type_source = 3;
        first_word.reg_source = first_op[1]-48;
    }
    if (num2 == 3)
    {
        first_word.type_target = 3;
        first_word.reg_target = second_op[1]-48;
    }
    if (num1 == 0)
    {
        first_word.type_source = 0;     /*miun miyadi */
        first_word.reg_source = 0;
        code_img[img_counter+1].address = IC + img_counter + 1;
        second_word.address = atoi(first_op);
        second_word.ARE = 4;        /*A=1,R=0,E=0*/
        L = 2;
    }
    if (num1 == 1)          /*label*/
    {
        first_word.type_source = 1;
        first_word.reg_source = 0;
        code_img[img_counter+1].address = IC + img_counter + 1;   /*we dont want to increase the img_counter `cause still in the first word*/
        L = 2;
         /*fill the decimal address of the second word in the next index;the code_word will be the the second_word-
          ARE and address of the label by the symbol table*/
    }
    if (num2 == 1)          /*label*/
    {
        first_word.type_target = 1;
        first_word.reg_target = 0;
        if (L < 2)       /*L=1; there is not second word yet*/
        {
            code_img[img_counter+1].address = IC + img_counter + 1; /*the index after the first word*/
            L = 2;
        }
        else            /*L=2;there is a second word that is related to the first opernad*/
        {
            L = 3;
            code_img[img_counter+2].address = IC + img_counter + 2;  /*after the assigment- go to the next word*/
        }
    }
    if (num2 == 0)
    {
        first_word.reg_target = 0;
        first_word.type_target = 0;
        if (L == 1)     /*first operand is an register */
            code_img[img_counter+1].address = IC + img_counter + 1;
        else if (L == 2)
            code_img[img_counter+2].address = IC + img_counter + 2;
        third_word.address = atoi(second_op);
        third_word.ARE = 4;        /*A=1,R=0,E=0*/
        L ++;
    }
    return L;
}


/*The function receives a string of the first operan and a number that represents its' type of miun.
It is initializes the rest fields of the first word and some of the fields of the second word- if it's possible/needed.
Also, initializes the adresses' values in the code image- when necessary.
Output: L- the number of words that are needed for the current command, in order to encode it in the memory.
Relevant to commands line with one operand.*/
int one_operand(char second_op[], int num2) /*num1,num2 returns from kind_operand function*/
{
    int L;
    L = 1;
    if (num2 == 3)
    {
        first_word.type_target = 3;
        first_word.reg_target = second_op[1];
    }
    if (num2 == 1)          /*label*/
    {
        first_word.type_target = 1;
        first_word.reg_target = 0;
        code_img[img_counter+1].address = IC + img_counter + 1; /*the index after the first word*/
        L = 2;
    }
    if (num2 == 0)
    {
        first_word.type_target = 0;
        first_word.reg_target = 0;
        second_word.ARE = 4;
        second_word.address = atoi(second_op);
        L = 2;
    }
    return L;
}

/*The function a number that represents the type of miun of the label in the command.
It is initializes the rest fields of the first word and a fields of the second word- if it's needed.
Also, initializes the address value in the code image.
Output: 2- the number of words that are needed for the current command, in order to encode it in the memory.
Relevant to the commands: jmp, jsr, bne.*/
int word_j(int num2)
{
    first_word.type_target = num2;
    first_word.reg_target = 0;
    code_img[img_counter+1].address = IC + img_counter + 1;
    if (num2 == 2)
        second_word.ARE = 4;        /*A=1**/
    return 2;
}


/*The function receives the name of an external label and the current address it apears at.
The function adds those to the ext_table which is used in the main function for the .ext file.*/
void addToExtTable (char label[], int codeAdd)
{
    if (ext_counter == ext_dynamic_size)     /*the dynamic space is full*/
    {
        ext_struct * new_size;
        new_size = (ext_struct *)realloc(symbol_table,(max_dynamic_size+START_SIZE_OF_SYMBOL_TABLE)*sizeof(ext_struct));
        if (!new_size)
        {
            there_is_error = TRUE;
            printf("\nMemory reallocation for the ext table- FAILED\n");
            exit(0);
        }
        ext_table = new_size;
        ext_dynamic_size += START_SIZE_OF_SYMBOL_TABLE; /*each time the space is full- adding 20*/
    }
    strcpy(ext_table[ext_counter].label,label); /*add the label to the table. kind of command=the name of the label */
    ext_table[ext_counter].codeAddress = codeAdd;
    ext_counter ++;
}


/*The function converts integer to a string, and returns a pointer to it.*/
char * itoa (int n)
{
    int i, sign;
    char s[N];
    char * conv;
    conv = s;
    if((sign=n)<0)
        n = (-1)*n;
    i = 0;
    do {
        s[i++] = n % 10 + '0';
    } while ((n/=10)>0);
    if(sign<0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
    return conv;
}

/*This function is an auxiliary function for the itoa function.
It reverse the order of the characters in the string.*/
void reverse(char s[])
{
    int c, i, j;
    for (i=0, j=strlen(s)-1; i<j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}


/*The function gets a string, and checks is it is a proper label (synbol) by the roles in page 29 of the maman.
Output: TRUE if it is a label, else- FALSE.*/
int legal_label(char label[])
{
    int check, check2;
    check = ((label[0] >= 65 && label[0] <= 90) || (label[0] >= 97 && label[0] <= 122)) && strlen(label) <= MAX_NAME;
    if (check) /*first char- big/small; MAX-31*/
    {   
        int i;
        char * r[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
        for (i=0; i<8;i++)
        {
            if (!strcmp(label,r[i]))    /*the label is equal to a  name-ERROR */
            {
                return FALSE;
            }
        }
        for (i=0; i<16; i++)
            if (!strcmp(label,act1) && !strcmp(label,act2) && !strcmp(label,act3) && !strcmp(label,act4) && !strcmp(label,act5) && !strcmp(label,act6) && !strcmp(label,act7) && !strcmp(label,act8) && !strcmp(label,act9) && !strcmp(label,act10) && !strcmp(label,act11) && !strcmp(label,act12) && !strcmp(label,act13) && !strcmp(label,act14) && !strcmp(label,act15) && !strcmp(label,act16))
            {
                return FALSE;
            }
        if (!strcmp(label,"extern") && !strcmp(label,"entry") && !strcmp(label,"data") && !strcmp(label,"string"))
        {
            return FALSE;
        }
        for (i=0; i< strlen(label); i++)
        {
            check2 = (label[i] >= 65 && label[i] <= 90) || (label[i] >= 97 && label[i] <= 122) || (label[i] >= 48 && label[i] <= 57);
            if (!check2)
                return FALSE;
        }
    }
    else
        return FALSE;
    return TRUE;
}

/*The function receives a string and a number. It checks if the string is a label, using the above function- legal_label.
Then, if it does- adds it to the symbol_table with the received number, which is the address of the symbol.*/
int thereISlabel(char kind_of_command[], int C)  /*return true if the label was added to the table succesfully*/
{
    int i;
    if (!legal_label(kind_of_command))
    {
        strcpy(error, err1);
        there_is_error = TRUE;
        printf ("\nIn line %d there is an error- %s.\n", code_counter, error);
        return FALSE;
    }
    for (i=0; i< symbol_counter; i++)       /*check if the label is in the table already */
        if (!strcmp(symbol_table[i].label, kind_of_command)) /*without : */
        {
            there_is_error = TRUE;
            printf("\nERROR- The symbol in line %d is already exists in the table.\n", code_counter);
            return FALSE;
        }
    if (symbol_counter == max_dynamic_size)     /*the dynamic space is full*/
    {
        symbol_struct * new_size;
        new_size = (symbol_struct *)realloc(symbol_table,(max_dynamic_size+START_SIZE_OF_SYMBOL_TABLE)*sizeof(symbol_struct));
        if (!new_size)
        {
            there_is_error = TRUE;
            printf("\nMemory reallocation for the symbols table- FAILED\n");
            exit(0);
        }
        symbol_table = new_size;
        max_dynamic_size += START_SIZE_OF_SYMBOL_TABLE; /*each time the space is full- adding 20*/
    }
    strcpy(symbol_table[symbol_counter].label,kind_of_command); /*add the label to the table. kind of command=the name of the label */
    symbol_table[symbol_counter].decimalAddress = C;
    symbol_counter ++;
    return TRUE;
}


/*At the end, when all the necessary parts of the command have been readen, this function checks if there is any
 extraneous text that is unnecessary. */
int check_empty(char command[])
{
    int i;
    for (i=0; i < strlen(command); i++)
    {
        if(command[i] != ' ' && command[i] != '\t' )
        {
            strcpy(error, err5);        /*extra*/
            there_is_error = TRUE;
            return FALSE;
        }
    }
    if (strcmp(error,err0))
    {
        return FALSE;
    }
    return TRUE;
}


/*The function receives a pointer to a word (first/second/third) and converts it to an integer.*/
int bitTOint(char * p)
{
    unsigned int value;
    value = 0;
    if ((*p)<0)
    {
        value += ((*p)+256);        /*add the 0-7 bits*/
    }
    else 
        value += (*p);
    value += ((*(p+1))<<8);         /*add the 8-15 bits*/
    value = ((value<<16)>>16);      /*get read from the most left 16 bits- relevant if their value is 1.*/
    value += ((*(p+2))<<16);        /*add the 16-23 bits*/
    return value;
}


/*The function receives a string and checks if it represents a real integer 
(only -/+ at first, and other characters that are numbers 0-9). */
int data_check(char data[])
{
    int i, minus;
    i = 0;
    minus = FALSE;
    if (data[i] == '-' || data[i] == '+')  /*data[0] */
    {
        if(data[i] == '-')
            minus = TRUE;
        strcpy(data, data+1);
    }
    while(isdigit(data[i]) && i < strlen(data))
    {
        i++;
    }
    if (!isdigit(data[i]) && i < strlen(data))
    {
        if (i == strlen(data)-1)
            strcpy(error,err5);
        else strcpy(error,err3);
        there_is_error = TRUE;
        return FALSE;
    }
    if(minus)
        strcpy(data, itoa(atoi(data)*(-1)));
    return TRUE;
}
