# Default Makefile
# Variables
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

# Main file
TARGET = math

# Object files .o (add all) 
OBJECTS = math.o sfuns.o

# txt output
Out.txt : $(TARGET)
	./$(TARGET) > Out.txt


$(TARGET) : $(OBJECTS)
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $(TARGET) $(OBJECTS)

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean :
	$(RM) Out.txt $(TARGET) $(OBJECTS)