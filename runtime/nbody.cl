typedef struct {
	float4 p;
	float4 v;
} Particle;

kernel void iterate (
		const float dt,
		const float eps,
		global Particle* old,
		global Particle* new,
		local float4* block
	) {
	int gid = get_global_id(0);
	int lid = get_local_id(0); // id inside the workgroup

	int gn = get_global_size(0); // total num of particles
	int ln = get_local_size(0); // workgroup size
	int num_blocks = gn / ln;


	if (gid >= gn) return; // don't process if out of bounds

	float4 p = old[gid].p;
	float4 v = old[gid].v;

	float4 a = (float4)(0.0f, 0.0f, 0.0f, 0.0f);

	for(int b = 0; b < num_blocks; b++) { // foreach block
		block[lid] = old[b * ln + lid].p; // cache one particle position
		barrier(CLK_LOCAL_MEM_FENCE); // wait for others in the workgroup

		for(int i = 0; i < ln; i++) { // foreach cached particle
			float4 q = block[i]; // recover the cached particle
			float4 d = q - p;
			float invr = rsqrt(d.x * d.x + d.y * d.y + d.z * d.z + eps);
			float f = q.w * invr * invr * invr;
			a += f * d; // accumulate acceleration
		}

		barrier(CLK_LOCAL_MEM_FENCE); // wait for others in the workgroup
	}

	//if (gid == 0) return; // particle 0 doesn't move

	const float4 t = (float4)(dt, dt, dt, 0.0f);
	p += t * v + 0.5f * t * t * a;
	v += t * a;
	
	new[gid].p = p;
	new[gid].v = v;
}