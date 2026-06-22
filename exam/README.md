# Adaptive recursive integrator with subdivision into three subintervals 

## Project Overview
This project compares the 2-split adaptive integrator from homework, against the new 3-split adaptive integrator. We test these two versions against eachother using Clenshaw-Curtis and the infinite intevals transformations from the book. 
Then they are both tested on the error function. 

## `Out.txt`
The console output generates three tables:

* **Table 1: Singularities & Hardware Limits:** Integrates $\sqrt{x}$ and $1/\sqrt{x}$. This table proves that standard adaptive integrators struggle with infinite spikes at boundaries (like $x=0$). The closed-rule 2-Split method requires an incredibly inefficient 17,000+ evaluations to brute-force the area, while the open-rule 3-Split method suffers from infinite recursion and `NaN` poisoning.
* **Table 2: The Clenshaw-Curtis Fix:** Re-evaluates the same functions after routing them through a Clenshaw-Curtis transformation. The substitution flattens the infinite spike, allowing both integrators to solve the area. The 3-Split method is better, solving it in just 27 evaluations, where the 2-split uses 52. 
* **Table 3: Infinite Limits:** Integrates Gaussian functions ($x e^{-x^2}$ and $e^{-x^2}$) across $[0, \infty]$ and $[-\infty, \infty]$. It demonstrates the infinite-limit wrapper mapping infinite domains down to finite intervals (like $[0, 1]$), allowing standard algorithms to calculate the area. Again the 3-split is supperior. 

## Understanding the Plots
The final phase of the project calculates $\text{erf}(1)$ to a precision down to $10^{-15}$, and generates data for two log-log plots:

* **Convergence Plot (`erf_error.png`):** Plots the *Actual Error* against the *Requested Accuracy (acc)*. Because the data points closely track the $y=x$ target line, it visually proves that the integrators correctly respect the user's requested error bounds until hitting the physical floating-point limit. 
We see that the two methods are simillar in accuracy, but 2-split flattens out at $10^{-14}$ but 3-split keeps going down. 
* **Evaluations Plot (`erf_evals.png`):** Plots the *Number of Evaluations* against the *Requested Accuracy*. It visually shows the computational efficiency of the algorithms. As the tolerance tightens, the 3-Split line remains lower than the 2-Split line, proving it achieves the same maximum precision for significantly less work.