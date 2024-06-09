FASTERAC_CFLAGS = $(shell pkg-config  --cflags libfasterac)
FASTERAC_LIBS   = $(shell pkg-config  --libs   libfasterac)

ROOT_CFLAGS     = $(shell root-config --cflags)
ROOT_LIBS       = $(shell root-config --libs)

CC        = g++
CFLAGS    = ${FASTERAC_CFLAGS} ${ROOT_CFLAGS} -Iinclude
LIBS      = ${FASTERAC_LIBS}   ${ROOT_LIBS}

SRCS      = $(wildcard src/*.c++)
OBJS      = $(patsubst src/%.c++,obj/%.o,$(SRCS))
EXE       = Fast2Root

all : $(EXE)

$(EXE): $(OBJS)
	${CC} -std=c++17 ${CFLAGS} ${LIBS} -lSpectrum $^ -o $@

s: CFLAGS += -DUSE_SIGNAL_DICT

s: $(OBJS)
	${CC} -std=c++17 ${CFLAGS} ${LIBS} -lSpectrum $^ -o $(EXE) ../lib/SignalDict/SignalDict.so

obj/%.o: src/%.c++ include/%.hh
	@mkdir -p obj
	${CC} -std=c++17 -c ${CFLAGS} $< -o $@

$(DICT_OBJ): $(DICT_SRC)
	${CC} -std=c++17 -c ${CFLAGS} $< -o $@

$(DICT_LIB): $(DICT_OBJ)
	${CC} -shared -o $@ $< ${ROOT_LIBS}

clean :
	rm -f obj/*.o
	rm -f $(EXE)
