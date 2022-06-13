OBJECTS_DIR	= obj
BINARY_DIR	= bin

TARGETS		= $(patsubst %.c, $(BINARY_DIR)/%, $(wildcard *.c))
OBJECTS		= $(patsubst %.c, $(OBJECTS_DIR)/%.o, $(wildcard *.c))

CC 			= gcc
CFLAGS 		= -g -Werror -std=gnu99 -fopenmp

.PHONY: all
all: $(TARGETS) $(OBJECTS)

$(BINARY_DIR)/%: $(OBJECTS_DIR)/%.o | $(BINARY_DIR) 
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS) 

$(OBJECTS_DIR)/%.o: %.c | $(OBJECTS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINARY_DIR) $(OBJECTS_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	@$(RM) -rv $(BINARY_DIR) $(OBJECTS_DIR)

