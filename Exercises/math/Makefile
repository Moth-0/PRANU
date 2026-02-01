CXXFLAGS = -Wall -Werror -O
LDLIBS = -lstdc++ -lm
RM = rm -f

Out.txt : math
	./math > Out.txt
math : math.o
	$(CXX) math.o -o math $(LDLIBS)
math.o : math.cc
	$(CXX) $(CXXFLAGS) -c math.cc -o math.o

clean :
	$(RM) Out.txt math.o math