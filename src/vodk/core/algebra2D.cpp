
#include "vodk/core/algebra2D.hpp"
#include <assert.h>
#include <stdio.h>

namespace vodk {

float normalized_angle(float a) {
	return vodk::modulo(a + PI, 2.0*PI) - PI;
}

float angular_distance(float a, float b) {
    float d = normalized_angle(b - a);
    d += (d > PI) ? -PI : (d < -PI) ? 2.0*PI : 0.0;
    return d;
}

void print(const Mat3& m) {
	printf(
		" | %f %f %f |\n | %f %f %f |\n | %f %f %f |\n",
		m._00, m._01, m._02,
		m._10, m._11, m._12,
		m._20, m._21, m._22
	);
}

void print(const Vec2& v) {
	printf("[%f %f]", v.x, v.y);
}

void print(const Vec3& v){
	printf("[%f %f]", v.x, v.y, v.z);
}

namespace unittest {



static float epsilon = 0.0001;

bool eq(float a, float b) {
	return a < b + epsilon && a > b - epsilon;
}

bool eq(const Vec2& a, const Vec2& b) {
	return eq(a.x, b.x) && eq(a.y, b.y);
}

bool eq(const Mat3& a, const Mat3& b) {
	return eq(a._00, b._00) && eq(a._01, b._01) && eq(a._02, b._02)
	    && eq(a._10, b._10) && eq(a._11, b._11) && eq(a._12, b._12)
	    && eq(a._20, b._20) && eq(a._21, b._21) && eq(a._22, b._22);
}

#define log_val(x) printf("%s = %f\n", #x, x);
#define assert_float_eq(x, y) if(!eq(x,y)) { \
printf("assertion (%s == %s) -> (%f == %f) failed\n", #x, #y, x, y);\
assert(false);\
}\


void mat3() {
	Vec2 v0 = Vec2(0.0, 0.0);
	Vec2 v1 = Vec2(1.0, 0.0);
	Vec2 v2 = Vec2(0.0, 1.0);
	Mat3 identity = mat3::identity();
	Mat3 translate_1_1 = mat3::translate(Vec2(1.0,1.0));
	Mat3 translate_2_2 = mat3::translate(Vec2(2.0,2.0));
	Mat3 foo = Mat3(
		0.1, 1.5, 0.2,
		1.7, 0.3, 2.4,
		0.1, 2.4, 1.2
	);
	Mat3 rotate_90 = mat3::rotate(PI/2.0);

	assert_float_eq(modulo(-0.1,1.0), 0.9);
	assert_float_eq(modulo(0.1,-1.0), -0.9);
	assert_float_eq(modulo(-0.1,-1.0), -0.1);
	assert_float_eq(modulo(0.1, 1.0), 0.1);
	assert_float_eq(modulo(10.1, 1.0), 0.1);

	assert_float_eq(angular_distance(0.1, 0.2), 0.1);
	assert_float_eq(angular_distance(0.2, 0.1), -0.1);
	assert_float_eq(angular_distance(0.2, -0.1), -0.3);
	assert_float_eq(angular_distance(-0.1, 0.2), 0.3);
	assert_float_eq(angular_distance(-0.1,PI+0.2), -PI+0.3);

	assert(eq(identity*identity, identity));
	assert(eq(v0, identity * v0));
	assert(eq(v1, identity * v1));
	assert(eq(v2, identity * v2));
	assert(eq(foo, identity * foo));
	assert(eq(foo, mat3::transposed(mat3::transposed(foo))));
	assert(eq(translate_2_2, translate_1_1*translate_1_1));
	assert(eq(rotate_90 * v1, v2));

	assert(eq(mat3::translate(Vec2(1.0,1.0)) * Vec2(1.0,1.0), Vec2(2.0,2.0)));
	assert(eq(mat3::translate(Vec2(0.0,0.0)) * Vec2(1.0,1.0), Vec2(1.0,1.0)));
}

} // unittest
} // vodk