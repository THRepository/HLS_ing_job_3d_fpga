#ifndef TEST_BENCH_H_
#define TEST_BENCH_H_

// Shared includes nad defines. Stopps multiples.
#ifndef PROJECT_SHARED_INC_DEF_FIR_FILTER_
#define PROJECT_SHARED_INC_DEF_FIR_FILTER_
    #include "HLS/hls.h"
#endif

#include "gpu_polygon.h"

#include <math.h>
#include <stdlib.h>

typedef struct vector_3d{
        float x;
        float y;
        float z;
    }vec_3d;

typedef struct polygon_3s{
    vec_3d tri[3];
    vec_3d normal;
    float vissibility;
}triangle;

typedef struct mesh_of_triangles_to_cube{
    triangle tris[12];
    vec_3d pos;
}mesh_cube;

typedef struct m_3x3{
    float matrix[3][3];
}matrix_3x3;

typedef struct m_4x4{
    float matrix[4][4];
}matrix_4x4;

#define PI 3.1415926
#define sample_size 108
#define num_triangles 12

float cross_product(vec_3d* v1, vec_3d* v2);
void rotate_3d(vec_3d* p0, vec_3d* p, float Phi, float Theta, float Psi);
void rotate_triangle(vec_3d* p0, triangle* t, float Phi, float Theta, float Psi);
void rotate_cube(mesh_cube* mc, float Phi, float Theta, float Psi, vec_3d* reference);
void vector_multiply_4(vec_3d* i, float* e_i, vec_3d* o, float* e_o, matrix_4x4* pm);
void ini_projection_4(int w, int h, float Theta, float Zf, float Zn, matrix_4x4* pm);
void projection_3d_to_2d(float w, float h, float z, matrix_4x4* projection_matrix, vec_3d* v);
void projection_on_triangle(float w, float h, float z, matrix_4x4* projection_matrix, triangle* t);
void projection_on_cube(float w, float h, float z, matrix_4x4* projection_matrix, mesh_cube* mc);
void cross_product_of_triangle(triangle* t);
void cross_product_on_all(mesh_cube * c);
void in_camera_vision(mesh_cube * c, vec_3d* camera);
void move_cube_x(mesh_cube* c, float x);
void move_cube_y(mesh_cube* c, float y);
void move_cube_z(mesh_cube* c, float z);
void position_cube(mesh_cube* c);
int main (void);

#endif