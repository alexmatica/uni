%{
	#include <string.h>
	#include <stdio.h>
	#include <stdlib.h>

	extern int yylex();
	extern int yyparse();
	extern FILE *yyin;
	extern int lineCount;
	extern void printPIF();
	void yyerror(const char *s);	
%}

%token IDENTIF
%token CONSTANT
%token PLUS
%token MINUS
%token TIMES
%token SLASH
%token OBRACK
%token CBRACK
%token SCOLON
%token COMMA
%token DOT
%token ASSIGN
%token EQ
%token NEQ
%token LT
%token GT
%token LEQ
%token GEQ
%token COLON
%token BBEGIN
%token DO
%token END
%token IF
%token ELSE
%token THEN
%token VAR
%token WHILE
%token TYPEINT
%token TYPEREAL
%token READ
%token WRITE
%token MOD

%%

program: VAR decl_list SCOLON compoundstmt DOT;
decl_list: decl | decl decl_list;
decl: decl_tokens COLON type;
decl_tokens: IDENTIF | IDENTIF COMMA decl_tokens;
type: TYPEINT | TYPEREAL;

compoundstmt: BBEGIN stmtlist END;
stmtlist: stmt | stmt stmtlist;
stmt: assignstmt | iostmt | ifstmt | whilestmt | compoundstmt;

assignstmt: IDENTIF ASSIGN expr SCOLON;
expr: term | term operator term;
term: IDENTIF | CONSTANT;
operator: PLUS | MINUS | TIMES | SLASH | MOD;

iostmt: READ OBRACK IDENTIF CBRACK SCOLON
		| WRITE OBRACK IDENTIF CBRACK SCOLON 
		| WRITE OBRACK CONSTANT CBRACK SCOLON
		| WRITE OBRACK expr CBRACK SCOLON;

ifstmt: IF condition THEN compoundstmt ELSE compoundstmt;
condition: expr relation expr;
relation: EQ | NEQ | LEQ | GEQ | LT | GT;

whilestmt: WHILE condition DO compoundstmt;

%%

int main(int argc, char **argv){
	++argv; --argc;

	if (argc > 0)
		yyin = fopen(argv[0], "r");
	else
		yyin = stdin;

	while(!feof(yyin)){
		yyparse();
	}

	printf ("The file is correct both lexically and syntactly!\n");
	printPIF();
	return 0;
}

void yyerror(const char *s){
	printf("Error at line %d: [%s]\n", lineCount, s);
	exit(1);
}