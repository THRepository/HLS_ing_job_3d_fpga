#include "gpu_vec_to_3d.h"

component
void gpu_3d(ihc::mm_master<float, ihc::aspace<3>, ihc::awidth<32>, ihc::dwidth<32> > &in_memory,
            hls_stable_argument hls_avalon_slave_register_argument int start_addr,
            hls_stable_argument hls_avalon_slave_register_argument int end_addr,
            hls_stable_argument hls_avalon_slave_register_argument float pos_x,
            hls_stable_argument hls_avalon_slave_register_argument float pos_y,
            hls_stable_argument hls_avalon_slave_register_argument float pos_z,

            ihc::mm_master<float, ihc::aspace<2>, ihc::awidth<32>, ihc::dwidth<32> > &out_memory){
    hls_memory hls_singlepump hls_bankwidth(sizeof(matrix_val_datatype))
	matrix_val_datatype p_matrix[4][4];

    hls_register
    matrix_val_datatype vec[pipline_buffer_size][3];

    hls_register
    int sample_size = end_addr - start_addr;

    for(int i = 0; i < sample_size;)
    {
        for(int j = 0; j < pipline_buffer_size; j++)
        {
            vec[j][0] = in_memory[start_addr + i    ];
            vec[j][1] = in_memory[start_addr + i + 1];
            vec[j][2] = in_memory[start_addr + i + 2];
        }

        vec[0][0] += pos_x;
        vec[0][1] += pos_y;
        vec[0][2] += pos_z;

        for(int j = 0; j < pipline_buffer_size; j++)
        {
            out_memory[i    ] = vec[j][0];
            out_memory[i + 1] = vec[j][1];
            out_memory[i + 2] = vec[j][2];
            i += 3;
        }
    }
}