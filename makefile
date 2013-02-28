SRCDIR:=src
TSTDIR:=src/test
BINDIR:=bin
OBJDIR:=bin/obj
SRCSUBDIRS:=$(notdir $(patsubst %/,%,$(filter %/,$(wildcard $(SRCDIR)/*/))))
GTESTLIBS:=-lgtest -lgtest_main
CXX:=g++ -std=c++11
CFLAGS:=-Wall -O3
LIBS:=-Llibs -lpthread -lrt
TSTFLAGS:=-O0 -Wall -g
TSTLIBS:=$(GTESTLIBS) $(LIBS)
BINS:=flow
INTLIBS:=libflow#$(addprefix libflow-, $(SRCSUBDIRS)) libflow

TSTBINS:=$(notdir $(basename $(wildcard $(TSTDIR)/*.cc)))
TSTOBJS:=$(addsuffix .o, $(notdir $(basename $(wildcard $(TSTDIR)/*.cc))))
OBJS:=$(notdir $(basename $(wildcard $(SRCDIR)/*.cc)))
OBJS:=$(addsuffix .o, $(filter-out $(BINS), $(OBJS)))
OBJS:=$(addprefix $(OBJDIR)/, $(OBJS))
BINS:=$(addprefix $(BINDIR)/, $(BINS))
TSTBINS:=$(addprefix $(BINDIR)/, $(TSTBINS))

all: compile

compile: makedirs $(BINS) libflow
	@echo "finished compiling"

libs: makedirs $(INTLIBS)
	@echo "compiled libs"

profile: CFLAGS=-Wall -O3 -DPROFILE
profile: LIBS+=-lprofiler
profile: clean compile

opt: CFLAGS=-Ofast -flto -mtune=native -DNDEBUG
opt: clean all

debug: CFLAGS=-O0 -g
debug: clean all

depend: cpplint
	@echo "compiled all dependencies"

makedirs:
	@mkdir -p include/flow
	@mkdir -p lib
	@mkdir -p libs
	@mkdir -p bin/obj

cpplint:
	@git submodule init;
	@git submodule update;

check: makedirs $(TSTBINS)
	@for t in $(TSTBINS); do ./$$t; done
	@echo "completed tests"

checkstyle:
	@python tools/cpplint/cpplint.py \
		--filter=-readability/streams,-readability/multiline_string\
		$(SRCDIR)/*.h $(SRCDIR)/*.cc $(SRCDIR)/*/*.h $(SRCDIR)/*/*.cc

clean:
	@rm -f $(OBJDIR)/*.o
	@rm -f libs/*.a
	@rm -rf include/*
	@rm -f $(BINS)
	@rm -f $(TSTBINS)
	@echo "cleaned"

.PRECIOUS: $(OBJS) $(TSTOBJS)
.PHONY: libs all compile profile opt depend makedirs check\
	cpplint checkstyle clean

libflow: $(OBJS)
	@cp $(SRCDIR)/*.h include/flow/;
	@cp -r $(SRCDIR)/* include/flow/;
	@rm -rf include/flow/test/;
	@echo "copied headers"

libflow-%: $(SRCDIR)/%/*.cc $(SRCDIR)/%/*.h
	$(eval LIBFILES:=$(notdir $(basename $(wildcard $(SRCDIR)/$*/*.cc))))
	$(eval LIBOBJS:=$(addprefix $(OBJDIR)/$(@F)-, $(addsuffix .o, $(LIBFILES))))
	@for i in $(LIBFILES); do \
	  $(CXX) $(CFLAGS) -o $(OBJDIR)/$(@F)-$$i.o -c $(SRCDIR)/$*/$$i.cc; \
	done;
	@ar rs libs/$(@F).a $(LIBOBJS) 2>/dev/null
	@echo "compiled libs/$(@F).a"

$(BINDIR)/%: $(OBJS) $(SRCDIR)/%.cc
	@$(CXX) $(CFLAGS) -o $(OBJDIR)/$(@F).o -c $(SRCDIR)/$(@F).cc
	@$(CXX) $(CFLAGS) -o $(BINDIR)/$(@F) $(OBJDIR)/$(@F).o $(OBJS) $(LIBS)
	@echo "compiled $(BINDIR)/$(@F)"

$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(SRCDIR)/%.h
	$(CXX) $(CFLAGS) -o $(OBJDIR)/$(@F) -c $<

$(BINDIR)/%-test: $(OBJS) $(TSTDIR)/*.cc
	@$(CXX) $(CFLAGS) -o $(OBJDIR)/$(@F).o -c $(TSTDIR)/$(@F).cc
	@$(CXX) $(CFLAGS) -o $(BINDIR)/$(@F) $(OBJDIR)/$(@F).o $(OBJS) $(TSTLIBS)
	@echo "compiled $(BINDIR)/$(@F)"

$(OBJDIR)/%-test.o: $(TSTDIR)/%.cc
	@$(CXX) $(CFLAGS) -o $(OBJDIR)/$(@F) -c $<
