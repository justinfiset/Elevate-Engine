layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

uniform mat4 model;
uniform mat4 viewProj;
uniform mat4 view;

out vec3 normal;
out vec2 textCord;
out vec3 fragPos;
out vec3 fragPosViewSpace;
out mat3 TBN;

void main()
{
    vec4 worldPosition = model * vec4(a_Position, 1.0);
    fragPos = vec3(worldPosition);
    
    fragPosViewSpace = vec3(view * worldPosition);
    
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    normal = normalMatrix * a_Normal; 
    textCord = a_TexCord;

    vec3 T = normalize(normalMatrix * a_Tangent);
    vec3 B = normalize(normalMatrix * a_Bitangent);
    vec3 N = normalize(normalMatrix * a_Normal);
    TBN = mat3(T, B, N);

    gl_Position = viewProj * worldPosition;
}