# Default Makefile
# Variables
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

TIME = time --append --output $@ --format "$$nthreads %e %U"

times.png: out.times Makefile
	echo '\
		set terminal png ;\
		set output "$@" ;\
		set xlabel "number of threads" ;\
		set ylabel "real time" ;\
		set title "Computation times for 1e10 terms" ;\
		a=1; b=10 ;\
		f(x) = a+b/x ;\
		fit f(x) "$<" using 1:2 via a,b ;\
		plot \
			"$<" using 1:2 with linespoints title "times" \
			,f(x) title "fit with 1/x" ;\
	' | tee log.times | gnuplot

N = 10000000000
out.times: main 
	>$@
	>log.main
	for nthreads in 1 2 4 8 16 32;\
		do $(TIME) ./main -terms $(N) -threads $$nthreads >> log.main ;\
	done

clean :
	rm -rf *.txt *.o log.* out.times