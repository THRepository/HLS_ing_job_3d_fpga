#ifndef GPU_POLYGON_H_
#define GPU_POLYGON_H_

// Shared includes nad defines. Stopps multiples.
#ifndef PROJECT_SHARED_INC_DEF_FIR_FILTER_
#define PROJECT_SHARED_INC_DEF_FIR_FILTER_
    #include "HLS/hls.h"
#endif
#include "HLS/ac_fixed_math.h"
#include "HLS/ac_int.h"

typedef ac_fixed<16, 8, true> fixed_16_9_t;

typedef ihc::mm_master<fixed_16_9_t,    ihc::aspace<1>, 
                       ihc::awidth<16>, ihc::dwidth<16>, 
                       ihc::align<16>,  ihc::latency<1>, 
                       ihc::waitrequest<true> >          input_memory_type;

typedef ihc::mm_master<fixed_16_9_t,    ihc::aspace<2>, 
                       ihc::awidth<4>,  ihc::dwidth<16>, 
                       ihc::align<16>,  ihc::latency<1>, 
                       ihc::waitrequest<true> >          var_memory_type;

typedef ihc::mm_master<int8,            ihc::aspace<3>, 
                       ihc::awidth<16>, ihc::dwidth<16>, 
                       ihc::align<16>,  ihc::latency<1>, 
                       ihc::waitrequest<true> >          output_memory_type;
component
void gpu_polygon(hls_stable_argument uint8 addr,
                 input_memory_type  &in_memory,
                 var_memory_type    &var_memory,
                 output_memory_type &out_memory);

#endif