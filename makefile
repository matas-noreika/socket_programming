# The build target (defaults to all)
TARGET= $(LOCALBIN)/simple_client.exe

# C compiler
CC= gcc

# local directories
LOCALBIN= bin
LOCALOBJ= obj
LOCALDEP= dep

# general variables
# retrieve all source files with the .c extension
SRC=$(wildcard src/*.c)
OBJ=$(SRC:src/%.c=$(LOCALOBJ)/%.o)
DEP=$(SRC:src/%.c=$(LOCALDEP)/%.d)

#prevents the deletion of object files by make
.PRECIOUS : $(LOCALOBJ)/%.o

# default make rule to build all executables
all : $(TARGET) 

# clean rule to remove all generated files
clean :
	-@rm $(LOCALBIN)/*.exe 2> /dev/null || true
	-@rm $(LOCALOBJ)/*.o 2> /dev/null || true
	-@rm $(LOCALOBJ)/*.d 2> /dev/null || true
	-@rm $(LOCALDEP)/*.d 2> /dev/null || true


-include $(DEP)

# general make rule for generating object files from c source files
$(LOCALOBJ)/%.o : src/%.c
	$(CC) -MMD -c $< -o $@
	mv -f $(LOCALOBJ)/$(*F).d $(LOCALDEP)/$(*F).d

# general make rule for generating binary executable from object file
$(LOCALBIN)/%.exe : $(LOCALOBJ)/%.o
	$(CC) $^ -o $@
