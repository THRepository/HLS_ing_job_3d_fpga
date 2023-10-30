#include "gpu_polygon.h"

#define phi 0
#define theta 1
#define psi 2

component
void gpu_polygon(hls_stable_argument uint8 addr,
                 input_memory_type  &in_memory,
                 var_memory_type    &var_memory,
                 output_memory_type &out_memory){
    hls_register fixed_16_9_t vectors[10];
    hls_register fixed_16_9_t vectors_copy[9];
    hls_register fixed_16_9_t vars[9];
    hls_register fixed_16_9_t sin[3];
    hls_register fixed_16_9_t cos[3];
    fixed_16_9_t rot_phi[3][3];
    fixed_16_9_t rot_theta[3][3];
    fixed_16_9_t rot_psi[3][3];

    #pragma unroll 1
    for(uint5 i = 0; i < 9; i++)
    {
        vectors[i] = in_memory[addr + i];
        vars[i]    = var_memory[i];
    }
    #pragma unroll 1
    for(uint3 i = 0; i < 3; i++)
    {
        sin[i] = sinpi_fixed(var_memory[i + 3]);
        cos[i] = cospi_fixed(var_memory[i + 3]);
    }

    #pragma unroll 1
    for(uint3 i = 0; i < 3; i++)
    {
        vectors[i*3]     = vectors[i*3]     + var_memory[0] - var_memory[6];
        vectors[i*3 + 1] = vectors[i*3 + 1] + var_memory[1] - var_memory[7];
        vectors[i*3 + 2] = vectors[i*3 + 2] + var_memory[2] - var_memory[8];
    }

    // #pragma unroll 1
    // for(uint3 i = 0; i < 3;i++ )
    // {
    //     vectors_copy[i*3 + 1] = vectors[i*3 + 1];
    //     vectors_copy[i*3 + 2] = vectors[i*3 + 2];

    //     vectors[i*3 + 1] = vectors_copy[i*3 + 1] * cos[phi] - vectors_copy[i*3 + 2] * sin[phi];
    //     vectors[i*3 + 2] = vectors_copy[i*3 + 2] * cos[phi] + vectors_copy[i*3 + 1] * sin[phi];
    // }

    // #pragma unroll 1
    // for(uint3 i = 0; i < 3;i++ )
    // {
    //     vectors_copy[i*3    ] = vectors[i*3    ];
    //     vectors_copy[i*3 + 2] = vectors[i*3 + 2];

    //     vectors[i*3    ] = vectors_copy[i*3    ] * cos[theta] - vectors_copy[i*3 + 2] * sin[theta];
    //     vectors[i*3 + 2] = vectors_copy[i*3 + 2] * cos[theta] + vectors_copy[i*3    ] * sin[theta];
    // }

    // #pragma unroll 1
    // for(uint3 i = 0; i < 3;i++ )
    // {
    //     vectors_copy[i*3    ] = vectors[i*3    ];
    //     vectors_copy[i*3 + 1] = vectors[i*3 + 1];

    //     vectors[i*3    ] = vectors_copy[i*3    ] * cos[psi] - vectors_copy[i*3 + 1] * sin[psi];
    //     vectors[i*3 + 1] = vectors_copy[i*3 + 1] * cos[psi] + vectors_copy[i*3    ] * sin[psi];
    // }

    rot_phi[0][0] = 1.0;
    rot_phi[0][1] = 0.0;
    rot_phi[0][2] = 0.0;
    rot_phi[1][0] = 0.0;
    rot_phi[1][1] = cos[phi];
    rot_phi[1][2] = -sin[phi];
    rot_phi[2][0] = 0.0;
    rot_phi[2][1] = sin[phi];
    rot_phi[2][2] = cos[phi];

    rot_theta[0][0] = cos[theta];
    rot_theta[0][1] = 0.0;
    rot_theta[0][2] = sin[theta];
    rot_theta[1][0] = 0.0;
    rot_theta[1][1] = 1.1;
    rot_theta[1][2] = 0.0;
    rot_theta[2][0] = -sin[theta];
    rot_theta[2][1] = 0.0;
    rot_theta[2][2] = cos[theta];

    rot_psi[0][0] = cos[psi];
    rot_psi[0][1] = -sin[psi];
    rot_psi[0][2] = 0.0;
    rot_psi[1][0] = sin[psi];
    rot_psi[1][1] = cos[psi];
    rot_psi[1][2] = 0.0;
    rot_psi[2][0] = 0.0;
    rot_psi[2][1] = 0.0;
    rot_psi[2][2] = 1.0;

    // vectors[3*i]     *=     cos[theta]*cos[psi] 
        //                   + cos[theta]*sin[psi] 
        //                   - sin[theta];

        // vectors[3*i + 1] *= sin[phi]*sin[theta]*cos[psi] - cos[phi]*sin[psi]
        //                   + sin[phi]*sin[theta]*sin[psi] + cos[phi]*cos[psi] 
        //                   + sin[phi]*cos[theta];

        // vectors[3*i + 2] *= cos[phi]*sin[theta]*cos[psi] + sin[phi]*sin[psi] 
        //                   + cos[phi]*sin[theta]*sin[psi] - sin[phi]*cos[psi] 
        //                   + cos[phi]*cos[theta];


    #pragma max_concurrency 1
    for(uint3 i = 0; i < 3; i++)
    {   
        vectors_copy[i*3    ] = 0.0;
        vectors_copy[i*3 + 1] = 0.0;
        vectors_copy[i*3 + 2] = 0.0;
        #pragma max_concurrency 1
        for(uint3 row = 0; row < 3; row++)
        {
            vectors_copy[i*3    ] += vectors[i*3    ] * rot_phi[0][row];
            vectors_copy[i*3 + 1] += vectors[i*3 + 1] * rot_phi[1][row];
            vectors_copy[i*3 + 2] += vectors[i*3 + 2] * rot_phi[2][row];
        }   
        vectors[i*3    ] = vectors_copy[i*3];
        vectors[i*3 + 1] = vectors_copy[i*3 + 1];
        vectors[i*3 + 2] = vectors_copy[i*3 + 2];
    }
    #pragma max_concurrency 1
    for(uint3 i = 0; i < 3; i++)
    {   
        vectors_copy[i*3    ] = 0.0;
        vectors_copy[i*3 + 1] = 0.0;
        vectors_copy[i*3 + 2] = 0.0;
        #pragma max_concurrency 1
        for(uint3 row = 0; row < 3; row++)
        {
            vectors_copy[i*3    ] += vectors[i*3    ] * rot_theta[0][row];
            vectors_copy[i*3 + 1] += vectors[i*3 + 1] * rot_theta[1][row];
            vectors_copy[i*3 + 2] += vectors[i*3 + 2] * rot_theta[2][row];
        }   
        vectors[i*3    ] = vectors_copy[i*3];
        vectors[i*3 + 1] = vectors_copy[i*3 + 1];
        vectors[i*3 + 2] = vectors_copy[i*3 + 2];
    }
    #pragma max_concurrency 1
    for(uint3 i = 0; i < 3; i++)
    {   
        vectors_copy[i*3    ] = 0.0;
        vectors_copy[i*3 + 1] = 0.0;
        vectors_copy[i*3 + 2] = 0.0;
        #pragma max_concurrency 1
        for(uint3 row = 0; row < 3; row++)
        {
            vectors_copy[i*3    ] += vectors[i*3    ] * rot_psi[0][row];
            vectors_copy[i*3 + 1] += vectors[i*3 + 1] * rot_psi[1][row];
            vectors_copy[i*3 + 2] += vectors[i*3 + 2] * rot_psi[2][row];
        }   
        vectors[i*3    ] = vectors_copy[i*3];
        vectors[i*3 + 1] = vectors_copy[i*3 + 1];
        vectors[i*3 + 2] = vectors_copy[i*3 + 2];
    }

    #pragma unroll 1
    for(uint3 i = 0; i < 3; i++)
    {
        vectors[3*i]     += var_memory[6];
        vectors[3*i + 1] += var_memory[7];
        vectors[3*i + 2] += var_memory[8];
    }

    fixed_16_9_t b[3];
    fixed_16_9_t c[3];
    fixed_16_9_t normal[3];

    #pragma unroll 1
    for(uint3 i = 0; i < 3; i++) // tree edges.
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


