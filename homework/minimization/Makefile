# Variables
MAKEFLAGS += -r
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

all : Out.txt higgs.png

HEADERS = minimization.h matrix.h QR.h

Out.txt : main 
	./main > Out.txt

fit_higs.data : Out.txt

higgs.png : fit_higs.data higgs.in Makefile
	@echo '\
	set terminal pngcairo size 800,400 ;\
	set output "$@" ;\
	set xlabel "E" ;\
	set ylabel "F(E|m,A,Γ)" ;\
	set grid ;\
	stats "$<" every ::0::0 using 1 nooutput ;\
	m = STATS_min ;\
	stats "$<" every ::0::0 using 2 nooutput ;\
	A = STATS_min ;\
	stats "$<" every ::0::0 using 3 nooutput ;\
	G = STATS_min ;\
	set title sprintf("Breit-Wigner Fit Result\nm = %g, A = %g, {/Symbol G} = %g", m, A, G) ;\
	plot "higgs.in" using 1:2:3 with yerrorbars pt 7 ps 0.5 lc rgb "red" title "Experimental Data", \
	     "$<" every ::1 using 1:2 with lines lw 2 lc rgb "black" title "Breit-Wigner Fit" \
	' | tee log.higgs.gpi | gnuplot

% : %.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

%.o : %.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean :
	$(RM) *.o *.txt *.log *.gpi *.data *.png