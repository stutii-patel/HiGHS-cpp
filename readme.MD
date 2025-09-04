# Heat Distribution Optimization with Strategic Source Allocation

##  Problem Setup

- You have multiple **sources** (e.g., power plants, heaters) with limited supply.
- You have multiple **consumers** (e.g., buildings, zones) with specific heat demands.
- Heat must be allocated from sources to consumers to minimize total transport cost.

###  Standard Linear Program Formulation

Let:
- \( x_{ij} \): heat allocated from source \(i\) to consumer \(j\)
- \( d_{ij} \): distance or base cost between source \(i\) and consumer \(j\)

Objective:

Minimize: 

$$
∑∑ x_{ij} * d_{ij}
$$

Subject to:
- Supply constraints: 

$$ 
    ∑ x_{ij} ≤ supply_i
$$
- Demand constraints: 

$$ 
    ∑ x_{ij} = demand_j 
$$
- Non-negativity: 

$$ 
    x_{ij} ≥ 0 
$$

# Penalty

## Strategic Optimization Goal

In real systems, it may be **more efficient** in the long run to:

> Avoid assigning nearby sources to nearby consumers **too early**, so those sources remain available to consumers who cannot be served by distant sources.

### Example Scenario:

- Source S1 is very close to Consumer C1.
- But it's more efficient to let S3 supply C1 so that **S1 remains available** for distant consumers who can't use S3.


## Solution: Adjusted Cost Function

### Idea

Modify the cost matrix to **penalize "greedy" short assignments** and **prioritize global efficiency**.

### Adjusted Cost Formula

For each source \( i \) and consumer \( j \):

$$
    adjustedCost_{ij} = baseCost_{ij} * (1 + α * proximityScore_{ij})
$$

Where:
- `base_cost_ij` = original distance or cost
- `α` = penalty weight (e.g., 0.5 to 2.0)
- `proximityScore_ij` = closeness of consumer `j` to source `i` (from 0 to 1)

Example proximity score:

$$
proximityScore_{ij} = 1.0 - (baseCost_{ij} / maxCostFromSource_i)
$$

This penalizes the use of close consumers early unless strategically efficient.