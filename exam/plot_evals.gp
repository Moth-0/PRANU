set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'erf_eval.png'

set title "Computational Cost: Evaluations vs Requested Accuracy" font ",14"
set xlabel "Requested Accuracy (acc)" font ",12"
set ylabel "Number of Function Evaluations" font ",12"

set logscale xy
set format x "10^{%L}"
set format y "10^{%L}" 

set xrange [1e-0:1e-16]
set yrange [1e0:1e7]

set grid xtics ytics mxtics mytics linetype 1 linecolor rgb "gray90", \
                                   linetype 1 linecolor rgb "gray95"

# Plot Column 3 (adp2 evals) and Column 5 (adp3 evals)
plot "erf_convergence.txt" using 1:3 skip 1 with linespoints \
         linewidth 2 pointtype 5 pointsize 1.2 linecolor rgb "red" \
         title "Evaluations (adaptive2)", \
     "erf_convergence.txt" using 1:5 skip 1 with linespoints \
         linewidth 2 pointtype 7 pointsize 1.2 linecolor rgb "blue" \
         title "Evaluations (adaptive3)"