%{

    int crtAddress = 0;
    int crtToken = 0;
    char sym[100][255];
    int PIFTokenCode[1000]; 
    int PIFAddress[1000];

    int findAddressOf(char *token){
        for (int i=0; i<crtAddress; i++)
            if (!strcmp(token, sym[i]))
                return i;
        return -1;    
    }

    void addSymbol(char *token){
        strcpy(sym[crtAddress], token);
        crtAddress++;    
    }
    
    void addToPif(int id, char *token){
        if (id == 0 || id == 1){
            int address = findAddressOf(token);  
            PIFTokenCode[crtToken] = id; 
            if (address != -1){
            	strcpy(sym[crtToken], token);
                PIFAddress[crtToken++] = address;
            }
            else {
                addSymbol(token);
                PIFAddress[crtToken++] = crtAddress-1;
            }
        }
        else{
        	PIFAddress[crtToken] = -1;
        	PIFTokenCode[crtToken++] = id;
        }   
    }

    void printPIF(){
    	for (int i=0; i < crtToken; i++){
    		printf("%3d", PIFTokenCode[i]);
    		if (PIFAddress[i] != -1)
    			printf(" - %3d [%-50s]\n", PIFAddress[i], sym[i]);
    		else
    			printf(" -   X\n");
    	}
    }
		
%}


digit         [0-9]
letter        [a-zA-Z]

%%
"+"                  			{ addToPif(2,"+");    }
"-"                  			{ addToPif(3,"-");	}
"*"                  			{ addToPif(4,"*");	}
"/"                  			{ addToPif(5,"/");	}
"("                  			{ addToPif(6,"(");	}
")"                  			{ addToPif(7,")");	}
";"                  			{ addToPif(8,";");	}
","                  			{ addToPif(9,",");	}
"."                  			{ addToPif(10,".");	}
":="                 			{ addToPif(11,":=");	}
"="                  			{ addToPif(12,"=");	}
"<>"                 			{ addToPif(13,"<>");	}
"<"                  			{ addToPif(14,"<");	}
">"                  			{ addToPif(15,">");	}
"<="                 			{ addToPif(16,"<=");	}
">="                 			{ addToPif(17,">=");	}
":"					 			{ addToPif(18,":");	}
"BEGIN"              			{ addToPif(19,"BEGIN");   }
"DO"                 			{ addToPif(20,"DO");      }
"END"                			{ addToPif(21,"END");     }
"IF"                 			{ addToPif(22,"IF");      }
"ELSE"							{ addToPif(23, "ELSE");		}
"THEN"               			{ addToPif(24,"THEN");    }
"VAR"                			{ addToPif(25,"VAR");     }
"WHILE"              			{ addToPif(26, "WHILE");   }
"INTEGER"						{ addToPif(27, "INTEGER");}
"REAL"							{ addToPif(28, "REAL");	}
"READ"							{ addToPif(29, "READ");	}
"WRITE"							{ addToPif(30, "WRITE");	}
{letter}({letter}|{digit})* 	{ addToPif(0, yytext);}
{digit}+             			{ addToPif(1, yytext);    }
[ \t\n\r]            
.                    			{ printf("Unknown character [%c]",yytext[0]); return -1;  }
%%

int yywrap(void){
	return 1;
}

int main()
{
	yylex();
	printPIF();
}
