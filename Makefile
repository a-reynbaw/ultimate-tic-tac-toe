# File:   Makefile
# Author: Anastasia Marinakou | sdi2400120
# Course: Data Structures and Programing Techniques (Even)

# Brief: use this Makefile to compile main.c and state.c

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Werror -Wextra -pedantic -Iinclude

# Directories
SRCDIR = src
INCDIR = include

# Source files
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/state.c

# Header files
HEADERS = $(INCDIR)/state.h

# Object files
OBJS = $(SOURCES:.c=.o)

# Output program name
PROGRAM = game

# Rule to build the executable
$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROGRAM)

# Rule to compile .c files into .o files, ensuring header dependencies
$(SRCDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(PROGRAM) $(SRCDIR)/*.o