# K-Means Clustering Algorithm

## Description

This C++ program implements the **K-Means clustering algorithm** to classify data points into clusters based on their attributes. It reads data from a user-provided file, runs the clustering algorithm, displays results in the console with colored formatting, and optionally exports the raw results to a `.txt` file.

## Features

- Customizable iteration count and error threshold.
- File parsing with detailed error feedback.
- Color-coded terminal output for better readability.
- Option to export raw clustering results to a file.
- Built-in manual for data formatting and error explanations.

## Input File Format

The input file must be a text file with space-separated values in the following format:

```
<iteration_count> <max_error> <number_of_classes>
<point1_attr1> <point1_attr2> ... <point1_attrN>
<point2_attr1> <point2_attr2> ... <point2_attrN>
...
<pointM_attr1> <pointM_attr2> ... <pointM_attrN>
```

Where:
- `N` is the number of attributes.
- `M` is the number of data points.

### Example:

```
100 0.01 3
1.0 2.0
2.0 1.0
3.0 3.0
8.0 9.0
9.0 8.0
```

## Output Format

The exported file contains raw formatted data with minimal markup:

```
Precision = <decimal_precision> Error = <final_error>
Centroid Cluster_id Point
<centroid_attributes> <cluster_id> <assigned_point_attributes>
...
```

## Error Messages

### Input Errors
- `CAN_NOT_OPEN_FILE`: Unable to open the file.
- `TOP_VARIABLE_VALUE`: Invalid format for the first three variables.
- `TOP_CLASS_COUNT_IS_NULL`: Number of classes is zero.
- `TOP_CLASS_COUNT_TO_BIG`: Number of classes exceeds number of points.
- `CLASS_ATRIBUTE_VALUE`: Attribute could not be converted to a number.
- `CLASS_ATRIBUTE_COUNT`: Attribute count mismatch across points.

### Output Errors
- `CAN_NOT_CREATE_FILE`: File cannot be created. Check path or disk health.

## Usage

1. Compile the program using a C++ compiler (Windows only, due to use of `Windows.h` and console commands).
2. Run the executable. You will be presented with a menu.
3. Select the option to load data from file.
4. Run the algorithm.
5. View results or export them.

## Build Instructions

Use a C++17-compatible compiler on Windows.

## Notes

- This program uses ANSI escape codes for colored console output.
- Do not resize the console during clustering execution to avoid formatting issues.
- Works best in Windows terminal or compatible consoles with virtual terminal processing enabled.

## Author

Łukasz Gębka
