## Comparitive Study

| Solver              | Licensing     | Strengths                                        | Limitations                      | Best For                                       |
| ------------------- | ------------- | ------------------------------------------------ | -------------------------------- | ---------------------------------------------- |
| **HiGHS**           | MIT (open)    | Fast LP/MIP, scalable, active development        | Newer ecosystem                  | Large-scale LP with open-source dependency     |
| **CLP / CBC**       | Open-source   | Stable, well-integrated, supports LP & MIP       | Outpaced by HiGHS                | Prototyping, moderate LP/MIP tasks             |
| **GLPK**            | GPL           | Established, simple to use                       | Performance lags significantly   | Educational or simpler LP use cases            |
| **OR‑Tools (GLOP)** | Apache 2.0    | Unified framework, flows, CP‑SAT, cross‑platform | GLOP not top-tier in LP speed    | Flow/network problems and mixed-model setups   |
| **Commercial**      | Paid/licensed | Top-tier performance and features                | License cost (free for academia) | Production, large-scale, commercial deployment |


## HiGHS

License: MIT

Features:

High-performance LP and MIP solver.

Actively developed.

C++ native API and C API.

Usage: You define LP problems programmatically using matrices or coefficient lists.

## Why HiGHS?
It is modern, well-documented, and optimized for performance — a great replacement for PuLP.
HiGHS is an open-source high-performance optimization solver, written in C++, designed for solving:

Linear Programming (LP)

Mixed-Integer Programming (MIP)

Quadratic Programming (QP) (limited support)

## Our Use case
We just need to model it as a Linear Program (LP) or Mixed Integer Program (MIP).

Precompute shortest distances using a graph library.

Use those distances as cost coefficients in our LP.

## Installation
```bash

git clone https://github.com/ERGO-Code/HiGHS.git

cd HiGHS
cmake -B build -S . && cmake --build build
sudo cmake --install build
```


## HiGHS inbuilt functions

### addCols
- **Purpose**: Adds variables (columns) to the linear program.

- When we call addCols, we are telling the solver:

    - How many new variables to add (num_new_col).

    - Their lower bounds (minimum allowed value for each variable).

    - Their upper bounds (maximum allowed value).

    - Their objective coefficients (the weight or cost for each variable in the objective function).

    - Optionally, the structure of these new variables in constraints (columns in the matrix) — but if we don’t add constraint data here, we just define variables without constraints at this point.

In our code:
```cpp
highs.addCols(2, obj.data(), lb.data(), ub.data(), 0, nullptr, nullptr, nullptr);

```

- Adds 2 variables (x and y)
- lb: lower bounds of variables (0, 0)
- ub: upper bounds (∞, ∞)
- obj: objective coefficients (2.0, 3.0) 2x+3y => minimize
- 0, nullptr, nullptr, nullptr: no constraints added with these variables here (you add constraints separately)

### addRows
- **Purpose**: Adds constraints (rows) to the linear program.
- When you call addRows, you specify:
    - Number of new constraints.
    - Lower and upper bounds for each constraint (defining ≤, ≥, or = relationships).
    - The matrix data describing how variables participate in each constraint:
        - Which columns (variables) appear in the constraint,
        - Their coefficients in the constraint equation,
        - Where each row starts in the arrays (for sparse storage).

```cpp

highs.addRows(1, row_lb.data(), row_ub.data(), 2, start.data(), index.data(), value.data());
```
- Adds 1 constraint.
- Constraint bounds: ≥1 (since row_lb = 1.0, row_ub = ∞)
- 2 = number of non-zero elements in this row (number of variables involved in the constraint)
- start defines where this row’s data starts in index and value
- index = variable indices involved (x=0, y=1)
- value = coefficients (1.0 for both x & y )

1≤1.0⋅x+1.0⋅y≤+∞

## TL;DR
| Function  | Adds                      | Parameters summary                                 | Example role                 |
| --------- | ------------------------- | -------------------------------------------------- | ---------------------------- |
| `addCols` | Variables (decision vars) | Number, bounds, objective coefficients, etc.       | Define $x,y$, objective cost |
| `addRows` | Constraints (linear eqs)  | Number, bounds, matrix data describing constraints | Define $x + y \geq 1$        |
