SHELL := /bin/bash

MAIN_LIBS = -lpthread -pthread
TEST_LIBS = -lgtest -lgtest_main $(MAIN_LIBS)

MAIN_SRC = $(wildcard src/*.cpp src/board/*.cpp src/gpnode/*.cpp src/evolution/*.cpp)
TEST_SRC = $(wildcard test/*.cpp)

TO_OBJS = $(patsubst %.cpp, obj/%.o, $(1))

MAIN_OBJS = $(call TO_OBJS,$(MAIN_SRC))
TEST_OBJS = $(filter-out obj/src/main.o, $(call TO_OBJS, $(MAIN_SRC) $(TEST_SRC)))

.PHONY: run_test
run_test: testexe
	@./testexe

.PHONY: run_main
run_main: mainexe
	@./mainexe --play | tee log

.PHONY: run_graph
run_graph: mainexe
	@./mainexe | tee log >(python graph_progress.py)

testexe: $(TEST_OBJS)
	@g++ -o $@ $^ $(TEST_LIBS)  

mainexe: $(MAIN_OBJS)
	@g++ -o $@ $^ $(MAIN_LIBS)

obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	@g++ -g -Wall -Wextra -Werror -Wfatal-errors -c -std=c++1y -I src -I src/evolution -I src/board -I src/gpnode -o $@ $^

.PHONY: clean
clean:
	@rm -rf obj
	@rm -f mainexe testexe tags

.PHONY: ctags
ctags:
	@ctags -R .
