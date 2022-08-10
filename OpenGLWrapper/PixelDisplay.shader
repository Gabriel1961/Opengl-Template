#Vertex Shader
#version 330 core
layout(location = 0) in vec2 v_position;
layout(location = 1) in vec2 uvCoords;
uniform mat4 transform = mat4(1);
out vec2 v_uvCoords;
void main()
{
	v_uvCoords = uvCoords;
	gl_Position.xy = (vec4(v_position, 1, 1) * transform).xy;
}

#Fragment Shader
#version 330 core
out vec4 v_color;
in vec2 v_uvCoords;
uniform vec4 color;
uniform sampler2D samp;
void main()
{
	v_color = texture(samp, v_uvCoords);
}
