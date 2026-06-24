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


## The Weights

#### The Higher-Order Rule (3/8, 2/8, 3/8)
These weights are calculated using the **Method of Undetermined Coefficients**. We mathematically force the approximation to be 100% exact for polynomials up to degree 2 (functions 1, x, and x^2) over the interval [0, 1]:

1. **Degree 0 (f(x) = 1):** Exact integral is 1.
   w1 + w2 + w3 = 1
2. **Degree 1 (f(x) = x):** Exact integral is 1/2.
   w1(1/6) + w2(1/2) + w3(5/6) = 1/2
3. **Degree 2 (f(x) = x^2):** Exact integral is 1/3.
   w1(1/6)^2 + w2(1/2)^2 + w3(5/6)^2 = 1/3

Solving this exact 3x3 linear system yields the only valid solution: **w1 = 3/8**, **w2 = 2/8**, and **w3 = 3/8**. 

#### The Lower-Order Rule (1/3, 1/3, 1/3)
These weights come geometrically from the **Composite Midpoint Rule** (Rectangle Rule). 

If you slice the interval [0, 1] into three equal chunks, every chunk has a width of exactly 1/3. To approximate the area of each chunk, you build a rectangle by sampling the function's height at the exact middle of each chunk (at 1/6, 1/2, and 5/6). 

Adding the area of those three rectangles (width * height) gives:
Total Area ≈ (1/3)f(1/6) + (1/3)f(1/2) + (1/3)f(5/6)
## Overall
The project has been implimented and all points have been considered. The 3-split model has been compared to the 2-split model from the homework, and the data has been reprecented. 
The error funnction has also been plotted, and compared to the 2-split model. 
* Score: 10/10
