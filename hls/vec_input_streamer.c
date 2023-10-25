#include "vec_input_streamer.h"

#define m_vector_start_offset 14

// Sample buffer supplies input to FIR-filter (used for verification/validation)
hls_avalon_slave_component component 
void vec_input(ihc::mm_master<float, ihc::aspace<1>, ihc::awidth<32>, ihc::dwidth<32> > &in_memory,
               ihc::stream_out<vec_3d>& vec_edge_0,
               ihc::stream_out<vec_3d>& vec_edge_2,
               ihc::stream_out<vec_3d>& vec_edge_3,
               hls_stable_argument hls_avalon_slave_register_argument int Nr_Vectors) {
    vec_3d sample_vec_0, sample_vec_1, sample_vec_2;

    for (int i = 0; i+9 <= Nr_Vectors;){
        sample_vec_0.x = in_memory[i];
        sample_vec_0.y = in_memory[i+1];
        sample_vec_0.z = in_memory[i+2];
        vec_edge_0.write(sample_vec_0);

        sample_vec_1.x = in_memory[i+3];
        sample_vec_1.y = in_memory[i+4];
        sample_vec_1.z = in_memory[i+5];
        vec_edge_2.write(sample_vec_1);

        sample_vec_2.x = in_memory[i+6];
        sample_vec_2.y = in_memory[i+7];
        sample_vec_2.z = in_memory[i+8];
        vec_edge_3.write(sample_vec_2);
        i += 9;
    }
}