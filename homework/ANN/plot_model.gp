# The multiplot was made by Gemini
# Set output format (made slightly larger to fit 4 plots nicely)
set terminal pngcairo size 1000,800 enhanced font 'Arial,12'
set output 'modelfit.png'

# Enable Multiplot (2 rows, 2 columns)
set multiplot layout 2,2 title 'Neural Network Calculus' font ',16'
set grid

# ----------------------------------------------------
# Top-Left: The Main Fit
# ----------------------------------------------------
set title 'Network Approximation vs Actual'
set key bottom right
plot 'func.in' using 1:2 with points pt 7 ps 0.6 lc rgb 'blue' title 'Training Data', \
     'model.data' using 1:2 with lines lw 3 lc rgb 'black' title 'Trained Function'

# ----------------------------------------------------
# Top-Right: First Derivative
# ----------------------------------------------------
set title 'First Derivative'
set key top right
plot 'model.data' using 1:3 with lines lw 3 lc rgb 'red' title 'Derivative'

# ----------------------------------------------------
# Bottom-Left: Second Derivative
# ----------------------------------------------------
set title 'Second Derivative'
set key top right
plot 'model.data' using 1:4 with lines lw 3 lc rgb 'green' title 'Second Derivative'

# ----------------------------------------------------
# Bottom-Right: Antiderivative
# ----------------------------------------------------
set title 'Antiderivative (Integral)'
set key top left
plot 'model.data' using 1:5 with lines lw 3 lc rgb 'purple' title 'Antiderivative'

unset multiplot