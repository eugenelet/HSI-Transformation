RCS = $(wildcard *.cpp)
PROGS = $(patsubst %.cpp,%,$(SRCS))
OBJS = $(SRCS:.cpp=.o)
TEMPS = $(SRCS:.cpp=.txt)
SRC = main.cpp
SRC2 = main_Taskbar.cpp
CFLAGS = `pkg-config --cflags --libs opencv` -O3
LDFLAGS = `pkg-config --libs opencv`
OUT = imgTrans

all: $(OUT)

$(OUT): $(SRC)
	g++ $(SRC) $(CFLAGS)  -o $@

task: $(SRC2)
	g++ $(SRC2) $(CFLAGS)  -o $@

clean:
	@rm -f $(PROGS) $(OBJS) $(TEMPS) $(OUT)
	@echo "Limpo!"
