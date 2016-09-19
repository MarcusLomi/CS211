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
    char *token;
    int length;
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

  /*Returns null if the token string length is zero*/
  if(strlen(ts)<=0){
    return NULL;
  }
  /*Creates string array after dynamically allocating memory for it*/
  else{
    TokenizerT *t;
    t=malloc(sizeof(TokenizerT));
    t->token=ts;
    return t;
  }

  //I NEED TO MALLOC IN THIS
  //TokenizerT *tPoint=0;
  //tPoint=malloc(sizeof(ts));


}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) {


    //FILL THIS SHIT IN LAST FAM
    //THIS SHIT DESTROYS THE TOKENIZER OBJECT, USE FREE() ON THE POINTER

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

  return NULL;
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
  int state=-1;
  int i;

  /*If argc==1, there are no tokens to print*/
  if(argc==1){
    printf("\nThere are no tokens to print.\n");
    return 0;
  }

  /*Creates a pointer to a copy of argv that will be passed as into the tokenizer*/
  char a[strlen(argv[1])+1];
  strcpy(a,argv[1]);

  printf("THIS SHIT IS S: %s ",a);
  //TokenizerT *t = malloc(sizeof(TokenizerT));
  TokenizerT *t=TKCreate(a);
  if(t!=NULL){
    printf("\nTHIS SHIT WORKS MAN!");
  }


 /* TokenizerT *p=0;
  for(i=1;i<argc;i++){
     char *s;
     s=argv[i];
     p=TKCreate(argv[i]);
     while(state!=-1){

        switch(state){
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
        }
     }

  }*/

  return 0;
}
