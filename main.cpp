#include <routingkit/contraction_hierarchy.h>
#include <routingkit/timer.h>
#include <routingkit/geo_position_to_node.h>
#include <routingkit/nested_dissection.h>
#include <routingkit/customizable_contraction_hierarchy.h>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace RoutingKit;
using namespace std;

typedef uint32_t NodeID;
typedef uint32_t SubgraphID;
typedef uint32_t distance_t;
const size_t n_queries = 10;

// stack of times of start_timer calls
thread_local static vector<chrono::time_point<std::chrono::steady_clock>> start_times;

void start_timer()
{
	start_times.push_back(chrono::steady_clock::now());
}

double stop_timer()
{
	auto stop_time = chrono::steady_clock::now();
	double diff_nano = chrono::duration_cast<chrono::nanoseconds>(stop_time - start_times.back()).count();
	start_times.pop_back();
	return diff_nano / 1.e9;
}

unsigned read_graph(vector<unsigned>& tail, vector<unsigned>& head, vector<unsigned>& weight, istream &in) {
    char line_id;
    unsigned int v, w, d;
    unsigned node_count = 0;

    while (in >> line_id) {
        switch (line_id) {
            case 'p':
                in.ignore(numeric_limits<streamsize>::max(), ' '); // ignore until next space
                in.ignore(numeric_limits<streamsize>::max(), ' '); // ignore until next space
                in >> node_count;
                in.ignore(numeric_limits<streamsize>::max(), '\n'); // skip to end of line
                break;
            case 'a':
                if (in >> v >> w >> d) {
                    tail.push_back(v);
                    head.push_back(w);
                    weight.push_back(d);
                }
                break;
            default:
                in.ignore(numeric_limits<streamsize>::max(), '\n'); // skip to end of line
                break;
        }
    }
    return node_count;
}

int main(int argc, char** argv){

    // generate file path
    string file_path = argv[1];
    string file_name = file_path.substr(7);

    cout << "----------------------------- Start Reading the Graph -----------------------------" << endl;
    cout << endl;

	std::vector<float>latitude;
	std::vector<float>longitude;
	std::vector<unsigned>tail;
	std::vector<unsigned>head;
	std::vector<unsigned> weight;
    unsigned node_count = 0;
    
    // read the graph
    start_timer();
    ifstream ifs(argv[1]);
    node_count = read_graph(tail, head, weight, ifs);
    latitude.resize(node_count, 0);
    longitude.resize(node_count, 0);
    cout << "Read graph " + file_name << endl;
    cout << endl;
    ifs.close();
    cout << "Node count: " << node_count << " Edge count: " << tail.size() << endl;

    cout << "----------------------------- Preproecssing -----------------------------\n" << endl;

	start_timer();
	std::vector<unsigned>node_order = compute_nested_node_dissection_order_using_inertial_flow(node_count, tail, head, latitude, longitude, [](std::string msg){cerr << msg << endl;});
    double order_time = stop_timer();
	start_timer();
    cout << "ordering done" << endl;
	CustomizableContractionHierarchy cch(node_order, tail, head, [](std::string msg){cerr << msg << endl;});
	double CH_construct_time = stop_timer();

    cout << "----------------------------- Customization -----------------------------\n" << endl;
	start_timer();
	CustomizableContractionHierarchyMetric metric(cch, weight);
	metric.customize();
	double customization_time = stop_timer();

    cout << "----------------------------- Query -----------------------------\n" << endl;

    // Generate random queries
    vector<pair<NodeID,NodeID>> queries;
    for (size_t i = 0; i < n_queries; i++) {
		NodeID a = 1 + rand() % node_count;
		NodeID b = 1 + rand() % node_count;
		queries.push_back(make_pair(a, b));
	}

    cout << "Generate " << n_queries << " random queries" << endl;

	start_timer();
	CustomizableContractionHierarchyQuery query(metric);
    // Answer queries
    for (pair<NodeID,NodeID> q : queries)
		unsigned distance = query.reset().add_source(q.first).add_target(q.second).run().get_distance();
    double random_query_time = stop_timer();
    cout << "Ran " << queries.size() << " random queries in " << random_query_time << "s" << endl;

    cout << "----------------------------- Statistics -----------------------------" << endl;
    cout << endl;
	cout << "Order: " + to_string(order_time) + "s" << endl;
	cout << "CH construct: " + to_string(CH_construct_time) + "s" << endl;
    cout << "Total Preprocessing: " + to_string(order_time + CH_construct_time) + "s" << endl;
    cout << "Total customization: " + to_string(customization_time) + "s" << endl;
	// cout << "CH size" + to_string(qu) + "MB" << endl;
	cout << "1000000 Query Time:   " + to_string(random_query_time) + "s" << endl;
	cout << "Average Query Time:   " + to_string(random_query_time/n_queries) + "s" << endl;
    cout << endl;
}