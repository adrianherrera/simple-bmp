#
# Compile with a static glibc
#

TARGET = simple_bmp
OBJ = $(TARGET).o
SRC = $(TARGET).c
CC = gcc
CFLAGS = -g -m32
LDFLAGS = -nostdlib -nostartfiles -static -m32
GLIBCDIR = 
STARTFILES = $(GLIBCDIR)/crt1.o $(GLIBCDIR)/crti.o `gcc -m32 --print-file-name=crtbegin.o`
ENDFILES = `gcc -m32 --print-file-name=crtend.o` $(GLIBCDIR)/crtn.o
LIBGROUP = -Wl,--start-group $(GLIBCDIR)/libc.a -lgcc -lgcc_eh -Wl,--end-group

.PHONY: clean all

all: $(TARGET) s2e_$(TARGET)

#
# Non-S2E
#

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(STARTFILES) $^ $(LIBGROUP) $(ENDFILES) 

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f *.o *.~ $(TARGET) s2e_$(TARGET)

#
# S2E
#

S2E_INC = $(S2EDIR)/install/bin/guest-tools32/include
S2E_OBJ = s2e_$(OBJ)
S2E_TARGET = s2e_$(TARGET)

$(S2E_TARGET): $(S2E_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(STARTFILES) $^ $(LIBGROUP) $(ENDFILES) 

$(S2E_OBJ): $(SRC)
	$(CC) $(CFLAGS) -DS2E -c $^ -I$(S2E_INC) -o $@
