CC = gcc
YACC = yacc
LEX = lex
CFLAGS = -Wall -g

TRANSLATOR_DIR = translator
EXAMPLES_DIR = examples
OUTPUT_DIR = output

all: translator

translator: $(TRANSLATOR_DIR)/iotiny.y $(TRANSLATOR_DIR)/iotiny.l
	@echo "Building yacc-based translator..."
	cd $(TRANSLATOR_DIR) && $(YACC) -d iotiny.y
	cd $(TRANSLATOR_DIR) && $(LEX) iotiny.l
	cd $(TRANSLATOR_DIR) && $(CC) $(CFLAGS) -o iotiny_translator y.tab.c lex.yy.c -ll
	@echo "Translator built: translator/iotiny_translator"

translate: translator
	@echo "=== Translating Example Programs ==="
	@echo
	@echo "Program 1: Temperature Monitoring"
	./$(TRANSLATOR_DIR)/iotiny_translator $(EXAMPLES_DIR)/program1.iotiny $(OUTPUT_DIR)/program1.c
	@echo
	@echo "Program 2: Smart Home Automation"
	./$(TRANSLATOR_DIR)/iotiny_translator $(EXAMPLES_DIR)/program2.iotiny $(OUTPUT_DIR)/program2.c
	@echo
	@echo "Translation complete! C files in $(OUTPUT_DIR)/"

compile: translate
	@echo "=== Compiling Generated C Code ==="
	@echo
	@echo "Compiling program1.c..."
	$(CC) $(CFLAGS) -o $(OUTPUT_DIR)/program1 $(OUTPUT_DIR)/program1.c
	@echo
	@echo "Compiling program2.c..."
	$(CC) $(CFLAGS) -o $(OUTPUT_DIR)/program2 $(OUTPUT_DIR)/program2.c
	@echo
	@echo "Compilation complete! Executables in $(OUTPUT_DIR)/"

run: compile
	@echo "=== Running Translated Programs ==="
	@echo
	@echo "========== Program 1 Output =========="
	./$(OUTPUT_DIR)/program1
	@echo
	@echo "========== Program 2 Output =========="
	./$(OUTPUT_DIR)/program2
	@echo
	@echo "Execution complete!"

test: run
	@echo
	@echo "=== Test Summary ==="
	@echo "✓ Translator built successfully"
	@echo "✓ 2 programs translated to C"
	@echo "✓ Generated C code compiled"
	@echo "✓ Programs executed successfully"

clean:
	rm -f $(TRANSLATOR_DIR)/y.tab.* $(TRANSLATOR_DIR)/lex.yy.c
	rm -f $(TRANSLATOR_DIR)/iotiny_translator
	rm -f $(OUTPUT_DIR)/*.c $(OUTPUT_DIR)/program1 $(OUTPUT_DIR)/program2
	@echo "Clean complete"

.PHONY: all translator translate compile run test clean
