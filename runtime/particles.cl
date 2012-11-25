#define BALL_RADIUS ((20.0 / 600.0))

float bind(float c, float a, float b) {
    if (c < a) c = a;
    if (c > b) c = b;
    else c = c;
    return c;
}

__kernel void newton (
		__global float2* p_in,
		__global float2* p_out,
		__global float2* v_in,
		__global float2* v_out,
		const uint count,
		const float dt
	) {

	uint i = get_global_id(0);
	if (i < count) {
		float2 p = p_in[i];
		float2 v = v_in[i];

		for (uint j = 0; j < count; j++)
			if (j != i) {
				float2 c = p - p_in[j];
				float d = length(c);				
				if (d < 2 * BALL_RADIUS) {
					// minimum translation distance to push balls appart
					//float2 mtd = c * (float)(2.0 * BALL_RADIUS - d);
					
					//p += mtd / 2.0;

					// Get the components of the velocity vectors
					// which are parallel to the collision.
					// The perpendicular component remains the same
					// for both balls
					c /= d;
					float ui = dot(v, c);
					float uj = dot(v_in[j], c);

					// ignore if the particles are moving away from each-other
					if (ui > 0.0 && uj < 0.0) continue;

					// Replace the collision velocity components
					// with the new ones
					v += (uj - ui) * c * 0.9;
				}
			}
		
		v.y -= 9.8 * dt;

		p += v * dt;	

		if (p.x < -1 + BALL_RADIUS || p.x > 1 - BALL_RADIUS) {
			v.x = -v.x;
		}

		if (p.y < -1 + BALL_RADIUS || p.y > 1 - BALL_RADIUS) {
			v.y = -v.y;
			v.x *= 0.9;
			v.y *= 0.9;
		}

		p.x = bind(p.x, -1 + BALL_RADIUS, 1 - BALL_RADIUS);
		p.y = bind(p.y, -1 + BALL_RADIUS, 1 - BALL_RADIUS);	

		if (length(v) < 0.000001) {
			v.x = 0;
			v.y = 0;
		}

		p_out[i] = p;
		v_out[i] = v;
	}
}
