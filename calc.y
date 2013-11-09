%{
#include"global.h"
#include <stdio.h>
#include <iostream>
using namespace std;
#include <stdlib.h>
#include<fstream>
#include<string.h>
#include "raexpr.h"

int yylex(void);
int yyerror(char *s);
RAExpr* Root;
PL* head;
PL* head_ptr = NULL;
%}

%type	<raexpr> Expression
%token <charvalue> WORD
%type <str> Str
%type <plist> Predicate
%token	CROSS UNION PI SIGMA RHO MINUS END
%token  LSB RSB LRB RRB COMMA
%left CROSS
%left MINUS
%start Input
%%


Input:
	  END
	| Expression END		{ 	                                        
	                                  //cout<<"In Input"<<endl;
	                                  Root = $1;
	                                  //cout<<"shut"<<endl;
	                                  cout<<Root->evaluate()<<endl;
	                                  }
	;

Expression:
        RHO LSB Str LRB Predicate RRB RSB LRB Expression RRB { //cout<<$3<<endl;
                                                                $$ = new Rho($3,$5,$9);
                                                                //cout<<"In Rho"<<endl;
                                                                }
        | PI LSB Predicate RSB LRB Expression RRB {
                                                        $$ = new Pi($3,$6);
                                                
                                                }	  
	| Expression CROSS Expression	        {
	                                        
	                                        $$ = new Cross($1,$3);
	                                        }
	                                        
	|  Expression MINUS Expression	        {
	                                        //cout<<"In Minus"<<endl;
	                                        $$ = new Minus($1,$3);
	                                        }
	|  Expression UNION Expression	        {
	                                        //cout<<"In Union"<<endl;
	                                        $$ = new Union($1,$3);
	                                        }                                                                               
	                                        
	| WORD			                { 
	                                        $$ = new BaseExpr($1); }	                                       				         
	
	;
Predicate:
        Predicate COMMA Predicate               {}
        | WORD                                  { head_ptr = head->update(&head_ptr,$1);
                                                        //cout<<head_ptr->attr<<endl;
                                                        $$ = head_ptr;
                                                        //cout<<$1<<endl;
                                                        }
        ;
Str:    WORD                                    { 
                                                        $$ = new Strt($1);
                                                  //cout<<$$<<endl;
                                                         }

%%

int yyerror(char *s) {
  printf("Process Ended\n");
}

int main(void) {
  yyparse();
  //cout<<"In Main"<<endl;
  char* result;
  //result = Root->evaluate();
  //cout<<"The End"<<endl;
}
