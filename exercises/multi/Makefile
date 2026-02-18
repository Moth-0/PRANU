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
		a=1; b=10 ;\
		f(n) = a+b/n ;\
		fit f(n) "$<" using 1:2 via a,b ;\
		plot "$<" using 1:2 with linespoints title "Times" ;\
	' | tee log.times | gnuplot

N = 10000000000
out.times: main Makefile
	>$@
	>log.main
	for nthreads in 1 2 3 4 5 6 7 8 9 10 ;\
		do $(TIME) ./main -terms $(N) -threads $$nthreads >> log.main ;\
	done

clean :
	rm -rf *.txt *.o log.* out.times