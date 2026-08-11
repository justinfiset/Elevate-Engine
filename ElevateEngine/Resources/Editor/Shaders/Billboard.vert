layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_TexCord;

uniform mat4 viewProj;
uniform mat4 view;
uniform vec3 worldPos;
uniform float scale;

out vec2 textCord;

void main()
{
    textCord = vec2(a_TexCord.x, 1.0 - a_TexCord.y);

    vec3 CameraRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 CameraUp    = vec3(view[0][1], view[1][1], view[2][1]);

    vec3 vertexWorldPos = worldPos 
        + CameraRight * a_Position.x * scale
        + CameraUp    * a_Position.y * scale;

    gl_Position = viewProj * vec4(vertexWorldPos, 1.0);
}