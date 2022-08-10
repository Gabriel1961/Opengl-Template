#Vertex Shader
#version 330 core
layout(location = 0) in vec2 v_position;
void main()
{
	gl_Position.xy = v_position;
}

#Fragment Shader
#version 330 core
out vec4 v_color;
uniform vec4 COLOR;
void main()
{
	v_color = COLOR;
}