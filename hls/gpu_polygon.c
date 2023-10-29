#include "gpu_polygon.h"

#define phi 0
#define theta 1
#define psi 2

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
    for(uint2 i = 0; i < 3; i++) // 0 = phi x | 1 = theta y | 2 = psi z
    {
        vectors[3*i]     *= cos[theta]*cos[psi] 
                          + cos[theta]*sin[psi] 
                          - sin[theta];

        vectors[3*i + 1] *= sin[phi]*sin[theta]*cos[psi] - cos[phi]*sin[psi]
                          + sin[phi]*sin[theta]*sin[psi] + cos[phi]*cos[psi] 
                          + sin[phi]*cos[theta];

        vectors[3*i + 2] *= cos[phi]*sin[theta]*cos[psi] + sin[phi]*sin[psi] 
                          + cos[phi]*sin[theta]*sin[psi] - sin[phi]*cos[psi] 
                          + cos[phi]*cos[theta];

        // vectors[3*i]     *= ( -sin[theta] + cos[theta] ) * ( sin[psi]   + cos[psi] );
        // vectors[3*i + 1] *= ( -sin[psi]   + cos[psi] )   * ( sin[phi]   + cos[phi] );
        // vectors[3*i + 2] *= ( -sin[phi]   + cos[phi] )   * ( sin[theta] + cos[theta] );
    }

    for(uint2 i = 0; i < 3; i++)
    {
        vectors[3*i]     += var_memory[6];
        vectors[3*i + 1] += var_memory[7];
        vectors[3*i + 2] += var_memory[8];
    }

    fixed_16_9_t b[3];
    fixed_16_9_t c[3];
    fixed_16_9_t normal[3];

    for(uint2 i = 0; i < 3; i++) // tree edges.
    {
        b[i] = vectors[i + 3] - vectors[i];
        c[i] = vectors[i + 6] - vectors[i];
    }

    // Mnemonic
    normal[0] = b[1] * c[2] - b[2] * c[1];
    normal[1] = b[2] * c[0] - b[0] * c[2];
    normal[2] = b[0] * c[1] - b[1] * c[0];
    
    // Normalizing factor
    fixed_16_9_t n = normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2];
    n = sqrt_fixed(n);

    if(n != (fixed_16_9_t) 0.0)
    {
        normal[0] /= n;
        normal[1] /= n;
        normal[2] /= n;
    }
    
    n = normal[0] * vectors[0] +
        normal[1] * vectors[1] +
        normal[2] * vectors[2];
    
    if(n < (fixed_16_9_t) 0.0)
    {
        vectors[9] =  1.0;
    }else{
        vectors[9] = -1.0;
    }

    #pragma unroll 1
    for(uint5 i = 0; i < 10; i++)
    {
        out_memory[i] = vectors[i].to_ac_int();
    }
}