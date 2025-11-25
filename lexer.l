%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
%}

%option noyywrap
%x COMMENT

DIGIT    [0-9]
LETTER   [a-zA-Z_]
ID       {LETTER}({LETTER}|{DIGIT})*

%%

"/*"                { BEGIN(COMMENT); }
<COMMENT>"*/"       { BEGIN(INITIAL); }
<COMMENT>\n         ;
<COMMENT>.          ;

"//"[^\n]*          ;

"var"       { return VAR; }
"const"     { return CONST; }
"func"      { return FUNC; }
"return"    { return RETURN; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"do"        { return DO; }
"for"       { return FOR; }
"break"     { return BREAK; }
"continue"  { return CONTINUE; }
"pass"      { return PASS; }

"int"       { return TYPE_INT; }
"tiny"      { return TYPE_TINY; }
"long"      { return TYPE_LONG; }
"float"     { return TYPE_FLOAT; }
"char"      { return TYPE_CHAR; }
"bool"      { return TYPE_BOOL; }
"void"      { return TYPE_VOID; }

"true"      { return TRUE; }
"false"     { return FALSE; }

"abs"            { return ABS; }
"min"            { return MIN; }
"max"            { return MAX; }
"sqrt"           { return SQRT; }
"pow"            { return POW; }
"checkLineLeft"  { return CHECKLINELEFT; }
"checkLineRight" { return CHECKLINERIGHT; }
"accelerate"     { return ACCELERATE; }
"setForward"     { return SETFORWARD; }
"setBackward"    { return SETBACKWARD; }
"brake"          { return BRAKE; }
"turnLeft"       { return TURNLEFT; }
"turnRight"      { return TURNRIGHT; }
"turnAngle"      { return TURNANGLE; }

"=="        { return EQ; }
"!="        { return NEQ; }
"<="        { return LTE; }
">="        { return GTE; }
"<"         { return LT; }
">"         { return GT; }

"&&"        { return AND; }
"||"        { return OR; }
"!"         { return NOT; }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULTIPLY; }
"/"         { return DIVIDE; }
"%"         { return MODULO; }

"="         { return ASSIGN; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
"["         { return LBRACKET; }
"]"         { return RBRACKET; }
";"         { return SEMICOLON; }
":"         { return COLON; }
","         { return COMMA; }

{DIGIT}+"."{DIGIT}+   { yylval.float_val = atof(yytext);  return FLOAT_LITERAL; }
{DIGIT}+              { yylval.int_val   = atoi(yytext);  return INTEGER; }

{ID}                  { yylval.string_val = strdup(yytext); return IDENTIFIER; }

[ \t\r\n]+            ;

.                     {
    printf("Caracter no reconocido: %s\n", yytext);
}

%%
