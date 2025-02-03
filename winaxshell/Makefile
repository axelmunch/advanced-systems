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

GCOVFLAGS=-O0 --coverage -lgcov -Wall -g -fprofile-arcs -ftest-coverage
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
	@echo "$(BLUE)=== Compilation completed ===$(NC)"

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

$(TEST_EXEC): $(TEST_OBJ) $(OBJ_NO_MAIN) $(MOCKS_DIR)
	$(CC) -o $(BIN_DIR)/$@ $(TEST_OBJ) $(OBJ_NO_MAIN) $(LDFLAGS) $(TEST_LDFLAGS)

$(GEXEC): $(TEST_OBJ) $(OBJ_NO_MAIN) $(MOCKS_DIR)
	$(CC) $(GCOVFLAGS) -o $(GCOV_DIR)/$@ $(LDFLAGS) $(TEST_OBJ) $(OBJ_NO_MAIN) $(TEST_LDFLAGS)

doc:
	doxygen $(DOC_DIR)/doxygen.conf

gcov: $(GEXEC)
	@echo "$(YELLOW)=== Running Unit Tests with Coverage ===$(NC)"
	$(GCOV_DIR)/$(GEXEC) --verbose --jobs=1 --color=always

	@echo "Generating Coverage Report..."
	@find ./ -name '*.gcda' -exec mv {} $(GCOV_DIR)/ \;
	@find ./ -name '*.gcno' -exec mv {} $(GCOV_DIR)/ \;

	lcov -o $(GCOV_DIR)/$(LCOV_REPORT) -c -f -d $(GCOV_DIR)
	genhtml -o $(GCOV_DIR)/report $(GCOV_DIR)/$(LCOV_REPORT)
	@echo "$(BLUE)Coverage report generated in $(GCOV_DIR)/report/index.html$(NC)"

package: gcov doc all
	@rm -rf $(AR_NAME)
	@tar cvfz $(AR_NAME) ./*

run:
	@echo "$(BLUE)=== Running WinAxShell ===$(NC)"
	@./$(BIN_DIR)/$(EXEC)

tests:
	@echo "$(YELLOW)=== Running WinAxShell test suite ===$(NC)"
	@./$(BIN_DIR)/$(TEST_EXEC) --verbose --jobs=1 --color=always

memcheck:
	@echo "$(YELLOW)=== Running Valgrind ===$(NC)"
	@valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BIN_DIR)/$(EXEC)

clean:
	@echo "$(YELLOW)=== Cleaning up compile files ===$(NC)"
	rm -rf $(OBJ) $(TEST_OBJ)
	rm -f /tmp/proc-exercise*
	find . -type f -name "*.gcda" -delete
	find . -type f -name "*.gcno" -delete

mrproper: clean
	@echo "$(YELLOW)=== Cleaning up all files ===$(NC)"
	rm -rf $(BIN_DIR)/*
	rm -rf $(DOC_DIR)/html
	rm -rf $(DOC_DIR)/latex
	rm -rf $(DOC_DIR)/man
	rm -rf $(GCOV_DIR)/*
	rm -rf mocks/*
	rm -rf $(AR_NAME)
	@echo "$(YELLOW)All files cleaned up$(NC)"

.PHONY: doc tests run
