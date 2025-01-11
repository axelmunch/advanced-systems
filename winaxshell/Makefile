CC=gcc
CFLAGS=-Wall -c -g
LDFLAGS=-I ./include
TEST_LDFLAGS=-lcriterion

SRC_DIR=./src
INC_DIR=./include
BIN_DIR=./bin
DOC_DIR=./doc
GCOV_DIR=./gcov
TEST_DIR=./tests
MOCKS_DIR=./mocks
BLUE=\033[1;34m
YELLOW=\033[1;33m
NC=\033[0m

GCOVFLAGS=-O0 --coverage -lgcov -Wall -g

LCOV_REPORT=report.info

SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(SRC:.c=.o)
EXEC=winaxshell

TEST_SRC=$(wildcard $(TEST_DIR)/*.c)
OBJ_NO_MAIN=$(filter-out $(SRC_DIR)/main.o, $(OBJ))
TEST_OBJ=$(TEST_SRC:.c=.o)
TEST_EXEC=winaxshell_tests

GEXEC=$(EXEC).cov

AR_NAME=archive_$(EXEC).tar.gz

all: $(SRC) $(EXEC) $(TEST_EXEC)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(MOCKS_DIR):
	@mkdir -p $(MOCKS_DIR)

%.o:%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

$(EXEC): $(OBJ)
	$(CC) -o $(BIN_DIR)/$@ -Wall $(LDFLAGS) $(OBJ)

$(TEST_OBJ): %.o: %.c
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

$(TEST_EXEC): $(TEST_OBJ) $(OBJ) $(MOCKS_DIR)
	$(CC) -o $(BIN_DIR)/$@ $(TEST_OBJ) $(OBJ_NO_MAIN) $(LDFLAGS) $(TEST_LDFLAGS)

doc:
	doxygen $(DOC_DIR)/doxygen.conf

gcov: $(GEXEC)
	$(GCOV_DIR)/$(GEXEC)

	find $(SRC_DIR) -name '*.gcno' -exec mv {} $(GCOV_DIR) \;
	find $(SRC_DIR) -name '*.gcda' -exec mv {} $(GCOV_DIR) \;

	gcov -o $(GCOV_DIR) $(SRC_DIR)/*.c
	lcov -o $(GCOV_DIR)/$(LCOV_REPORT) -c -d $(GCOV_DIR)
	genhtml -o $(GCOV_DIR)/report $(GCOV_DIR)/$(LCOV_REPORT)

package: gcov doc all
	rm -rf $(AR_NAME)
	tar cvfz $(AR_NAME) ./*

run:
	@echo "$(BLUE)Running $(EXEC)$(NC)"
	@./$(BIN_DIR)/$(EXEC)

tests:
	@echo "$(YELLOW)Running WinAxShell test suite...$(NC)"
	@./$(BIN_DIR)/$(TEST_EXEC) --verbose --jobs=1

memcheck: 
	@echo "$(YELLOW)Running Valgrind...$(NC)"
	@valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BIN_DIR)/$(TEST_EXEC)
	@valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BIN_DIR)/$(EXEC) 

clean:
	rm -rf $(OBJ) $(TEST_OBJ)
	rm -f /tmp/proc-exercise*

mrproper: clean
	rm -rf $(BIN_DIR)/*
	rm -rf $(DOC_DIR)/html
	rm -rf $(DOC_DIR)/latex
	rm -rf $(DOC_DIR)/man
	rm -rf $(GCOV_DIR)/*
	rm -rf mocks/*

.PHONY: doc tests run
