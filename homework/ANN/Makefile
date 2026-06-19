# Variables
MAKEFLAGS += -r
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

.SECONDARY:

all : model_vs_actual.png

HEADERS = network.h minimization.h matrix.h QR.h

Out.txt : main 
	./main > Out.txt

model.data : main func.in
	./main

model_vs_actual.png : plot_model.gp model.data
	gnuplot plot_model.gp

func.in : rndData
	./rndData 200 > func.in

% : %.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

%.o : %.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean :
	$(RM) *.o *.txt *.log *.gpi *.data *.png