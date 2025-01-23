# IPPD - Introdução à Programação Paralela e Distribuída
### (Introduction to Parallel and Distributed Programming)

---

This repository contains all the class exercises for Introduction to Parallel and Distributed Programming.

### Dijkstra 
Final Project of the Course.
Original project repository: https://github.com/DanielHReq/DijkstraParalelizado

#### Execution Times

| Type          | Vertices * Edges   | Time (ms)      |
|---------------|--------------------|----------------|
| **SEQUENTIAL**                                      |
|               | 10 * 40            | 0.009          |
|               | 100 * 400          | 5.044          |
|               | 1000 * 4000        | 4669.937       |
| **PARALLELIZED**                                    |
| 4 THREADS     | 10 * 40            | 0.177          |
|               | 100 * 400          | 1.844          |
|               | 1000 * 4000        | 1285.598       |
| 8 THREADS     | 10 * 40            | 0.394          |
|               | 100 * 400          | 1.615          |
|               | 1000 * 4000        | 672.938        |
| 16 THREADS    | 10 * 40            | 26.046         |
|               | 100 * 400          | 2.451          |
|               | 1000 * 4000        | 413.901        |

---

#### Analysis

- **Where Sequential is Better**
  - **Small Graphs (10 * 40):**
     - The sequential approach is significantly faster for small graphs, taking only **0.009 ms**, outperforming all parallelized versions, including the one with **4 threads (0.177 ms)**.
     - The overhead of creating threads and synchronizing tasks in parallel mode makes the cost higher than simply processing the graph sequentially.

- **Where Parallelization is Better**
  - **Medium Graphs (100 * 400):**
     - Parallelization with **8 threads (1.615 ms)** and **4 threads (1.844 ms)** achieves faster execution times compared to sequential processing (**5.044 ms**). At this size, the benefits of parallelism outweigh the initial thread overhead.
     - Using **16 threads** introduces additional overhead, resulting in a slightly higher time (**2.451 ms**).

  - **Large Graphs (1000 * 4000):**
     - For larger graphs, the advantage of parallelism becomes clear. The version with **16 threads** is the fastest (**413.901 ms**), followed by **8 threads (672.938 ms)**, both significantly more efficient than the sequential approach (**4669.937 ms**).
     - With larger graphs, the workload is naturally better distributed among threads, maximizing computational resources.

---

#### **General Conclusions**
1. **Sequential:**
   - Best suited for small graphs where the overhead of parallelism outweighs its benefits.
2. **Parallelized:**
   - Effective for medium to large graphs, where dividing the workload compensates for the additional thread management costs.
   - The optimal number of threads depends on graph size and hardware:
     - **4 threads** are sufficient for gains in medium-sized graphs.
     - **16 threads** excel in large graphs but show reduced efficiency for smaller graphs due to higher overhead.

---

