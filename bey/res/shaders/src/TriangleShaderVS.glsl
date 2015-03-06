attribute vec3 a_position; // local pos
attribute vec4 a_color;

varying vec4 v_color;

void main()
{
	gl_Position = vec4(a_pos, 1.0);	
	v_color = a_color;
}