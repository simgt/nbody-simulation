-- Vertex

uniform vec2 viewport_size;

in vec2 position;

out vec2 frag_emit_pos;
out vec4 frag_color;

void main() {
	gl_Position = vec4(position, 0, 1);

  	frag_emit_pos = viewport_size / 2.0 + (position * viewport_size / 2.0);
    frag_color = vec4(1, 1, 1, 1);
}

-- Fragment

uniform float point_size;

in vec2 frag_emit_pos;
in vec4 frag_color;

out vec4 out_color;

void main() {
	if (length(gl_FragCoord.xy - frag_emit_pos) > point_size / 2.0)
		discard;
    
    out_color = frag_color;
}