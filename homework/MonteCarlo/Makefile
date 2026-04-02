# Variables
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

all : circ.png

circ.png : files 
	echo '\
	set terminal pngcairo ;\
	set output "$@" ;\
	set title "Circle integral as function of N" ;\
	set xlabel "N" ;\
	set ylabel "Err" ;\
	set grid ;\
	set logscale x ;\
	set format x "10^{%T}" ;\
	plot "circ.data" using 1:3 skip 1 with lines lw 2 lc rgb "black" title "Estimate", \
		 1.7/sqrt(x) with lines lw 2 dt 2 lc rgb "red" title "1/sqrt(x)" \
	' | tee log.fig1.gpi | gnuplot

files : main
	./main > Out.txt

main : main.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

main.o : main.cc montecarlo.h
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean :
	$(RM) *.o *.txt *.log *.gpi *.data