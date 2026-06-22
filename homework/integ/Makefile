# Variables
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

all : err_x.png err_acc.png

err_x.png : files 
	echo '\
	set terminal pngcairo ;\
	set output "$@" ;\
	set title "Error Function" ;\
	set xlabel "x" ;\
	set ylabel "err(x)" ;\
	set grid ;\
	plot "err1.data" using 1:2 with lines lw 2 lc rgb "black" title "err(x)" \
	' | tee log.fig1.gpi | gnuplot

err_acc.png : files 
	echo '\
	set terminal pngcairo ;\
	set output "$@" ;\
	set title "Error Function" ;\
	set xlabel "ln(acc)" ;\
	set ylabel "ln(err(1))" ;\
	set logscale xy ;\
	set format xy "10^{%T}" ;\
	set grid ;\
	plot "err2.data" using 1:2 with lines lw 2 lc rgb "black" title "err(1)" \
	' | tee log.fig2.gpi | gnuplot

files : main
	./main > Out.txt

main : main.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

main.o : main.cc integrate.h
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean :
	$(RM) main *.o *.txt *.log *.gpi *.data *.png