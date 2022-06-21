#version 460 core
layout (location = 0) in vec2 verticles;
layout (location = 1) in vec2 textCrd;
layout (location = 2) in float id;
layout (location = 3) in vec3 data;

out vec2 TextCrd;
out float ID;
out vec3 Data;

void main(){
	ID = id;
	TextCrd = textCrd;
	Data = data;
	gl_Position = vec4(vec2(verticles),0.0,1.0);
}
