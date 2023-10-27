#include "gpu_vec_to_3d.h"

component
void gpu_3d(ihc::mm_master<float, ihc::aspace<1>, ihc::awidth<32>, ihc::dwidth<32> > &in_memory,
            hls_stable_argument hls_avalon_slave_register_argument int start_addr,
            hls_stable_argument hls_avalon_slave_register_argument int end_addr,
            hls_stable_argument hls_avalon_slave_register_argument float pos_x,
            hls_stable_argument hls_avalon_slave_register_argument float pos_y,
            hls_stable_argument hls_avalon_slave_register_argument float pos_z,
            hls_stable_argument hls_avalon_slave_register_argument float phi,
            hls_stable_argument hls_avalon_slave_register_argument float theta,
            hls_stable_argument hls_avalon_slave_register_argument float psi,

            ihc::mm_master<float, ihc::aspace<2>, ihc::awidth<32>, ihc::dwidth<32> > &out_memory){
    
    hls_memory hls_singlepump hls_bankwidth(sizeof(float))
	float PHI_matrix[3][3] = {  {1,     0,        0},
                                {0,     cos(phi), -sin(phi)},
                                {0,     sin(phi), cos(phi)}
                             };

    hls_memory hls_singlepump hls_bankwidth(sizeof(float))
	float THETA_matrix[3][3] = {  {cos(theta),     0,     sin(theta)},
                                  {0,              1,     0},
                                  {-sin(theta),    0,     cos(theta)}
                               };

    hls_memory hls_singlepump hls_bankwidth(sizeof(float))
	float PSI_matrix[3][3] = {  {cos(psi),    -sin(psi),    0},
                                {sin(psi),     cos(psi),    0},
                                {0,            0,           1}
                             };
    hls_register
    float vec[3];

    hls_register
    int sample_size = end_addr - start_addr;

    float buff;

    for(int i = 0; i < sample_size;)
    {
        vec[0] = in_memory[start_addr + i    ];
        vec[1] = in_memory[start_addr + i + 1];
        vec[2] = in_memory[start_addr + i + 2];

        vec[0] += pos_x;
        vec[1] += pos_y;
        vec[2] += pos_z;

        vec[0] = vec[0] * PHI_matrix[0][0] + vec[0] * PHI_matrix[1][0] + vec[0] * PHI_matrix[2][0];
        vec[1] = vec[1] * PHI_matrix[0][1] + vec[1] * PHI_matrix[1][1] + vec[1] * PHI_matrix[2][1];
        vec[2] = vec[2] * PHI_matrix[0][2] + vec[2] * PHI_matrix[1][2] + vec[2] * PHI_matrix[2][2];

        vec[0] = vec[0] * THETA_matrix[0][0] + vec[0] * THETA_matrix[1][0] + vec[0] * THETA_matrix[2][0];
        vec[1] = vec[1] * THETA_matrix[0][1] + vec[1] * THETA_matrix[1][1] + vec[1] * THETA_matrix[2][1];
        vec[2] = vec[2] * THETA_matrix[0][2] + vec[2] * THETA_matrix[1][2] + vec[2] * THETA_matrix[2][2];

        vec[0] = vec[0] * PSI_matrix[0][0] + vec[0] * PSI_matrix[1][0] + vec[0] * PSI_matrix[2][0];
        vec[1] = vec[1] * PSI_matrix[0][1] + vec[1] * PSI_matrix[1][1] + vec[1] * PSI_matrix[2][1];
        vec[2] = vec[2] * PSI_matrix[0][2] + vec[2] * PSI_matrix[1][2] + vec[2] * PSI_matrix[2][2];

        out_memory[i    ] = vec[0];
        out_memory[i + 1] = vec[1];
        out_memory[i + 2] = vec[2];
        i += 3;
    }
}