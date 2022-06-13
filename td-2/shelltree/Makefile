FLEX	= flex
BISON	= bison -d -v

CC		= gcc 
CFLAGS	= -g -Wall -std=gnu99
LDLIBS	= -lreadline -lfl

shelltree: shelltree.o display.o evaluator.o analysis.tab.o lex.yy.o
	$(CC) $(CFLAGS) -o shelltree shelltree.o display.o evaluator.o analysis.tab.o lex.yy.o $(LDLIBS)

shelltree.o: shelltree.c shelltree.h

display.o: shelltree.h display.h display.c

evaluator.o: shelltree.h evaluator.h evaluator.c

lex.yy.o: lex.yy.c analysis.tab.h shelltree.h

analysis.tab.c analysis.tab.h: analysis.y
	$(BISON) analysis.y

lex.yy.c: analysis.l shelltree.h analysis.tab.h
	$(FLEX) analysis.l

.PHONY: clean
clean:
	rm -f shelltree *.o analysis.tab.* *.output lex.yy.*
