# Makefile

CC	= g++
LEX = flex
YACC = bison

SRCFILES = lexer.c parser.c expression.c main.cc
OBJFILES = $(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(SRCFILES)))
APP = interpreter.elf

CFLAGS= -g -ansi
CXXFLAGS = $(CFLAGS)

all: $(APP)

$(OBJFILES): parser.h

CLEAN += $(APP)
$(APP):	$(OBJFILES)
		$(CC) $(CFLAGS) $+ -o $@

CLEAN += lexer.c lexer.h
lexer.c: lexer.l
		$(LEX) $(LEXFLAGS) $<

CLEAN += parser.c parser.h
parser.c parser.h: parser.y lexer.c
		$(YACC) $(YACCFLAGS) $<

TESTS = $(patsubst %.in,%,$(wildcard tests/*.in))

test: $(APP)
	for t in $(TESTS); do \
		./$(APP) < $${t}.in > $${t}.app_out ;\
		cmp $${t}.app_out $${t}.out ;\
	done

clean:
		rm -f *.o *~ $(CLEAN) $(addsuffix .app_out,$(TESTS))
