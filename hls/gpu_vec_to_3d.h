#ifndef GPU_VEC_TO_3D_H_
#define GPU_VEC_TO_3D_H_

// Shared includes nad defines. Stopps multiples.
#ifndef PROJECT_SHARED_INC_DEF_FIR_FILTER_
#define PROJECT_SHARED_INC_DEF_FIR_FILTER_
    #include "HLS/hls.h"

    typedef struct vector_3d{
        float x;
        float y;
        float z;
    }vec_3d;
#endif

#include "HLS/math.h"
//#include "HLS/extendedmath.h"

typedef float matrix_val_datatype;
const int pipline_buffer_size = 1;


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

            ihc::mm_master<float, ihc::aspace<2>, ihc::awidth<32>, ihc::dwidth<32> > &out_memory);
#endif