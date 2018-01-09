%{

    #include "miniLang.tab.h"
    #include <stdio.h>

    int crtAddress = 0;
    int crtToken = 0;
    int identifCount = 0;
    char identifs[1000][255];
    char sym[1000][255];
    char dbgSym[1000][255];
    int PIFTokenCode[10000]; 
    int PIFAddress[10000];
    int lineCount = 1;
    FILE *asm_source;

    int ifCounter = 0;
    int ifQueue[20];
    int ifIndex = 0;

    int whileCounter = 0;
    int whileQueue[20];
    int whileIndex = 0;
    
    int beginQueue[20];
    int beginCount = 0;

    int isIdentifier(char *token){
        for (int i=0; i<identifCount; i++)
            if(!strcmp(token, identifs[i]))
                return 1;
        return 0;
    }

    int findAddressOf(const char *token){
        for (int i=0; i<crtAddress; i++)
            if (!strcmp(token, sym[i]))
                return i;
        return -1;    
    }

    void addSymbol(const char *token, int code){
        strcpy(sym[crtAddress], token);
        crtAddress++;  
        
        if (code == IDENTIF){
            strcpy(identifs[identifCount++], token);
        }
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
                addSymbol(token, id);
                PIFAddress[crtToken++] = crtAddress-1;
            }
        }
        else{
        	PIFAddress[crtToken++] = -1;
        }   
    }
    
    void translate();

    void printPIF(){
    	for (int i=0; i < crtToken; i++){
    		printf("%3d", PIFTokenCode[i]);
    		if (PIFAddress[i] != -1)
    			printf(" - %3d [%s]\n", PIFAddress[i], dbgSym[i]);
    		else
    			printf(" -   X [%s]\n", dbgSym[i]);
    	}
    	translate();
    }
    
    void push_regs(){
        fprintf(asm_source, "\n\tpush rdi\n\tpush rsi\n\tpush rbp\n");
    }

    void pop_regs(){
        fprintf(asm_source, "\tpop rbp\n\tpop rsi\n\tpop rdi\n\n");
    }

    void do_scanf(char *memLocation){
        push_regs();
        fprintf(asm_source,"\tmov rsi, %s\n", memLocation);
        fprintf(asm_source,"\tmov rdi, format\n");
        fprintf(asm_source,"\tcall scanf\n");
        pop_regs();
    }

    void do_printf(){
        push_regs();
        fprintf(asm_source, "\tmov rsi, rax\n");
        fprintf(asm_source, "\tmov rdi, format\n");
        fprintf(asm_source, "\tcall printf\n");
        pop_regs();
    }

    void dereferenceOrNot(const char* reg, char *token){
        if (isIdentifier(token))
            fprintf(asm_source, "\tmov %s, [%s]\n", reg, token);
        else
            fprintf(asm_source, "\tmov %s, %s\n", reg, token);        
    }


    //  final result is saved in EAX
    void translate_expression(int count){
        dereferenceOrNot("rax", dbgSym[count]);
        dereferenceOrNot("rbx", dbgSym[count+2]);

        switch(PIFTokenCode[count+1]){
            case PLUS:
                fprintf(asm_source, "\tadd rax, rbx\n");
                break;
            case MINUS:
                fprintf(asm_source, "\tsub rax, rbx\n");
                break;
            case TIMES:
                fprintf(asm_source, "\tmov rdx, 0\n");
                fprintf(asm_source, "\timul rax, rbx\n");
                break;
            case SLASH:
                fprintf(asm_source, "\tmov rdx, 0\n");
                fprintf(asm_source, "\tdiv rbx\n");
                break;
            case MOD:
                fprintf(asm_source, "\tmov rdx, 0\n");
                fprintf(asm_source, "\tdiv rbx\n");
                fprintf(asm_source, "\tmov rax, rdx\n");
                break;
        }
    }

    void translate_compare(int count){
        fprintf(asm_source, "\tcmp rcx, rax\n");
        switch(PIFTokenCode[count]){
            case EQ:
                fprintf(asm_source, "\tjne else_branch%d\n", ifCounter);
                break;
            case NEQ:
                fprintf(asm_source, "\tje else_branch%d\n", ifCounter);
                break;
            case LT:
                fprintf(asm_source, "\tjge else_branch%d\n", ifCounter);
                break;
            case GT:
                fprintf(asm_source, "\tjle else_branch%d\n", ifCounter);
                break;
            case LEQ:
                fprintf(asm_source, "\tjg else_branch%d\n", ifCounter);
                break;
            case GEQ:
                fprintf(asm_source, "\tjl else_branch%d\n", ifCounter);
                break;
        }
    }

    void translate_compare_while(int count){
        fprintf(asm_source, "\tcmp rcx, rax\n");
        switch(PIFTokenCode[count]){
            case EQ:
                fprintf(asm_source, "\tjne while_end%d\n", whileCounter);
                break;
            case NEQ:
                fprintf(asm_source, "\tje while_end%d\n", whileCounter);
                break;
            case LT:
                fprintf(asm_source, "\tjge while_end%d\n", whileCounter);
                break;
            case GT:
                fprintf(asm_source, "\tjle while_end%d\n", whileCounter);
                break;
            case LEQ:
                fprintf(asm_source, "\tjg while_end%d\n", whileCounter);
                break;
            case GEQ:
                fprintf(asm_source, "\tjl while_end%d\n", whileCounter);
                break;
        }
    }

    void translate(){
        asm_source = fopen("source.asm","w");
        
        fprintf(asm_source, "global main\nextern scanf\nextern printf\nextern exit\n\n");
        fprintf(asm_source, "SECTION .data\n");
        fprintf(asm_source, "\tformat db \"%s\", 0\n", "\%ld");
        
        for(int i=0; i<identifCount; i++)
            fprintf(asm_source, "\t%s dq 0h\n", identifs[i]);
            
        fprintf(asm_source, "\n\nSECTION .text\nmain:\n");
        
        //  variable declarations have been taken care of; move to the actual code
        int count = 0;

        while (PIFTokenCode[count] != BBEGIN)
            count ++;
            
        int aux;
        for (count; count < crtToken;){
            switch (PIFTokenCode[count]){
                case READ:
                    do_scanf(dbgSym[count+2]);
                    count += 5;
                    fprintf(asm_source, "\n");
                    break;
               case WRITE:
                    // count + 1 = (
                    // count + 2 = identif / const
                    // count + 3 = ) / matemathical operator
                    // count + 4 = ; / identif / const
                    if (PIFTokenCode[count + 3] != CBRACK){
                        translate_expression(count+2);
                        count += 7;
                    }
                    else{
                        dereferenceOrNot("rax", dbgSym[count+2]);
                        count += 5;
                    }
                    do_printf();
                    break;
                case IF:
                    aux = PIFTokenCode[count+2];

                    //if the third deal is not a comparator, then we have an expression before the comparator
                    if (aux != LT && aux != GT && aux != EQ && aux != NEQ && aux != LEQ && aux != GEQ){
                        translate_expression(count + 1);
                        fprintf(asm_source, "\tmov rcx, rax\n");
                        count += 4;
                    }
                    else {
                        dereferenceOrNot("rcx", dbgSym[count+1]);
                        count += 2;
                    }

                    // save comparator position
                    aux = count;

                    // if the last deal is not THEN, we have an expression after the comparator
                    if (PIFTokenCode[count+2] != THEN){
                        translate_expression(count+1);
                        count += 5;
                    }
                    else {
                        dereferenceOrNot("rax", dbgSym[count+1]);
                        count += 3;
                    }

                    translate_compare(aux);
                    fprintf(asm_source, "\n");

                    beginQueue[beginCount++] = IF;
                    ifQueue[ifIndex++] = ifCounter++;

                    break;
                case ELSE:
                    fprintf(asm_source, "else_branch%d:\n", ifQueue[ifIndex-1]);
                    beginQueue[beginCount++] = ELSE; 

                    count++;
                    break;
                case END:
                    aux = beginQueue[beginCount-1];
                    beginCount--;

                    if (aux == IF){
                        if (PIFTokenCode[count + 1] == ELSE){
                            fprintf(asm_source, "\tjmp after_else%d\n", ifQueue[ifIndex-1]);
                        }
                        else{
                            ifIndex--;
                        }
                    } else if (aux == ELSE){
                        fprintf(asm_source, "after_else%d:\n\n", ifQueue[ifIndex-1]);
                        ifIndex--;
                    } else if (aux == WHILE){
                        fprintf(asm_source, "jmp before_while%d\n", whileQueue[whileIndex-1]);
                        fprintf(asm_source, "while_end%d:\n\n", whileQueue[whileIndex-1]);
                        whileIndex--;
                    }
                    count++;
                    break;
                case BBEGIN:
                    count ++;
                    break;
                case ASSIGN:
                    aux = count - 1;
                    if (PIFTokenCode[count+2] != SCOLON){
                        translate_expression(count+1);
                        count += 5;
                    } else{
                        dereferenceOrNot("rax", dbgSym[count+1]);
                        count += 3;
                    }
                    fprintf(asm_source, "\tmov [%s], rax\n", dbgSym[aux]);
                    break;
                case WHILE:
                    fprintf(asm_source, "before_while%d:\n", whileCounter);

                    aux = PIFTokenCode[count+2];

                    if (aux != LT && aux != GT && aux != EQ && aux != NEQ && aux != LEQ && aux != GEQ){
                        translate_expression(count + 1);
                        fprintf(asm_source, "\tmov rcx, rax\n");
                        count += 4;
                    }
                    else {
                        dereferenceOrNot("rcx", dbgSym[count+1]);
                        count += 2;
                    }

                    aux = count;
                    if (PIFTokenCode[count+2] != DO){
                        translate_expression(count+1);
                        count += 5;
                    }
                    else {
                        dereferenceOrNot("rax", dbgSym[count+1]);
                        count += 3;
                    }

                    translate_compare_while(aux);
                    fprintf(asm_source, "\n");

                    beginQueue[beginCount++] = WHILE;
                    whileQueue[whileIndex++] = whileCounter++;

                    break;
                default:
                    count ++;
                    break;
            }
        }
        
        fprintf(asm_source, "push 0\ncall exit\n"); 
        fclose(asm_source);
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
