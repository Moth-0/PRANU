CXXFLAGS = -Wall -Werror -O
LDLIBS = -lstdc++ -lm
RM = del /f

Out.txt : hello
	hello > Out.txt
hello : hello.o
	$(CXX) hello.o -o hello $(LDLIBS)
hello.o : hello.cc
	$(CXX) $(CXXFLAGS) -c hello.cc -o hello.o

clean :
	$(RM) Out.txt hello.o hello