# Variables
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

all : main Out.txt

Out.txt : main Makefile
	./main > Out.txt
	cat Out.txt

main : main.cc matrix.h
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean :
	$(RM) *.o *.txt