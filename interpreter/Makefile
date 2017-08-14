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
CPPLINTFLAGS = --filter=-build/include --root=interpreter

HFILES = context.h error.h expression.h statement.h value.h
SRCFILES = \
		   context.cc \
		   error.cc \
		   expression.cc \
		   main.cc \
		   value.cc \
		   lexer.cc \
		   parser.cc
OBJFILES = $(patsubst %.cc,%.o,$(SRCFILES))
APP = interpreter.elf

CFLAGS= -g -std=c++11 -DYYERROR_VERBOSE -MMD -pthread
CFLAGS += -O3
CXXFLAGS = $(CFLAGS)

all: $(APP)

# Include generated dependency files if they exist.
-include $(SRCFILES:%.cc=%.d)

$(OBJFILES): parser.h $(HFILES)

CLEAN += $(APP)
$(APP):	$(OBJFILES)
		$(CC) $(CFLAGS) $+ -o $@

CLEAN += lexer.cc lexer.h
lexer.cc: lexer.l
		$(LEX) $(LEXFLAGS) $<

CLEAN += parser.cc parser.h
parser.cc: parser.y lexer.cc
		$(YACC) $(YACCFLAGS) $<

# Both parser.{cc,h} are generated on the same run, but if we write tule like
# parser.cc parser.h:
#     bison parser.y
# Then bison will be called twice in the parallel build.
parser.h: parser.cc

TESTS = $(patsubst %.in,%,$(wildcard tests/*.in))

# stderr is directed to the same file as stdout, so it can be compared with a
# single reference output file.
test: $(APP)
	for t in $(TESTS); do \
		./$(APP) < $${t}.in > $${t}.app_out 2>&1 ;\
		cmp $${t}.app_out $${t}.out ;\
	done

clean:
		rm -f *.d *.o *~ $(CLEAN) $(addsuffix .app_out,$(TESTS))

LINTFILES = $(filter-out parser.cc lexer.cc,$(SRCFILES))
lint:
	$(CPPLINT) $(CPPLINTFLAGS) $(LINTFILES) $(HFILES)
