# Makefile

CC	= g++
LEX = flex
YACC = bison

SRCFILES = lexer.c parser.c expression.cc main.cc
OBJFILES = $(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(SRCFILES)))
APP = interpreter.elf

CFLAGS= -g -std=c++11
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

# stderr is directed to the same file as stdout, so it can be compared with a
# single reference output file.
test: $(APP)
	for t in $(TESTS); do \
		./$(APP) < $${t}.in > $${t}.app_out 2>&1 ;\
		cmp $${t}.app_out $${t}.out ;\
	done

clean:
		rm -f *.o *~ $(CLEAN) $(addsuffix .app_out,$(TESTS))
