%{

#include <stdlib.h>
#include <stdio.h>


int stack[128];
int indentation = 0;

void indent(char* a);

    
%}

%option yylineno
%option noyywrap

%%

^[ ]*\n {}

^#.*\n {}

^[ ]*[^ \n]+ {

  int lastChar = yyleng - 1;
  indent(yytext) ;
  while ((lastChar >= 0) && (yytext[lastChar] != ' ')){
    unput(yytext[lastChar]);
    lastChar--;
  }
}
        
[ ]

and {printf("AND %s\n", yytext);
}

break {printf("BREAK %s\n", yytext);
}

def {printf("DEF %s\n", yytext);
}

elif {printf("ELIF %s\n", yytext);
}

else {printf("ELSE %s\n", yytext);
}

for {printf("FOR %s\n", yytext);
}

if {printf("IF %s\n", yytext);
}

not {printf("NOT %s\n", yytext);
}

or {printf("OR %s\n", yytext);
}

return {printf("RETURN %s\n", yytext);
}

while {printf("WHILE %s\n", yytext);
}

= {printf("ASSIGN %s\n", yytext);
}

\+ {printf("PLUS %s\n", yytext);
}

- {printf("MINUS %s\n", yytext);
}

\* {printf("TIMES %s\n", yytext);
}

\/ {printf("DIVIDEDBY %s\n", yytext);
}

== {printf("EQ %s\n", yytext);
}

!= {printf("NEQ %s\n", yytext);
}

> {printf("GT %s\n", yytext);
}

>= {printf("GTE %s\n", yytext);
}

\< {printf("LT %s\n", yytext);
}

\<= {printf("LTE %s\n", yytext);
}

\( {printf("LPAREN %s\n", yytext);}


\) {printf("RPAREN %s\n", yytext);}

, {printf("COMMA %s\n", yytext);}

: {
    printf("COLON %s\n", yytext);
}

"\n" {
    printf("NEWLINE \n");
}

True {
  printf ("BOOLEAN 1\n");
}

False {
  printf ("BOOLEAN 0\n");
}

([a-z]|[A-Z]|_)([a-z]|[A-Z]|[0-9]|_)* {printf("IDENTIFIER %s\n", yytext);
}

[0-9]+\.[0-9]*|[0-9]*\.[0-9]+ {printf ( "FLOAT %.6f\n", atof(yytext));
}

[0-9]+ {printf( "INTEGER %d\n", atoi(yytext));
}


<<EOF>> { indent((char *)""); return 0;}

%%

void indent(char * a) {
  int n = 0;
  while ((*a == ' ' )){
    n++;
    a++;
  }
  if (n == stack[indentation]){
    return;
  }
  if (n > stack[indentation]){
    printf("INDENT\n");
    stack[++indentation] = n;
    return;
  }
  while (n < stack[indentation]){
    printf("DEDENT\n");
    stack[indentation--] = '\0';
  }
}

int main(){
  yylex();
  return 0;
}
