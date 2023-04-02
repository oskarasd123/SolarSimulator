#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 fUV;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Transform;

void main()
{
   fUV = aUV;
   gl_Position = u_Projection * u_View * u_Transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

#shader fragment
#version 450 core
out vec4 FragColor;

in vec2 fUV;

uniform sampler2D u_Texture;

void main()
{
   FragColor = texture(u_Texture, fUV);
}