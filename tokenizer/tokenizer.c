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
    const char *tags[6];

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
    t->tags[4]="Invalid";
    t->tags[5]="Zero";

    return t;
  }

}

/*Advances the objects index counter past white space so the
  algorithm can pick up back on an integer*/
int whiteSpace(TokenizerT *tk){
    int c = tk->index;
    if(tk->token[c]=='\0'){
        /*Reaches the null terminator, can finish*/
        return -1;
    }
    while(isspace(tk->token[c])){
        if(c==strlen((tk->token)+1)){
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
        /*Goes to hexErrorOut because the char is not valid*/
        return 10;
    }
    if(isspace(tk->token[tk->index])){
        /*Goes to white space in case syncheck ends up on a blank space anyway*/
        return 0;
    }

    /*Sets start value to the current index as the potential start of a new valid token*/
    tk->start=tk->index;

    /*Goes to typeCheck*/
    return 4;
}

int typeCheck(TokenizerT *tk){
    int c=tk->index;
    if(tk->token[tk->start]=='0'){
        if(tolower(tk->token[tk->start+1])=='x'){

            tk->index+=1;
            /*Goes to hexCheck*/
            return 8;
        }
        else if(tk->token[tk->start+1]=='\0'||isspace(tk->token[tk->start+1])){
            /*Zero is by itself. Goes to print token*/
            tk->end=c;
            tk->index=tk->end;
            tk->id=5;
            return 1;

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
        tk->id=5;
        tk->end=c-1;
        return 11;
    }
    c+=1;
    /*A keeps track of the number of chars after '0x'*/
    int a=1;
    while(isalnum(tk->token[c])){
        if(isalpha(tk->token[c])){
            if(tolower(tk->token[c])!='a'
            &&tolower(tk->token[c])!='b'
            &&tolower(tk->token[c])!='c'
            &&tolower(tk->token[c])!='d'
            &&tolower(tk->token[c])!='e'
            &&tolower(tk->token[c])!='f'){
                /*The string contains a letter that is not a valid hex character*/
                /*Goes to hexError out*/
                if(a>1){
                  tk->index=c;
                  tk->end=c-1;
                  tk->id=3;
                  return 11;
                }


                tk->end=tk->index-1;
                tk->id=5;
                return 11;

            }// end if

        }//end if
        c+=1;
        a+=1;
    }// end while

    if(isspace(tk->token[c])){
        tk->end=c;
        tk->index=tk->end;
        tk->id=3;
        /*Goes to print state for complete tokens*/
        return 1;
    }
    else if(tk->token[c]=='\0'){
        /*End is c-1 because terminator isn't copied over*/
        tk->end=c-1;
        tk->index=tk->end;
        tk->id=3;
        /*Goes to print state for complete tokens*/
        return 1;
    }

    return 0;
}

int octalCheck(TokenizerT *tk){

    int c=tk->index;
    int a=1;
    while(isdigit(tk->token[c])){
        if(tk->token[c]=='8'||tk->token[c]=='9'){
            /*Skips string because the number is an octal with invalid integers*/
            if(a==2){
              tk->index=c;
              tk->end=c-1;
              tk->id=5;
              return 11;
            }
            tk->index=c;
            tk->end=c-1;
            tk->id=2;
            return 11;

        }
        c+=1;
        a+=1;
    }
    if(!isdigit(tk->token[c])){
        if(a==2){
          tk->index=c;
          tk->end=c-1;
          tk->id=5;
          return 11;
        }
      tk->index=c;
      tk->end=c-1;
      tk->id=2;
      return 11;
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
    if(tk->token[c]=='.'||tolower(tk->token[c])=='e'){
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
    tk->end=c-1;
    tk->index=c;
    tk->id=0;
    return 11;
}

int floatCheck(TokenizerT *tk){

    /*Starts off on the character after the first decimal point found, or it starts on e*/
    int c=tk->index;
    if(!isdigit(tk->token[c])){
        /*Bad string with no number after the first decimal*/
        if(tk->token[c]!='+'&&tk->token[c]!='-'){
            tk->index=c-1;
            tk->end=c-2;
            if(tk->token[tk->start]=='0'){
                tk->id=5;
            }
            else{
                tk->id=0;
            }
            return 11;
        }

    }

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
        tk->index=c;
        tk->end=c-1;
        tk->id=1;
        return 11;
    }
    /*At this point it should reach the letter e*/
    if(tolower(tk->token[c])!='e'){

        if(tk->token[c-1]=='.'){

            tk->index=c-1;
            tk->end=c-2;
            tk->id=0;
            return 11;
        }
        tk->index=c;
        tk->end=c-1;
        tk->id=1;
        return 11;
    }
    c+=1;
    if(!isdigit(tk->token[c])&&tk->token[c]!='+'&&tk->token[c]!='-'){
        tk->index=c-1; //was c-1
        tk->end=c-2;
        tk->id=1;
        return 11;

    }

    /*When there is nothing after the +*/
    if((tolower(tk->token[c])=='e'||tk->token[c]=='+')&&!isdigit(tk->token[c+1])){
        tk->index=c-1;
        tk->end=c-2;
        tk->id=1;
        return 11;
    }
    /*Saves the potential endpoint in case of bad floats like 1.1e+++ or 1.1e*/
    tk->index=c-1;
    tk->end=c-2;
    c+=1;
    /*Checking for digits after e*/

    while(isdigit(tk->token[c])){
            c+=1;
    }

    /*Token is well formed and can print out correctly*/
    if(isspace(tk->token[c])||tk->token[c]=='\0'){
        tk->end=c-1;
        tk->index=tk->end;
        tk->id=1;
        /*Goes to print token*/
        return 1;
    }
    if(!isdigit(tk->token[c])){
        tk->index=c; //was c-1
        tk->end=c-1;
        tk->id=1;
        return 11;
    }

    tk->id=1;
    return 11;
}

/*
 * This is a method for outputting escape character hex values.
 */

int hexErrorOut(TokenizerT *tk){
    int c=tk->index;
    while(!isdigit(tk->token[c])&&!isspace(tk->token[c])&&tk->token[c]!='\0'){
        printf("\nInvalid [0x%02x]",tk->token[c]);
        c+=1;
    }

    /*Returns to white space method*/
    tk->index=c;
    return 0;
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
    printout=(char*)malloc(size * sizeof(char));

    for(s=s;s<e+1;s++){
        printout[i]=tk->token[s];
        i+=1;
    }

    printout[i]='\0';

    return printout;
}

/*
 * Mkain will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {

    /*The state defaults to checking for white space*/
    int state=0;

    /*If argc==1, there are no tokens to print*/
    if(argc==1){
        return 0;
    }

    /*Creates a pointer to a copy of argv that will be passed as into the tokenizer*/
    char a[strlen(argv[1])+1];
    strcpy(a,argv[1]);
    char *tkPrint;
    TokenizerT *t=TKCreate(a);

     while(state!=-1){

        switch(state){
            case 0://WHITESPACE skipper cycles through string
                state = whiteSpace(t);
                break;
            case 1://TKGETNEXTTOKEN attempts to get the next token after finding a decimal
                tkPrint=TKGetNextToken(t);
                printf("\n%s %s",t->tags[t->id],tkPrint);
                free(tkPrint);
                t->index+=1;
                t->id=0;
                state=0;
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
            case 6://DECIMAL CHECKER confirms potential decimal numbers
                state=decimalCheck(t);
                break;
            case 7:
                state=octalCheck(t);//OCTAL CHECKER confirms potential octals
                break;
            case 8:
                state=hexCheck(t);
                break;
            case 10: //Prints out hex errors
                state=hexErrorOut(t);
                break;
            case 11://printout to error path
                tkPrint=TKGetNextToken(t);
                printf("\n%s %s",t->tags[t->id],tkPrint);
                free(tkPrint);
                state=hexErrorOut(t);
                break;

        }//end switch

     }//end while
     TKDestroy(t);

  return 0;
}
