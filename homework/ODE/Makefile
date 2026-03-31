# Variables
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

all : plot_harm.png plot_damp.png plot_lv.png plot_grav.png

plot_grav.png : files
	echo '\
	set terminal pngcairo ;\
	set output "$@" ;\
	set size square ;\
	set title "Gravity" ;\
	set xlabel "x" ;\
	set ylabel "y" ;\
	set tics out ;\
	plot "data/circ.data" using (1/$$2)*cos($$1):(1/$$2)*sin($$1) with lines lc rgb "black" title "circular", \
	 	 "data/elip.data" using (1/$$2)*cos($$1):(1/$$2)*sin($$1) with lines lc rgb "red" dt 2 title "ellipsical", \
		 "data/rel.data" using (1/$$2)*cos($$1):(1/$$2)*sin($$1) with lines lc rgb "blue" dt 2 title "relativistic", \
		 "< echo 0 0" with points pt 7 lc rgb "gold" title "Star" \
	' | tee log.fig4.gpi | gnuplot

plot_harm.png: files
	echo '\
	set terminal pngcairo ;\
	set output "$@" ;\
	set title "Harmonic" ;\
	set xlabel "Time" ;\
	set ylabel "y" ;\
	set tics out ;\
	plot "data/harmonic.data" using 1:2 skip 1 with lines lc rgb "black" title "y1", \
	 	 "data/harmonic.data" using 1:3 skip 1 with lines lc rgb "red" title "y2" \
	' | tee log.fig1.gpi | gnuplot

plot_damp.png : files
	echo '\
	set terminal pngcairo ;\
	set output "$@" ;\
	set title "Damped Oscilations" ;\
	set xlabel "Time" ;\
	set ylabel "y" ;\
	set tics out ;\
	plot "data/damped.data" using 1:2 skip 1 with lines lc rgb "black" title "y1", \
		 "data/damped.data" using 1:3 skip 1 with lines lc rgb "red" title "y2" \
	' | tee log.fig2.gpi | gnuplot

plot_lv.png : files 
	echo '\
	set terminal pngcairo ;\
	set output "$@" ;\
	set title "Lotka-Volterra" ;\
	set xlabel "Time" ;\
	set ylabel "y" ;\
	set tics out ;\
	plot "data/lv.data" using 1:2 skip 1 with lines lc rgb "black" title "y1", \
		 "data/lv.data" using 1:3 skip 1 with lines lc rgb "red" title "y2" \
	' | tee log.fig3.gpi | gnuplot


files : main
	./main

main : main.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

main.o : main.cc ODE.h 
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean :
	$(RM) *.o *.txt *.log *.gpi *.data