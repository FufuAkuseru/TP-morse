CC      = gcc
CFLAGS  = -Wall -Wextra -Wconversion
SRC     = $(wildcard *.c)
OBJ     = $(SRC:.c=.o)
EXE     = morse
LDFLAGS =

ifeq ($(DEBUG),yes)
	CFLAGS  += -g
	LDFLAGS +=
else
	CFLAGS  += -O2 -DNDEBUG
	LDFLAGS +=
endif

all: $(OBJ) $(EXE)

$(EXE): $(OBJ)
	@printf "\t%-7s: %-18s → %s\n" "CCLD" $< $@
	@$(CC) -o $@ $^ $(LDFLAGS)
	@if [ ! -d obj ];\
	then printf "\t%-7s: %-18s\n" "MKDIR" "obj";\
	mkdir -p obj;\
	fi
	@mv *.o obj/

clean:
	rm -rf *.o *.so *.a *.la

mrproper: clean
	rm -rf $(EXE)

rebuild: mrproper all

.PHONY: all clean mrproper rebuild
