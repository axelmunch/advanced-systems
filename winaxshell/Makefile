CC=gcc
CFLAGS=-Wall -c -g
LDFLAGS=-I ./include

SRC_DIR=./src
INC_DIR=./include
BIN_DIR=./bin
DOC_DIR=./doc
GCOV_DIR=./gcov

GCOVFLAGS=-O0 --coverage -lgcov -Wall -g

LCOV_REPORT=report.info

SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(SRC:.c=.o)
EXEC=winaxshell

GEXEC=$(EXEC).cov

AR_NAME=archive_$(EXEC).tar.gz

all: $(SRC) $(EXEC)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

%.o:%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

$(EXEC): $(OBJ)
	@mkdir -p bin
	$(CC) -o $(BIN_DIR)/$@ -Wall $(LDFLAGS) $(OBJ)

$(GEXEC):
	$(CC) $(GCOVFLAGS) -o $(GCOV_DIR)/$@ -Wall $(LDFLAGS) $(SRC)

doc:
	doxygen $(DOC_DIR)/doxygen.conf

gcov: $(GEXEC)
	$(GCOV_DIR)/$(GEXEC) -h

	find ./ -maxdepth 1 -name *.gcno -exec mv {} $(GCOV_DIR) \;
	find ./ -maxdepth 1 -name *.gcda -exec mv {} $(GCOV_DIR) \;

	gcov -o $(GCOV_DIR) $(GEXEC)
	lcov -o $(GCOV_DIR)/$(LCOV_REPORT) -c -f -d $(GCOV_DIR)
	genhtml -o $(GCOV_DIR)/report $(GCOV_DIR)/$(LCOV_REPORT)

package: gcov doc all
	rm -rf $(AR_NAME)
	tar cvfz $(AR_NAME) ./*

run:
	@echo "Running $(EXEC)"
	@./$(BIN_DIR)/$(EXEC)

clean:
	rm -rf $(OBJ)
	rm -f /tmp/proc-exercise*

mrproper: clean
	rm -rf $(BIN_DIR)/*
	rm -rf $(DOC_DIR)/html
	rm -rf $(DOC_DIR)/latex
	rm -rf $(DOC_DIR)/man
	rm -rf $(GCOV_DIR)/*

.PHONY: doc
