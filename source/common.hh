#pragma once

#include <Eigen/Dense>

#define WINDOW_W 600
#define WINDOW_H 600
#define PARTICLES_COUNT 4096
#define PARTICLES_RADIUS 15

typedef unsigned char uchar;
typedef unsigned int uint;
typedef Eigen::Vector4f Vec4f;

struct Particle {
	Vec4f position;
	Vec4f velocity;
};