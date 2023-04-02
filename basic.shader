#shader vertex
#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

out vec2 fUV;

void main()
{
   fUV = aUV;
   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
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