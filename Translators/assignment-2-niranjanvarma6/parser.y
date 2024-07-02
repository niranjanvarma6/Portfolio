%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "hash.h"
#include "parser.h"

int _error = 0;
extern int yylex();
void yyerror(YYLTYPE* loc, const char* err);
char* concat(int n, ...);
struct hash* symbols;
char* final_code;

%}

%locations
%define parse.error verbose

%define api.pure full
%define api.push-pull push

%define api.value.type { char* }


%token IDENTIFIER
%token FLOAT INTEGER BOOLEAN
%token EQUALS PLUS MINUS TIMES DIVIDEDBY LPAREN RPAREN
%token EQ NEQ GT GTE LT LTE 
%token NOT AND OR
%token WHILE FOR BREAK COLON DEF 
%token IF ELIF ELSE
%token NEWLINE INDENT DEDENT COMMA RETURN


%right NOT
%left EQ NEQ GT GTE LT LTE
%left PLUS MINUS
%left TIMES DIVIDEDBY

%start program

%%

program
  : mainStatement {final_code = $1;}
  ;


mainStatement
  : statement {$$ = $1;}
  | mainStatement statement {
        $$ = concat(2, $1, $2);
        free($1);
        free($2);
    }
  ;


statement
  : assignStatement {$$ = $1;}
  | ifStatement {$$ = $1;}
  | whileStatement {$$ = $1;}
  | error NEWLINE {
        $$ = concat(1, "");
        _error = 1;
    }
  | break {$$ = $1;}
  ;

expression
  : value {$$ = $1;}
  | notStatement {$$ = $1;}
  | expression PLUS expression{
        $$ = concat(3, $1, " + ", $3);
        free($1);
        free($3);
    }
  | expression MINUS expression{
        $$ = concat(3, $1, " - ", $3);
        free($1);
        free($3);
    }
  | expression TIMES expression {
        $$ = concat(3, $1, " * ", $3);
        free($1);
        free($3);
    }
  | expression DIVIDEDBY expression{
        $$ = concat(3, $1, " / ", $3);
        free($1);
        free($3);
    }
  | expression GT expression{
        $$ = concat(3, $1, " > ", $3);
        free($1);
        free($3);
    }
  | expression GTE expression{
        $$ = concat(3, $1, " >= ", $3);
        free($1);
        free($3);
    }
  | expression LT expression{
        $$ = concat(3, $1, " < ", $3);
        free($1);
        free($3);
    }
  | expression LTE expression{
        $$ = concat(3, $1, " <= ", $3);
        free($1);
        free($3);
    }
  ;
  | expression EQ expression {
        $$ = concat(3, $1, " == ", $3);
        free($1);
        free($3);
    }
  | expression NEQ expression{
        $$ = concat(3, $1, " != ", $3);
        free($1);
        free($3);
    }
value
  : IDENTIFIER {
        if (hash_contains(symbols, $1)){
          $$ = $1;            
        } else {
          fprintf(stderr, "Error: unrecognized symbol (%s) on line %d\n", $1, @1.first_line);
          _error = 1;
        }
    }
  | FLOAT {$$ = $1;}
  | INTEGER {$$ = $1;}
  | BOOLEAN {
      $$ = malloc(2 * sizeof(char));
      if(strcmp($1, "False") == 0){
        strcpy($$, "0");
        free($1);
      }
      else{
        strcpy($$, "1");
        free($1);
      }      
    }
  | LPAREN expression RPAREN{
        $$ = concat(3, "(", $2, ")");
        free($2);
    }
  ;

notStatement
  : NOT value {
        $$ = concat(2, "!", $2);
        free($2);
    }
  ;

assignStatement
  : IDENTIFIER EQUALS expression NEWLINE{
        hash_insert(symbols, $1, $3);
        $$ = concat(4, $1, " = ", $3, ";\n");
        free($1);
        free($3);
    }
  ;

ifStatement
  : IF condition COLON NEWLINE block elseifStatement elseStatement{
        $$ = concat(7, "if (", $2, ") ", $5, $6, $7, "\n");
        free($2);
        free($5);
        free($6);
        free($7);
    }
  ;

elseifStatement
  : %empty {$$ = concat(1, "");}
  | elseifStatement ELIF condition COLON NEWLINE block {
        $$ = concat(5, $1, " else if (", $3, ") ", $6);
        free($1);
        free($3);
        free($6);
    }
  ;

elseStatement
  : %empty {$$ = concat(1, "");}
  | ELSE COLON NEWLINE block {
        $$ = concat(2, " else ", $4);
        free($4);
    }

block
  : INDENT mainStatement DEDENT {
        $$ = concat(3, "{\n", $2, "}");
        free($2);
    }
  ;
condition
  : expression {$$ = $1;}
  | condition AND condition {
        $$ = concat(3, $1, " && ", $3);
        free($1);
        free($3);
    }
  | condition OR condition {
        $$ = concat(3, $1, " || ", $3);
        free($1);
        free($3);
    }
  ;
whileStatement
  : WHILE condition COLON NEWLINE block {
        $$ = concat(5, "while (", $2, ") ", $5, "\n");
        free($2);
        free($5);
    }
  ;

break
  : BREAK NEWLINE { $$ = concat(1, "break;\n"); }
  ;

%%


void yyerror(YYLTYPE* loc, const char* err) {
    fprintf(stderr, "Error (line %d): %s\n", loc->first_line, err);
}


char* concat(const int n, ...) {
    int a = 0;
    char* strings[n];
    va_list args;
    va_start(args, n);
    for (int i = 0; i < n; i++) {
        strings[i] = va_arg(args, char*);
        a += strlen(strings[i]);
    }

    char* final_string = malloc((a + 1) * sizeof(char));
    final_string[0] = '\0';
    for (int i = 0; i < n; i++) {
        strncat(final_string, strings[i], a);
        a -= strlen(strings[i]);
    }
    return final_string;
}

int main() {
    symbols = hash_create();
    if (!yylex() && !_error) {
        printf("#include <stdio.h> \n");
        printf("int main() { \n");

        struct hash_iter* iter = hash_iter_create(symbols);
        while (hash_iter_has_next(iter)) {
            char* key;
            hash_iter_next(iter, &key);
            printf("double %s;\n", key);
        }
        hash_iter_free(iter);


        printf("\n /* Begin program */ \n");
        printf("\n %s \n", final_code);
        printf("/* End program */\n\n");
        
        iter = hash_iter_create(symbols);
        while (hash_iter_has_next(iter)) {
            char* key;
            hash_iter_next(iter, &key);
            printf("printf(\"%s: %%lf\\n\", %s);\n", key, key);
        }
        hash_iter_free(iter);
        
        printf("}\n");
        free(final_code);
        final_code = NULL;
    }
    hash_free(symbols);
}
