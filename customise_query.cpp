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
const size_t n_queries = 1000;

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

int main(int argc, char** argv){
    if (argc != 6) {
        std::cout << "Usage: " << argv[0] << " first_out head order metric cores" << std::endl;
        return 1;
    }

    std::string first_out_file = argv[1];
    std::string head_file = argv[2];
    std::string order_file = argv[3];
    std::string weight_file = argv[4];
    int cores = std::stoi(argv[5]);

    cout << "----------------------------- Start Reading the Graph -----------------------------" << endl;
    cout << endl;

    std::vector<unsigned> first_out = RoutingKit::load_vector<unsigned>(first_out_file);
    std::vector<unsigned> tail = RoutingKit::invert_inverse_vector(first_out);
    std::vector<unsigned> head = RoutingKit::load_vector<unsigned>(head_file);
    std::vector<unsigned> node_order = RoutingKit::load_vector<unsigned>(order_file);
    std::vector<unsigned> weight = RoutingKit::load_vector<unsigned>(weight_file);
    unsigned node_count = first_out.size()-1;
    std::cout << "read input " << first_out.size() << " " << tail.size() << " " << head.size() << " " << node_order.size() << " " << weight.size() << std::endl;

    cout << "----------------------------- Preproecssing -----------------------------\n" << endl;

    start_timer();
	CustomizableContractionHierarchy cch(node_order, tail, head);
    std::cout << "built CCH" << std::endl;
	double CH_construct_time = stop_timer();

    cout << "----------------------------- Customization -----------------------------\n" << endl;
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
    std::cout << "built metric" << std::endl;

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
		query.reset().add_source(q.first).add_target(q.second).run();
    double random_query_time = stop_timer();
    cout << "Ran " << queries.size() << " random queries in " << random_query_time << "s" << endl;

    cout << "----------------------------- Statistics -----------------------------" << endl;
    cout << endl;
    cout << "Total customization: " + to_string(customization_time) + "s" << endl;
	cout << n_queries << " Query Time:   " + to_string(random_query_time) + "s" << endl;
	cout << "Average Query Time:   " + to_string(random_query_time/n_queries) + "s" << endl;
    cout << endl;
}