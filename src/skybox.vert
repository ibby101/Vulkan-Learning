#version 450

layout(binding = 0) uniform UniformBufferObject {
	mat4 view;
	mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec3 fragTexCoord;

void main(){
	// removing translation from view matrix for skybox
	mat4 viewNoTranslation = mat4(mat3(ubo.view));

	vec4 pos = ubo.proj * viewNoTranslation * vec4(inPosition, 1.0);

	// setting z = w so that depth will be 1.0
	gl_Position = pos.xyww;

	// using position as texture coordinate for cubemap sampling
	fragTexCoord = inPosition;
}