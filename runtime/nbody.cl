float bind(float c, float a, float b) {
    if (c < a) c = a;
    if (c > b) c = b;
    else c = c;
    return c;
}

typedef struct {
	float4 p;
	float4 v;
} Particle;

kernel void resolve (
		const float dt1,
		const float eps,
		global Particle* old,
		global Particle* new,
		local float4* pblock
	) {
	const float4 dt = (float4)(dt1, dt1, dt1, 0.0f);

	int gti = get_global_id(0);
	int ti = get_local_id(0);

	int n = get_global_size(0);
	int nt = get_local_size(0);
	int nb = n/nt;


	if (gti >= n) return;

	float4 p = old[gti].p;
	float4 v = old[gti].v;

	float4 a = (float4)(0.0, 0.0, 0.0, 0.0);

	for(int jb=0; jb < nb; jb++) { /* Foreach block ... */
		pblock[ti] = old[jb*nt+ti].p; /* Cache ONE particle position */
		barrier(CLK_LOCAL_MEM_FENCE); /* Wait for others in the work-group */

		for(int j=0; j<nt; j++) { /* For ALL cached particle positions ... */
			float4 p2 = pblock[j]; /* Read a cached particle position */
			float4 d = p2 - p;
			float invr = rsqrt(d.x*d.x + d.y*d.y + d.z*d.z + eps);
			float f = p2.w*invr*invr*invr;
			a += f*d; /* Accumulate acceleration */
		}

		barrier(CLK_LOCAL_MEM_FENCE); /* Wait for others in work-group */
	}

	if (gti != 0) { // particle 0 doesn't move
		p += dt*v + 0.5f*dt*dt*a;
		v += dt*a;
	}

	if (p.x < -1 || p.x > 1)
		v.x = -v.x;

	if (p.y < -1 || p.y > 1)
		v.y = -v.y;

	if (p.x < -1 || p.x > 1 || p.y < -1 || p.y > 1) {
		//v.x *= 0.5;
		//v.y *= 0.5;
	}

	p.x = bind(p.x, -1, 1);
	p.y = bind(p.y, -1, 1);
	
	new[gti].p = p;
	new[gti].v = v;
}