#ifndef GLOBAL_H
#define GLOBAL_H
#include "raexpr.h"
typedef union YYSTYPE
{
	char *charvalue;
	RAExpr* raexpr;
	PL* plist;	
}YYSTYPE;
extern YYSTYPE yylval;
#endif
