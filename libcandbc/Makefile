#libcandbc library makefile

CC=gcc
CC_FLAGS=-c -O0

OUTPUT_DIR=obj

LIBCANDBC=candbc.dll

TARGETS+=$(LIBCANDBC)
TARGETS_OUT=$(TARGETS:%=$(OUTPUT_DIR)/%)

LIBCANDBC_IMPORT=$(OUTPUT_DIR)/libcandbc.a
LIBCANDBC_DEF=$(OUTPUT_DIR)/candbc.def


LIBCANDBC_OBJS= dbcModel.o \
dbcReader.o \
dbcWriter.o \
lexer.o \
parser.o

LIBCANDBC_OBJS_OUT=$(LIBCANDBC_OBJS:%=$(OUTPUT_DIR)/%)

INCLUDE_DIRS=.

all: folders $(TARGETS)

clean: 
	rm -f $(LIBCANDBC_OBJS_OUT) $(TARGETS_OUT) $(LIBCANDBC_IMPORT) $(LIBCANDBC_DEF)

folders:
	mkdir -p $(OUTPUT_DIR)


$(LIBCANDBC): $(LIBCANDBC_OBJS)
	$(CC) -shared -Wl,-no-undefined -Wl,--output-def,$(LIBCANDBC_DEF) -Wl,--enable-runtime-pseudo-reloc -Wl,--out-implib,$(LIBCANDBC_IMPORT) $(LIBCANDBC_OBJS_OUT) -o $(OUTPUT_DIR)/$@

%.o:%.c
	$(CC) $(CC_FLAGS) -I $(INCLUDE_DIRS) $< -o $(OUTPUT_DIR)/$@
