
digit         [0-9]
letter        [a-zA-Z]

%%
"+"                  			{ printf("+\n");	}
"-"                  			{ printf("-\n");	}
"*"                  			{ printf("*\n");	}
"/"                  			{ printf("/\n");	}
"("                  			{ printf("(\n");	}
")"                  			{ printf(")\n");	}
";"                  			{ printf(";\n");	}
","                  			{ printf(",\n");	}
"."                  			{ printf(".\n");	}
":="                 			{ printf(":=\n");	}
"="                  			{ printf("=\n");	}
"<>"                 			{ printf("<>\n");	}
"<"                  			{ printf("<\n");	}
">"                  			{ printf(">\n");	}
"<="                 			{ printf("<=\n");	}
">="                 			{ printf(">=\n");	}
":"					 			{ printf(":\n");	}
"BEGIN"              			{ printf("BEGIN\n");   }
"DO"                 			{ printf("DO\n");      }
"END"                			{ printf("END\n");     }
"IF"                 			{ printf("IF\n");      }
"THEN"               			{ printf("THEN\n");    }
"VAR"                			{ printf("VAR\n");     }
"WHILE"              			{ printf("WHILE\n");   }
{letter}({letter}|{digit})* 	{ printf("%s\n", yytext);}
{digit}+             			{ printf("%s\n", yytext);    }
[ \t\n\r]            
.                    			{ printf("Unknown character [%c]\n",yytext[0]);   }
%%

int yywrap(void){
	return 1;
}