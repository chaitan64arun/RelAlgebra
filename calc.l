%{
int yyerror();
#include <stdlib.h>
#include"global.h"
#include "calc.h"
%}
number [0-9]+

%%
"X"		        return CROSS;
"-"			return MINUS;
"U"			return UNION;
"pi"			return PI;
"sigma"			return SIGMA;
"rho"			return RHO;
[a-zA-Z_.]+			{yylval.charvalue=yytext;
				return WORD;
				}

","     return(COMMA);
"("     return(LRB);
")"     return(RRB);
"["     return(LSB);
"]"     return(RSB);
"\n"	return(END);

%%
