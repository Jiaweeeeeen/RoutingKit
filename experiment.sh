#!/bin/bash

# Define your source files and output binary name
# SOURCE_FILES="src/road_network_h2h.cpp src/util.cpp src/query.cpp"
OUTPUT_BINARY="main"
SOURCE_FILES = main.cpp
GRAPH_T_FILES=("USA-road-t-NY-index" "USA-road-t-BAY-index" "USA-road-t-COL-index" "USA-road-t-FLA-index" "USA-road-t-CAL-index" "USA-road-t-E-index" "USA-road-t-W-index" "USA-road-t-CTR-index" "USA-road-t-USA-index" "EUR-road-t-index")
GRAPH_D_FILES=("USA-road-d-NY-index" "USA-road-d-BAY-index" "USA-road-d-COL-index" "USA-road-d-FLA-index" "USA-road-d-CAL-index" "USA-road-d-E-index" "USA-road-d-W-index" "USA-road-d-CTR-index" "USA-road-d-USA-index" "EUR-road-d-index")
# BOOST_PATH="/opt/homebrew/Cellar/boost/1.84.0_1/include"
INCLUDE = include
LIB = lib


# Compilation step
# g++ -std=c++2a -O3 -pthread $SOURCE_FILES -o $OUTPUT_BINARY
g++ -I $INCLUDE -L $LIB -std=c++11 $SOURCE_FILES -o $OUTPUT_BINARY -lroutingkit
# g++ -std=c++2a -O3 -pthread -I $BOOST_PATH $SOURCE_FILES -o $OUTPUT_BINARY

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program..."
    for ((i=0; i<${#GRAPH_T_FILES[@]}; i++));
    do
        ./$OUTPUT_BINARY "graphs/${GRAPH_T_FILES[$i]}"
        ./$OUTPUT_BINARY "graphs/${GRAPH_D_FILES[$i]}"
    done
else
    echo "Compilation failed."
fi