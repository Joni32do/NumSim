# Donor Cell Scheme

## Used Formulas

$$ \begin{align*}
    [(uv)_x]_{i,j} = &(1 - \alpha) \\
                     & u(i - 1, j + \frac{1}{2})^+ v(i - 1, j) \\
                     & u(i - 1, j + \frac{1}{2})^- v(i, j) \\
                     & u(i,j+\frac{1}{2})^+ v(i, j) \\
                     & u(i,j+\frac{1}{2})^+ v(i + 1, j) \\
                     &+ \alpha \frac{u_{i, j + \frac{1}{2}} v_{i + \frac{1}{2}, j} - u_{i - 1, j + \frac{1}{2}} v_{i - \frac{1}{2}, j}}{dx}
\end{align*}$$

