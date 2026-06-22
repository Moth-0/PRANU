# Variables
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

all : Out.txt times.png

Out.txt : main Makefile
	./main > Out.txt 
	cat Out.txt

times.data : time
	>$@
	for N in $$(seq 100 100 1000); do \
		time --format "$$N %e" --output $@ --append \
		./$< -size $$N 1>out.txt 2>err.txt ;\
	done 
	$(RM) out.txt err.txt

times.png: times.data Makefile
	echo '\
	set terminal png ;\
	set output "$@" ;\
	set title "Runtime of QR-factorization" ;\
	set xlabel "N" ;\
	set ylabel "time (s)" ;\
	set tics out ;\
	f(x) = a*x**3 ;\
	fit f(x) "times.data" using 1:2 via a ;\
	plot [0:1000] \
		"times.data" using 1:2 with points title "times.data",\
		f(x) with lines title "a*N**3";\
	' | tee log.fig3.gpi | gnuplot

main : main.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

main.o : main.cc matrix.h QR.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

time : time.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

time.o : time.cc matrix.h QR.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean :
	$(RM) main time *.o *.txt *.png *.data *.log *.gpi