#version 150

uniform mat3 ciNormalMatrix;
uniform mat4	ciModelViewProjection;
in vec4			ciPosition;
in vec3	ciNormal;

out vec3 normal;
void main(void) {
	gl_Position = ciModelViewProjection * ciPosition;
	//normal = normalize(ciNormalMatrix * ciNormal);
	normal = normalize(ciNormal);
}