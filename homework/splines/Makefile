# Variables
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

all : Out.txt plot.png

Out.txt : main
	./main 1>Out.txt 2>main.log
	cat Out.txt

plot.png : main Makefile
	echo '\
	set terminal pngcairo ;\
	set output "$@" ;\
	set title "Linear Interpolation" ;\
	set xlabel "x" ;\
	set ylabel "y" ;\
	set tics out ;\
	plot "points.data" using 1:2 with points pt 7 lc rgb "black" title "Data", \
	 "inter.data" using 1:2 with lines lw 2 lc rgb "red" title "linterp", \
	 "inter.data" using 1:3 with lines lw 2 lc rgb "blue" dt 2 title "linterpInter" \
	' | tee log.fig.gpi | gnuplot

main : main.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

main.o : main.cc spline.h
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean :
	$(RM) *.o *.txt *.log