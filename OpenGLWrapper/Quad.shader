#Vertex Shader
#version 330 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv_coords;
uniform mat4 transform;
out vec2 v_uv_coords;
void main()
{
	v_uv_coords = uv_coords;
	gl_Position = (transform* vec4(position, 0, 1));
}

#Fragment Shader
#version 330 core
out vec4 v_color;
in vec2 v_uv_coords;
uniform vec4 color;
uniform int useTex = 0;
uniform sampler2D samp;
void main()
{
	if (useTex == 0)
		v_color = color;
	else
		v_color = texture(samp, v_uv_coords);
}
