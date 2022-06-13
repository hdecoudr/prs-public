%{
#include "shelltree.h"
#include "analysis.tab.h"
%}

ID  ([-.$%=/\\*?A-Za-z0-9]+)
ID2 ([^\"]*)
ID3 ([^\']*)

%%

[ \t]+  ;
^[ \t]*	;
{ID}|\"{ID2}\"|\'{ID3}\' {
    if (yytext[0] == '\"' || yytext[0] == '\'')
    {
        yytext[yyleng - 1] = '\0';
        strcpy(yylval.Identifier, yytext + 1);
    }
    else
    {
        strcpy(yylval.Identifier, yytext);
    }

    return IDENTIFIER;
}
\<          return IN;
\>          return OUT;
"2>"        return ERR;
"&>"        return ERR_OUT;
">>"        return OUT_APPEND;
"||"        return OR;
"&&"        return AND;
<<EOF>>     end_of_file();
.|\n        return yytext[0];

%%

int yyparse_string(char* s)
{
    yypush_buffer_state(yy_create_buffer(NULL, 0));
    yy_scan_string(s);
    int ret = yyparse();
    yypop_buffer_state();
    return ret;
}

