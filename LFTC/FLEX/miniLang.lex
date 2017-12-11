%{

    #include "miniLang.tab.h"
    #include <stdio.h>

    int crtAddress = 0;
    int crtToken = 0;
    char sym[1000][255];
    char dbgSym[1000][255];
    int PIFTokenCode[10000]; 
    int PIFAddress[10000];
    int lineCount = 1;

    int findAddressOf(const char *token){
        for (int i=0; i<crtAddress; i++)
            if (!strcmp(token, sym[i]))
                return i;
        return -1;    
    }

    void addSymbol(const char *token){
        strcpy(sym[crtAddress], token);
        crtAddress++;    
    }
    
    void addToPif(int id, const char *token){
        //printf("%s\n", token);
        PIFTokenCode[crtToken] = id;
        strcpy(dbgSym[crtToken], token);
        if (id == IDENTIF || id == CONSTANT){
            int address = findAddressOf(token);  
            if (address != -1){
                PIFAddress[crtToken++] = address;
            }
            else {
                addSymbol(token);
                PIFAddress[crtToken++] = crtAddress-1;
            }
        }
        else{
        	PIFAddress[crtToken++] = -1;
        }   
    }

    void printPIF(){
    	for (int i=0; i < crtToken; i++){
    		printf("%3d", PIFTokenCode[i]);
    		if (PIFAddress[i] != -1)
    			printf(" - %3d [%s]\n", PIFAddress[i], dbgSym[i]);
    		else
    			printf(" -   X [%s]\n", dbgSym[i]);
    	}
    }
		
%}

%option noyywrap
digit         [0-9]
letter        [a-zA-Z]

%%
"+"                  			{ addToPif(PLUS,"+");  return PLUS;  }
"-"                  			{ addToPif(MINUS,"-");	return MINUS;}
"*"                  			{ addToPif(TIMES,"*");	return TIMES;}
"/"                  			{ addToPif(SLASH,"/");	return SLASH;}
"("                  			{ addToPif(OBRACK,"(");	return OBRACK;}
")"                  			{ addToPif(CBRACK,")");	return CBRACK;}
";"                  			{ addToPif(SCOLON,";");	return SCOLON;}
","                  			{ addToPif(COMMA,",");	return COMMA;}
"."                  			{ addToPif(DOT,".");	return DOT;}
":="                 			{ addToPif(ASSIGN,":="); return ASSIGN;}
"="                  			{ addToPif(EQ,"=");	return EQ;}
"<>"                 			{ addToPif(NEQ,"<>");	return NEQ;}
"<"                  			{ addToPif(LT,"<");	return LT;}
">"                  			{ addToPif(GT,">");	return GT;}
"<="                 			{ addToPif(LEQ,"<=");	return LEQ;}
">="                 			{ addToPif(GEQ,">=");	return GEQ;}
":"					 			{ addToPif(COLON,":");	return COLON;}
"BEGIN"              			{ addToPif(BBEGIN,"BEGIN");   return BBEGIN;}
"DO"                 			{ addToPif(DO,"DO");      return DO;}
"END"                			{ addToPif(END,"END");     return END;}
"IF"                 			{ addToPif(IF,"IF");      return IF;}
"ELSE"							{ addToPif(ELSE, "ELSE");		return ELSE;}
"THEN"               			{ addToPif(THEN,"THEN");    return THEN;}
"VAR"                			{ addToPif(VAR,"VAR");     return VAR;}
"WHILE"              			{ addToPif(WHILE, "WHILE");   return WHILE;}
"INTEGER"						{ addToPif(TYPEINT, "INTEGER"); return TYPEINT;}
"REAL"							{ addToPif(TYPEREAL, "REAL");	return TYPEREAL;}
"READ"							{ addToPif(READ, "READ");	return READ;}
"WRITE"							{ addToPif(WRITE, "WRITE");	return WRITE;}
"MOD"                           { addToPif(MOD, "MOD"); return MOD;}
{letter}({letter}|{digit})* 	{ addToPif(IDENTIF, yytext); return IDENTIF;}
"_"("_"|{letter}|{digit})+      { addToPif(IDENTIF, yytext); return IDENTIF;}
(\+|\-)?"0"|(\+|\-)?[1-9]{digit}*    { addToPif(CONSTANT, yytext);    return CONSTANT;}
(\+|\-)?("0"|[1-9]{digit}*)"."{digit}* { addToPif(CONSTANT, yytext);    return CONSTANT;}
[ \t\r]
[\n]                            { lineCount++; }            
.                    			{ printf("Lexical error at line %d: [%c]",lineCount, yytext[0]); exit(1);}
%%
