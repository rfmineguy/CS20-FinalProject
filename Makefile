CC := g++
CFLAGS := -Wall -std=c++11 -ggdb
CFLAGS_HARSH := -Wall -Wextra -Werror -std=c++11 -ggdb

TEST_MAIN_SRC := src/test_main.cpp
TEST_SRC 	  := include/CircularQueue.hpp
TEST_MAIN_BIN := test_main


# CLOC FLAGS
CLOC := cloc
CLOC_FLAGS := --by-file
CLOC_DIRS := src/ include/

$(TEST_MAIN_BIN): $(TEST_MAIN_SRC)
	-$(CC) $(CFLAGS_HARSH) $^ -o $@
	
cloc:
	$(CLOC) $(CLOC_FLAGS) $(CLOC_DIRS)
	
clean:
	$(shell rm $(TEST_MAIN_BIN))