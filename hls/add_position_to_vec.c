#include "add_position_to_vec.h"

component 
void add_to_vec(ihc::stream_in<vec_3d>& vec_edge_0,
                ihc::stream_in<vec_3d>& vec_edge_1,
                ihc::stream_in<vec_3d>& vec_edge_2,
                ihc::stream_out<vec_3d>& vec_edge_0_moved,
                ihc::stream_out<vec_3d>& vec_edge_1_moved,
                ihc::stream_out<vec_3d>& vec_edge_2_moved,
                hls_stable_argument hls_avalon_slave_register_argument short x,
                hls_stable_argument hls_avalon_slave_register_argument short y,
                hls_stable_argument hls_avalon_slave_register_argument short z) {
    
    vec_3d vec_to_send_0, vec_to_send_1, vec_to_send_2;

    vec_to_send_0 = vec_edge_0.read();
    vec_to_send_0.x = vec_to_send_0.x + x;
    vec_to_send_0.y = vec_to_send_0.y + y;
    vec_to_send_0.z = vec_to_send_0.z + z;
    vec_edge_0_moved.write(vec_to_send_0);

    vec_to_send_1 = vec_edge_1.read();
    vec_to_send_1.x = vec_to_send_1.x + x;
    vec_to_send_1.y = vec_to_send_1.y + y;
    vec_to_send_1.z = vec_to_send_1.z + z;
    vec_edge_1_moved.write(vec_to_send_1);

    vec_to_send_2 = vec_edge_2.read();
    vec_to_send_2.x = vec_to_send_2.x + x;
    vec_to_send_2.y = vec_to_send_2.y + y;
    vec_to_send_2.z = vec_to_send_2.z + z;
    vec_edge_2_moved.write(vec_to_send_2);
}