#version 330 core

in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

uniform vec4 color;
uniform vec4 region;

void main()
{
    TexCoords = vec2(region.x + vertex.z * (region.y - region.x),
                     region.z + vertex.w * (region.w - region.z));
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
