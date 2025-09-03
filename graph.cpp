/**
 * 
 * s1 s2 s3
c1 [ 1 2 3]
c2 [1 2 1]
c3 [0 1 0]
c4 [2 0 3]
...
c10 [10 3 5]
 
    
 */

#include "highs/Highs.h"
#include <iostream>
#include <vector>
/*
	c1	c2
s1	x_11	x_12
s2	x_21	x_22
s3	x_31	x_32
x_ij
Actual amount of heat sent from source i to consumer j (in units)

distance

      c1  c2
s1   1   2
s2   2   2
s3   3   1

c1=5
c2=3


🔹 Consumer demand constraints:

Each consumer must receive exact heat required:

c1: x_11 + x_21 + x_31 = 5

c2: x_12 + x_22 + x_32 = 3

🔹 Source supply constraints:

Each source must not exceed capacity:

s1: x_11 + x_12 <= 10

s2: x_21 + x_22 <= 10

s3: x_31 + x_32 <= 10
*/
int main() {
    Highs highs;

    // Variables: x_11, x_12, x_21, x_22, x_31, x_32
    std::vector<double> obj = {1, 2, 2, 2, 3, 1}; // Cost = distance
    std::vector<double> lb(6, 0.0);
    std::vector<double> ub(6, kHighsInf);

    highs.addCols(6, obj.data(), lb.data(), ub.data(), 0, nullptr, nullptr, nullptr);

    // Constraints:

    // Row 0: x_11 + x_21 + x_31 = 5 (consumer c1)
    // Row 1: x_12 + x_22 + x_32 = 3 (consumer c2)
    // Row 2: x_11 + x_12 <= 10 (supply from s1)
    // Row 3: x_21 + x_22 <= 10 (supply from s2)
    // Row 4: x_31 + x_32 <= 10 (supply from s3)

    std::vector<double> row_lb = {5, 3, 0, 0, 0};
    std::vector<double> row_ub = {5, 3, 10, 10, 10};

    // Sparse matrix: 5 rows
    std::vector<int> start = {0, 3, 6, 8, 10}; // starting index of each row in index/value
    std::vector<int> index = {
        0, 2, 4,      // row 0: x_11 + x_21 + x_31
        1, 3, 5,      // row 1: x_12 + x_22 + x_32
        0, 1,         // row 2: x_11 + x_12
        2, 3,         // row 3: x_21 + x_22
        4, 5          // row 4: x_31 + x_32
    };
    std::vector<double> value(index.size(), 1.0); // all coefficients are 1.0

    highs.addRows(5, row_lb.data(), row_ub.data(), index.size(), start.data(), index.data(), value.data());

    highs.run();

    auto sol = highs.getSolution();

    std::vector<std::string> var_names = {"x_11", "x_12", "x_21", "x_22", "x_31", "x_32"};

    std::cout << "Optimal heat allocation:\n";
    for (int i = 0; i < 6; ++i) {
        std::cout << var_names[i] << " = " << sol.col_value[i] << "\n";
    }

    return 0;
}
