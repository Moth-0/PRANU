# Variables
MAKEFLAGS += -r
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

all : run_partA run_partB

HEADERS = matrix.h newton.h QR.h ODE.h

run_partA : partA
	./partA

run_partB : wavefunction.png convergence.png
	
wavefunction.png : partB.data Makefile
	@echo '\
	set terminal pngcairo size 800,400 ;\
	set output "$@" ;\
	set title "Plotted wavefunction" ;\
	set xlabel "r" ;\
	set ylabel "f(r)" ;\
	set grid ;\
	stats "$<" every ::0::0 using 1 nooutput ;\
	E0_S = STATS_min ;\
	plot "$<" every ::1 using 1:2 with lines lw 2 lc rgb "black" title sprintf("Found E0=%g, f(r)", E0_S), \
	     x*exp(-x) with lines lw 2 dt 2 lc rgb "red" title "E0=1/2, f_0(r) = r exp(-r)" \
	' | tee log.wavefunction.gpi | gnuplot

partB.data : partB
	./partB -r_min 0.1 -r_max 8.0 -plt


# --- Convergence Studies --- Below made by AI

conv_rmin.data: partB Makefile
	@echo "Calculating r_min convergence..."
	@rm -f $@
	@for val in 0.8 0.7 0.6 0.5 0.4 0.3 0.2 0.1 0.05 0.01 0.005 0.001; do \
		E=`./partB -r_min $$val` ; \
		echo "$$val $$E" >> $@ ; \
	done

conv_rmax.data: partB
	@echo "Calculating r_max convergence..."
	@rm -f $@
	@for val in 1 2 4 6 8 10 12 15 20; do \
		E=`./partB -r_max $$val` ; \
		echo "$$val $$E" >> $@ ; \
	done

conv_acc.data: partB
	@echo "Calculating acc convergence..."
	@rm -f $@
	@for val in 0.1 0.05 0.01 0.005 0.001 0.0005 0.0001; do \
		E=`./partB -acc $$val` ; \
		echo "$$val $$E" >> $@ ; \
	done

conv_eps.data: partB
	@echo "Calculating eps convergence..."
	@rm -f $@
	@for val in 0.1 0.05 0.01 0.005 0.001 0.0005 0.0001; do \
		E=`./partB -eps $$val` ; \
		echo "$$val $$E" >> $@ ; \
	done

# A master target to generate all four data files at once
convergence_data: conv_rmin.data conv_rmax.data conv_acc.data conv_eps.data

convergence.png: convergence_data Makefile
	@echo "Plotting convergence..."
	@echo '\
	set terminal pngcairo size 1000,800 ;\
	set output "$@" ;\
	set multiplot layout 2,2 title "Convergence Study (Exact E_0 = -0.5)" ;\
	set grid ;\
	\
	set xlabel "r_{min}" ;\
	set ylabel "Calculated E_0" ;\
	plot "conv_rmin.data" using 1:2 with linespoints pt 7 lc rgb "blue" title "Numerical", -0.5 dt 2 lc rgb "black" title "Exact" ;\
	\
	set xlabel "r_{max}" ;\
	plot "conv_rmax.data" using 1:2 with linespoints pt 7 lc rgb "red" title "Numerical", -0.5 dt 2 lc rgb "black" title "Exact" ;\
	\
	set logscale x ;\
	set xrange [*:*] reverse ;\
	set xlabel "acc" ;\
	plot "conv_acc.data" using 1:2 with linespoints pt 7 lc rgb "green" title "Numerical", -0.5 dt 2 lc rgb "black" title "Exact" ;\
	\
	set xlabel "eps" ;\
	plot "conv_eps.data" using 1:2 with linespoints pt 7 lc rgb "purple" title "Numerical", -0.5 dt 2 lc rgb "black" title "Exact" ;\
	\
	unset multiplot \
	' | gnuplot


% : %.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

%.o : %.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean :
	$(RM) *.o *.txt *.log *.gpi *.data *.png partA partB