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
    ARITHMETIC_EXPRESSION,
    ELSE,
    ELSE_STATEMENT,
    ENDL,
    INTEGER,
    DOUBLE,
    STRING,
    CHARACTER,
    COMMENT,
    UNKNOWN,
    END,
    PUNCTUATION,
    IN_OUT_OPERATOR,
    ASSIGN_OPERATOR,
    ARITHMETIC_OPERATOR,
    USER_ARITHMETIC_EXPRESSION,
    LOGICAL_OPERATOR,
    USER_INPUT, // Unknown type na variable kay depende sa user
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
        case ARITHMETIC_EXPRESSION: return "ARITHMETIC_EXPRESSION";
        case USER_ARITHMETIC_EXPRESSION: return "USER_ARITHMETIC_EXPRESSION";
        case ELSE: return "ELSE";
        case ELSE_STATEMENT: return "ELSE_STATEMENT";
        case ENDL: return "ENDL";
        case INTEGER: return "INTEGER";
        case DOUBLE: return "DOUBLE";
        case STRING: return "STRING";
        case CHARACTER: return "CHARACTER";
        case COMMENT: return "COMMENT";
        case UNKNOWN: return "UNKNOWN";
        case END: return "END";
        case PUNCTUATION: return "PUNCTUATION";
        case IN_OUT_OPERATOR: return "IN_OUT_OPERATOR";
        case ASSIGN_OPERATOR: return "ASSIGN_OPERATOR";
        case ARITHMETIC_OPERATOR: return "ARITHMETIC_OPERATOR";
        case LOGICAL_OPERATOR: return "LOGICAL_OPERATOR";
        case USER_INPUT: return "USER_INPUT";
        default: return "tokenToString Error";
    }
} 