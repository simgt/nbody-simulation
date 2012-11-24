// Simple compute kernel which computes the square of an input array 

__kernel void position (
		__global float2* position,
		const uint count,
		const float dt
	) {                                      
	uint i = get_global_id(0);
	if (i < count) {
		float2 p = {0, 0};
		position[i].x += dt;
	}
}
