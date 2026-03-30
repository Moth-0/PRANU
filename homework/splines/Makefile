# Variables
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

all : partA.png partB.png partC.png

partA.png : files Makefile
	echo '\
	set terminal pngcairo ;\
	set output "$@" ;\
	set title "Linear Interpolation" ;\
	set xlabel "x" ;\
	set ylabel "y" ;\
	set tics out ;\
	plot "points_A.data" using 1:2 with points pt 7 lc rgb "black" title "Data", \
	 "inter_A.data" using 1:2 with lines lw 2 lc rgb "red" title "linterp", \
	 "inter_A.data" using 1:3 with lines lw 2 lc rgb "blue" dt 2 title "linterpInter" \
	' | tee log.fig1.gpi | gnuplot

partB.png : files Makefile
	echo '\
	set terminal pngcairo size 1500,500;\
	set output "$@" ;\
	set multiplot layout 1,3 title "Quadratic Interpolation" ;\
	set xlabel "x" ;\
	set ylabel "y" ;\
	set tics out ;\
	set key  top left ;\
	\
	set title "Constant: (x, 1)" ;\
	plot "points_B.data" using 1:2 with points pt 7 lc rgb "black" title "Data", \
	 "inter_B.data" using 1:2 with lines lw 2 lc rgb "red" title "eval", \
	 "inter_B.data" using 1:3 with lines lw 2 lc rgb "blue" dt 2 title "deriv", \
	 "inter_B.data" using 1:4 with lines lw 2 lc rgb "green" dt 2 title "inter" ;\
	\
	set title "Linear: (x, x)" ;\
	plot "points_B.data" using 1:3 with points pt 7 lc rgb "black" title "Data", \
	 "inter_B.data" using 1:5 with lines lw 2 lc rgb "red" title "eval", \
	 "inter_B.data" using 1:6 with lines lw 2 lc rgb "blue" dt 2 title "deriv", \
	 "inter_B.data" using 1:7 with lines lw 2 lc rgb "green" dt 2 title "inter" ;\
	\
	set title "Quadratic: (x, x^2)" ;\
	plot "points_B.data" using 1:4 with points pt 7 lc rgb "black" title "Data", \
	 "inter_B.data" using 1:8 with lines lw 2 lc rgb "red" title "eval", \
	 "inter_B.data" using 1:9 with lines lw 2 lc rgb "blue" dt 2 title "deriv", \
	 "inter_B.data" using 1:10 with lines lw 2 lc rgb "green" dt 2 title "inter" ;\
	\
	unset multiplot \
	' | tee log.fig2.gpi | gnuplot

partC.png : files Makefile
	echo '\
	set terminal pngcairo size 1500,500;\
	set output "$@" ;\
	set multiplot layout 1,3 title "Cubic Interpolation" ;\
	set xlabel "x" ;\
	set ylabel "y" ;\
	set tics out ;\
	set key  top left ;\
	\
	set title "Linear: (x, x)" ;\
	plot "points_C.data" using 1:1 with points pt 7 lc rgb "black" title "Data", \
	 "inter_C.data" using 1:2 with lines lw 2 lc rgb "red" title "eval", \
	 "inter_C.data" using 1:3 with lines lw 2 lc rgb "blue" dt 2 title "deriv", \
	 "inter_C.data" using 1:4 with lines lw 2 lc rgb "cyan" dt 2 title "d^2", \
	 "inter_C.data" using 1:5 with lines lw 2 lc rgb "green" dt 2 title "inter" ;\
	\
	set title "Quadratic: (x, x^2)" ;\
	plot "points_C.data" using 1:2 with points pt 7 lc rgb "black" title "Data", \
	 "inter_C.data" using 1:6 with lines lw 2 lc rgb "red" title "eval", \
	 "inter_C.data" using 1:7 with lines lw 2 lc rgb "blue" dt 2 title "deriv", \
	 "inter_C.data" using 1:8 with lines lw 2 lc rgb "cyan" dt 2 title "d^2", \
	 "inter_C.data" using 1:9 with lines lw 2 lc rgb "green" dt 2 title "inter" ;\
	\
	set title "cubic: (x, x^3)" ;\
	plot "points_C.data" using 1:3 with points pt 7 lc rgb "black" title "Data", \
	 "inter_C.data" using 1:10 with lines lw 2 lc rgb "red" title "eval", \
	 "inter_C.data" using 1:11 with lines lw 2 lc rgb "blue" dt 2 title "deriv", \
	 "inter_C.data" using 1:12 with lines lw 2 lc rgb "cyan" dt 2 title "d^2", \
	 "inter_C.data" using 1:13 with lines lw 2 lc rgb "green" dt 2 title "inter" ;\
	\
	unset multiplot \
	' | tee log.fig3.gpi | gnuplot

files : main
	./main

main : main.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

main.o : main.cc linspline.h quadspline.h cubespline.h
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean :
	$(RM) *.o *.txt *.log *.gpi *.data