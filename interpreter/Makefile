# Makefile

CC	= g++
LEX = flex
YACC = bison

CPPLINT = tools/cpplint.py
# build/include demands that header files included with quotes should be
# referenced by the directory name. This error doesn't make a lot of sense in
# case when header is already in the same directory. And prefixing header with
# "./" just makes things worse, because now cpplint complains that it is "./"
# instead of "interpreter". 
CPPLINTFLAGS = --filter=-build/include

SRCFILES = lexer.c parser.c context.cc expression.cc error.cc main.cc
OBJFILES = $(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(SRCFILES)))
APP = interpreter.elf

CFLAGS= -g -std=c++11 -DYYERROR_VERBOSE
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
parser.c: parser.y lexer.c
		$(YACC) $(YACCFLAGS) $<

# Both parser.{c,h} are generated on the same run, but if we write tule like
# parser.c parser.h:
#     bison parser.y
# Then bison will be called twice in the parallel build.
parser.h: parser.c

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

lint:
	$(CPPLINT) $(CPPLINTFLAGS) $(filter %.cc,$(SRCFILES))
