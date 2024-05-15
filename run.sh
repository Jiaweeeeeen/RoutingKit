# g++ -I include -L lib -std=c++2a -O3 src/convert_road_dimacs_coordinates.cpp -o convert -lroutingkit
# ./convert experiments/EUR/EUR-road-d.co experiments/EUR/latitude experiments/EUR/longtitude   
# g++ -I include -L lib -std=c++2a -O3 src/convert_road_dimacs_graph.cpp -o convert -lroutingkit
# ./convert experiments/EUR/EUR-road-t.gr experiments/EUR/first_out experiments/EUR/head experiments/EUR/weight_t 
# ./convert experiments/EUR/EUR-road-t.gr experiments/EUR/first_out experiments/EUR/head experiments/EUR/weight_d

############################################## Preprocess ##############################################
# g++ -I include -L lib -std=c++2a -O3 preprocess.cpp -o p -lroutingkit
# ./p experiments/NY/first_out experiments/NY/head experiments/NY/latitude experiments/NY/longtitude 1
# ./p experiments/BAY/first_out experiments/BAY/head experiments/BAY/latitude experiments/BAY/longtitude 1
# ./p experiments/COL/first_out experiments/COL/head experiments/COL/latitude experiments/COL/longtitude 1
# ./p experiments/FLA/first_out experiments/FLA/head experiments/FLA/latitude experiments/FLA/longtitude 1
# ./p experiments/CAL/first_out experiments/CAL/head experiments/CAL/latitude experiments/CAL/longtitude 1
# ./p experiments/E/first_out experiments/E/head experiments/E/latitude experiments/E/longtitude 1
# ./p experiments/W/first_out experiments/W/head experiments/W/latitude experiments/W/longtitude 1
# ./p experiments/CTR/first_out experiments/CTR/head experiments/CTR/latitude experiments/CTR/longtitude 1
# ./p experiments/USA/first_out experiments/USA/head experiments/USA/latitude experiments/USA/longtitude 1

############################################## Customize and query ##############################################
# g++ -I include -L lib -std=c++2a -O3 customise_query.cpp -o cq -lroutingkit
# ./cq experiments/NY/first_out experiments/NY/head experiments/NY/node_order experiments/NY/weight_t 1
# ./cq experiments/BAY/first_out experiments/BAY/head experiments/BAY/node_order experiments/BAY/weight_t 1
# ./cq experiments/COL/first_out experiments/COL/head experiments/COL/node_order experiments/COL/weight_t 1
# ./cq experiments/FLA/first_out experiments/FLA/head experiments/FLA/node_order experiments/FLA/weight_t 1
# ./cq experiments/CAL/first_out experiments/CAL/head experiments/CAL/node_order experiments/CAL/weight_t 1
# ./cq experiments/E/first_out experiments/E/head experiments/E/node_order experiments/E/weight_t 1
# ./cq experiments/W/first_out experiments/W/head experiments/W/node_order experiments/W/weight_t 1
# ./cq experiments/CTR/first_out experiments/CTR/head experiments/CTR/node_order experiments/CTR/weight_t 1
# ./cq experiments/USA/first_out experiments/USA/head experiments/USA/node_order experiments/USA/weight_t 1

# ./cq experiments/NY/first_out experiments/NY/head experiments/NY/node_order experiments/NY/weight_d 1
# ./cq experiments/BAY/first_out experiments/BAY/head experiments/BAY/node_order experiments/BAY/weight_d 1
# ./cq experiments/COL/first_out experiments/COL/head experiments/COL/node_order experiments/COL/weight_d 1
# ./cq experiments/FLA/first_out experiments/FLA/head experiments/FLA/node_order experiments/FLA/weight_d 1
# ./cq experiments/CAL/first_out experiments/CAL/head experiments/CAL/node_order experiments/CAL/weight_d 1
# ./cq experiments/E/first_out experiments/E/head experiments/E/node_order experiments/E/weight_d 1
# ./cq experiments/W/first_out experiments/W/head experiments/W/node_order experiments/W/weight_d 1
# ./cq experiments/CTR/first_out experiments/CTR/head experiments/CTR/node_order experiments/CTR/weight_d 1
# ./cq experiments/USA/first_out experiments/USA/head experiments/USA/node_order experiments/USA/weight_d 1

############################################## All ##############################################
g++ -I include -L lib -std=c++2a -O3 main.cpp -o main -lroutingkit
./main experiments/NY/first_out experiments/NY/head experiments/NY/latitude experiments/NY/longtitude experiments/NY/weight_t 1
./main experiments/BAY/first_out experiments/BAY/head experiments/BAY/latitude experiments/BAY/longtitude experiments/BAY/weight_t 1
./main experiments/COL/first_out experiments/COL/head experiments/COL/latitude experiments/COL/longtitude experiments/COL/weight_t 1
./main experiments/FLA/first_out experiments/FLA/head experiments/FLA/latitude experiments/FLA/longtitude experiments/FLA/weight_t 1
./main experiments/CAL/first_out experiments/CAL/head experiments/CAL/latitude experiments/CAL/longtitude experiments/CAL/weight_t 1
./main experiments/E/first_out experiments/E/head experiments/E/latitude experiments/E/longtitude experiments/E/weight_t 1
./main experiments/W/first_out experiments/W/head experiments/W/latitude experiments/W/longtitude experiments/W/weight_t 1
./main experiments/CTR/first_out experiments/CTR/head experiments/CTR/latitude experiments/CTR/longtitude experiments/CTR/weight_t 1
./main experiments/USA/first_out experiments/USA/head experiments/USA/latitude experiments/USA/longtitude experiments/USA/weight_t 1
# ./main experiments/EUR/first_out experiments/EUR/head experiments/EUR/latitude experiments/EUR/longtitude experiments/EUR/weight_d 1

./main experiments/NY/first_out experiments/NY/head experiments/NY/latitude experiments/NY/longtitude experiments/NY/weight_d 1
./main experiments/BAY/first_out experiments/BAY/head experiments/BAY/latitude experiments/BAY/longtitude experiments/BAY/weight_d 1
./main experiments/COL/first_out experiments/COL/head experiments/COL/latitude experiments/COL/longtitude experiments/COL/weight_d 1
./main experiments/FLA/first_out experiments/FLA/head experiments/FLA/latitude experiments/FLA/longtitude experiments/FLA/weight_d 1
./main experiments/CAL/first_out experiments/CAL/head experiments/CAL/latitude experiments/CAL/longtitude experiments/CAL/weight_d 1
./main experiments/E/first_out experiments/E/head experiments/E/latitude experiments/E/longtitude experiments/E/weight_d 1
./main experiments/W/first_out experiments/W/head experiments/W/latitude experiments/W/longtitude experiments/W/weight_d 1
./main experiments/CTR/first_out experiments/CTR/head experiments/CTR/latitude experiments/CTR/longtitude experiments/CTR/weight_d 1
./main experiments/USA/first_out experiments/USA/head experiments/USA/latitude experiments/USA/longtitude experiments/USA/weight_d 1
# ./main experiments/EUR/first_out experiments/EUR/head experiments/EUR/latitude experiments/EUR/longtitude experiments/EUR/weight_d 1