float bound(float c, float a, float b) {
    if (c < a) c = a;
    if (c > b) c = b;
    else c = c;
    return c;
}

__kernel void position (
		__global float2* position,
		__global float2* velocity,
		const uint count,
		const float dt
	) {

	uint i = get_global_id(0);
	if (i < count) {
		float2 p = position[i];
		float2 v = velocity[i];

		v.y -= 9.8 * dt;
		p += v * dt;

		if (p.x < -1 || p.x > 1) {
			p.x = bound(p.x, -1.0, 1.0);
			v.x = -v.x;
		}

		if (p.y < -1) {
			p.y = -1;
			v.y = -v.y;
			v.x /= 1.1;
			v.y /= 1.1;
		}

		position[i] = p;
		velocity[i] = v;
	}
}
