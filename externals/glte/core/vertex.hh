#pragma once

#include "common.hh"

namespace te {
	struct Vertex {
		Vec3f position;
		Vec3f normal;
		Vec4f color;
		Vec3f texcoord;
	};
}