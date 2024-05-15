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
        std::cout << "Usage: " << argv[0] << " first_out head latitude longtitude cores" << std::endl;
        return 1;
    }

    std::string first_out_file = argv[1];
    std::string head_file = argv[2];
    std::string latitude_file = argv[3];
    std::string longitude_file = argv[4];
    int cores = std::stoi(argv[5]);

    cout << "----------------------------- Start Reading the Graph -----------------------------" << endl;
    cout << endl;

    std::vector<unsigned> first_out = RoutingKit::load_vector<unsigned>(first_out_file);
    std::vector<unsigned> tail = RoutingKit::invert_inverse_vector(first_out);
    std::vector<unsigned> head = RoutingKit::load_vector<unsigned>(head_file);
    std::vector<float> latitude = RoutingKit::load_vector<float>(latitude_file);
    std::vector<float> longitude = RoutingKit::load_vector<float>(longitude_file);
    unsigned node_count = first_out.size()-1;


    std::cout << "read input " << first_out.size() << " " << tail.size() << " " << head.size() << " " << latitude.size() << " " << longitude.size() << std::endl;

    cout << "----------------------------- Preproecssing -----------------------------\n" << endl;

	start_timer();
	// std::vector<unsigned>node_order = compute_nested_node_dissection_order_using_inertial_flow(node_count, tail, head, latitude, longitude, [](std::string msg){cerr << msg << endl;});
	std::vector<unsigned>node_order = compute_nested_node_dissection_order_using_inertial_flow(node_count, tail, head, latitude, longitude);
    double order_time = stop_timer();
    std::cout << "ordering done" << std::endl;
    std::string order_file = first_out_file.substr(0, first_out_file.size() - 9) + "node_order";
    std::cout << "order file: " << order_file << std::endl;
    RoutingKit::save_vector(order_file, node_order);
    std::cout << "save order" << std::endl;
    start_timer();
	// CustomizableContractionHierarchy cch(node_order, tail, head, [](std::string msg){cerr << msg << endl;});
    CustomizableContractionHierarchy cch(node_order, tail, head);
    std::cout << "built CCH" << std::endl;
	double CH_construct_time = stop_timer();

    cout << "----------------------------- Statistics -----------------------------" << endl;
    cout << endl;
	cout << "Order: " + to_string(order_time) + "s" << endl;
	cout << "CH construct: " + to_string(CH_construct_time) + "s" << endl;
    cout << "Total Preprocessing: " + to_string(order_time + CH_construct_time) + "s" << endl;
}
   