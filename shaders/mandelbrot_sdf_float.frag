#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

layout(origin_upper_left) in vec4 gl_FragCoord;

layout( push_constant ) uniform constants
{
	vec2	min;
	vec2	max;
	int		max_iter;
	float	time;
}push_constants;

vec2	mulz(vec2 z1, vec2 z2)
{
	return (vec2(z1.x * z2.x - z1.y * z2.y,
					z1.x * z2.y + z1.x * z2.y));
}

float	mandelbrot_sdf(vec2 z1)
{
	float	zrs;
	float	zis;
	float	zir;
	vec2	c = z1;
	vec2	z = vec2(0, 0);
	vec2	dz = vec2(1, 0);

	float modz = 0;
	for (int i = 0; i < push_constants.max_iter; i++)
	{
		vec2 z_new = mulz(z, z) + c;
		vec2 dz_new = 2 * mulz(z, dz) + 1;
		z = z_new;
		dz = dz_new;
		modz = z.x * z.x + z.y * z.y;
		if (modz > 50.0)
			break;
	}
	return (modz * log(modz) / (dz.x * dz.x + dz.y * dz.y));
}

void	main()
{
	vec3	color;
	vec2	uv = vec2(gl_FragCoord.xy);
	float	coloradd = 0.203005;

	//vec2 min = vec2(0.278587, -0.012560);
	//vec2 max = vec2(0.285413, -0.007440);
	vec2 min = push_constants.min;
	vec2 max = push_constants.max;

	vec2 z = uv * (max - min) + min;
	vec2 c = mix(vec2(min.x, min.y), vec2(max.x, max.y), gl_FragCoord.xy
		/ vec2(1920, 1080));

	float dist = mandelbrot_sdf(c);
	vec3 col = vec3(
		cos(dist * 10), 
		sin(dist * 100), 
		log(sin(dist * 150) * 10) 
	);
	if (dist <= 0)
		col = vec3(0.0);
	col = clamp(col, 0.0, 1.0);
	color = vec3(col);
	/*
	else
	{
		col %= 30;
		if (col >= 15)
			col = 30 - col;
		float time = push_constants.time / 500;
		color = vec3(sin(col + time)
			, 1.0 / (col + sin(time / 10.0))
			, 1.0 / (col + cos(time)));
	}
	*/
	outColor = vec4(color, 1.0);
}
