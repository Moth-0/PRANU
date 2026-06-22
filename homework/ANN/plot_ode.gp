# Set output format
set terminal pngcairo size 800,800 enhanced font 'Arial,12'
set output 'ODE.png'

# Create a 2x1 grid for a professional presentation
set multiplot layout 2,1 title 'Physics-Informed Neural Network: y'''' + y = 0'

# ----------------------------------------------------
# Top Plot: The Fit
# ----------------------------------------------------
set title 'Network Prediction vs. Exact Analytical Solution'
set xlabel 'x'
set ylabel 'y'
set grid
set key bottom left box

# Plot the exact cosine function as a solid black line, 
# and your network's predictions as red dots on top of it.
plot cos(x) with lines lw 3 lc rgb 'black' title 'Exact Solution: cos(x)', \
     'ode_results.data' using 1:2 with points pt 7 ps 0.8 lc rgb 'red' title 'PINN Prediction'

# ----------------------------------------------------
# Bottom Plot: The Error
# ----------------------------------------------------
set title 'Absolute Approximation Error'
set xlabel 'x'
set ylabel '| Prediction - Exact |'
set grid
set key top right box

# Gnuplot can calculate math on the fly! 
# We tell it to take Column 2 (your prediction), subtract cos(Column 1), and take the absolute value.
plot 'ode_results.data' using 1:(abs($2 - cos($1))) with lines lw 2 lc rgb 'blue' title 'Absolute Error'

unset multiplot