#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

layout(origin_upper_left) in vec4 gl_FragCoord;

layout( push_constant ) uniform constants
{
	dvec2	min;
	dvec2	max;
	int		max_iter;
	float	time;
}push_constants;

dvec2	mulz(dvec2 z1, dvec2 z2)
{
	return (dvec2(z1.x * z2.x - z1.y * z2.y,
					z1.x * z2.y + z1.x * z2.y));
}

int nb_iter;

double mandelbrot_sdf(dvec2 z1)
{
	dvec2	c = z1;
	dvec2	z = dvec2(0, 0);
	dvec2	dz = dvec2(1, 0);

	double modz = 0;
	for (nb_iter = 0; nb_iter < push_constants.max_iter; nb_iter++)
	{
		dvec2 z_new = mulz(z, z) + c;
		dvec2 dz_new = 2 * mulz(z, dz) + 1;
		z = z_new;
		dz = dz_new;
		modz = z.x * z.x + z.y * z.y;
		if (modz > 50.0)
			break;
	}
	return (modz * log(float(modz)) / (dz.x * dz.x + dz.y * dz.y));
}

void	main()
{
	vec3	color;
	vec2	uv = vec2(gl_FragCoord.xy);
	float	coloradd = 0.203005;

	//vec2 min = vec2(0.278587, -0.012560);
	//vec2 max = vec2(0.285413, -0.007440);
	dvec2 min = push_constants.min;
	dvec2 max = push_constants.max;

	dvec2 z = uv * (max - min) + min;
	dvec2 c = mix(dvec2(min.x, min.y), dvec2(max.x, max.y), dvec2(gl_FragCoord.xy)
		/ dvec2(1920, 1080));

	float time = push_constants.time / 500;
	double dist = mandelbrot_sdf(c);
	dvec3 col = vec3(
		dist * 100000000.0, 
		sinh(float(1.0 - dist * 10000)), 
		dist * 10000.0
	);
	if (nb_iter >= push_constants.max_iter)
		col = vec3(0.0);
	col = clamp(col, 0.0, 1.0);
	color = vec3(vec3(col));
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
