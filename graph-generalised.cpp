#include "highs/Highs.h"
#include <iostream>
#include <vector>

void solveHeatDistribution(const std::vector<std::vector<double>>& cost,
                           const std::vector<double>& supply,
                           const std::vector<double>& demand) {
    Highs highs;

    int num_sources = supply.size();
    int num_consumers = demand.size();
    int num_vars = num_sources * num_consumers;

    // Flatten cost matrix into objective vector
    std::vector<double> obj;
    for (int i = 0; i < num_sources; ++i) {
        for (int j = 0; j < num_consumers; ++j) {
            obj.push_back(cost[i][j]);
        }
    }

    // Lower and upper bounds for variables (x_ij >= 0, no upper bound)
    std::vector<double> lb(num_vars, 0.0);
    std::vector<double> ub(num_vars, kHighsInf);

    highs.addCols(num_vars, obj.data(), lb.data(), ub.data(), 0, nullptr, nullptr, nullptr);

    // Constraints:
    std::vector<double> row_lb;
    std::vector<double> row_ub;
    std::vector<int> start;
    std::vector<int> index;
    std::vector<double> value;

    int entry_counter = 0;

    // Consumer demand constraints: sum_i x_ij == demand[j]
    for (int j = 0; j < num_consumers; ++j) {
        start.push_back(entry_counter);
        for (int i = 0; i < num_sources; ++i) {
            int var_idx = i * num_consumers + j; // x_ij
            index.push_back(var_idx);
            value.push_back(1.0);
            entry_counter++;
        }
        row_lb.push_back(demand[j]);
        row_ub.push_back(demand[j]);
    }

    // Source capacity constraints: sum_j x_ij <= supply[i]
    for (int i = 0; i < num_sources; ++i) {
        start.push_back(entry_counter);
        for (int j = 0; j < num_consumers; ++j) {
            int var_idx = i * num_consumers + j;
            index.push_back(var_idx);
            value.push_back(1.0);
            entry_counter++;
        }
        row_lb.push_back(0.0);
        row_ub.push_back(supply[i]);
    }

    highs.addRows(row_lb.size(), row_lb.data(), row_ub.data(),
                  index.size(), start.data(), index.data(), value.data());

    highs.run();

    auto sol = highs.getSolution();

    std::cout << "\nOptimal Heat Allocation Matrix (units):\n";
    for (int i = 0; i < num_sources; ++i) {
        for (int j = 0; j < num_consumers; ++j) {
            int var_idx = i * num_consumers + j;
            std::cout << "x[" << i << "][" << j << "] = " << sol.col_value[var_idx] << "\t";
        }
        std::cout << "\n";
    }

    std::cout << "\nFraction of each source's capacity used:\n";
    for (int i = 0; i < num_sources; ++i) {
        double total_supplied = 0.0;
        for (int j = 0; j < num_consumers; ++j) {
            int var_idx = i * num_consumers + j;
            total_supplied += sol.col_value[var_idx];
        }
        std::cout << "Source " << i << ": " << total_supplied << " / " << supply[i]
                  << " (" << (total_supplied / supply[i]) * 100.0 << "%)\n";
    }
   

    std::cout << "\nFraction of each consumer's demand met by each source:\n";
    for (int j = 0; j < num_consumers; ++j) {
        std::cout << "Consumer " << j << " (demand = " << demand[j] << "):\n";
        for (int i = 0; i < num_sources; ++i) {
            int var_idx = i * num_consumers + j;
            double heat_from_i = sol.col_value[var_idx];
            double frac = (demand[j] > 0) ? (heat_from_i / demand[j]) : 0.0;
            std::cout << "  Source " << i << ": " << heat_from_i << " units (" 
                      << frac * 100.0 << "%)\n";
        }
    }

}
int main() {
    // Example with 3 sources and 2 consumers

    // distance source to consumer
    std::vector<std::vector<double>> cost = {
        {8, 2},
        {12, 2},
        {5, 1}
    };

    // source heat supply 
    std::vector<double> supply = {2, 10, 3};
    
    // consumer heating demand 
    std::vector<double> demand = {5, 3};


    solveHeatDistribution(cost, supply, demand);

    return 0;
}
