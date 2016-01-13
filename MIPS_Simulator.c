#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_SYMBOL_TABLE_SIZE 100
#define INT_MIN     (-2147483647 - 1)

typedef enum {
 nullsym, nulsym, identsym, numbersym, plussym, minussym,
 multsym, slashsym, oddsym, eqlsym, neqsym, lessym,
 leqsym, gtrsym, geqsym, lparentsym, rparentsym, commasym,
 semicolonsym, periodsym, becomessym, beginsym, endsym, ifsym,
 thensym, whilesym, dosym, callsym, constsym, varsym,
 procsym, writesym, readsym, elsesym
} token_type;

typedef enum {
nullop,
LIT, OPR, LOD, STO, CAL, INC, JMP, JPC, SIO
}operation_type;

typedef enum{
OPR_RET, OPR_NEG, OPR_ADD, OPR_SUB, OPR_MUL,
 OPR_DIV, OPR_ODD, OPR_MOD, OPR_EQL, OPR_NEQ,
  OPR_LSS, OPR_LEQ, OPR_GTR, OPR_GEQ
}opcode_type;


typedef struct wordNode{
    char *word;
    token_type type;

    struct wordNode *next;
} wordNode;

typedef struct symbol {
    token_type kind;
    char name[12];
    int val;
    int level; //L value
    int addr;   //M value
    int filledBy; //level of the last updateTable function
}symbol;

typedef struct instruction{
    int op;
    int l;
    int m;
    }instruction;


wordNode *programLevel(wordNode *, symbol *, int *, int *, int *, int *, instruction *);
wordNode *blockLevel(wordNode *, symbol *, int *, int *, int *, int *, instruction *);
wordNode *statementLevel(wordNode *, symbol *, int *, int *, int *, int *, instruction *, int *);
wordNode *conditionLevel(wordNode *, symbol *, int *, int *, int *, int *, instruction *, int *);
wordNode *expressionLevel(wordNode *, symbol *, int *, int *, int *, int *, instruction *, int *);
wordNode *termLevel(wordNode *, symbol *, int *, int *, int *, int *, instruction *, int *);
wordNode *factorLevel(wordNode *, symbol *, int *, int *, int *, int *, instruction *, int *);

wordNode *skipAhead(wordNode *);
void createSymbol(token_type, char *, int , int *, int *, int *, symbol *);
void updateTable(token_type, char *, int , int *, int , int *, int *, symbol *);
void printInstructionList(instruction *, int *);
void emit(int , int , int , instruction *, int *, int *);
int valueLookUp(symbol *, char *, int *, char *);
void updateValue(symbol *, char *, int *, int *, int *);
int relationExecute(int, int, token_type);

//Assignment 1 Functions
void initPrint(instruction *code, FILE *output, int regAMT, int i)
{


    if(i < 10 && i < regAMT)
    {
    switch(code[i].op)
    {
    //LIT
    case 1:

        printf( "\n %i     lit   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n %i     lit   %i      %i", i, code[i].l, code[i].m);
        break;

    //OPR
    case 2:
        printf( "\n %i     opr   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n %i     opr   %i      %i", i, code[i].l, code[i].m);
        break;

    //LOD
    case 3:
        printf("\n %i     lod   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n %i     lod   %i      %i", i, code[i].l, code[i].m);
        break;

    //STO
    case 4:
        printf("\n %i     sto   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n %i     sto   %i      %i", i, code[i].l, code[i].m);
        break;

    //CAL
    case 5:
        printf( "\n %i     cal   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n %i     cal   %i      %i", i, code[i].l, code[i].m);
        break;

    //INC
    case 6:
        printf("\n %i     inc   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n %i     inc   %i      %i", i, code[i].l, code[i].m);
        break;

    //JMP
    case 7:
        printf("\n %i     jmp   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n %i     jmp   %i      %i", i, code[i].l, code[i].m);
        break;

    //JPC
    case 8:
        printf("\n %i     jpc   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n %i     jpc   %i      %i", i, code[i].l, code[i].m);
        break;

    //SIO
    case 9:
        printf("\n %i     sio   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n %i     sio   %i      %i", i, code[i].l, code[i].m);
        break;
        }

    }



    //When i is double digits
        else if(i < 99 && i < regAMT)
    {
    switch(code[i].op)
    {
    //LIT
    case 1:
        printf("\n%i     lit   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n%i     lit   %i      %i", i, code[i].l, code[i].m);
        break;

    //OPR
    case 2:
        printf( "\n%i     opr   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n%i     opr   %i      %i", i, code[i].l, code[i].m);
        break;

    //LOD
    case 3:
        printf("\n%i     lod   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n%i     lod   %i      %i", i, code[i].l, code[i].m);
        break;

    //STO
    case 4:
        printf("\n%i     sto   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n%i     sto   %i      %i", i, code[i].l, code[i].m);
        break;

    //CAL
    case 5:
        printf( "\n%i     cal   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n%i     cal   %i      %i", i, code[i].l, code[i].m);
        break;

    //INC
    case 6:
        printf( "\n%i     inc   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n%i     inc   %i      %i", i, code[i].l, code[i].m);
        break;

    //JMP
    case 7:
        printf("\n%i     jmp   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n%i     jmp   %i      %i", i, code[i].l, code[i].m);
        break;

    //JPC
    case 8:
        printf("\n%i     jpc   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n%i     jpc   %i      %i", i, code[i].l, code[i].m);
        break;

    //SIO
    case 9:
        printf("\n%i     sio   %i      %i", i, code[i].l, code[i].m);
        fprintf(output, "\n%i     sio   %i      %i", i, code[i].l, code[i].m);
        break;
        }

    }


}

void postPrint(instruction inst, int pc, int bp, int sp, FILE *output, int regAMT)
{


    if(inst.m <= 9)
    {

        printf("      %i", pc);
        fprintf(output, "      %i", pc);
    }
    else
    {

        printf("     %i", pc);
        fprintf(output, "     %i", pc);
    }

    if(pc <= 9)
    {

        printf("      %i", bp);
        fprintf(output, "      %i", bp);
    }
    else
    {

        printf("     %i", bp);
        fprintf(output, "     %i", bp);
    }

    if(bp <= 9)
    {

        printf("      %i", sp);
        fprintf(output, "      %i", sp);
    }
    else
    {

        printf( "     %i", sp);
        fprintf(output, "     %i", sp);
    }

    if(sp <= 9)
    {

        printf("       ");
        fprintf(output, "       ");
    }
    else
    {

        printf("      ");
        fprintf(output, "      ");
    }



}

void stackPrint(FILE *output, int sp, int *stack, int actIndex, int *actRec)
{
    int i = 1;
    int recCount = 0;
    while(i <= sp)
    {
       if(i == actRec[recCount] + 1 && recCount < actIndex)
            {
                fprintf(output, "| ");

                printf("| ");
                recCount++;
             }


        fprintf(output, "%i ", stack[i]);
        printf("%i ", stack[i]);
        i++;
    }
}

int base(int level, int b, int *stack)
{
    while (level > 0)
    {
        b = stack[b + 2]; // NOTE THE CHANGE! We now have + 2 !
        level--;
    }
 return b;
}

void ALU(instruction ir, int *bp, int *pc, int *sp, int *stack, int *actIndex, int *actRec)
{
    switch(ir.m)
    {
    //RET
    case 0:
        actRec[*actIndex] = -1;
        *actIndex -= 1;

        *sp = *bp - 1;
        *pc = stack[*sp + 4];
        *bp = stack[*sp + 3];
        break;

    //NEG
    case 1:
        stack[*sp] = stack[*sp] * -1;
        break;

    //ADD
    case 2:
        *sp = *sp - 1;
        stack[*sp] = stack[*sp] + stack[*sp + 1];
        break;

    //SUB
    case 3:
        *sp = *sp - 1;
        stack[*sp] = stack[*sp] - stack[*sp + 1];
        break;

    //MUL
    case 4:
        *sp = *sp - 1;
        stack[*sp] = stack[*sp] * stack[*sp + 1];
        break;

    //DIV
    case 5:
        *sp = *sp - 1;
        stack[*sp] = stack[*sp] / stack[*sp + 1];
        break;

    //ODD
    case 6:
        stack[*sp] = stack[*sp] % 2;
        break;

    //MOD
    case 7:
        *sp = *sp - 1;
        stack[*sp] = stack[*sp] % stack[*sp + 1];
        break;

    //EQL
    case 8:
        *sp = *sp - 1;
        stack[*sp] = stack[*sp] == stack[*sp + 1];
        break;

    //NEQ
    case 9:
        *sp = *sp - 1;
        stack[*sp] = stack[*sp] != stack[*sp + 1];
        break;

    //LSS
    case 10:
        *sp = *sp - 1;
        stack[*sp] = stack[*sp] < stack[*sp + 1];
        break;

    //LEQ
    case 11:
        *sp = *sp - 1;
        stack[*sp] = stack[*sp] <= stack[*sp + 1];
        break;

    //GTR
    case 12:
        *sp = *sp - 1;
        stack[*sp] = stack[*sp] > stack[*sp + 1];
        break;

    //GEQ
    case 13:
        *sp = *sp - 1;
        stack[*sp] = stack[*sp] >= stack[*sp + 1];
        break;
    }
}

void VM(instruction ir, int *bp, int *pc, int *sp, int *stack, int *halt, int *actIndex, int *actRec)
{


    switch(ir.op)
    {
    //LIT
    case 1:
    *sp = *sp + 1;
    actRec[*actIndex] = actRec[*actIndex] + 1;
    stack[*sp ] = ir.m;
        break;

    //OPR
    case 2:
    ALU(ir, bp, pc, sp, stack, actIndex, actRec);
        break;

    //LOD
    case 3:
        *sp+=1;
        stack[*sp] = stack[ base(ir.l, *bp, stack) + ir.m ];
        break;

    //STO
    case 4:
        stack[ base(ir.l, *bp, stack) + ir.m ] = stack[*sp ];
        *sp-=1;
        actRec[*actIndex] = actRec[*actIndex] - 1;
        break;

    //CAL
    case 5:
        stack[*sp + 1] = 0;
        stack[*sp + 2] = base(ir.l, *bp, stack);
        stack[*sp + 3] = *bp;
        stack[*sp + 4] = *pc;
        *bp = *sp + 1;
        *pc = ir.m;
        break;

    //INC
    case 6:
        if(*actIndex > 2)
        {
            *halt = 1;
            break;
        }

        *sp = *sp + ir.m;
        *actIndex += 1;
        actRec[*actIndex] = *sp;
        break;

    //JMP
    case 7:
        *pc = ir.m;
        break;

    //JPC
    case 8:
        if(stack[*sp] == 0 )
            *pc = ir.m;
        *sp-=1;
        break;

    //SIO
    case 9:
        if(ir.m == 0)
        {
            //print(stack[*sp];
            *sp-=1;
        }

        else if(ir.m == 1)
        {
            sp+=1;
            //read(stack[*sp];
        }

        else if(ir.m == 2)
        {
            *halt = 1;
        }

        break;

    }


}

void printCode(instruction ir)
{
    printf("%i %i %i\n", ir.op, ir.l, ir.m);
}

//Assignment 2 Functions
wordNode *createWord(char word[12], FILE *lexemeTable, FILE *lexemeList, int printTokenList)
{
    fprintf(lexemeTable,"%s\t", word);
    wordNode *newWord = (wordNode *)malloc(sizeof(wordNode));
    newWord->next = NULL;
    stringToNode(word, newWord);

    if(word[0] == '\0')
        newWord->type = nulsym;

    else if(isdigit(word[0]))
        newWord->type = numbersym;

    else
        {
            if(isalpha(word[0]))
                {
                        if(strcmp(word, "odd") == 0)
                            newWord->type = oddsym;

                        else if(strcmp(word, "begin") == 0)
                            newWord->type = beginsym;

                        else if(strcmp(word, "end") == 0)
                            newWord->type = endsym;

                        else if(strcmp(word, "if") == 0)
                            newWord->type = ifsym;

                        else if(strcmp(word, "then") == 0)
                            newWord->type = thensym;

                        else if(strcmp(word, "while") == 0)
                            newWord->type = whilesym;

                        else if(strcmp(word, "do") == 0)
                            newWord->type = dosym;

                        else if(strcmp(word, "call") == 0)
                            newWord->type = callsym;

                        else if(strcmp(word, "const") == 0)
                            newWord->type = constsym;

                        else if(strcmp(word, "var") == 0)
                            newWord->type = varsym;

                        else if(strcmp(word, "procedure") == 0)
                            newWord->type = procsym;

                        else if(strcmp(word, "write") == 0)
                            newWord->type = writesym;

                        else if(strcmp(word, "read") == 0)
                            newWord->type = readsym;

                        else if(strcmp(word, "else") == 0)
                            newWord->type = elsesym;

                        else
                            newWord->type = identsym;
                }


            else
            {
                    if(strcmp(word, "+") == 0)
                        newWord->type = plussym;

                    else if(strcmp(word, "-") == 0)
                        newWord->type = minussym;

                    else if(strcmp(word, "*") == 0)
                        newWord->type = multsym;

                    else if(strcmp(word, "/") == 0)
                        newWord->type = slashsym;

                    else if(strcmp(word, "=") == 0)
                        newWord->type = eqlsym;

                    else if(strcmp(word, ":=") == 0)
                        newWord->type = becomessym;

                    else if(strcmp(word, "!=") == 0)
                        newWord->type = neqsym;

                    else if(strcmp(word, "<") == 0)
                        newWord->type = lessym;

                    else if(strcmp(word, "<=") == 0)
                        newWord->type = leqsym;

                    else if(strcmp(word, ">") == 0)
                        newWord->type = gtrsym;

                    else if(strcmp(word, ">=") == 0)
                        newWord->type = geqsym;

                    else if(strcmp(word, "(") == 0)
                        newWord->type = lparentsym;

                    else if(strcmp(word, ")") == 0)
                        newWord->type = rparentsym;

                    else if(strcmp(word, ",") == 0)
                        newWord->type = commasym;

                    else if(strcmp(word, ";") == 0)
                        newWord->type = semicolonsym;

                    else if(strcmp(word, ".") == 0)
                        newWord->type = periodsym;

            }
        }


    fprintf(lexemeTable, "%i\n", newWord->type);

    if(printTokenList == 1)
    {
        fprintf(lexemeList, "%i ", newWord->type);
        printf("%i ", newWord->type);
        if(newWord->type == 2 || newWord->type == 3)
        {
            fprintf(lexemeList, "%s ", newWord->word);
            printf("%s ", newWord->word);
        }
    }



    return newWord;

}

wordNode *stringParser(char nextWord[100], FILE *lexemeTable, FILE *lexemeList, int printTokenList)
{
    int i = 0;
    wordNode *tempHead = malloc(sizeof(wordNode));
    tempHead->word = NULL;
    wordNode *tempCurrent;
    char newWord[12] = "";

    while(nextWord[i] != '\0')
    {
        if(nextWord[i] == '.' || nextWord[i] == ',' || nextWord[i] == ';' ||
            nextWord[i] == ':' || nextWord[i] == '+' || nextWord[i] == '-' ||
            nextWord[i] == '*' || nextWord[i] == '/' || nextWord[i] == '(' ||
            nextWord[i] == ')' || nextWord[i] == '=' || nextWord[i] == '<' ||
            nextWord[i] == '>')
            {

                    //Separate 0 to i - 1 from nextWord, save it to newWord, assuming 0 to i - 1 exists
                    if(i > 0)
                    {
                        strncpy(newWord, nextWord, i);
                        newWord[i] = '\0';


                        //add newWord to tempList
                        if(tempHead->word == NULL && newWord != NULL)
                        {
                            //if head has not been filled, fill head and set up current
                            tempHead = createWord(newWord, lexemeTable, lexemeList, printTokenList);
                            tempCurrent = tempHead;
                        }

                        else if(newWord != NULL)
                        {
                            //Otherwise, add to current
                            tempCurrent->next = createWord(newWord, lexemeTable, lexemeList, printTokenList);
                            tempCurrent = tempCurrent->next;
                        }



                        //Remove 0 to i - 1 from nextWord
                        wordSplitter(nextWord, i);

                        i = -1;

                    }

                    //Add i to templist and remove i from nextWord
                    if(nextWord[0] == ':' || nextWord[0] == '>' || nextWord[0] == '<' || nextWord[0] == '!' && nextWord[1] == '=')
                    {
                            //Since the equals character is actually two characters

                            if(tempHead->word == NULL)
                            {
                                newWord[0] = nextWord[0];
                                newWord[1] = nextWord[1];
                                newWord[2] = '\0';
                                tempHead = createWord(newWord, lexemeTable, lexemeList, printTokenList);
                                tempCurrent = tempHead;

                            }

                            else
                            {
                                newWord[0] = nextWord[0];
                                newWord[1] = nextWord[1];
                                newWord[2] = '\0';
                                tempCurrent->next = createWord(newWord, lexemeTable, lexemeList, printTokenList);
                                tempCurrent=tempCurrent->next;
                            }

                            wordSplitter(nextWord, 2);
                                if(nextWord[0] == NULL)
                            i = -1;

                    }


                    else
                    {
                        //Add i to tempList
                        strncpy(newWord, nextWord, 1);
                        newWord[1] = '\0';

                        if(tempHead->word == NULL)
                        {
                            tempHead = createWord(newWord, lexemeTable, lexemeList, printTokenList);
                            tempCurrent = tempHead;
                        }

                        else
                        {
                            tempCurrent->next = createWord(newWord, lexemeTable, lexemeList, printTokenList);
                            tempCurrent = tempCurrent->next;
                        }


                        wordSplitter(nextWord, 1);
                        i = -1;
                    }


            }

        i++;
    }

    if(tempHead->word == NULL && nextWord[0] != '\0')
    {
        //if head has not been filled, fill head and set up current
        tempHead = createWord(nextWord, lexemeTable, lexemeList, printTokenList);
        //tempHead->next = malloc(sizeof(wordNode));
        tempCurrent = tempHead;
    }

    else if (nextWord[0] != '\0')
    {
        //Otherwise, add to current
        tempCurrent->next = createWord(nextWord, lexemeTable, lexemeList, printTokenList);
        tempCurrent = tempCurrent->next;
    }


    return tempHead;
}

void wordSplitter(char *word, int target)
{
    char newWord[12] = "";

    int i = 0;

    while( word[target] != '\0')
    {
        newWord[i] = word[target];
        i++;
        target++;
    }

    newWord[i] = '\0';

     strcpy(word, newWord);
}


void stringToNode(const char word[12], wordNode *newWord)
{
    newWord->word = malloc(sizeof(char) * 12);
    strcpy(newWord->word, word);
}

void printList(wordNode *head, FILE * cleanInput)
{

    int tabCount = 0;
    int i = 0;
    int beginVal = 0;
    int newline = 1;
    wordNode *printer = head;
    while(printer->next != NULL)
    {
        //Subtract tab values
        if(printer->type == 33)
            tabCount--;

        else if(printer->type == 22)
        {
            beginVal -= 1;
            tabCount = beginVal;
        }

        //If newline is true, print tabs and make newline false
        if(newline == 1)
        {
            for(i = 0; i < tabCount; i++)
            fprintf(cleanInput,"\t");
            newline = 0;
        }


        //Print word. No spaces for punctuation, one space for everything else
        if(printer->type == 17 || printer->type == 18 || printer->type == 19)
            fprintf(cleanInput, "%s", printer->word);

        else
        fprintf(cleanInput, " %s", printer->word);

        //Add tab values

        if(printer->type == 21)
            {
                beginVal++;
                tabCount++;
            }

        else if(printer->type == 24 || printer->type == 26 || printer ->type == 33)
           tabCount++;

        //If newline, print newline character and make newline true
        if(printer->type == 18 || printer->type == 21 || printer->type == 24 ||
           printer->type == 26)
        {
            fprintf(cleanInput, "\n");
            newline = 1;
        }
        printer = printer->next;
    }


    if(printer->type == 17 || printer->type == 18 || printer->type == 19)
        fprintf(cleanInput, "%s", printer->word);

    else
        fprintf(cleanInput, " %s", printer->word);

}

void printListToConsole(wordNode *head, FILE * cleanInput)
{
    wordNode *printer = head;

    while(printer->next != NULL)
    {
        printf("%s | %i\n", printer->word, printer->type);
        printer = printer->next;
    }
}
void printListType(wordNode *head)
{
    wordNode *printer = head;
    while(printer->next != NULL)
    {
        printf("%i ", printer->type);
        printer = printer->next;
    }

    printf("%i ", printer->type);
}

//Assignment 3 Functions
void printError(int errorVal, int *halt)
{
    switch(errorVal)
    {
    case 1:
        printf("\nUse = instead of :=");
        *halt = 1;
        break;

    case 2:
        printf("\n= must be followed by a number.");
        *halt = 1;
        break;

    case 3:
        printf("\nIdentifier must be followed by =.");
        *halt = 1;
        break;

    case 4:
        printf("\nconst, var, procedure must be followed by identifier.");
        *halt = 1;
        break;

    case 5:
        printf("\nSemicolon or comma missing.");
        *halt = 1;
        break;

    case 6:
        printf("\nIncorrect symbol after procedure declaration.");
        *halt = 1;
        break;

    case 7:
        printf("\nStatement expected.");
        *halt = 1;
        break;

    case 8:
        printf("\nIncorrect symbol after statement part in block.");
        *halt = 1;
        break;

    case 9:
        printf("\nPeriod expected.");
        *halt = 1;
        break;

    case 10:
        printf("\nSemicolon between statements missing.");
        *halt = 1;
        break;

    case 11:
        printf("\nUndeclared identifier.");
        *halt = 1;
        break;

    case 12:
        printf("\nAssignment to constant or procedure is not allowed.");
        *halt = 1;
        break;

    case 13:
        printf("\nAssignment operator expected.");
        *halt = 1;
        break;

    case 14:
        printf("\ncall, read, write must be followed by an identifier.");
        *halt = 1;
        break;

    case 15:
        printf("\nCall of a constant or variable is meaningless.");
        *halt = 1;
        break;

    case 16:
        printf("\nthen expected.");
        *halt = 1;
        break;

    case 17:
        printf("\nSemicolon or } expected.");
        *halt = 1;
        break;

    case 18:
        printf("\ndo expected.");
        *halt = 1;
        break;

    case 19:
        printf("\nIncorrect symbol following statement.");
        *halt = 1;
        break;

    case 20:
        printf("\nRelational operator expected.");
        *halt = 1;
        break;

    case 21:
        printf("\nExpression must not contain a procedure identifier.");
        *halt = 1;
        break;

    case 22:
        printf("\nRight parenthesis missing.");
        *halt = 1;
        break;

    case 23:
        printf("\nThe preceding factor cannot begin with this symbol.");
        *halt = 1;
        break;

    case 24:
        printf("\nAn expression cannot begin with this symbol.");
        *halt = 1;
        break;

    case 25:
        printf("\nThis number is too large.");
        *halt = 1;
        break;

    case 26:
        printf("\nThis variable is unrecognized");
        *halt = 1;
        break;

    }

}

int relationCheck(wordNode *head)
{
    if(head->type == eqlsym || head->type == neqsym || head->type == lessym ||
       head->type == leqsym || head->type == gtrsym || head->type == geqsym )
        return 1;
    else
        return 0;
}
wordNode *moveForward(wordNode *head)
{
    printf("\n%s ->", head->word);
    head = head->next;
   printf("%s", head->word);
    return head;
}

wordNode *skipAhead(wordNode *head)
{
    printf("\nProceeding  until next semicolon");
    while(head->type != semicolonsym)
        head = moveForward(head);
    return head;
}
wordNode *programLevel(wordNode *head, symbol *symbolTable, int *halt, int *level, int * index, int *instruction, struct instruction *code)
{


    head = moveForward(head);
    head = blockLevel(head, symbolTable, halt, level, index, instruction, code);

    if(strcmp(head->word, ".") != 0)
    {
        while(head->next != NULL)
        {
            if(strcmp(head->word, ".") == 0)
                return head;
        }

        printError(9, halt);
    }



    return head;
}

wordNode *blockLevel(wordNode *head, symbol *symbolTable, int *halt, int *level, int * index, int *instruction, struct instruction *code)
{


    char tempName[12] = "";
    int varCount = 4;

    //produces no assembly code
    if(head->type == constsym)
    {
        do{
            head = moveForward(head);

            //Check for const name
            if(head->type != identsym)
            {
                printError(4, halt);
                if(head->type != eqlsym)
                    head = moveForward(head);
            }

            else
            {
                //Saves tempName
                strcpy(tempName, head->word);
                head = moveForward(head);
            }

            //Check for equal sign
            if(head->type != eqlsym)
            {
                printError(3, halt);
                    if(head->type != numbersym)
                        head = moveForward(head);
            }

            else
            head = moveForward(head);

            //Check for value
            if(head->type != numbersym)
            {
                printError(2, halt);
                    if(head->type != semicolonsym && head->type != commasym)
                        head = moveForward(head);
            }

            else
            {
                //Creates new Symbol with name, value, and type
                updateTable(constsym, tempName, atoi(head->word), level, 0, index, instruction, symbolTable);
                head = moveForward(head);
            }


        }while(head->type == commasym);

        //Check for semicolon
        if(head->type != semicolonsym)
        {
            printError(5, halt);
            if(head-> type != varsym && head->type != procsym && head->type != beginsym)
                head = moveForward(head);

        }

        else
        head = moveForward(head);

    }

    //Var count defines inc = var count + 4
    //When vars get recorded in symbolTable, they are given their index, starting at 4
    //and their level, based on what proc they are created in
    if(head->type == varsym)
    {
        do{
            varCount++;
            head = moveForward(head);

            //Check for variable name
            if(head->type != identsym)
            {
                printError(4, halt);
               if(head->type != eqlsym)
                    head = moveForward(head);
            }

            else
            {
                //Sends Name, level, type, Address, NO VALUE
                updateTable(varsym, head->word, NULL, level, varCount - 1, index, instruction, symbolTable);
                head = moveForward(head);
            }

            }while(head->type == commasym);

        //Check for semicolon
        if(head->type != semicolonsym)
        {
            printError(5, halt);
            if(head->type != procsym && head->type != beginsym)
                head = moveForward(head);
        }

        else
        head = moveForward(head);
    }

    if(head->type == procsym)
    {
        do{
            head = moveForward(head);
            //Check for name of procedure
            if(head->type != identsym)
            {
                if(head->type == semicolonsym)
                {
                    printError(4, halt);
                    head = moveForward(head);
                }

                else
                {
                    printError(6, halt);
                    while(head->next->type != constsym && head->next->type != varsym && head->next->type != procsym && head->next->type != beginsym)
                        head = moveForward(head);
                }

                head = blockLevel(head, symbolTable, halt, level, index, instruction, code);

            }

            else
            {
                //Create an unfinished jump, save the instruction index where it will be saved
                int tempCX = *instruction;
                emit(7, 0, 0, code, instruction, halt);
                *level = *level + 1;

                //Sends name, type, level, and address
                //Address being the instruction level the proc begins at
                updateTable(procsym, head->word, NULL, level, *instruction, index, instruction, symbolTable);

                head = moveForward(head);



                //head = moveForward(head);

                //Check for semicolon
                if(head->type != semicolonsym)
                {
                    printError(5, halt);
                    if(head->type != constsym && head->type != varsym && head->type != procsym && head->type != beginsym)
                        head = moveForward(head);
                }

                else
                head = moveForward(head);

                //Move to blockLevel
                head = blockLevel(head, symbolTable, halt, level, index, instruction, code);

                //Change the temporary jump to jump to the current instruction, skipping the subroutine until its called in main
                code[tempCX].m = *instruction;
            }


            //Check for semicolon after the procedure has ended
            if(head->type != semicolonsym)
            {
                printError(5, halt);
                if(head->type != beginsym)
                    head = moveForward(head);
            }

            else
            head = moveForward(head);

        }while(head->type == procsym);
    }


        //INC 4 + var count
        emit(6, 0, varCount, code, instruction, halt);
        head = statementLevel(head, symbolTable, halt, level, index, instruction, code, &varCount);


    return head;
}

wordNode *statementLevel(wordNode *head, symbol *symbolTable, int *halt, int *level, int * index, int *instruction, struct instruction *code, int *varMax)
{

    int statementValue = 0;
    int varIndex = 4;

    if(head->type == identsym)
    {
        //TEST TO SEE IF IDENTSYM IS A CONST!!!

        //Save name of identsym
        char tempName[12];
        strcpy(tempName, head->word);


        head = moveForward(head);
        if(head->type != becomessym)
        {
            if(head->type == eqlsym)
                printError(1, halt);

            else
            printError(3, halt);
        }

        head = moveForward(head);

        //Must send value as reference
        head = expressionLevel(head, symbolTable, halt, level, index, instruction, code, &statementValue);

        updateValue(symbolTable, tempName, index, &statementValue, level);

        //MaxVals increases?

        emit(4, *level - valueLookUp(symbolTable, tempName, index, "level"),
                valueLookUp(symbolTable, tempName, index, "addr"),
                code, instruction, halt);

    }

    else if(head->type == callsym)
    {

        head = moveForward(head);
        //CAL at head->word's address
        if(head->type != identsym)
        {

            printError(14, halt);
            //return head;
        }

        else
        {
            if(valueLookUp(symbolTable, head->word, index, "type") == varsym || valueLookUp(symbolTable, head->word, index, "type") == constsym)
            printError(15, halt);

            else
            emit(5, *level,  valueLookUp(symbolTable, head->word, index, "addr"), code, instruction, halt);

        }

        head = moveForward(head);
    }


    else if(head->type == beginsym)
    {

        head = moveForward(head);
        head = statementLevel(head, symbolTable, halt, level, index, instruction, code, varMax);

        while(head->type == semicolonsym)
        {

            head = moveForward(head);
            head = statementLevel(head, symbolTable, halt, level, index, instruction, code, varMax);

        }
        if(head->type != endsym)
        {
            printError(19, halt);
        }

        if(*level > 0)
        {
            emit(2, 0, 0, code, instruction, halt);
            *level = *level - 1;
        }

        else
            emit(9, 0, 2, code, instruction, halt);

        if(head->next->type == nulsym)
        {
            printError(9, halt);
            //return head;
        }


        head = moveForward(head);
    }

    else if(head->type == ifsym)
    {

        head = moveForward(head);
        head = conditionLevel(head, symbolTable, halt, level, index, instruction, code, &statementValue);

        if(head->type != thensym)
        {
            printError(16, halt);
            //return head;
        }

        head = moveForward(head);
        int ctemp = *instruction;
        emit(1, 0, statementValue, code, instruction, halt);
        emit(8, 0, 0, code, instruction, halt);
        head = statementLevel(head, symbolTable, halt, level, index, instruction, code, varMax);
        code[ctemp].m = *instruction;

        if(head->type == elsesym)
        {
            if(statementValue == 1)
                emit(1, 0, 0, code, instruction, halt);

            else if(statementValue == 0)
                emit(1, 0, 0, code, instruction, halt);

            ctemp = *instruction;
            emit(8, 0, 0, code, instruction, halt);
            head = statementLevel(head, symbolTable, halt, level, index, instruction, code, varMax);
            code[ctemp].m = *instruction;

        }
    }

    else if(head->type == whilesym)
    {
        int cx1 = *instruction;

        head = moveForward(head);
        head = conditionLevel(head, symbolTable, halt, level, index, instruction, code, &statementValue);

        int cx2 = *instruction;
        emit(8, 0, 0, code, instruction, halt);
        if(head->type != dosym)
        {
            printError(18, halt);
            //return head;
        }

        head = moveForward(head);
        head = statementLevel(head, symbolTable, halt, level, index, instruction, code, varMax);

        emit(7, 0, cx1, code, instruction, halt);
        code[cx2].m = *instruction;
    }

    else if(head->type == readsym)
    {
        head = moveForward(head);
        if(head->type != identsym)
        {
            printError(14, halt);
            if(head->type != semicolonsym)
                head = moveForward(head);
        }

        else if(valueLookUp(symbolTable, head->word, index, "type") == constsym ||
                valueLookUp(symbolTable, head->word, index, "type") == procsym)
        {
            printError(12, halt)
                moveForward(head);
        }

        else
        {
            emit(9, 0, 1, code, instruction, halt);
            emit(4, *level - valueLookUp(symbolTable, head->word, index, "level"),
                valueLookUp(symbolTable, head->word, index, "addr"),
                code, instruction, halt);
            head = moveForward(head);
        }

        if(head->type != semicolonsym)
        {
            printError(5, halt);
            if(head->type != beginsym && head->type != callsym && head->type != ifsym &&
               head->type != whilesym && head->type != readsym && head->type != identsym)
                head = moveForward(head);
        }

        else
            head = moveForward(head);


    }

    else if(head->type == writesym)
    {
        head = moveForward(head);
        if(head->type != identsym)
        {
            printError(14, halt);
            if(head->type != semicolonsym)
                head = moveForward(head);
        }

        else
        {
            if(valueLookUp(symbolTable, head->word, index, "val") == INT_MIN)
            {
                printError(11, halt);
            }

            else
            {
                token_type tempType = valueLookUp(symbolTable, head->word, index, "type");

                if(tempType == 29)
                emit(   3,
                        *level - valueLookUp(symbolTable, head->word, index, "level"),
                        valueLookUp(symbolTable, head->word, index, "addr"),
                        code, instruction, halt);

                else if(tempType == 28)
                emit( 1, 0, valueLookUp(symbolTable, head->word, index, "val"), code, instruction, halt);


                emit(9, 0, 0, code, instruction, halt);
                head = moveForward(head);
            }

        }

        if(head->type != semicolonsym)
        {
            printError(5, halt);
            if(head->type != beginsym && head->type != callsym && head->type != ifsym &&
               head->type != whilesym && head->type != readsym && head->type != identsym)
                head = moveForward(head);
        }

        else
            head = moveForward(head);


    }
    return head;
}

wordNode *conditionLevel(wordNode *head, symbol *symbolTable, int *halt, int *level, int * index, int *instruction, struct instruction *code, int *statementValue)
{
    //if(*halt == 1)
      //  return head;

    int conditionValue = 0;
    //Needs to be expanded so that it makes statementValue 0 or 1 if the condition is true or false
    if(head->type == oddsym)
    {
        head = moveForward(head);
        head = expressionLevel(head, symbolTable, halt, level, index, instruction, code, &conditionValue);
    }

    else
    {
        int val1 = 0;
        head = expressionLevel(head, symbolTable, halt, level, index, instruction, code, &val1);

        if(relationCheck(head) == 0)
        {
            printError(20 , halt);
            //return head;
        }
        token_type relationToken = head->type;

        head = moveForward(head);

        int val2;
        head = expressionLevel(head, symbolTable, halt, level, index, instruction, code, &val2);

        *statementValue = relationExecute(val1, val2, relationToken);

        switch(conditionType)
        {
            case(eqlsym):
                emit(OPR, 0, OPR_EQL, code, instruction, halt);
            break;

            case(neqsym):
                emit(OPR, 0, OPR_NEQ, code, instruction, halt);
            break;

            case(lessym):
                emit(OPR, 0, OPR_LSS, code, instruction, halt);
            break;

            case(leqsym):
                emit(OPR, 0, OPR_LEQ, code, instruction, halt);
            break;

            case(gtrsym):
                emit(OPR, 0, OPR_GTR, code, instruction, halt);
            break;

            case(geqsym):
                emit(OPR, 0, OPR_GEQ, code, instruction, halt);
            break;

        }

    }

    return head;
}

wordNode *expressionLevel(wordNode *head, symbol *symbolTable, int *halt, int *level, int * index, int *instruction, struct instruction *code, int *statementValue)
{
    //We know expression level and lower is going to combine into a single value eventually
    if(*halt == 1)
        return head;

    int expressionValue = 0;
    token_type addop;

    if(head->type == slashsym || head->type == multsym)
    {
        printError(24, halt);

    }
    if(head->type == plussym || head->type == minussym)
    {
        addop = head->type;
        head = moveForward(head);
        termLevel(head, symbolTable, halt, level, index, instruction, code, &expressionValue);
        if(addop == minussym)
        {
            //Multiply the value by -1
            expressionValue = expressionValue * -1;
            emit(OPR, 0, OPR_NEG, code, instruction, halt);
        }
    }

    else
    {
        head = termLevel(head, symbolTable, halt, level, index, instruction, code, &expressionValue);
    }



    *statementValue = *statementValue + expressionValue;

    while(head->type == plussym || head->type == minussym)
    {
        addop = head->type;
        head = moveForward(head);
        head = termLevel(head, symbolTable, halt, level, index, instruction, code, &expressionValue);
        if(addop == plussym)
        {
            //Add new value to value
            *statementValue = *statementValue + expressionValue;
            emit(OPR, 0, OPR_ADD, code, instruction, halt);
        }
        else
        {
            //Subtract new value from value
            *statementValue = *statementValue - expressionValue;
            emit(OPR, 0, OPR_SUB, code, instruction, halt);
        }
    }

    return head;
}

wordNode *termLevel(wordNode *head, symbol *symbolTable, int *halt, int *level, int * index, int *instruction, struct instruction *code, int *expressionValue)
{
    if(halt == 1)
        return head;
    int total = 0;
    int termValue = 0;

    token_type mulop;
    head = factorLevel(head, symbolTable, halt, level, index, instruction, code, &termValue);
    *expressionValue = termValue;

    while(head->type == multsym || head->type == slashsym)
    {
        mulop = head->type;
        head = moveForward(head);
        head = factorLevel(head, symbolTable, halt, level, index, instruction, code, &termValue);
        if(mulop == multsym)
        {
            *expressionValue = *expressionValue * termValue;
            emit(OPR, 0, OPR_MUL, code, instruction, halt);
        }
        else
        {
            *expressionValue = *expressionValue / termValue;
            emit(OPR, 0, OPR_DIV, code, instruction, halt);
        }
    }

    return head;
}

wordNode *factorLevel(wordNode *head, symbol *symbolTable, int *halt, int *level, int * index, int *instruction, struct instruction *code, int *termValue)
{
    if(halt == 1)
        return head;

    int factorValue = 0;
    if(head->type == identsym)
    {

        *termValue = valueLookUp(symbolTable, head->word, index, "val");

        if(*termValue == INT_MIN)
        {
            printError(11, halt);
        }
        token_type tempType = valueLookUp(symbolTable, head->word, index, "type");

        if(tempType == 29)
        emit(   3,
                *level - valueLookUp(symbolTable, head->word, index, "level"),
                valueLookUp(symbolTable, head->word, index, "addr"),
                code, instruction, halt);

        else if(tempType == 28)
        emit( 1, 0, *termValue, code, instruction, halt);

        head = moveForward(head);
    }

    else if(head->type == numbersym)
    {
        *termValue = atoi(head->word);

        emit(1, *level, *termValue, code, instruction, halt);
        head = moveForward(head);
    }
    else if(head->type == lparentsym)
    {
        head = moveForward(head);
        head = expressionLevel(head, symbolTable, halt, level, index, instruction, code, &factorValue);

        if(head->type != rparentsym)
        {
            printError( 22, halt);
            return head;
        }

        *termValue = factorValue;
        head = moveForward(head);
    }

    else
    {
        //Either 11 or 25
        printError(11, halt);
        //return head;
    }

    return head;
}

void createSymbol(token_type newKind, char newName[12], int newVal, int *newLevel, int *index, int *instruction, symbol *symbolTable)
{
    symbolTable[*index].kind = newKind;
    strcpy(symbolTable[*index].name, newName);
    symbolTable[*index].val = newVal;
    symbolTable[*index].level = *newLevel;
    symbolTable[*index].addr = *instruction;

}

void updateTable(token_type newKind, char newName[12], int newVal, int *newLevel, int newAddr, int *index, int *instruction, symbol *symbolTable)
{

    int tempIndex = *index;
    while(tempIndex > 0)
    {
        tempIndex--;

        if(strcmp(symbolTable[tempIndex].name, newName) == 0)
        {
            symbolTable[tempIndex].val = newVal;
            return;
        }
    }

    if(tempIndex == 0)
        createSymbol(newKind, newName, newVal, newLevel, index, &newAddr, symbolTable);


    *index = *index + 1;
}

void emit(int newOp, int newL, int newM, instruction *code, int *cx, int *halt)
{

    if(*cx > 500)
        printError(25, halt);

    else
    {
        code[*cx].op = newOp;
        code[*cx].l = newL;
        code[*cx].m = newM;

        *cx = *cx + 1;
    }


}

void printSymbolTable(symbol *symbolTable, int *index)
{


    int i = 0;
    printf("\n\nSymbol Table");
    for(i = 0; i < *index; i++)
    {
        if(symbolTable[i].kind == constsym)
            printf("\nName: %s \t| Value: %i \t| Kind: %i ", symbolTable[i].name, symbolTable[i].val, symbolTable[i].kind);

        else
            printf("\nName: %s \t| Value: %i \t| Kind: %i \t| L value: %i \t| M Value: %i", symbolTable[i].name, symbolTable[i].val, symbolTable[i].kind,
                                                                        symbolTable[i].level, symbolTable[i].addr);
    }

    printf("\n\n");

}

void printInstructionList(instruction *code, int *index)
{
    int i = 0;
    printf("\n\nInstruction Table");
    for(i = 0; i < *index; i++)
    {
        printf("\nindex: %i \t| type: %i \t| L: %i \t| M: %i", i, code[i].op, code[i].l,
                                                        code[i].m);
    }

    printf("\n\n");
}

int valueLookUp(symbol *symbolTable, char *word, int *index, char *function)
{

    int tempIndex = *index - 1;

    while(strcmp(word, symbolTable[tempIndex].name) != 0 && tempIndex >= 0)
    {
                tempIndex -= 1;
    }


    if(tempIndex == -1)
        return INT_MIN;

    else if(strcmp(function, "val") == 0)
        return symbolTable[tempIndex].val;

    else if(strcmp(function, "addr") == 0)
        return symbolTable[tempIndex].addr;

    else if(strcmp(function, "level") == 0)
        return symbolTable[tempIndex].level;

    else if(strcmp(function, "type") == 0 || strcmp(function, "kind") == 0  )
        return symbolTable[tempIndex].kind;


}

void updateValue(symbol *symbolTable, char *word, int *index, int *value, int *level)
{
    int tempIndex = *index - 1;

    if(tempIndex >= 0)
    {
        while(strcmp(word, symbolTable[tempIndex].name) != 0)
            tempIndex -= 1;


        if(symbolTable[tempIndex].filledBy <= *level)
        {
            symbolTable[tempIndex].val = *value;
            symbolTable[tempIndex].filledBy = *level;
        }
    }


}

int relationExecute(int val1, int val2, token_type relationToken)
{
    if(relationToken == eqlsym && val1 == val2)
        return 0;

    else if(relationToken == neqsym && val1 != val2)
        return 0;

    else if(relationToken == lessym && val1 < val2)
        return 0;

    else if(relationToken == leqsym && val1 <= val2)
        return 0;

    else if(relationToken == gtrsym && val1 > val2)
        return 0;

    else if(relationToken == geqsym && val1 >= val2)
        return 0;

    else
        return 1;
}


int main(int argc, char *argv[])
{
    //Open input file
    FILE *input = fopen("input.txt", "r");
    FILE *lexemeList = fopen("lexemelist.txt", "w");
    FILE *lexemeTable = fopen("lexemetable.txt", "w");
    FILE *cleanInput = fopen("cleaninput.txt", "w");


    int printTokenList = 0;
    int printAssemblyCode = 0;
    int printSymbols = 0;
    char *tester;

    printf("\nWould you like the Token List to be printed? y/n \n");
    scanf("%s", tester);

    if(strcmp(tester, "y") == 0 || strcmp(tester, "Y") == 0)
        printTokenList = 1;

    printf("\nWould you like the Symbol Table to be printed? y/n \n");
    scanf("%s", tester);

    if(strcmp(tester, "y") == 0 || strcmp(tester, "Y") == 0)
        printSymbols = 1;

    printf("\nWould you like the Assembly Code to be printed? y/n \n");
    scanf("%s", tester);

    if(strcmp(tester, "y") == 0 || strcmp(tester, "Y") == 0)
        printAssemblyCode = 1;


    printf("\n\n");

    fprintf(lexemeTable, "lexeme\ttoken type\n");

    char nextWord[100] = "";
    int commentSkip = 0;

    //Scan in first word for head node
    fscanf(input, "%s", nextWord);

    //create head,
    wordNode *head = createWord(nextWord, lexemeTable, lexemeList, printTokenList);
    head->next = malloc(sizeof(wordNode));


    wordNode *current = head;

    while(fscanf(input, "%s", nextWord) != EOF)
   {

        if(strcmp(nextWord, "/*") == 0)
                commentSkip = 1;

        if(commentSkip == 0)
        {

            current->next = stringParser(nextWord, lexemeTable, lexemeList, printTokenList);

            while(current->next != NULL)
            {
                    current = current->next;
            }

       }
        if(commentSkip == 1 && strcmp(nextWord, "*/") == 0)
            {
                commentSkip = 0;
            }
   }

    current->next = malloc(sizeof(wordNode));
    current->next->type = nulsym;

    //Assignment 3 begins
    symbol *symbolTable = malloc(sizeof(symbol) * 100);
    int currentLevel = 0;
    int currentInstruction = 0;
    int currentIndex = 0;
    int halt = 0;

    int stack[2000];
    instruction code[500];

    wordNode *newHead = malloc(sizeof(wordNode));
    newHead->type = nullsym;
    newHead->next = head;
    programLevel(newHead, symbolTable, &halt, &currentLevel, &currentIndex, &currentInstruction, code);

    if(halt == 1)
        return 0;


    printf("\nNo errors, program is syntactically correct");

    if(printSymbols == 1)
    printSymbolTable(symbolTable, &currentIndex);
    //printInstructionList(code, &currentInstruction);


    //Assignment 1 begins

    int regAMT = currentInstruction;
    int sp = 0, bp = 1, pc = 0;
    int activationIndex = -1;
    int activationRecourds[3] = {-1, -1, -1};

    instruction ir;
    ir.l = 0; ir.m = 0; ir.op = 0;

    FILE *output;
    output = fopen("stacktrace.txt", "w");

    int i = 0;

    if(printAssemblyCode == 1)
    {
        fprintf(output, "Line   OP    L      M");

        printf("\nInstruction List\n");
        printf("Line   OP    L      M");
        for( i = 0; i < regAMT; i++)
        initPrint(code, output, regAMT, i);

        i = 0;

        fprintf(output, "\n");
        fprintf(output, "                           pc     bp     sp     stack\n");
        fprintf(output, "Initial values             %i      %i      %i", pc, bp, sp);

        printf("\nStack Trace");
        printf("\n");
        printf("                           pc     bp     sp     stack\n");
        printf("Initial values             %i      %i      %i", pc, bp, sp);

        while(pc < regAMT && halt == 0)
        {

            initPrint(code, output, regAMT, pc);


            ir = code[pc];
            pc++;
            //printCode(ir);
            VM(ir, &bp, &pc, &sp, stack, &halt, &activationIndex, activationRecourds);

            postPrint(ir, pc, bp, sp, output, regAMT);
            stackPrint(output, sp, stack, activationIndex, activationRecourds);

        }
    }



}
