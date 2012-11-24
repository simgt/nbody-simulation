-- Vertex

in vec2 position;
out vec4 frag_color;

void main() {
	gl_Position = vec4(position, 0, 1);
    
    frag_color = vec4(1, 1, 1, 1);
}

-- Fragment

in vec4 frag_color;
out vec4 out_color;

void main() {
    out_color = frag_color;
}