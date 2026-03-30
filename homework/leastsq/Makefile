# Variables
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

all : plot.png Out.txt

Out.txt : activity.data
	cat Out.txt

plot.png : activity.data Makefile
	echo '\
	set terminal pngcairo ;\
	set output "$@" ;\
	set title "Decay data and fit" ;\
	set xlabel "time (days)" ;\
	set ylabel "ln(activity)" ;\
	set tics out ;\
	load "params.gp" ;\
	f(x) = ln_a-l*x ;\
	fp(x) = (ln_a + da) - (l + dl) * x;\
	fm(x) = (ln_a - da) - (l - dl) * x;\
	plot "activity.data" using 1:2:3 with yerrorbars pt 7 lc rgb "black" title "Data", \
		f(x) with lines lw 2 lc rgb "red" title "QR fit", \
		fp(x) with lines lw 1 lc rgb "red" dt 2 notitle, \
		fm(x) with lines lw 1 lc rgb "red" dt 2 notitle \
	' | tee log.fig.gpi | gnuplot

activity.data : main
	./main 1>Out.txt 2>main.log

main : main.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

main.o : main.cc matrix.h QR.h
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean :
	$(RM) *.o *.txt *.log *.data *.gpi *.gp