#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float width;
uniform float height;

void main()
{
	vec4 color = texture2D(texture1, TexCoord);
	if (color.a < 0.15){
		FragColor = texture2D(texture0, TexCoord);
	}else{
		FragColor = color;
	}
	vec2 pp = TexCoord * vec2(width,height);
	if(pp.x > 0.0 && pp.x < 10.0 && pp.y > 0.0 && pp.y < 10.0){
		FragColor = vec4(1.0,0.0,0.0,1.0);
	}
}

