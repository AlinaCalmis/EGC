#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;

void main()
{
	// TODO: compute world space vectors
	vec3 world_position = vec3 (Model * vec4(v_position, 1));
	vec3 world_normal = normalize( mat3(Model) * normalize(v_normal));

	vec3 L = normalize(world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(V - L);

	// TODO: define ambient light component
	float ambient_light = material_kd * 0.25;

	// TODO: compute diffuse light component
	float diffuse_light = material_kd * max(dot(world_normal, L), 0);

	// TODO: compute specular light component

	vec3 R = reflect(-L, world_normal);
	int prod = 0;
	if (diffuse_light > 0)
	{
		prod = 1;
	}

	float specular_light = material_ks * prod * pow( max( dot(V, R), 0), material_shininess);

	

	// TODO: compute light
	float light = specular_light + diffuse_light + ambient_light;

	// TODO: send color light output to fragment shader
	color = light * object_color;



	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
