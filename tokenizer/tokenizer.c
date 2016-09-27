/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
    /*Saves a copy of the token stream inside the object*/
    char *token;

    /*Saves the index the algorithm left of on in the string when checking. */
    int index;

    /*Holds current token id for printouts*/
    int id;
    const char *tags[5];

    /*Has the start and end points for the substring that will become a token printout*/
    int start;
    int end;

};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 *
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate( char * ts ) {

    printf("\nYou are now trying to create a tokenizer with %s\n",ts);

  /*Returns null if the token string length is zero
    there is no need to create the object in such a case*/
  if(strlen(ts)<=0){
    return NULL;
  }

  /*Allocates memory and creates a pointer for the Tokenizer object when the string length>0*/
  else{

    /*Initialize all the variables in the token object*/
    TokenizerT *t;
    t=malloc(sizeof(TokenizerT));
    t->token=ts;
    t->index=0;
    t->id=0;
    t->tags[0]="Decimal";
    t->tags[1]="Float";
    t->tags[2]="Octal";
    t->tags[3]="Hex";
    t->tags[4]="Error";

    return t;
  }

  //I NEED TO MALLOC IN THIS
  //TokenizerT *tPoint=0;
  //tPoint=malloc(sizeof(ts));


}

/*Advances the objects index counter past white space so the
  algorithm can pick up back on an integer*/
int whiteSpace(TokenizerT *tk){
    int c = tk->index;
    printf("\n C is now: %d",c);
    if(tk->token[c]=='\0'){
        /*Reaches the null terminator, can finish*/
        printf("\n IT REACHED THE END");
        return -1;
    }
    if(tk->token[c]=='\n'){
        /*Reaches the null terminator, can finish*/
        printf("\n ESCAPE CHARS WORK");
        return -1;
    }
    while(isspace(tk->token[c])){
        if(c==strlen((tk->token)+1)){
            printf("\n IT REACHED THE END22");
            return -1;
        }
        c+=1;
    }
    tk->index=c;
    tk->start=c;

    /*Goes to syntax check*/
    return 3;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) {
    free(tk);
}




/*
 * Checks if the potential token starts with a letter or a decimal point,
 * in which case it is not legal unless that letter follows the rules for hex
 * characters.
 */
int synCheck(TokenizerT *tk){

    if(!isdigit(tk->token[tk->index])){
        /*Goes to skipString*/
        return 5;
    }
    if(isspace(tk->token[tk->index])){
        /*Goes to white space in case syncheck ends up on a blank space anyway*/
        return 0;
    }

    /*Sets start value to the current index as the potential start of a new valid token*/
    tk->start=tk->index;
    printf("\nSCHECK: The start now equals the index %d",tk->index);

    /*Goes to typeCheck*/
    return 4;
}

int typeCheck(TokenizerT *tk){
    if(tk->token[tk->start]=='0'){
        if(tolower(tk->token[tk->start+1])=='x'){

            tk->index+=1;
            /*Goes to hexCheck*/
            return 8;
        }
        else if(tk->token[tk->start+1]=='\0'||isspace(tk->token[tk->start+1])){
            /*The integer 0 is by itself*/
            return 6;
        }
        else if(tk->token[tk->start+1]=='.'){
            return 6;
        }
        /*Goes to octalCheck*/
        return 7;
    }
    else if(isdigit(tk->token[tk->start])){
        /*Goes to float/decimal checker*/
        return 6;
    }

    /*Returns to whiteSpace*/
    return 0;
}

int hexCheck(TokenizerT *tk){
    /*Starts off this method on the x*/

    int c=tk->index;
    if(tk->token[tk->index+1]=='\0'||isspace(tk->token[tk->index+1])){
        /*Skips strings with 0x followed by nothing*/
        return 5;
    }
    c+=1;
    while(isalnum(tk->token[c])){
        if(isalpha(tk->token[c])){
            if(tolower(tk->token[c])!='a'
            &&tolower(tk->token[c])!='b'
            &&tolower(tk->token[c])!='c'
            &&tolower(tk->token[c])!='d'
            &&tolower(tk->token[c])!='e'
            &&tolower(tk->token[c])!='f'){
                /*The string contains a letter that is not a valid hex character*/
                /*Goes to skipString*/
                return 5;
            }// end if

        }//end if
        c+=1;
    }// end while

    if(isspace(tk->token[c])){
        tk->end=c;
        tk->index=tk->end;
        tk->id=3;
        return 1;
    }
    else if(tk->token[c]=='\0'){
        /*End is c-1 because terminator isn't copied over*/
        tk->end=c-1;
        tk->index=tk->end;
        tk->id=3;
        return 1;
    }

    return 0;
}

int octalCheck(TokenizerT *tk){

    int c=tk->index;
    while(isdigit(tk->token[c])){
        if(tk->token[c]=='8'||tk->token[c]=='9'){
            /*Skips string because the number is an octal with invalid integers*/
            return 5;
        }
        c+=1;
    }
    if(isalpha(tk->token[c])){
        return 5;
    }
    else if(isspace(tk->token[c])){
        tk->end=c;
        tk->index=tk->end;
        tk->id=2;
        return 1;
    }
    else if(tk->token[c]=='\0'){
        /*End is c-1 because terminator isn't copied over*/
        tk->end=c-1;
        tk->index=tk->end;
        tk->id=2;
        return 1;
    }
    return 0;
}

int decimalCheck(TokenizerT *tk){
    int c=tk->index;
    while(isdigit(tk->token[c])){
        c+=1;
    }
    if(tk->token[c]=='.'){
        c+=1;
        tk->index=c;
        /*Goes to float confirm*/
        return 2;
    }
    if(isspace(tk->token[c])||tk->token[c]=='\0'){
        tk->end=c-1;
        tk->index=tk->end;
        tk->id=0;

        /*Goes to print token*/
        return 1;
    }

    /*Goes to SkipString otherwise*/
    return 5;
}

int floatCheck(TokenizerT *tk){
    /*Starts off on a decimal number after the decimal point*/
    int c=tk->index;
    while(isdigit(tk->token[c])){
            c+=1;
    }
    if(isspace(tk->token[c])||tk->token[c]=='\0'){
        tk->end=c-1;
        tk->index=tk->end;
        tk->id=1;
        /*Goes to print token*/
        return 1;
    }
    if(tk->token[c]=='.'){
        /*Bad string with two decimal points*/
        return 5;
    }
    /*At this point it should reach the letter e*/
    if(tolower(tk->token[c])!='e'){
        printf("\nThe letter is not e:");
        return 5;
    }
    c+=1;
    if(tk->token[c]!='+'&&tk->token[c]!='-'){
        printf("\n NO PLUS AND MINUS:");
        return 5;
    }
    c+=1;
    while(isdigit(tk->token[c])){
            c+=1;
    }
    if(isspace(tk->token[c])||tk->token[c]=='\0'){
        tk->end=c-1;
        tk->index=tk->end;
        tk->id=1;
        /*Goes to print token*/
        return 1;
    }
    printf("\n IT REACHED THE END FOR WHATEVER REASON:");
    return 5;
}

/*Skips the invalid string to get to the next potential token*/
int skipString(TokenizerT *tk){
    int c=tk->index;
    while(!isspace(tk->token[c])){
        if(tk->token[c]=='\0'){
            tk->end=c-1;
            tk->index=tk->end;
            tk->id=4;
            /*Goes to print token*/
            return 1;
        }
        else if(strlen(tk->token)==tk->index){
            tk->end=c-1;
            tk->index=tk->end;
            tk->id=4;
            /*Goes to print token*/
            return 1;
        }
        c+=1;
    }
    tk->end=c-1;
    tk->index=tk->end;
    tk->id=4;
    /*Goes to print token*/
    return 1;
    //READD THE PRINTS LATER MARCUS
    // THIS LOGIC SOMEWHERE WAS CAUSING PROBLEMS, REINCLUDE FOR LATER

    //return 0; //It has reached white space and will now go to whiteSpace method
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk ) {
    int i=0;
    int s;
    int e;
    s=tk->start;
    e=tk->end;
    int size=((e+1)-(s+1)+2)+1;
    char *printout;
    printout=(char*)malloc(size * sizeof(char)); //[((e+1)-(s+1)+2)];
    //printf("%d",(((e+1)-(s+1)+2)));
    printf("\nStart is: %d\n End is: %d\n", s, e);

    for(s=s;s<e+1;s++){
        //printf("FAIL");
        printf("\nWe are now copying over %c to %c:",tk->token[s],printout[i]);
        printout[i]=tk->token[s];
        printf("\nPlacing the char into index %d: ",i);
        i+=1;
        //printf("Copying over: %s\n",tk->token[s]);
    }

    printf("\nThe last spot in the array is %d:\n",i);
    printout[i]='\0';
    int x = strlen(printout);
    //char *test=printout;
    printf("\nprintout is: %s",printout);
    printf("\nLength is %d",x);

    return printout;
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {

    /*The state defaults to checking for white space*/
    int state=0;
    int l;

    /*If argc==1, there are no tokens to print*/
    if(argc==1){
    printf("\nThere are no tokens to print.\n");
    return 0;
    }

    l=(strlen(argv[1])+1);
    printf("\nThe length of this array is: %d\n",l);

    /*Creates a pointer to a copy of argv that will be passed as into the tokenizer*/
    char a[strlen(argv[1])+1];
    strcpy(a,argv[1]);
    char *tkPrint;
    printf("The string is : %s ",a);
    //TokenizerT *t = malloc(sizeof(TokenizerT));
    TokenizerT *t=TKCreate(a);
    if(t!=NULL){
    printf("\nThe string is not null\n");
    }
    //int b = strlen((argv[1]+1));
    printf("\n The index is %d",t->index);

     while(state!=-1){
            printf("\n Switching to state: %d\n",state);
        switch(state){
            case 0://WHITESPACE skipper cycles through string
                state = whiteSpace(t);
                break;
            case 1://TKGETNEXTTOKEN attempts to get the next token after finding a decimal

                tkPrint=TKGetNextToken(t);
                printf("\n%s %s",t->tags[t->id],tkPrint);
                free(tkPrint);
                printf("\n The index finished at %d",t->index);
                t->index+=1;
                t->id=0;
                state=0;
                //state=-1; //TEMPORARY KLL STATE AFTER FIRST TOKEN
                break;
            case 2:
                state=floatCheck(t);
                break;
            case 3://SYNTAX CHECKER, makes sure it doesn't start with an incorrect letter
                state=synCheck(t);
                break;
            case 4://TYPECHECKER for decimal/float or octal
                state=typeCheck(t);
                break;
            case 5://STRING SKIPPER advances to next possible token after finding syntax error
                state=skipString(t);
                break;
            case 6://DECIMAL CHECKER confirms potential decimal numbers
                state=decimalCheck(t);
                break;
            case 7:
                state=octalCheck(t);//OCTAL CHECKER confirms potential octals
                break;
            case 8:
                state=hexCheck(t);
                break;

        }//end switch

     }//end while
     TKDestroy(t);

  return 0;
}
