#include "gpu_polygon.h"

component
void gpu_polygon(uint8 addr,
                 input_memory_type  &in_memory,
                 var_memory_type    &var_memory,
                 output_memory_type &out_memory){
    fixed_16_9_t vectors[10];
    fixed_16_9_t vars[9];

    #pragma unroll 1
    for(uint5 i = 0; i < 9; i++)
    {
        vectors[i] = in_memory[addr + i];
        vars[i]    = var_memory[i];
    }

    #pragma unroll 1
    for(uint2 i = 0; i < 3; i++)
    {
        vectors[i*3]     = vectors[i*3]     + var_memory[0] - var_memory[6];
        vectors[i*3 + 1] = vectors[i*3 + 1] + var_memory[1] - var_memory[7];
        vectors[i*3 + 2] = vectors[i*3 + 2] + var_memory[2] - var_memory[8];
    }

    fixed_16_9_t sin[3];
    fixed_16_9_t cos[3];

    for(uint2 i = 0; i < 3; i++)
    {
        sin[i] = sinpi_fixed(var_memory[i + 3]);
        cos[i] = cospi_fixed(var_memory[i + 3]);
    }

    for(uint2 i = 0; i < 3; i++)
    {
        vectors[3*i]     *= (vectors[3*i]) * (-sin[1] + cos[1]) * (sin[2] + cos[2]);
        vectors[3*i + 1] *= (vectors[3*i + 1]) * (-sin[2] + cos[2]) * (sin[0] + cos[0]);
        vectors[3*i + 2] *= (vectors[3*i + 2]) * (-sin[0] + cos[0]) * (sin[1] + cos[1]);
    }

    for(uint2 i = 0; i < 3; i++)
    {
        vectors[3*i]     += var_memory[6];
        vectors[3*i + 1] += var_memory[7];
        vectors[3*i + 2] += var_memory[8];
    }

    #pragma unroll 1
    for(uint5 i = 0; i < 10; i++)
    {
        out_memory[i] = vectors[i].to_ac_int();
    }
}