#pragma once

enum Token {
    PROG_BEGIN,   
    DECLARE, 
    VAR_NAME, 
    ASSIGN,
    INPUT,
    OUTPUT,
    IF,
    IF_STATEMENT,
    LOGICAL_EXPRESSION,
    ELSE,
    ELSE_STATEMENT,
    ENDL,
    INTEGER,
    DOUBLE,
    STRING,
    COMMENT,
    UNKNOWN,
    END,
    SPACE,
    PUNCTUATION,
    IN_OUT_OPERATOR
};
const char* tokenToString(Token t) {
    switch (t) {
        case PROG_BEGIN: return "PROG_BEGIN";
        case DECLARE: return "DECLARE";
        case VAR_NAME: return "VAR_NAME";
        case ASSIGN: return "ASSIGN";
        case INPUT: return "INPUT";
        case OUTPUT: return "OUTPUT";
        case IF: return "IF";
        case IF_STATEMENT: return "IF_STATEMENT";
        case LOGICAL_EXPRESSION: return "LOGICAL_EXPRESSION";
        case ELSE: return "ELSE";
        case ELSE_STATEMENT: return "ELSE_STATEMENT";
        case ENDL: return "ENDL";
        case INTEGER: return "INTEGER";
        case DOUBLE: return "DOUBLE";
        case STRING: return "STRING";
        case COMMENT: return "COMMENT";
        case UNKNOWN: return "UNKNOWN";
        case END: return "END";
        case SPACE: return "SPACE";
        case PUNCTUATION: return "PUNCTUATION";
        case IN_OUT_OPERATOR: return "IN_OUT_OPERATOR";
        default: return "tokenToString Error";
    }
} 