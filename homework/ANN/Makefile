# Variables
MAKEFLAGS += -r
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

.SECONDARY:

all : modelfit.png ODE.png

HEADERS = network.h minimization.h matrix.h QR.h

Out.txt : func.in main
	./main > Out.txt

model.data : Out.txt

modelfit.png : model.data plot_model.gp 
	gnuplot plot_model.gp

func.in : rndData
	./rndData 200 > func.in

ODE.png : ode_results.data
	gnuplot plot_ode.gp

ode_results.data : mainC
	./mainC

% : %.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

%.o : %.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean :
	$(RM) main rndData *.o *.txt *.log *.gpi *.data *.png *.in