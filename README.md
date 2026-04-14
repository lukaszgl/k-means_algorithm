# K-Means Clustering Algorithm
> **Note:** This repository is published as a completed university project, demonstrating core software engineering and algorithm design principles. It was developed entirely through manual programming, without the assistance of LLMs or AI code generation tools.
## Description

This C++ program implements the **K-Means clustering algorithm** to classify data points into clusters based on their attributes. It reads data from a user-provided file, runs the clustering algorithm, displays results in the console with colored formatting, and optionally exports the raw results to a `.txt` file.


## Features

- **Object-Oriented Design:** Encapsulated clustering engine ensuring memory safety and clean state management.
- **Robust Exception Handling:** Graceful recovery from file I/O errors, invalid data formatting, and divide-by-zero edge cases.
- **Dynamic Terminal UI:** Color-coded console output with auto-scaling table columns based on data precision.
- **Customizable Parameters:** User-defined iteration limits and maximum error thresholds.
- **Data Export:** Option to export raw, minimally formatted clustering results to a `.txt` file for downstream analysis.
  
## Usage
### Prerequisites
- A C++17 compatible compiler.
- **Windows OS:** This application utilizes `<Windows.h>` for terminal ANSI escape codes and virtual terminal processing.

### Build & Run
1. Clone the repository and open the project files in Visual Studio (or your preferred Windows C++ IDE).
2. Compile the executable.
3. Run the application in a standalone terminal window (avoid resizing the window during execution to maintain UI table formatting).
4. Follow the on-screen menu to extract data, set precision, and run the algorithm.
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
- `CAN_NOT_OPEN_FILE`: Unable to locate/open the file. Verify the path and extension.
- `TOP_VARIABLE_VALUE`: Invalid data types found in the configuration header (first three variables).
- `TOP_CLASS_COUNT_IS_NULL`: Target number of classes cannot be zero.
- `TOP_CLASS_COUNT_TOO_BIG`: The requested number of classes exceeds the total number of provided data points.
- `CLASS_ATRIBUTE_VALUE`: An attribute could not be parsed as a numerical value.
- `CLASS_ATRIBUTE_COUNT`: Inconsistent number of attributes detected across points.

### Output Errors
- `CAN_NOT_CREATE_FILE`: Unable to write the output file. Verify directory write permissions or disk health.

## Author

Łukasz Gębka
