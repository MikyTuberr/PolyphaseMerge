# Polyphase Merge Sorting Algorithm

This project implements the **Polyphase Merge** algorithm for efficiently merging multiple sorted input files into a single sorted output file. The algorithm is particularly useful for **external sorting** when data exceeds the available RAM and must be processed in chunks from disk.

### Features:
- Efficient merging of sorted data using **Polyphase Merge** technique.
- Designed to handle large datasets that exceed RAM memory capacity.
- The sorting process is optimized by minimizing disk I/O operations using a **Fibonacci distribution strategy**.

### How It Works:

1. **Distribution Phase**: The data from the input file is divided into several series using Fibonacci series-based distribution across two tapes.
2. **Merging Phase**: The data is merged from the two tapes into an output tape using the Polyphase Merge algorithm. The merging process minimizes the number of disk accesses by optimally managing the tapes.
