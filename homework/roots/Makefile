# Variables
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

all : main 

HEADERS = matrix.h newton.h QR.h

main : main.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

main.o : main.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean :
	$(RM) *.o *.txt *.log *.gpi *.data *.png