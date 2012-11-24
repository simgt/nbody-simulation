// Simple compute kernel which computes the square of an input array 

__kernel void position (
		__global float2* position,
		__global float2* velocity,
		const uint count,
		const float dt
	) {

	uint i = get_global_id(0);
	if (i < count) {
		position[i] += velocity[i] * dt;

		if (position[i].x < -1 || position[i].x > 1
		 || position[i].y < -1 || position[i].y > 1) {
			position[i].x = 0;
			position[i].y = 0;
		}
	}
}
