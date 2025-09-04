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
    double total_supply = std::accumulate(supply.begin(), supply.end(), 0.0);
    double total_demand = std::accumulate(demand.begin(), demand.end(), 0.0);
    if (total_demand > total_supply) {
        std::cerr << "Error: Total demand exceeds total supply. Problem infeasible.\n";
        return;
    }
    // Flatten cost matrix into objective vector
    std::vector<double> obj;

    // Objective function
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
    std::vector<int> start;  // Tells where each row's data starts in the index[] and value[] arrays
    std::vector<int> index; //Column indices (i.e., variable indices) of non-zero values
    std::vector<double> value; // Coefficient values for the constraints (same order as index[])

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
    start.push_back(entry_counter);
    highs.addRows(row_lb.size(), row_lb.data(), row_ub.data(),
                  index.size(), start.data(), index.data(), value.data());

    highs.run();

    auto sol = highs.getSolution();
    if (highs.getModelStatus() != HighsModelStatus::kOptimal) {
        std::cerr << "Error: Problem not solved to optimality. Status: "
                  << highs.modelStatusToString(highs.getModelStatus()) << "\n";
        return;
    }

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

    double total_cost = 0.0;
    for (int i = 0; i < num_sources; ++i)
    {
        for (int j = 0; j < num_consumers; ++j)
        {
            int var_idx = i * num_consumers + j;
            total_cost += sol.col_value[var_idx] * cost[i][j];
        }
    }
    std::cout << "\nVerified Total Cost = " << total_cost << "\n";
    std::cout << "Objective from HiGHS = " << highs.getInfo().objective_function_value << "\n";

    std::cout << "\nStart: ";
    for (auto s : start)
        std::cout << s << " ";
    std::cout << "\nindex: ";
    for (auto i : index)
        std::cout << i << " ";
    std::cout << "\nvalue: ";
    for (auto v : value)
        std::cout << v << " ";
}
int main() {
    // Example with 2 sources and 3 consumers

    // distance source to consumer
    std::vector<std::vector<double>> cost = {
        {1, 2, 3},
        {4, 5, 6}
    };

    // source heat supply 
    std::vector<double> supply = {5, 8};
    
    // consumer heating demand 
    std::vector<double> demand = {3,4, 6};


    solveHeatDistribution(cost, supply, demand);

    return 0;
}
