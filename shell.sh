bison -d calc.y
mv calc.tab.h calc.h
mv calc.tab.c calc.y.c
flex calc.l
mv lex.yy.c calc.lex.c
g++ -c calc.lex.c -o calc.lex.o
g++ -c calc.y.c -o calc.y.o
g++ -o calc calc.lex.o calc.y.o -lfl
