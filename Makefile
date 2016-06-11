SHELL := /bin/bash

TEST_LIBS = -lgtest -lpthread -lgtest_main

MAIN_SRC = $(wildcard src/*.cpp)
TEST_SRC = $(wildcard test/*.cpp)

TO_OBJS = $(patsubst %.cpp, obj/%.o, $(1))

MAIN_OBJS = $(call TO_OBJS,$(MAIN_SRC))
TEST_OBJS = $(filter-out obj/src/main.o, $(call TO_OBJS, $(MAIN_SRC) $(TEST_SRC)))

.PHONY: run_test
run_test: testexe
	@./testexe

.PHONY: run_main
run_main: mainexe
	@./mainexe --play

.PHONY: run_graph
run_graph: mainexe
	./mainexe | tee >(python graph_progress.py)

testexe: $(TEST_OBJS)
	@g++ -o $@ $^ $(TEST_LIBS)  

mainexe: $(MAIN_OBJS)
	@g++ -o $@ $^

obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	@g++ -g -Wall -Wextra -Werror -c -std=c++1y -I src -o $@ $^

.PHONY: clean
clean:
	@rm -rf obj
	@rm -f mainexe testexe tags

.PHONY: ctags
ctags:
	@ctags -R .
