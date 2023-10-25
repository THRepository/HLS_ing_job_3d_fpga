#ifndef VEC_INPUT_STREAMER_H_
#define VEC_INPUT_STREAMER_H_

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

hls_avalon_slave_component component 
void vec_input(ihc::mm_master<float, ihc::aspace<1>, ihc::awidth<32>, ihc::dwidth<32> > &in_memory,
               ihc::stream_out<vec_3d>& vec_edge_0,
               ihc::stream_out<vec_3d>& vec_edge_2,
               ihc::stream_out<vec_3d>& vec_edge_3,
               hls_stable_argument hls_avalon_slave_register_argument int Nr_Vectors);

#endif