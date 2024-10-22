#include <routingkit/contraction_hierarchy.h>
#include <routingkit/timer.h>
#include <routingkit/geo_position_to_node.h>
#include <routingkit/nested_dissection.h>
#include <routingkit/customizable_contraction_hierarchy.h>
#include <routingkit/vector_io.h>
#include <routingkit/inverse_vector.h>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace RoutingKit;
using namespace std;

typedef uint32_t NodeID;
typedef uint32_t SubgraphID;
typedef uint32_t distance_t;
string experiments_folder = "experiments/";
const int micro_unit = 1000000;

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

string order_path(string G, string P){
    return experiments_folder + G + "." + P + ".order";
}

string graph_path(string G){
    return experiments_folder + G + "/";
}

string query_source_file(string G){
    return experiments_folder + G + ".q.s";
}

string query_target_file(string G){
    return experiments_folder + G + ".q.t";
}

int main(int argc, char** argv){
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << "graph cores" << std::endl;
		return 1;
	}
    std::string g = argv[1];
    int cores = std::stoi(argv[2]);

    std::string first_out_file = graph_path(g) + "first_out";
    std::string head_file = graph_path(g) + "head";
    std::string order_file = order_path(g, "flowcutter20");
    std::string weight_file = graph_path(g) + "travel_time";
	std::string query_sources_file = query_source_file(g);
	std::string query_targets_file = query_target_file(g);

    // cout << "----------------------------- Start Reading the Graph -----------------------------" << endl;
    // cout << endl;

    std::vector<unsigned> first_out = RoutingKit::load_vector<unsigned>(first_out_file);
    std::vector<unsigned> tail = RoutingKit::invert_inverse_vector(first_out);
    std::vector<unsigned> head = RoutingKit::load_vector<unsigned>(head_file);
    std::vector<unsigned> node_order = RoutingKit::load_vector<unsigned>(order_file);
    std::vector<unsigned> weight = RoutingKit::load_vector<unsigned>(weight_file);
	std::vector<unsigned> query_sources = RoutingKit::load_vector<unsigned>(query_sources_file);
	std::vector<unsigned> query_targets = RoutingKit::load_vector<unsigned>(query_targets_file);
    unsigned node_count = first_out.size()-1;
    // std::cout << "read input " << first_out.size() << " " << tail.size() << " " << head.size() << " " << node_order.size() << " " << weight.size() << std::endl;

    // cout << "----------------------------- Preproecssing -----------------------------\n" << endl;

    start_timer();
	// CustomizableContractionHierarchy cch(node_order, tail, head);
    CustomizableContractionHierarchy cch(node_order, tail, head, [](std::string msg){cerr << msg << endl;});
    // std::cout << "built CCH" << std::endl;
	double CH_construct_time = stop_timer();

    // cout << "----------------------------- Customization -----------------------------\n" << endl;
	start_timer();
	CustomizableContractionHierarchyMetric metric(cch, weight);
	if (cores > 1) {
		RoutingKit::CustomizableContractionHierarchyParallelization parallel_custom(cch);
		parallel_custom.customize(metric, static_cast<unsigned int>(cores));
	}
	else {
        metric.customize();
	}
    double customization_time = stop_timer();
    // std::cout << "built metric" << std::endl;

    // cout << "----------------------------- Query -----------------------------\n" << endl;
    int n_queries = (int) query_sources.size();
    start_timer();
	RoutingKit::CustomizableContractionHierarchyQuery query(metric);
	// double time = -RoutingKit::get_micro_time();
	for (int i = 0; i < (int) query_sources.size(); ++i) {
		unsigned s = query_sources[i]; unsigned t = query_targets[i];
		query.reset().add_source(s).add_target(t).run();
	}
    double random_query_time = stop_timer();
    // cout << "Ran " << (int) query_sources.size() << " random queries in " << random_query_time << "s" << endl;

    cout << "----------------------------- Statistics -----------------------------" << endl;
    cout << endl;
    cout << "Graph: " << g << endl;
    cout << "CH initialization: " + to_string(CH_construct_time) + "s" << endl;
    cout << "Total customization: " + to_string(customization_time) + "s" << endl;
	cout << n_queries << " Query Time:   " + to_string(random_query_time) + "s" << endl;
	cout << "Average Query Time:   " + to_string(random_query_time/n_queries*micro_unit) + " microseconds" << endl;
    cout << endl;
}