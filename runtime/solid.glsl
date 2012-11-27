-- Vertex

// COLORS

float interpolate( float val, float y0, float x0, float y1, float x1 ) {
    return (val-x0)*(y1-y0)/(x1-x0) + y0;
}

float base( float val ) {
    if ( val <= -0.75 ) return 0;
    else if ( val <= -0.25 ) return interpolate( val, 0.0, -0.75, 1.0, -0.25 );
    else if ( val <= 0.25 ) return 1.0;
    else if ( val <= 0.75 ) return interpolate( val, 1.0, 0.25, 0.0, 0.75 );
    else return 0.0;
}

float red(float gray) {
    return base(gray - 0.5);
}
float green(float gray) {
    return base(gray);
}
float blue(float gray) {
    return base(gray + 0.5);
}

uniform vec2 viewport_size;
uniform float max_radius;
uniform vec4 camera;

in vec4 position;
in vec4 normal;

out vec2 frag_emit_pos;
out vec4 frag_color;
out float frag_radius;

void main() {
	gl_Position = vec4((position.xy - camera.xy) / camera.z, 0, 1);

  	frag_emit_pos = viewport_size / 2.0 + (gl_Position.xy * viewport_size / 2.0);
    frag_radius = clamp(position.w * max_radius, 1, max_radius); //point_radius;//
    float v = (clamp(length(normal), 0, 60) / 60) * 2 - 1;
    frag_color = vec4(red(v), green(v), blue(v), 1); //vec4(1,1,1,1); //
}

-- Fragment

in vec2 frag_emit_pos;
in vec4 frag_color;
in float frag_radius;

out vec4 out_color;

void main() {
	if (length(gl_FragCoord.xy - frag_emit_pos) > frag_radius)
		discard;
    
    out_color = frag_color;
}