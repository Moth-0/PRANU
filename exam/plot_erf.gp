set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'erf_error.png'

set title "Convergence of Adaptive Integrators for erf(1)" font ",14"
set xlabel "Requested Accuracy (acc)" font ",12"
set ylabel "Actual Error (|Exact - Calculated|)" font ",12"

set logscale xy

set format x "10^{%L}"
set format y "10^{%L}"

set xrange [1e-0:1e-16]
# Give the Y-axis a little breathing room at the bottom
set yrange [1e-17:1e-0]

set grid xtics ytics mxtics mytics linetype 1 linecolor rgb "gray90", \
                                   linetype 1 linecolor rgb "gray95"

plot "erf_convergence.txt" using 1:2 skip 1 with linespoints \
         linewidth 2 pointtype 5 pointsize 1.2 linecolor rgb "red" \
         title "Actual Error (adaptive2)", \
     "erf_convergence.txt" using 1:4 skip 1 with linespoints \
         linewidth 2 pointtype 7 pointsize 1.2 linecolor rgb "blue" \
         title "Actual Error (adaptive3)", \
     x with lines \
         dashtype 2 linewidth 2 linecolor rgb "gray30" \
         title "Target Error Threshold (y=x)"