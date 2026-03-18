CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm

all: main rmax.png dr.png energy.png times.png

main: main.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

plots: plots.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

time: time.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<


dr.data: plots Makefile
	@echo "# rmax dr Energy" > $@
	for dr in 0.5 0.4 0.3 0.2 0.1 0.05 0.02; do \
		./$< -rmax 10 -dr $$dr >> $@; \
	done

rmax.data: plots Makefile
	@echo "# rmax dr Energy" > $@
	for rmax in 2 3 4 5 6 7 8 9 10; do \
		./$< -rmax $$rmax -dr 0.1 >> $@; \
	done

energy.data: main Makefile
	./$<

dr.png: dr.data
	gnuplot -e "set term png; set output '$@'; \
		set title 'Energy Convergence vs dr (rmax=10)'; \
		set xlabel 'Step size (dr)'; set ylabel 'Energy'; \
		set xrange [*:*] reverse; \
		plot '$<' using 2:3 with linespoints title 'E0'"

rmax.png: rmax.data
	gnuplot -e "set term png; set output '$@'; \
		set title 'Energy Convergence vs rmax (dr=0.1)'; \
		set xlabel 'rmax'; set ylabel 'Energy'; \
		plot '$<' using 1:3 with linespoints title 'E0'"

energy.png: energy.data 
	gnuplot -e "set term png; set output '$@'; \
		set title 'Reduced Radial Wavefunctions'; \
		set xlabel 'r (Bohr)'; set ylabel 'f(r)'; \
		f1s(x) = 2*x*exp(-x); \
		f2s(x) = (1.0/sqrt(2.0))*x*(1.0 - 0.5*x)*exp(-0.5*x); \
		plot '$<' using 1:2 with points title 'Numerical 1s', \
		     '$<' using 1:3 with points title 'Numerical 2s', \
		     f1s(x) with lines lc 'black' title 'Analytical 1s', \
		     f2s(x) with lines lc 'red' title 'Analytical 2s'"

times.png: times.data
	gnuplot -e "set term png; set output '$@'; \
		set title 'Execution Time vs Matrix Size'; \
		set xlabel 'Matrix Size (n)'; set ylabel 'Time (seconds)'; \
		a=1 ; \
		f(x) = a*x*x*x ;\
		fit f(x) '$<' using 1:2 via a ;\
		plot '$<' using 1:2 with linespoints title 'Measured Time', \
			  f(x) with lines lc 'black' title 'n^3'"

times.data: times Makefile
	@echo "Time to Diag" > $@
	for n in 3 10 20 50 100 150 200 300; do \
		./$< -n $$n >> $@; \
	done

clean:
	rm -f main *.data *.png *.txt *.o