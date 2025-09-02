#include "highs/Highs.h"
#include <iostream>
#include <vector>

int main() {
    Highs highs;

    std::vector<double> obj = {2.0, 3.0};
    std::vector<double> lb = {0.0, 1.0};
    std::vector<double> ub = {kHighsInf, kHighsInf};

    highs.addCols(2,obj.data(),  lb.data(), ub.data(), 0,nullptr, nullptr, nullptr);

    //1.0∗x+1.0∗y≥1.0
    std::vector<int> start = {0};
    std::vector<int> index = {0, 1}; // x and y
    std::vector<double> value = {1.0, 1.0};
    std::vector<double> row_lb = {2.5}; // lowerbound
    std::vector<double> row_ub = {kHighsInf}; //upperbound

    highs.addRows(1, row_lb.data(), row_ub.data(), 2,start.data(), index.data(), value.data());

    highs.run();

    auto sol = highs.getSolution();
    std::cout << "x = " << sol.col_value[0] << "\n";
    std::cout << "y = " << sol.col_value[1] << "\n";
    return 0;
}
