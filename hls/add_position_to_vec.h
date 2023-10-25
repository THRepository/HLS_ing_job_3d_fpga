#ifndef ADD_POSITION_TO_VEC_H_
#define ADD_POSITION_TO_VEC_H_

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

component 
void add_to_vec(ihc::stream_in<vec_3d>& vec_edge_0,
                ihc::stream_in<vec_3d>& vec_edge_1,
                ihc::stream_in<vec_3d>& vec_edge_2,
                ihc::stream_out<vec_3d>& vec_edge_0_moved,
                ihc::stream_out<vec_3d>& vec_edge_1_moved,
                ihc::stream_out<vec_3d>& vec_edge_2_moved,
                hls_stable_argument hls_avalon_slave_register_argument short x,
                hls_stable_argument hls_avalon_slave_register_argument short y,
                hls_stable_argument hls_avalon_slave_register_argument short z);

#endif