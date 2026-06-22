# Variables
MAKEFLAGS += -r
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

.SECONDARY:

all : Out.txt erf_error.png erf_evals.png

HEADERS = adaptive2.h adaptive3.h ClenshawCurtis.h adaptive_inf.h

Out.txt : main
	./main > Out.txt
	cat Out.txt

erf_error.png : Out.txt plot_erf.gp
	gnuplot plot_erf.gp

erf_evals.png : Out.txt plot_evals.gp
	gnuplot plot_evals.gp

% : %.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

%.o : %.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean :
	$(RM) main *.o *.txt *.png