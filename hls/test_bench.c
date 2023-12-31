#include "test_bench.h"

float cross_product(vec_3d* v1, vec_3d* v2)
{
    float norm = sqrt(v1->x * v1->x + v1->y * v1->y + v2->x * v2->x + v2->y * v2->y);
    float cp = (v1->x/norm) * (v2->y/norm) - (v1->y/norm) * (v2->x/norm);
    return cp/norm;
}

void rotate_3d(vec_3d* p0, vec_3d* p, float Phi, float Theta, float Psi)
{
    if(Phi == 0.0 and Theta == 0.0 and Psi == 0.0){ // If all 3 angles are 0 do nothing.
        return;
    }
    vec_3d new_vec_rotate;

    /*                    | 1     0          0     |
    Rx(Phi) = [x, y, z] * | 0  cos(Phi)  -sin(Phi) |
                          | 0  sin(Phi)   cos(Phi) |*/
    if(Phi != 0.0)
    {
        // Easy to read matrix multiplication with rotation around x axis.
        new_vec_rotate.x = (p->x - p0->x)*1 + (p->y - p0->y)*0           + (p->z - p0->z)*0           + p0->x;
        new_vec_rotate.y = (p->x - p0->x)*0 + (p->y - p0->y)*cos(Phi)    + (p->z - p0->z)*(-sin(Phi)) + p0->y;
        new_vec_rotate.z = (p->x - p0->x)*0 + (p->y - p0->y)*sin(Phi)    + (p->z - p0->z)*cos(Phi)    + p0->z;

        p->x = new_vec_rotate.x;
        p->y = new_vec_rotate.y;
        p->z = new_vec_rotate.z;
    }
    
    /*                      | cos(Theta)   0  sin(Theta) |
    Ry(Theta) = [x, y, z] * |     0        1      0      |
                            | -sin(Theta)  0  cos(Theta) |*/
    if(Theta != 0.0)
    {
        // Easy to read matrix multiplication with rotation around y axis.
        new_vec_rotate.x = (p->x - p0->x)*cos(Theta)    + (p->y - p0->y)*0 + (p->z - p0->z)*sin(Theta) + p0->x;
        new_vec_rotate.y = (p->x - p0->x)*0             + (p->y - p0->y)*1 + (p->z - p0->z)*0          + p0->y;
        new_vec_rotate.z = (p->x - p0->x)*(-sin(Theta)) + (p->y - p0->y)*0 + (p->z - p0->z)*cos(Theta) + p0->z;
        
        p->x = new_vec_rotate.x;
        p->y = new_vec_rotate.y;
        p->z = new_vec_rotate.z;
    }
    
    /*                   | cos(Psi)  -sin(Psi)  0 |
    Rz(Psi) =[x, y, z] * | sin(Psi)   cos(Psi)  0 |
                         |    0          0      1 |*/
    if(Psi != 0.0)
    {
        // Easy to read matrix multiplication with rotation around z axis.
        new_vec_rotate.x = (p->x - p0->x)*cos(Psi)    + (p->y - p0->y)*sin(Psi) + (p->z - p0->z)*0 + p0->x;
        new_vec_rotate.y = (p->x - p0->x)*(-sin(Psi)) + (p->y - p0->y)*cos(Psi) + (p->z - p0->z)*0 + p0->y;
        new_vec_rotate.z = (p->x - p0->x)*0           + (p->y - p0->y)*0        + (p->z - p0->z)*1 + p0->z;

        p->x = new_vec_rotate.x;
        p->y = new_vec_rotate.y;
        p->z = new_vec_rotate.z;
    }
}

/*void rotate_3d_compound(vec_3d* p0, vec_3d* p, float Phi, float Theta, float Psi)
{
	if(Phi == 0.0){
		if( Theta == 0.0){
			if( Psi == 0.0){
				return;
			}
		}
	}

    vec_3d new_vec_rotate;

    new_vec_rotate.x = (p->x - p0->x)*(cos(Theta) * cos(Psi)) + (p->y - p0->y)*(sin(Phi) * sin(Theta) * cos(Psi) - cos(Phi) * sin(Psi)) + (p->z - p0->z)*(cos(Phi) * sin(Theta) * cos(Psi) + sin(Phi) * sin(Psi)) + p0->x;
    new_vec_rotate.y = (p->x - p0->x)*(cos(Theta) * sin(Psi)) + (p->y - p0->y)*(sin(Phi) * sin(Theta) * sin(Psi) + cos(Phi) * cos(Psi)) + (p->z - p0->z)*(cos(Phi) * sin(Theta) * sin(Psi) - sin(Phi) * cos(Psi)) + p0->y;
    new_vec_rotate.z = (p->x - p0->x)*(-sin(Theta)) +           (p->y - p0->y)*(sin(Phi) * cos(Theta)) +                                  (p->z - p0->z)*(cos(Phi) * cos(Theta)) + p0->z;

    p->x = new_vec_rotate.x;
    p->y = new_vec_rotate.y;
    p->z = new_vec_rotate.z;

    
    x'(Phi)   = x(cos(Theta) * cos(Psi)) + y(sin(Phi) * sin(Theta) * cos(Psi) - cos(Phi) * sin(Psi)) + z(cos(Phi) * sin(Theta) * cos(Psi) + sin(Phi) * sin(Psi))
	y'(Theta) = x(cos(Theta) * sin(Psi)) + y(sin(Phi) * sin(Theta) * sin(Psi) + cos(Phi) * cos(Psi)) + z(cos(Phi) * sin(Theta) * sin(Psi) - sin(Phi) * cos(Phi))
	z'(Psi)   = x(-sin(Theta)) + y(sin(Phi) * cos(Theta)) + z( cos(Phi) * cos(Theta))

//}
*/

void rotate_triangle(vec_3d* p0, triangle* t, float Phi, float Theta, float Psi)
{
    rotate_3d(p0, &t->tri[0], Phi, Theta, Psi);
    rotate_3d(p0, &t->tri[1], Phi, Theta, Psi);
    rotate_3d(p0, &t->tri[2], Phi, Theta, Psi);
}

void rotate_cube(mesh_cube* mc, float Phi, float Theta, float Psi, vec_3d* reference)
{
    for(int i = 0; i < 12; i++)
    {
        rotate_triangle(reference, &mc->tris[i], Phi, Theta, Psi);
    }
}

void vector_multiply_4(vec_3d* i, float* e_i, vec_3d* o, float* e_o, matrix_4x4* pm)
{
    o->x = i->x * pm->matrix[0][0] + i->y * pm->matrix[1][0] + i->z * pm->matrix[2][0] + (*e_i) * pm->matrix[3][0];
    o->y = i->x * pm->matrix[0][1] + i->y * pm->matrix[1][1] + i->z * pm->matrix[2][1] + (*e_i) * pm->matrix[3][1];
    o->z = i->x * pm->matrix[0][2] + i->y * pm->matrix[1][2] + i->z * pm->matrix[2][2] + (*e_i) * pm->matrix[3][2];
    *e_o = i->x * pm->matrix[0][3] + i->y * pm->matrix[1][3] + i->z * pm->matrix[2][3] + (*e_i) * pm->matrix[3][3];
}

void ini_projection_4(int w, int h, float Theta, float Zf, float Zn, matrix_4x4* pm)
{
    pm->matrix[0][0] = (h/w)*(1/tan(Theta/2)); pm->matrix[0][1] = 0.0;            pm->matrix[0][2] = 0.0;               pm->matrix[0][3] = 0.0;
    pm->matrix[1][0] = 0.0;                    pm->matrix[1][1] = 1/tan(Theta/2); pm->matrix[1][2] = 0.0;               pm->matrix[1][3] = 0.0;
    pm->matrix[2][0] = 0.0;                    pm->matrix[2][1] = 0.0;            pm->matrix[2][2] = (Zf/(Zf-Zn));      pm->matrix[2][3] = 1.0;
    pm->matrix[3][0] = 0.0;                    pm->matrix[3][1] = 0.0;            pm->matrix[3][2] = -((Zf*Zn)/(Zf-Zn)); pm->matrix[3][3] = 0.0;
}


void projection_3d_to_2d(float w, float h, float z, matrix_4x4* projection_matrix, vec_3d* v)
{
    vec_3d projected_matrix_out;
    //float Z_original = v->z;
    float matrix_extended_in  = 1.0;
    float matrix_extended_out = 0.0;

    //vector_multiply_3(v, &projected_matrix_out, projection_matrix);
    vector_multiply_4(v, &matrix_extended_in, &projected_matrix_out, &matrix_extended_out, projection_matrix);

    // extra: devide all elements with the last element. And transfer to vector memory.
    if (matrix_extended_out != 0.0)
    {
        v->x = projected_matrix_out.x/matrix_extended_out;
        v->y = projected_matrix_out.y/matrix_extended_out;
        v->z = projected_matrix_out.z/matrix_extended_out;
    }

    // scale from value between -1 and 1 to -w/2 and h/2
    v->x += 1.0;     v->y += 1.0;     v->z += 1.0;
    v->x *= 0.5 * w; v->y *= 0.5 * h; v->z *= 0.5 * z;
}

void projection_on_triangle(float w, float h, float z, matrix_4x4* projection_matrix, triangle* t)
{
    projection_3d_to_2d(w, h, z, projection_matrix, &t->tri[0]);
    projection_3d_to_2d(w, h, z, projection_matrix, &t->tri[1]);
    projection_3d_to_2d(w, h, z, projection_matrix, &t->tri[2]);
}

void projection_on_cube(float w, float h, float z, matrix_4x4* projection_matrix, mesh_cube* mc)
{
    for(int i = 0; i < 12; i++)
    {
        projection_on_triangle(w, h, z, projection_matrix, &mc->tris[i]);
    }
}

// Ax = Bx X Cx
// Ay = By X Cy
// Az = Bz X Cz
void cross_product_of_triangle(triangle* t)
{
    vec_3d b, c;
    // Två vektorer med ett av hörnet som referens punkt.
    b.x = t->tri[1].x - t->tri[0].x;
    b.y = t->tri[1].y - t->tri[0].y;
    b.z = t->tri[1].z - t->tri[0].z;

    c.x = t->tri[2].x - t->tri[0].x;
    c.y = t->tri[2].y - t->tri[0].y;
    c.z = t->tri[2].z - t->tri[0].z;
    
    // Mnemonic
    t->normal.x = b.y * c.z - b.z * c.y;
    t->normal.y = b.z * c.x - b.x * c.z;
    t->normal.z = b.x * c.y - b.y * c.x;
    
    // normalisering till unit vector (vektor med en magnitud som exakt är 1).
    float n = sqrt(t->normal.x * t->normal.x + t->normal.y * t->normal.y + t->normal.z * t->normal.z);
    //printf("%.6f\n", n);
    t->normal.x /= n;
    t->normal.y /= n;
    t->normal.z /= n;
}

void cross_product_on_all(mesh_cube * c)
{
    for(int i = 0; i < 12; i++)
    {
        cross_product_of_triangle(&c->tris[i]);
    }
}

void in_camera_vision(mesh_cube * c, vec_3d* camera)
{
    for(int i = 0; i < 12; i++)
    {
        c->tris[i].vissibility = c->tris[i].normal.x * c->tris[i].tri[0].x - camera->x +
                                 c->tris[i].normal.y * c->tris[i].tri[0].y - camera->y +
                                 c->tris[i].normal.z * c->tris[i].tri[0].z - camera->z;
    }
}

void move_cube_x(mesh_cube* c, float x)
{
    for(int i = 0; i < 12; i++)
    {
        c->tris[i].tri[0].x += x;
        c->tris[i].tri[1].x += x;
        c->tris[i].tri[2].x += x;
    }
}

void move_cube_y(mesh_cube* c, float y)
{
    for(int i = 0; i < 12; i++)
    {
        c->tris[i].tri[0].y += y;
        c->tris[i].tri[1].y += y;
        c->tris[i].tri[2].y += y;
    }
}

void move_cube_z(mesh_cube* c, float z)
{
    for(int i = 0; i < 12; i++)
    {
        c->tris[i].tri[0].z += z;
        c->tris[i].tri[1].z += z;
        c->tris[i].tri[2].z += z;
    }
}

void position_cube(mesh_cube* c)
{
    move_cube_x(c, c->pos.x);
    move_cube_y(c, c->pos.y);
    move_cube_z(c, c->pos.z);
}

int main()
{
    float x_pos = 10.0;
    float y_pos = 10.0;
    float z_pos = 10.0;
    float phi = 1.0;
    float theta = 0.0;
    float psi = 0.0;
    float x_ref = 0.0;
    float y_ref = 0.0;
    float z_ref = 0.0;

    fixed_16_9_t samples[sample_size] = {
                            0.0, 0.0, 0.0, // South 1
                            0.0, 1.0, 0.0,
                            1.0, 1.0, 0.0,

                            0.0, 0.0, 0.0, // South 2
                            1.0, 1.0, 0.0,
                            1.0, 0.0, 0.0,

                            1.0, 0.0, 0.0, // East 1
                            1.0, 1.0, 0.0,
                            1.0, 1.0, 1.0,

                            1.0, 0.0, 0.0, // East 2
                            1.0, 1.0, 1.0,
                            1.0, 0.0, 1.0,

                            1.0, 0.0, 1.0, // North 1
                            1.0, 1.0, 1.0,
                            0.0, 1.0, 1.0,

                            1.0, 0.0, 1.0, // North 2
                            0.0, 1.0, 1.0,
                            0.0, 0.0, 1.0,

                            0.0, 0.0, 1.0, // West 1
                            0.0, 1.0, 1.0,
                            0.0, 1.0, 0.0,

                            0.0, 0.0, 1.0, // West 2
                            0.0, 1.0, 0.0,
                            0.0, 0.0, 0.0,

                            0.0, 1.0, 0.0, // Top 1
                            0.0, 1.0, 1.0,
                            1.0, 1.0, 1.0,

                            0.0, 1.0, 0.0, // Top 2
                            1.0, 1.0, 1.0,
                            1.0, 1.0, 0.0,

                            1.0, 0.0, 1.0, // Bot 1
                            0.0, 0.0, 1.0,
                            0.0, 0.0, 0.0,

                            1.0, 0.0, 1.0, // Bot 2
                            0.0, 0.0, 0.0,
                            1.0, 0.0, 0.0,
                        };
    fixed_16_9_t input_vars[9] = {(fixed_16_9_t) x_pos, (fixed_16_9_t) y_pos, (fixed_16_9_t) z_pos,
                                  (fixed_16_9_t) phi, (fixed_16_9_t) theta, (fixed_16_9_t) psi,
                                  (fixed_16_9_t) x_ref, (fixed_16_9_t) y_ref, (fixed_16_9_t) z_ref};
    int8 output_data[sample_size + sample_size/9];

    // mm_master interface class instance
    input_memory_type  mm_mem_in (samples,     sizeof(fixed_16_9_t)*sample_size);
    var_memory_type    mm_mem_var(input_vars,  sizeof(fixed_16_9_t)*9);
    output_memory_type mm_mem_out(output_data, sizeof(int8)*(sample_size + sample_size/9));

///// Prepere sofware cube
    mesh_cube c1, c1_projected;

    // South //
    c1.tris[0].tri[0].x =  0.0; c1.tris[0].tri[0].y =  0.0; c1.tris[0].tri[0].z =  0.0;
    c1.tris[0].tri[1].x =  0.0; c1.tris[0].tri[1].y =  1.0; c1.tris[0].tri[1].z =  0.0;
    c1.tris[0].tri[2].x =  1.0; c1.tris[0].tri[2].y =  1.0; c1.tris[0].tri[2].z =  0.0;

    c1.tris[1].tri[0].x =  0.0; c1.tris[1].tri[0].y =  0.0; c1.tris[1].tri[0].z =  0.0;
    c1.tris[1].tri[1].x =  1.0; c1.tris[1].tri[1].y =  1.0; c1.tris[1].tri[1].z =  0.0;
    c1.tris[1].tri[2].x =  1.0; c1.tris[1].tri[2].y =  0.0; c1.tris[1].tri[2].z =  0.0;
    // South //

    // East //
    c1.tris[2].tri[0].x =  1.0; c1.tris[2].tri[0].y =  0.0; c1.tris[2].tri[0].z =  0.0;
    c1.tris[2].tri[1].x =  1.0; c1.tris[2].tri[1].y =  1.0; c1.tris[2].tri[1].z =  0.0;
    c1.tris[2].tri[2].x =  1.0; c1.tris[2].tri[2].y =  1.0; c1.tris[2].tri[2].z =  1.0;

    c1.tris[3].tri[0].x =  1.0; c1.tris[3].tri[0].y =  0.0; c1.tris[3].tri[0].z =  0.0;
    c1.tris[3].tri[1].x =  1.0; c1.tris[3].tri[1].y =  1.0; c1.tris[3].tri[1].z =  1.0;
    c1.tris[3].tri[2].x =  1.0; c1.tris[3].tri[2].y =  0.0; c1.tris[3].tri[2].z =  1.0;
    // East //

    // North //
    c1.tris[4].tri[0].x =  1.0; c1.tris[4].tri[0].y =  0.0; c1.tris[4].tri[0].z =  1.0;
    c1.tris[4].tri[1].x =  1.0; c1.tris[4].tri[1].y =  1.0; c1.tris[4].tri[1].z =  1.0;
    c1.tris[4].tri[2].x =  0.0; c1.tris[4].tri[2].y =  1.0; c1.tris[4].tri[2].z =  1.0;

    c1.tris[5].tri[0].x =  1.0; c1.tris[5].tri[0].y =  0.0; c1.tris[5].tri[0].z =  1.0;
    c1.tris[5].tri[1].x =  0.0; c1.tris[5].tri[1].y =  1.0; c1.tris[5].tri[1].z =  1.0;
    c1.tris[5].tri[2].x =  0.0; c1.tris[5].tri[2].y =  0.0; c1.tris[5].tri[2].z =  1.0;
    // North //

    // West //
    c1.tris[6].tri[0].x =  0.0; c1.tris[6].tri[0].y =  0.0; c1.tris[6].tri[0].z =  1.0;
    c1.tris[6].tri[1].x =  0.0; c1.tris[6].tri[1].y =  1.0; c1.tris[6].tri[1].z =  1.0;
    c1.tris[6].tri[2].x =  0.0; c1.tris[6].tri[2].y =  1.0; c1.tris[6].tri[2].z =  0.0;

    c1.tris[7].tri[0].x =  0.0; c1.tris[7].tri[0].y =  0.0; c1.tris[7].tri[0].z =  1.0;
    c1.tris[7].tri[1].x =  0.0; c1.tris[7].tri[1].y =  1.0; c1.tris[7].tri[1].z =  0.0;
    c1.tris[7].tri[2].x =  0.0; c1.tris[7].tri[2].y =  0.0; c1.tris[7].tri[2].z =  0.0;
    // West //

    // Top //
    c1.tris[8].tri[0].x =  0.0; c1.tris[8].tri[0].y =  1.0; c1.tris[8].tri[0].z =  0.0;
    c1.tris[8].tri[1].x =  0.0; c1.tris[8].tri[1].y =  1.0; c1.tris[8].tri[1].z =  1.0;
    c1.tris[8].tri[2].x =  1.0; c1.tris[8].tri[2].y =  1.0; c1.tris[8].tri[2].z =  1.0;

    c1.tris[9].tri[0].x =  0.0; c1.tris[9].tri[0].y =  1.0; c1.tris[9].tri[0].z =  0.0;
    c1.tris[9].tri[1].x =  1.0; c1.tris[9].tri[1].y =  1.0; c1.tris[9].tri[1].z =  1.0;
    c1.tris[9].tri[2].x =  1.0; c1.tris[9].tri[2].y =  1.0; c1.tris[9].tri[2].z =  0.0;
    // Top //

    // Bot //
    c1.tris[10].tri[0].x =  1.0; c1.tris[10].tri[0].y =  0.0; c1.tris[10].tri[0].z =  1.0;
    c1.tris[10].tri[1].x =  0.0; c1.tris[10].tri[1].y =  0.0; c1.tris[10].tri[1].z =  1.0;
    c1.tris[10].tri[2].x =  0.0; c1.tris[10].tri[2].y =  0.0; c1.tris[10].tri[2].z =  0.0;

    c1.tris[11].tri[0].x =  1.0; c1.tris[11].tri[0].y =  0.0; c1.tris[11].tri[0].z =  1.0;
    c1.tris[11].tri[1].x =  0.0; c1.tris[11].tri[1].y =  0.0; c1.tris[11].tri[1].z =  0.0;
    c1.tris[11].tri[2].x =  1.0; c1.tris[11].tri[2].y =  0.0; c1.tris[11].tri[2].z =  0.0;
    // Bot //

    // cube position.
    c1.pos.x = x_pos;  c1.pos.y  = y_pos; c1.pos.z = z_pos;

    vec_3d custom_reference;
    //custom_reference.x = c1.pos.x + 0.5; custom_reference.y = c1.pos.y + 0.5; custom_reference.z = c1.pos.z + 0.5;
    custom_reference.x = x_ref; custom_reference.y = y_ref; custom_reference.z = z_ref;

    int maxx  =  380; // (getmaxx()/2);
    int maxy  =  380;

    float Zfar  = (380/2); // 1000.0
    float Znear = 0.1;

    //matrix_3x3 projection_matrix;
    //ini_projection_3(maxx, maxy, M_PI/2, Zfar, Znear, &projection_matrix);
    matrix_4x4 projection_matrix;
    ini_projection_4(maxx, maxy, PI/2, Zfar, Znear, &projection_matrix);

    vec_3d camera;
    camera.x = 0.0; camera.y = 0.0; camera.z = 0.0;

    float t = 0.0;

    c1_projected = c1;
///// Prepere sofware cube done

    printf("Test start!\n");

    // Test set positions on vectors.

    position_cube(&c1_projected);
    rotate_cube(&c1_projected, phi*PI, theta*PI, psi*PI, &custom_reference);
    cross_product_on_all(&c1_projected);
    in_camera_vision(&c1_projected, &camera);


    gpu_polygon(0, mm_mem_in, mm_mem_var, mm_mem_out);
    

    int loop = 0;
    int errors = 0;
    float dif = 0.0;
    printf("comp => x : %d, y : %d, z : %d\n", mm_mem_out[0].to_int(), mm_mem_out[1].to_int(), mm_mem_out[2].to_int());
    printf("soft => x : %d, y : %d, z : %d\n", (int) c1_projected.tris[0].tri[0].x, (int) c1_projected.tris[0].tri[0].y, (int) c1_projected.tris[0].tri[0].z);

    printf("comp => x : %d, y : %d, z : %d\n", mm_mem_out[3].to_int(), mm_mem_out[4].to_int(), mm_mem_out[5].to_int());
    printf("soft => x : %d, y : %d, z : %d\n", (int) c1_projected.tris[0].tri[1].x, (int) c1_projected.tris[0].tri[1].y, (int) c1_projected.tris[0].tri[1].z);
    
    printf("comp => x : %d, y : %d, z : %d\n", mm_mem_out[6].to_int(), mm_mem_out[7].to_int(), mm_mem_out[8].to_int());
    printf("soft => x : %d, y : %d, z : %d\n", (int) c1_projected.tris[0].tri[2].x, (int) c1_projected.tris[0].tri[2].y, (int) c1_projected.tris[0].tri[2].z);
    
    printf("comp => v : %d\n", mm_mem_out[9].to_int());
    printf("soft => v : %.6f\n", c1_projected.tris[0].vissibility);

    
    /*
    for(int i = 0; i < sample_size && loop < 12;)
    {
        if(c1_projected.tris[loop].vissibility < 0.0)
        {
            // printf("%.6f | %.6f | %.6f\n", samples[i], sample_prossesed[i], c1_projected.tris[loop].tri[0].x);
            dif = sample_prossesed[i++] - c1_projected.tris[loop].tri[0].x;
            if(dif > 1.0 || dif < -1.0){
                errors++;
                }
            // printf("%.6f | %.6f | %.6f\n", samples[i], sample_prossesed[i], c1_projected.tris[loop].tri[0].y);
            dif = sample_prossesed[i++] != c1_projected.tris[loop].tri[0].y;
            if(dif > 1.0 || dif < -1.0){
                errors++;
                }
            dif = sample_prossesed[i++] != c1_projected.tris[loop].tri[0].z; 
            // printf("%.6f | %.6f | %.6f\n\n", samples[i], sample_prossesed[i], c1_projected.tris[loop].tri[0].z);
            if(dif > 1.0 || dif < -1.0){
                errors++;
                }

            dif = sample_prossesed[i++] != c1_projected.tris[loop].tri[1].x; 
            // printf("%.6f | %.6f | %.6f\n", samples[i], sample_prossesed[i], c1_projected.tris[loop].tri[1].x);
            if(dif > 1.0 || dif < -1.0){
                errors++;
                }
            dif = sample_prossesed[i++] != c1_projected.tris[loop].tri[1].y; 
            // printf("%.6f | %.6f | %.6f\n", samples[i], sample_prossesed[i], c1_projected.tris[loop].tri[1].y);
            if(dif > 1.0 || dif < -1.0){
                errors++;
                }
            dif = sample_prossesed[i++] != c1_projected.tris[loop].tri[1].z; 
            // printf("%.6f | %.6f | %.6f\n\n", samples[i], sample_prossesed[i], c1_projected.tris[loop].tri[1].z);
            if(dif > 1.0 || dif < -1.0){
                errors++;
                }

            dif = sample_prossesed[i++] != c1_projected.tris[loop].tri[2].x;
            // printf("%.6f | %.6f | %.6f\n", samples[i], sample_prossesed[i], c1_projected.tris[loop].tri[2].x);
            if(dif > 1.0 || dif < -1.0){
                errors++;
                }
            dif = sample_prossesed[i++] != c1_projected.tris[loop].tri[2].y; 
            // printf("%.6f | %.6f | %.6f\n", samples[i], sample_prossesed[i], c1_projected.tris[loop].tri[2].y);
            if(dif > 1.0 || dif < -1.0){
                errors++;
                }
            dif = sample_prossesed[i++] != c1_projected.tris[loop].tri[2].z; 
            // printf("%.6f | %.6f | %.6f\n\n", samples[i], sample_prossesed[i], c1_projected.tris[loop].tri[2].z);
            if(dif > 1.0 || dif < -1.0){
                errors++; 
                }
        }else{

        }
        loop += 1;
    }*/
    printf("Number of errors in 3d vector processes is : %d\n", errors);

    projection_on_cube(maxx, maxy, Zfar, &projection_matrix, &c1_projected);


    printf("Test complete!\n");
    return 0;
}
