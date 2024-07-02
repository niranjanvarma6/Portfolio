%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"
#include "hash.h"
#include "parser.h"

extern int yylex();
void yyerror(YYLTYPE* loc, const char* err);
char* concat_strings(int n, ...);
char* py_bool_to_c(char* py_bool);

struct Node* root;
struct Node* current_block;

/*
 * Here, target_program is a string that will hold the final generated target
 * program.
 */
char* target_program;

/*
 * Here, symbols is a hash table used to keep track of all unique identifiers.
 * It will be used to figure out what variables to initialize in the final
 * generated C program.
 */
struct hash* symbols;



int have_err = 0;
%}



/*
 * Enable location tracking and verbose error messages.
 */
%locations
%define parse.error verbose


/*
 * All program constructs will be represented as strings, specifically as
 * their corresponding C translation.
 */
%union {
    char* string;
    struct Node* node;
}

/*
 * Because the lexer can generate more than one token at a time (i.e. DEDENT
 * tokens), we'll use a push parser.
 */
%define api.pure full
%define api.push-pull push

/*
 * These are all of the terminals in our grammar, i.e. the syntactic
 * categories that can be recognized by the lexer.
 */
%token <string> IDENTIFIER
%token <string> FLOAT INTEGER BOOLEAN
%token <node> INDENT DEDENT NEWLINE
%token <node> AND BREAK DEF ELIF ELSE FOR IF NOT OR RETURN WHILE
%token <node> ASSIGN PLUS MINUS TIMES DIVIDEDBY
%token <node> EQ NEQ GT GTE LT LTE
%token <node> LPAREN RPAREN COMMA COLON
%type <node> program statements statement primary_expression  expression assign_statement block negated_expression if_statement elif_blocks else_block while_statement break_statement condition  

/*
 * Here, we're defining the precedence of the operators.  The ones that appear
 * later have higher precedence.  All of the operators are left-associative
 * except the "not" operator, which is right-associative.
 */
%left OR
%left AND
%right NOT
%left EQ NEQ GT GTE LT LTE
%left PLUS MINUS
%left TIMES DIVIDEDBY

/*
 * This is our goal/start symbol.
 */
%start program

%%

/*
 * Each of the CFG rules below recognizes a particular program construct in
 * Python and creates a new string containing the corresponding C translation.
 * Since we're allocating strings as we go, we also free them as we no longer
 * need them.  Specifically, each string is freed after it is combined into a
 * larger string.
 */

/*
 * This is the goal/start symbol.  Once all of the statements in the entire
 * source program are translated, this symbol receives the string containing
 * all of the translations and assigns it to the global target_program, so it
 * can be used outside the parser.
 */
program
  : statements { 
  root = create_node(NODE_BLOCK, NULL);
  add_child(root, $1); 
  }
  ;


statements
  : statement { $$ = $1; }
  | statements statement {
        add_child($1, $2);
        $$ = $1;
    }
  ;

statement
  : assign_statement { $$ = $1; }
  | if_statement { $$ = $1; }
  | while_statement { $$ = $1; }
  | break_statement { $$ = $1; }
  | error NEWLINE { $$ = create_node(NODE_EXPRESSION, NULL); }
  ;

primary_expression
  : IDENTIFIER { $$ = create_node(NODE_IDENTIFIER, $1); }
  | FLOAT { $$ = create_node(NODE_FLOAT, $1); }
  | INTEGER { $$ = create_node(NODE_INTEGER, $1); }
  | BOOLEAN { $$ = create_node(NODE_BOOLEAN, $1); }
  | LPAREN expression RPAREN { $$ = $2; }
  ;

negated_expression
  : NOT primary_expression {
        $$ = create_node(NODE_EXPRESSION, "!"); 
        add_child($$, $2);
    }
  ;

expression
  : primary_expression { $$ = $1; }
  | negated_expression { $$ = $1; }
  | expression PLUS expression {
        $$ = create_node(NODE_PLUS, "+"); 
        add_child($$, $1); 
        add_child($$, $3);
    }
  | expression MINUS expression {
        $$ = create_node(NODE_MINUS, "-"); 
        add_child($$, $1); 
        add_child($$, $3);
    }
  | expression TIMES expression {
        $$ = create_node(NODE_TIMES, "*"); 
        add_child($$, $1); 
        add_child($$, $3);
    }
  | expression DIVIDEDBY expression {
        $$ = create_node(NODE_DIVIDEDBY, "/"); 
        add_child($$, $1); 
        add_child($$, $3);
    }
  | expression EQ expression {
        $$ = create_node(NODE_EQ, "=="); 
        add_child($$, $1); 
        add_child($$, $3);
    }
  | expression NEQ expression {
        $$ = create_node(NODE_NEQ, "!="); 
        add_child($$, $1); 
        add_child($$, $3);
    }
  | expression GT expression {
        $$ = create_node(NODE_GT, ">"); 
        add_child($$, $1); 
        add_child($$, $3);
    }
  | expression GTE expression {
        $$ = create_node(NODE_GTE, ">="); 
        add_child($$, $1); 
        add_child($$, $3);
    }
  | expression LT expression {
        $$ = create_node(NODE_LT, "<"); 
        add_child($$, $1); 
        add_child($$, $3);
    }
  | expression LTE expression {
        $$ = create_node(NODE_LTE, "<="); 
        add_child($$, $1); 
        add_child($$, $3);
    }
  ;

assign_statement
  : IDENTIFIER ASSIGN expression NEWLINE {
        $$ = create_node(NODE_ASSIGNMENT, NULL); 

        add_child($$, create_node(NODE_EXPRESSION, $1)); 
        add_child($$, $3);

    }
  ;

block
  : INDENT statements DEDENT {
        
        $$ = create_node(NODE_BLOCK, NULL);
        add_child($$, $2);

        
    }
  ;

condition
  : expression { $$ = $1; }
  | condition AND condition {
        $$ = create_node(NODE_EXPRESSION, "&&"); 
        add_child($$, $1); 
        add_child($$, $3);
    }
  | condition OR condition {
        $$ = create_node(NODE_EXPRESSION, "||"); 
        add_child($$, $1); 
        add_child($$, $3);
    }
  ;

if_statement
  : IF condition COLON NEWLINE block elif_blocks else_block {
        $$ = create_node(NODE_IF, NULL); 
        add_child($$, $2); 
        add_child($$, $5); 
        add_child($$, $6); 
        add_child($$, $7);
    }
  ;

elif_blocks
  : %empty { $$ = NULL; }
  | elif_blocks ELIF condition COLON NEWLINE block {
        struct Node* elif_node = create_node(NODE_ELIF, NULL); 
        add_child(elif_node, $3); 
        add_child(elif_node, $6); 
        if ($1) {
            struct Node* prev_elif = $1;
            while (prev_elif->children[1])
                prev_elif = prev_elif->children[1];
            add_child(prev_elif, elif_node);
        } else {
            $$ = elif_node;
        }
    }
  ;

else_block
  : %empty { $$ = NULL; }
  | ELSE COLON NEWLINE block {
        $$ = create_node(NODE_ELSE, NULL); 
        add_child($$, $4);
    }
  ;

while_statement
  : WHILE condition COLON NEWLINE block {
        $$ = create_node(NODE_WHILE, NULL); 
        add_child($$, $2); 
        add_child($$, $5);
    }
  ;

break_statement
  : BREAK NEWLINE { $$ = create_node(NODE_BREAK, NULL); }
  ;


%%

/*
 * This is our simple error reporting function.  It prints the line number
 * and text of each error.
 */
void yyerror(YYLTYPE* loc, const char* err) {
    fprintf(stderr, "Error (line %d): %s\n", loc->first_line, err);
}

/*
 * This function generates a new string by concatenating multiple strings
 * together.  The function is variadic, so any number of strings can be passed.
 * The total number of strings passed must be specified as the first argument,
 * `n`.  A new string is generated by concatenating the strings in order, and
 * this string is returned.  Memory is allocated for the returned string, which
 * must be freed by the caller.
 */
char* concat_strings(const int n, ...) {
    int l = 0;
    char* strings[n];

    /*
     * Loop through the arguments to calculate the total length, and remember
     * the string pointers.
     */
    va_list args;
    va_start(args, n);
    for (int i = 0; i < n; i++) {
        strings[i] = va_arg(args, char*);
        l += strlen(strings[i]);
    }

    /*
     * Concatenate all strings together into one.
     */
    char* result = malloc((l + 1) * sizeof(char));
    result[0] = '\0';
    for (int i = 0; i < n; i++) {
        strncat(result, strings[i], l);
        l -= strlen(strings[i]);
    }
    return result;
}


/*
 * This function translates a Python boolean val into the corresponding
 * C string val.  The translated string is returned and must be freed by
 * the caller.
 */
char* py_bool_to_c(char* py_bool) {
    char* result = malloc(2 * sizeof(char));
    if (strcmp(py_bool, "True") == 0) {
        strcpy(result, "1");
    } else {
        strcpy(result, "0");
    }
    return result;
}

struct Node* create_node(NodeType type, char* val) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    if (node == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for AST node\n");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->children = NULL;
    node->child_count = 0;
    node->val = val;
    return node;
}

void add_child(struct Node* parent, struct Node* child) {
    if (parent->children == NULL) {
        parent->children = (struct Node**)malloc(sizeof(struct Node*));
    } else {
        parent->children = (struct Node**)realloc(parent->children, (parent->child_count + 1) * sizeof(struct Node*));
    }
    if (parent->children == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for child nodes\n");
        exit(EXIT_FAILURE);
    }
    parent->children[parent->child_count++] = child;
}

// Convert node type enum to string
const char* node_type_to_string(NodeType type) {
    switch (type) {
        case NODE_ASSIGNMENT: return "ASSIGNMENT";
        case NODE_INTEGER : return "INTEGER";
        case NODE_FLOAT : return "FLOAT";
        case NODE_PLUS : return "PLUS";
        case NODE_MINUS : return "MINUS";
        case NODE_TIMES : return "TIMES";
        case NODE_DIVIDEDBY : return "DIVIDEDBY";
        case NODE_GT : return "GT";
        case NODE_GTE : return "GTE";
        case NODE_LTE : return "LT";
        case NODE_EQ : return "EQ";
        case NODE_NEQ : return "NEQ";
        case NODE_IDENTIFIER : return "IDENTIFIER";
        case NODE_EXPRESSION: return "EXPRESSION";
        case NODE_BOOLEAN: return "BOOLEAN";
        case NODE_BLOCK: return "BLOCK";
        case NODE_IF: return "IF";
        case NODE_ELIF: return "ELIF";
        case NODE_ELSE: return "ELSE";
        case NODE_WHILE: return "WHILE";
        case NODE_BREAK: return "BREAK";
        default: return "UNKNOWN";
    }
}

void childside_print(struct Node* n, const char* name, int level) {
    if (n == NULL) {
        return;
    }

    char temp[200];
    strncpy(temp, name, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';

    int length = strlen(temp);
    if (length > 0) {
        temp[length - 1] = '\0'; 
    }
    printf("%s [label=%s]\n", temp, node_type_to_string(n->type));

    char recursive_val[200];
    for (int i = 0; i < n->child_count; i++) {
        strncpy(recursive_val, name, sizeof(recursive_val) - 1);
        recursive_val[sizeof(recursive_val) - 1] = '\0';

        if (i == 0) {
            strcat(recursive_val, "lhs_");
        } else if (i == 1) {
            strcat(recursive_val, "rhs_");
        }

        if (n->children[i]) {
            childside_print(n->children[i], recursive_val, level + 1);
        }
    }
}




void print_all(struct Node* n, char* name, int level) {
    char temp [150]; 
    strcpy(temp, name);
    if (level != 0){
      int length = strlen(temp);
      if (length > 0) {
          temp[length - 1] = '\0'; 
      }
    }  
    printf("%s [label=%s] \n", temp, node_type_to_string(n->type));
    if (level < 1){
      for (int i = 0; i < n->child_count; i++){
        char recursive_val[200];
        snprintf(recursive_val, sizeof(recursive_val), "%s%d_", name, i);
        print_all(n->children[i], recursive_val, ++level);
      }
    }
    else{
      for (int i = 0; i < n->child_count; i++){
        char recursive_val[150];
        snprintf(recursive_val, sizeof(recursive_val), "%s%d_", name, i);
        childside_print(n->children[i], recursive_val, ++level);        
      }
    }    
}

int main() {
    if (!yylex() && !have_err) {
        printf("digraph AST {\n"); 
        print_all(root, "n", 0); 
        printf("}\n"); 
    }
}

