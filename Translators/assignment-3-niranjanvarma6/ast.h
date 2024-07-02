#ifndef AST_H
#define AST_H

/* AST node types */
typedef enum {
    NODE_BLOCK,
    NODE_ASSIGNMENT,
    NODE_IDENTIFIER,
    NODE_INTEGER,
    NODE_FLOAT,
    NODE_PLUS,
    NODE_MINUS,
    NODE_TIMES,
    NODE_DIVIDEDBY,
    NODE_EQ,
    NODE_NEQ,
    NODE_GT,
    NODE_GTE,
    NODE_LT,
    NODE_LTE,
    NODE_BOOLEAN,
    NODE_IF,
    NODE_ELSE,
    NODE_WHILE,
    NODE_BREAK,
    NODE_EXPRESSION,
    NODE_ELIF
} NodeType;

/* AST node structure */
struct Node {
    NodeType type;
    struct Node** children;
    int child_count;
    char* val;
};

/* Function prototypes */
struct Node* create_node(NodeType type, char* val);
void add_child(struct Node* parent, struct Node* child);

#endif /* AST_H */
