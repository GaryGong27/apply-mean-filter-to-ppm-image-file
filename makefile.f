CFLAGS= -O2 -ansi
all: filter.o read_writeppm.o
	$(CXX) -o filter $?
