OBJECTS_DIR	= obj
BINARY_DIR	= bin
SOURCES_DIR = sources

TARGET		= $(BINARY_DIR)/yesno
SOURCES		= $(wildcard $(SOURCES_DIR)/*.c)
OBJECTS		= $(SOURCES:$(SOURCES_DIR)/%.c=$(OBJECTS_DIR)/%.o)

CC 			= gcc
CFLAGS		= -Wall -pedantic -std=gnu99 -Iinclude

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINARY_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJECTS_DIR)/%.o: $(SOURCES_DIR)/%.c | $(OBJECTS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINARY_DIR) $(OBJECTS_DIR):
	mkdir -p $@

.PHONY: clean
clean: 
	@$(RM) -rv $(BINARY_DIR) $(OBJECTS_DIR)

-include $(OBJECTS:.o=.d)

