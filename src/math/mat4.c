#include "mat4.h"

#include <inttypes.h>
#include <string.h>

#include <math.h>

/* Shame */
#define M_PI 3.14159265358979323846
#define M_TAU 6.283185307179586

static float to_radians(float degrees);

void mat4_init(struct mat4* matrix, float diagonal)
{
    memset(matrix->elements, 0, 16 * sizeof(float));

    matrix->elements[0 + 0 * 4] = diagonal;
    matrix->elements[1 + 1 * 4] = diagonal;
    matrix->elements[2 + 2 * 4] = diagonal;
    matrix->elements[3 + 3 * 4] = diagonal;
}

void mat4_identity(struct mat4* matrix)
{
    mat4_init(matrix, 1.0f);
}

void mat4_mul(struct mat4* left, const struct mat4* right)
{
    /* TODO: Vectorize (SIMD) */
    float data[16];
    for (uint32_t row = 0; row < 4; ++row)
    {
        for (uint32_t col = 0; col < 4; ++col)
        {
            float sum = 0.0f;
            for (uint32_t e = 0; e < 4; ++e)
            {
                sum += left->elements[e + row * 4] * right->elements[col + e * 4];
            }
            data[col + row * 4] = sum;
        }
    }
    memcpy(left->elements, data, sizeof(data));
}

struct mat4 mat4_orthographic(float left, float right, float bottom, float top, float near, float far)
{
    struct mat4 result;
    mat4_identity(&result);

    result.elements[0 + 0 * 4] = 2.0f / (right - left);
    result.elements[1 + 1 * 4] = 2.0f / (top - bottom);
    result.elements[2 + 2 * 4] = 2.0f / (near - far);

    result.elements[3 + 0 * 4] = (left + right) / (left - right);
    result.elements[3 + 1 * 4] = (bottom + top) / (bottom - top);
    result.elements[3 + 2 * 4] = (far + near) / (far - near);

    return result;
}

struct mat4 mat4_perspective(float fov, float aspect_ratio, float near, float far)
{
    struct mat4 result;
    mat4_identity(&result);

    const float q = 1.0f / tanf(to_radians(0.5f * fov));
    const float a = q / aspect_ratio;

    const float b = (near + far) / (near - far);
    const float c = (2.0f * near * far) / (near - far);

    result.elements[0 + 0 * 4] = a;
    result.elements[1 + 1 * 4] = q;
    result.elements[2 + 2 * 4] = b;
    result.elements[2 + 3 * 4] = -1.0f;
    result.elements[3 + 2 * 4] = c;

    return result;
}

struct mat4 mat4_look_at(struct vec3 camera, struct vec3 object, struct vec3 up)
{
    struct mat4 result;
    mat4_identity(&result);

    vec3_sub(&object, &camera);
    struct vec3 f = vec3_normalize(&object);
    vec3_normalized(&up);
    struct vec3 s = vec3_cross(&f, &up);
    struct vec3 u = vec3_cross(&s, &f);

    result.elements[0 + 0 * 4] = s.x;
    result.elements[0 + 1 * 4] = s.y;
    result.elements[0 + 2 * 4] = s.z;

    result.elements[1 + 0 * 4] = u.x;
    result.elements[1 + 1 * 4] = u.y;
    result.elements[1 + 2 * 4] = u.z;

    result.elements[2 + 0 * 4] = -f.x;
    result.elements[2 + 1 * 4] = -f.y;
    result.elements[2 + 2 * 4] = -f.z;

    camera.x *= -1;
    camera.y *= -1;
    camera.z *= -1;

    struct mat4 translation;
    mat4_identity(&translation);
    mat4_translation(&translation, &camera);
    mat4_mul(&result, &translation);
    return result;
}

void mat4_translation(struct mat4* matrix, const struct vec3* translation)
{
    struct mat4 result;
    mat4_identity(&result);

    result.elements[3 + 0 * 4] = translation->x;
    result.elements[3 + 1 * 4] = translation->y;
    result.elements[3 + 2 * 4] = translation->z;

    mat4_mul(matrix, &result);
}

void mat4_rotation(struct mat4* matrix, const struct vec3* angles)
{
    float angle = vec3_magnitude(angles);
    struct vec3 axis = vec3_normalize(angles);

    struct mat4 result;
    mat4_identity(&result);

    const float r = to_radians(angle);
    const float c = cosf(r);
    const float s = sinf(r);
    const float omc = 1.0f - c;

    const float x = axis.x;
    const float y = axis.y;
    const float z = axis.z;

    result.elements[0 + 0 * 4] = x * x * omc + c;
    result.elements[0 + 1 * 4] = y * x * omc + z * s;
    result.elements[0 + 2 * 4] = x * z * omc - y * s;

    result.elements[1 + 0 * 4] = x * y * omc - z * s;
    result.elements[1 + 1 * 4] = y * y * omc + c;
    result.elements[1 + 2 * 4] = y * z * omc + x * s;

    result.elements[2 + 0 * 4] = x * z * omc + y * s;
    result.elements[2 + 1 * 4] = y * z * omc - x * s;
    result.elements[2 + 2 * 4] = z * z * omc + c;

    mat4_mul(matrix, &result);
}

void mat4_scale(struct mat4* matrix, const struct vec3* scale)
{
    struct mat4 result;
    mat4_identity(&result);

    result.elements[0 + 0 * 4] = scale->x;
    result.elements[1 + 1 * 4] = scale->y;
    result.elements[2 + 2 * 4] = scale->z;
    
    mat4_mul(matrix, &result);
}

static float to_radians(float degrees)
{
    return degrees * (M_PI / 180.0f);
}