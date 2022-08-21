#shader vertext
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float bRender;

out vec2 v_TexCoord;
out float v_bRender;
void main()
{
	v_TexCoord = texCoord;
	gl_Position = position;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 v_TexCoord;
in float v_bRender;

uniform sampler2D u_Texture;

void main()
{
	// color = vec4(0.2, 0.3, 0.8, 1.0); // red
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
};
