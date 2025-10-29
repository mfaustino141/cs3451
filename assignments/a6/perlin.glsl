#define Terrain 1		/* Uncomment this macro for Step II */

struct Light 
{
    vec3 position;          /* light position */
    vec3 Ia;                /* ambient intensity */
    vec3 Id;                /* diffuse intensity */
    vec3 Is;                /* specular intensity */     
};

uniform vec3 ka;            /* object material ambient */
uniform vec3 kd;            /* object material diffuse */
uniform vec3 ks;            /* object material specular */
uniform float shininess;    /* object material shininess */

/////////////////////////////////////////////////////
//// Step 1 - Part 1: Hash function
//// In this function, you will create a function that takes in an xy coordinate and returns a 'random' 2d vector.
//// You are asked to implement your own version by combining different GLSL built-in functions to produce the result.
//// You are allowed to leverage the reference Hash() implementation online (but you need to put the source link in comments). 
//// We also provide a default implementation of Hash() to obtain the image results shown in the assignments. 
//// You implementation does not need to match the reference results.
/////////////////////////////////////////////////////

vec2 hash2(vec2 v)
{
	vec2 rand = vec2(0,0);
	
	/* Your implementation starts */

	// Provided defulat implementation
	// rand  = 52.5 * fract(v.yx * 0.31 + vec2(0.31, 0.113));
    // rand = -1.0 + 3.1 * fract(rand.x * rand.y * rand.yx);

	// Link for Hash() implementation: https://www.shadertoy.com/view/tdG3Rd
    float x = fract(sin(dot(v, vec2(12.9898, 78.233))) * 43758.5453);
    float y = fract(sin(dot(v, vec2(39.3468, 11.135))) * 96321.9754);
	
    return vec2(x, y) * 2.0 - 1.0;
	
	/* Your implementation ends */

	return rand;
}

/////////////////////////////////////////////////////
//// Step 1 - Part 2: Perlin Noise
//// In this function, you will implement the Perlin noise with a single octave.
//// The input is a 2D position p. We calculate the grid cell index i and fraction f. 
//// You will use i and f to compute the Perlin noise at point p and return it as noise.
/////////////////////////////////////////////////////

float perlin_noise(vec2 p) 
{
    float noise = 0.0;
	vec2 i = floor(p);
    vec2 f = fract(p);
	
	/* Your implementation starts */
    vec2 s = f * f * (3.0 - 2.0 * f);

	vec2 g00 = hash2(i);
    vec2 g10 = hash2(i + vec2(1.0, 0.0));
    vec2 g01 = hash2(i + vec2(0.0, 1.0));
    vec2 g11 = hash2(i + vec2(1.0, 1.0));

    vec2 f00 = f - vec2(0.0, 0.0);
    vec2 f10 = f - vec2(1.0, 0.0);
    vec2 f01 = f - vec2(0.0, 1.0);
    vec2 f11 = f - vec2(1.0, 1.0);

    float nx0 = mix(dot(g00, f00), dot(g10, f10), s.x);
    float nx1 = mix(dot(g01, f01), dot(g11, f11), s.x);
    noise = mix(nx0, nx1, s.y);
	/* Your implementation ends */
	
	return noise;
}

/////////////////////////////////////////////////////
//// Step 1 - Part 3: Octave synthesis
//// In this function, you will synthesize the noise octave by invoking the perlin_noise function, which should be implemented in the previous step. 
//// Given a point p and an octave number num, the task is to compute the Perlin noise octave by accumulating the contributions from each frequency level. 
//// At each level, the amplitude should be halved, while the frequency should be doubled. 
//// The octave number num must be greater than 0.
/////////////////////////////////////////////////////

float noise_octave(vec2 p, int num)
{
	float sum = 0;
	
	/* Your implementation starts */
	for(int i = 0; i < num; i++){
		sum += pow(2.0, -float(i)) * perlin_noise(pow(2.0, float(i)) * p);
	}
	/* Your implementation ends */
	
	return sum;
}

/////////////////////////////////////////////////////
//// Step 2 - Part 1: Calculate vertex height
//// Create a function that takes in a 2D point and returns its height using the noise_octave() funciton you have implemented.
//// There is no standard answer for this part. Think about what functions will create what shapes.
//// If you want steep mountains with flat tops, use a function like sqrt(noise_octave(v,num)). 
//// If you want jagged mountains, use a function like e^(noise_octave(v,num)).
//// You can also add functions on top of each other and change the frequency of the noise by multiplying v by some value other than 1.
//// In the starter code, we provide our default implementation for your reference.
/////////////////////////////////////////////////////

float height(vec2 v)
{
    float h = 0;
	
	/* Your implementation starts */
	
	// Provided default implementation
	// h = 0.75 * noise_octave(v, 10);
	// if(h<0) h *= .5;

	h = exp(noise_octave(v * 0.9, 10)) - 1.0; 
	if(h < 0) h *= 0.1;
	
	/* Your implementation ends */
	
	return h;
}

/////////////////////////////////////////////////////
//// Step 2 - Part 2: Compute normal for a given 2D point using its height specified by noise function
//// In this function, you are asked to create a function that takes in a 2D point and returns its normal
//// You need to compute the normal vector at p by find the points d to the left/right and d forward/backward, and then use a cross product to calculate the normal vector. 
//// Be sure to normalize the result after you calculate the cross product.
//// This function will be called in shading_terrain to calculate the normal vector to be used in the shading model.
/////////////////////////////////////////////////////

vec3 compute_normal(vec2 v, float d)
{	
	vec3 normal_vector = vec3(0,0,0);
	
	/* Your implementation starts */
    vec3 v1 = vec3(v.x + d, v.y, height(vec2(v.x + d, v.y)));
    vec3 v2 = vec3(v.x - d, v.y, height(vec2(v.x - d, v.y)));
    vec3 v3 = vec3(v.x, v.y + d, height(vec2(v.x, v.y + d)));
    vec3 v4 = vec3(v.x, v.y - d, height(vec2(v.x, v.y - d)));

    vec3 dx = v1 - v2;
    vec3 dy = v3 - v4;

    normal_vector = normalize(cross(dx, dy));
	/* Your implementation ends */
	
	return normal_vector;
}

/////////////////////////////////////////////////////
//// Step 2 - Part 3: Phong shading
//// In this function, you will implement the Phong shading model to be used to shade your mountain.
//// It is the standard version we have practiced in our previous assignments, and you are allowed to 
//// reuse the code you have implemented previously.
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////
//// Input variables for shading_phong
/////////////////////////////////////////////////////
//// light: the light struct
//// e: eye position
//// p: position of the point
//// s: light source position (you may also use light.position)
//// n: normal at the point
/////////////////////////////////////////////////////

vec4 shading_phong(Light light, vec3 e, vec3 p, vec3 s, vec3 n) 
{
	vec4 color=vec4(0.0,0.0,0.0,1.0);
	
    /* your implementation starts */
    vec3 l = normalize(s - p);
    float nDotl = max(dot(n, l), 0.0);
    vec3 v = normalize(e - p);
    vec3 r = reflect(-l, n);
    float vDotr = max(dot(v, r), 0.0);

    vec3 LPhong = (ka * light.Ia) + (kd * light.Id * nDotl) + (ks * light.Is * pow(vDotr, shininess));
	color = vec4(LPhong, 1.f);
	/* your implementation ends */
	
	return color;
}

//// shade the noise function 
vec3 shading_noise(vec3 p) 
{
	float h = 0.5 + 0.5 * (noise_octave(p.xy, 4));
	return vec3(h, h, h);
}

/////////////////////////////////////////////////////
//// Step 2 - Part 4: Shade the terrain
//// In this function, you will calculate the emissive color of each input position
//// We provide a default implementation that is commented out by default
//// You are asked to implement your own version to calculate natural colors for your customized scene
/////////////////////////////////////////////////////

vec3 shading_terrain(vec3 pos) 
{
	const Light light = Light(vec3(3, 1, 3), vec3(1, 1, 1), vec3(2, 2, 2), vec3(1, 1, 1));

	//// calculate Phong shading color with normal
	
	vec3 n = compute_normal(pos.xy, 0.01);
	vec3 e = position.xyz;
	vec3 p = pos.xyz;
	vec3 s = light.position;
	vec3 phong_color = shading_phong(light, e, p, s, n).xyz;

	//// calculate emissive color
	vec3 emissive_color = vec3(0.0,0.0,0.0);
	
	/* your implementation starts */
	
	// Provided default implementation
	// float h = pos.z + .8;
	// h = clamp(h, 0.0, 1.0);
	// emissive_color = mix(vec3(.4,.6,.2), vec3(.4,.3,.2), h);

    float h = clamp(pos.z, 0.0, 1.0);

    vec3 water_color = vec3(0.05, 0.1, 0.2);
    vec3 grass_color = vec3(0.15, 0.25, 0.1);
    vec3 rock_color = vec3(0.35, 0.28, 0.2);
    vec3 light_rock_color = vec3(0.45, 0.45, 0.4);
    vec3 snow_color = vec3(0.9, 0.9, 0.9);

    if (h < 0.03)
        emissive_color = water_color;
    else if (h < 0.04)
        emissive_color = mix(water_color, grass_color, smoothstep(0.03, 0.04, h));
    else if (h < 0.6)
        emissive_color = mix(grass_color, rock_color, smoothstep(0.04, 0.6, h));
    else if (h < 0.85)
        emissive_color = mix(rock_color, light_rock_color, smoothstep(0.6, 0.8, h));
    else
        emissive_color = mix(light_rock_color, snow_color, smoothstep(0.85, 1.0, h));
	/* your implementation ends */

	return phong_color * emissive_color;
}