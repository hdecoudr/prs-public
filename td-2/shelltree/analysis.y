%{
#include "shelltree.h"
extern int yylex(void);
%}

%union
{
    Expression* Expr;
    char**      ArgsList;
    char        Identifier[ID_LENGTH];
}

%token <identifier> IDENTIFIER
%nonassoc '&'
%left ';' AND OR
%left '|'
%token IN OUT OUT_APPEND ERR ERR_OUT
%left  IN OUT OUT_APPEND ERR ERR_OUT

%type <Expr> expression_or_empty
%type <Expr> expression
%type <ArgsList> command
%type <ArgsList> file

%%
command_line : expression_or_empty '\n'
    {
        processed = $1;
        YYACCEPT;
    }
    | error '\n'
    {
        yyclearin;
        YYABORT;
    }
    ;

expression_or_empty :
    {
        $$ = new_node(EMPTY, NULL, NULL, NULL);
    }
    | expression
		

expression :
    command
    {
        $$ = new_node(SIMPLE, NULL, NULL, $1);
    }
    | expression ';' expression
    {
        $$ = new_node(SEQUENCE, $1, $3, NULL);
    }
    | expression AND expression
    {
        $$ = new_node(SEQUENCE_AND, $1, $3, NULL);
    }
    | expression OR expression
    {
        $$ = new_node(SEQUENCE_OR, $1, $3, NULL);
    }
    | expression '|' expression
    {
        $$ = new_node(PIPE, $1, $3, NULL);
    }
    | expression IN file
    {
        $$ = new_node(REDIRECTION_I, $1, NULL, $3);
    }
    | expression OUT file
    {
        $$ = new_node(REDIRECTION_O, $1, NULL, $3);
    }
    | expression ERR file
    {
        $$ = new_node(REDIRECTION_E, $1, NULL, $3);
    }
    | expression ERR_OUT file
    {
        $$ = new_node(REDIRECTION_EO, $1, NULL, $3);
    }
    | expression OUT_APPEND file
    {
        $$ = new_node(REDIRECTION_A, $1, NULL, $3);
    }
    | expression '&'
    {
        $$ = new_node(BACKGROUND, $1, NULL, NULL);
    }
    | '(' expression ')'
    {
        $$ = $2;
    }
    ;

file : IDENTIFIER
    {
        char** p = new_args_list();
        $$ = append_to_args_list(p, yylval.Identifier);
    }
    ;

command : IDENTIFIER
    {
        char** p = new_args_list();
        $$ = append_to_args_list(p, yylval.Identifier);
    }
    | command IDENTIFIER
    {
        $$ = append_to_args_list($1, yylval.Identifier);
    }
    ;
%%

