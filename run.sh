############################################## Customize and query ##############################################

g++ -I include -L lib -std=c++2a -O3 customise_query.cpp -o cq -lroutingkit
./cq NY 1
./cq BAY 1
./cq COL 1
./cq FLA 1
./cq NW 1
./cq NE 1
./cq CAL 1
./cq LKA 1
./cq E 1
./cq W 1
./cq CTR 1
./cq USA 1
./cq EUR 1

# ./cq NY 6
# ./cq BAY 6
# ./cq COL 6
# ./cq FLA 6
# ./cq NW 6
# ./cq NE 6
# ./cq CAL 6
# ./cq LKA 6
# ./cq E 6
# ./cq W 6
# ./cq CTR 6
# ./cq USA 6
# ./cq EUR 6