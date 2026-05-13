# Variables
MAKEFLAGS += -r
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

all : Out.txt

HEADERS = minimization.h matrix.h QR.h

Out.txt : main 
	./main > Out.txt

higgs.png : Out.txt higgs.data Makefile
	@echo '\
	set terminal pngcairo size 800,400 ;\
	set output "$@" ;\
	set xlabel "E" ;\
	set ylabel "F(E|m,A,Γ)" ;\
	set grid ;\
	stats "$<" every ::0::0 using 1:2 nooutput ;\
	m = STATS_min_x ;\
	A = STATS_min_y ;\
	stats "$<" every ::0::0 using 3 nooutput ;\
	G = STATS_MIN ;\
	set title sprintf("Breit-Wigner Fit Result\nm = %g, A = %g, {/Symbol G} = %g", m, A, G) ;\
	plot "$<" every ::1 using 1:2 with lines lw 2 lc rgb "black" \
	' | tee log.higgs.gpi | gnuplot

% : %.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

%.o : %.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean :
	$(RM) *.o *.txt *.log *.gpi *.data *.png