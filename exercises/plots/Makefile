# Default Makefile
# Variables
CXXFLAGS = -Wall -Werror -O -std=c++23
CXX = c++
LDLIBS = -lstdc++ -lm
RM = rm -f

all: erf.png sgamma.png lngamma.png

erf.png: erf.data Makefile
	echo '\
	set terminal png ;\
	set output "$@" ;\
	set xlabel "x" ;\
	set ylabel "erf(x)" ;\
	set tics out ;\
	plot [0:5] \
		"erf.data" using 1:2 with points pointtype 4 title "custom erf(x)"\
		,erf(x) title "Builtin erf(x)";\
	' | tee log.fig1.gpi | gnuplot

sgamma.png: sgamma.data Makefile
	echo '\
	set terminal png ;\
	set output "$@" ;\
	set xlabel "x" ;\
	set ylabel "sgamma(x)" ;\
	set tics out ;\
	plot [0:5] \
		"sgamma.data" using 1:2 with points pointtype 4 title "custom sgamma(x)"\
		,gamma(x) title "Builtin gamma(x)";\
	' | tee log.fig2.gpi | gnuplot

lngamma.png: lngamma.data Makefile
	echo '\
	set terminal png ;\
	set output "$@" ;\
	set xlabel "x" ;\
	set ylabel "sgamma(x)" ;\
	set tics out ;\
	plot [0:5] \
		"lngamma.data" using 1:2 with points pointtype 4 title "custom lngamma(x)"\
		,lgamma(x) title "Builtin lgamma(x)";\
	' | tee log.fig3.gpi | gnuplot

erf.data: main
	./$< -func erf -xmin 0 -xmax 5 -dx 0.255 >> $@ 2>>log.erf

sgamma.data: main
	./$< -func sgamma -xmin 1 -xmax 5 -dx 0.255 >> $@ 2>>log.sgamma

lngamma.data: main
	./$< -func lngamma -xmin 1 -xmax 5 -dx 0.255 >> $@ 2>>log.lngamma

clean :
	$(RM) *.data *.png log.*