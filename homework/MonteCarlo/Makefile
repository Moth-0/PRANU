# Variables
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

all : main circle_error.png sine_wave.png

circle_error.png : files Makefile
	echo '\
	set terminal pngcairo ;\
	set output "$@" ;\
	set title "Circle integral error as function of N" ;\
	set xlabel "N" ;\
	set ylabel "Err" ;\
	set grid ;\
	set logscale x ;\
	set format x "10^{%T}" ;\
	plot "circ.data" using 1:3 skip 1 with lines lw 2 lc rgb "black" title "Estimate", \
		 1.7/sqrt(x) with lines lw 2 dt 2 lc rgb "red" title "1/sqrt(x)" \
	' | tee log.fig1.gpi | gnuplot

sine_wave.png : files Makefile
	echo '\
	set terminal pngcairo ;\
	set output "$@" ;\
	set title "Actual error of integration methods" ;\
	set xlabel "N" ;\
	set ylabel "Err" ;\
	set grid ;\
	set logscale x ;\
	set format x "10^{%T}" ;\
	plot "sine.data" using 1:2 skip 1 with lines lw 2 dt 2 lc rgb "black" title "Plain", \
		 "sine.data" using 1:3 skip 1 with lines lw 2 dt 2 lc rgb "red" title "Quasi" \
	' | tee log.fig2.gpi | gnuplot

files : main
	./main > Out.txt

main : main.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

main.o : main.cc montecarlo.h
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean :
	$(RM) *.o *.txt *.log *.gpi *.data *.png