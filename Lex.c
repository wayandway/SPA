#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define NO_KEYWORDS 7
#define ID_LENGTH 12

char id[ID_LENGTH];

enum tsymbol { tnull=-1,
    tconst, telse, tif, tint, treturn, tvoid, twhile
};

struct tokenType {
    int number;
    union {
        char id[ID_LENGTH];
        int num;
    } value;
};

char *keyword[NO_KEYWORDS] = {
    "const", "else", "if", "int", "return", "void", "while"
};

enum tsymbol tnum[NO_KEYWORDS] = {
    tconst, telse, tif, tint, treturn, tvoid, twhile
};

void lexicalError(int n)
{
    printf("*** Lexical Error : ");
    switch (n) {
        case 1 : printf("an identifier length must be less than 12.\n");
            break;
        case 2 : printf("next character must be &.\n");
            break;
        case 3 : printf("next character must be |.\n");
            break;
        case 4 : printf("invalid character!\n");
            break;
    }
};

int superLetter(char ch)
{
    if (isalpha(ch) || ch == '_') return 1;
    else return 0;
}

int superLetterOrDigit(char ch)
{
    if (isalnum(ch) || ch == '_') return 1;
    else return 0;
}

int hexValue(char ch)
{
    switch (ch) {
        case '0' : case '1' : case '2' : case '3' : case '4' :
        case '5' : case '6' : case '7' : case '8' : case '9' :
            return (ch - '0');
        case 'A' : case 'B' : case 'C' : case 'D' : case 'E' : case 'F' :
            return (ch - 'A' + 10);
        case 'a' : case 'b' : case 'c' : case 'd' : case 'e' : case 'f' :
            return (ch - 'a' + 10);
        default: return -1;
    }
}

struct tokenType scanner()
{
    struct tokenType token;
    int i, index;
    char ch, id[ID_LENGTH];

    token.number = tnull;
    
    do {
        while(isspace(ch = getchar()));
        if(superLetter(ch)) {
            i = 0;
            do {
                if(i < ID_LENGTH) id[i++] = ch;
                ch = getchar();
            } while (superLetterOrDigit(ch));
            if (i = ID_LENGTH) lexicalError(1);
            id[i] = '\0';
            ungetc(ch, stdin);
            
            for(index = 0; index<NO_KEYWORDS; index++)
                if(!strcmp(id, keyword[index])) break;
    
            if(index < NO_KEYWORDS)
                token.number = tnum[index];
            else {
                token.number = tint;
                strcpy(token.value.id, id);
            }
        }
    }while(token.number == tnull);
    return token;
}

int main(void)
{
    FILE *fp;
    int i = 0;
    struct tokenType t;
    if((fp=fopen("wdy\test.txt", "r")) == NULL) {
        printf("Error File Empty");
        exit(0);
    }
    do {
        t = scanner();
        printf("token%d(%d", ++i, t.number);
        if(t.number==4) printf(", %s )", t.value.id);
        else if(t.number ==5) printf(", %d )", t.value.num);
        else printf(")");
        
        if(i%2==0) printf("\n");
        else printf(" ");
    } while(t.number != teof);
    return 0;
}