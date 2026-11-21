shader_type canvas_item;

uniform vec4 outline_color : source_color = vec4(0.231, 0.510, 0.965, 1.0);
uniform float pulse_speed : hint_range(0.0, 10.0) = 2.0;

void fragment() {
	vec4 base = texture(TEXTURE, UV);
	float pulse = 0.85 + 0.15 * sin(TIME * pulse_speed);
	COLOR = mix(base, outline_color * pulse, 0.12);
}

