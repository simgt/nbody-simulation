float bind(float c, float a, float b) {
    if (c < a) c = a;
    if (c > b) c = b;
    else c = c;
    return c;
}

__kernel void newton (
		__global float2* position_in,
		__global float2* position_out,
		__global float2* velocity_in,
		__global float2* velocity_out,
		const uint count,
		const float dt
	) {

	uint i = get_global_id(0);
	if (i < count) {
		float2 p = position_in[i];
		float2 v = velocity_in[i];

		v.y -= 9.8 * dt;
		p += v * dt;

		if (p.x < -1 || p.x > 1) {
			p.x = bind(p.x, -1.0, 1.0);
			v.x = -v.x;
		}

		if (p.y < -1) {
			p.y = -1;
			v.y = -v.y;
			v.x *= 0.9;
			v.y *= 0.7;
		}

		position_out[i] = p;
		velocity_out[i] = v;
	}
}
