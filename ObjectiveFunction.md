
## Objective Function

The total cost to minimize is a weighted combination of several components:

### 1. Distance-based Transport Cost

Encourages connecting buildings to nearby sources:

cost[i][j] = distance[i][j] × demand[j]


###  2. Tie-Breaking Penalty (Smoothness)

Gently discourages multiple sources with identical distances using a small epsilon-weighted quadratic term:

\[
\text{penalty}_{ij}^{\text{epsilon}} = \epsilon \cdot \left(\frac{\text{distance}_{ij}}{\text{max\_distance}}\right)^2
\]

### 3. Underutilization/Overutilization Penalty (Source)

Penalizes **not fully utilizing** the capacity of a source. For each variable \( x_{ij} \), we add:

\[
\text{penalty}_{ij}^{\text{underutilization}} = \frac{\lambda}{\text{supply}_i}
\]

This reflects the cost of **leaving unused capacity** in the source. The full underutilization penalty per source is:

\[
\text{Penalty}_i = \frac{\text{supply}_i - \sum_j x_{ij}}{\text{supply}_i}
\]

This expression is **linear** and equivalent to:

\[
\sum_{i,j} x_{ij} \cdot \left( \frac{\lambda}{\text{supply}_i} \right)
\]

---

### 🧮 Full Objective Expression

\[
\text{Minimize} \quad \sum_{i,j} x_{ij} \cdot \left[
	\text{distance}_{ij} \cdot \text{demand}_j
	+ \epsilon \cdot \left(\frac{\text{distance}_{ij}}{\text{max\_distance}}\right)^2
	+ \frac{\lambda}{\text{supply}_i}
\right]
\]

Where:
- \( x_{ij} \) is the decision variable: heat from source \( i \) to building \( j \)
- \( \epsilon \) is a small tie-breaking weight (e.g. 10)
- \( \lambda \) is the underutilization penalty weight (e.g. 10–100)

---

## 🧩 Output

After solving, the program outputs:
- The **heat allocation matrix** \( x_{ij} \)
- **Fraction of each source’s capacity** that was used
- **Percentage of each building’s demand** met by each source
- A **mapping of source-to-building paths** for later use or visualization

---

## 🛠️ Parameters to Tune

| Parameter | Meaning | Suggested Range |
|----------|---------|-----------------|
| `epsilon` | Tie-breaking penalty on distance | 1–50 |
| `lambda` | Penalty for underutilized sources | 10–500 |
| `max_distance` | Maximum distance used for normalization | Computed from input |
