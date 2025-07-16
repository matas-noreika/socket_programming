#The build target (defaults to all)
TARGET= $(LOCALBIN)/simple_client.exe

#C compiler
CC= gcc
#pre-processor flags
CPPFLAGS= -MMD

#local directories
LOCALBIN= bin
LOCALOBJ= obj
LOCALDEP= dep

#general variables
#retrieve all source files with the .c extension
SRC=$(wildcard src/*.c)
OBJ=$(SRC:src/%.c=$(LOCALOBJ)/%.o)

#default make rule to build all executables
all: $(TARGET) 
	
#general make rule for generating object files from c source files
$(LOCALOBJ)/%.o: src/%.c
	$(CC) $(CPPFLAGS) -c $< -o $@

#general make rule for generating binary executable from object file
$(LOCALBIN)/%.exe: $(LOCALOBJ)/%.o
	$(CC) $^ -o $@
