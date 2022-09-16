#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_gpu_shader_fp64 : enable

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

int		mandelbrot(dvec2 z)
{
	double	zrs;
	double	zis;
	double	zir;
	dvec2		c = z;

	for (int i = 0; i < push_constants.max_iter; i++)
	{
		zrs = c.x * c.x;
		zis = c.y * c.y;
		zir = c.x * c.y;
		if (zrs + zis > 4.0)
			return (i);
		c.x = zrs - zis + z.x;
		c.y = zir + zir + z.y;
	}
	return (push_constants.max_iter);
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
	dvec2 c = mix(dvec2(min.x, min.y), dvec2(max.x, max.y), gl_FragCoord.xy
		/ dvec2(1920, 1080));

	int col = mandelbrot(c);
	if (col == push_constants.max_iter)
		color = vec3(0.0, 0.0, 0.0);
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
	outColor = vec4(color, 1.0);
}
