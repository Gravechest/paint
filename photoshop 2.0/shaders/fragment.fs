#version 460 core

out vec4 FragColor;

in vec2 TextCrd;
in float ID;
in vec3 Data;

uniform sampler2D canvas;
uniform sampler2D font;

uniform vec3 colorSel;
uniform float camZoom;
uniform vec2 camOffset;
uniform int toolSel;
uniform int toolSize;

float mux(vec3 p){
	if(p.x < p.y){
		if(p.y < p.z){
			return p.z;
		}
		else{
			return p.y;
		}
	}
	else if(p.x < p.z){
		return p.z;
	}
	else{
		return p.x;
	}
}

float dot2( vec2 v ) { return dot(v,v); }

float sdSegment(vec2 p,vec2 a,vec2 b )
{
    vec2 pa = p-a, ba = b-a;
    float h = clamp(dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h );
}

float sdTrapezoid(vec2 p,float r1,float r2,float he )
{
    vec2 k1 = vec2(r2,he);
    vec2 k2 = vec2(r2-r1,2.0*he);
    p.x = abs(p.x);
    vec2 ca = vec2(p.x-min(p.x,(p.y<0.0)?r1:r2), abs(p.y)-he);
    vec2 cb = p - k1 + k2*clamp( dot(k1-p,k2)/dot2(k2), 0.0, 1.0 );
    float s = (cb.x<0.0 && ca.y<0.0) ? -1.0 : 1.0;
    return s*sqrt( min(dot2(ca),dot2(cb)) );
}

void canvasCol(){
	ivec2 crd = ivec2(TextCrd*camZoom+camOffset);
	if(crd.x < 0 || crd.x > 99 || crd.y < 0 || crd.y > 99){
		if(mod(crd.x,2)+mod(crd.y,2)==1){
			FragColor = mix(vec4(0.5,0.5,0.5,1.0),FragColor,FragColor);
		}
		else{
			FragColor = mix(vec4(0.3,0.3,0.3,1.0),FragColor,FragColor);
		}	

	}
	else{
		FragColor = mix(texelFetch(canvas,crd,0),FragColor,FragColor);
	}
}

void main(){
	switch(int(ID)){
	case 0:
		if(TextCrd.x < 0.01){
			if(TextCrd.y < 0.01){
				FragColor += vec4(1.0-TextCrd.x*100);
			}
			if(TextCrd.y > 0.99){
				FragColor += vec4(TextCrd.y-0.99)*100;
			}
			FragColor += vec4(1.0-TextCrd.x*100);
			canvasCol();
			return;
		}
		if(TextCrd.x > 0.99){
			if(TextCrd.y < 0.01){
				FragColor += vec4(1.0-TextCrd.y*100);
			}
			if(TextCrd.y > 0.99){
				FragColor += vec4(TextCrd.y-0.99)*100;
			}
			FragColor += vec4(TextCrd.x-0.99)*100;
			canvasCol();
			return;
		}
		if(TextCrd.y < 0.01){
			FragColor = vec4(1.0-TextCrd.y*100);
			canvasCol();
			return;
		}
		if(TextCrd.y > 0.99){
			FragColor = vec4(TextCrd.y-0.99)*100;
			canvasCol();
			return;
		}
		canvasCol();
		break;
	case 1:
		FragColor.rgb = colorSel;
		break;
	case 2:
		if(colorSel.r < TextCrd.x+0.01 && colorSel.r > TextCrd.x-0.01){
			FragColor.rgb = vec3(0.5);
		}
		else{
			FragColor.r = TextCrd.x;
		}
		break;
	case 3:
		if(colorSel.g < TextCrd.x+0.01 && colorSel.g > TextCrd.x-0.01){
			FragColor.rgb = vec3(0.5);
		}
		else{
			FragColor.g = TextCrd.x;
		}
		break;
	case 4:
		if(colorSel.b < TextCrd.x+0.01 && colorSel.b > TextCrd.x-0.01){
			FragColor.rgb = vec3(0.5);
		}
		else{
			FragColor.b = TextCrd.x;
		}
		break;
	case 5:
		if(distance(TextCrd,vec2(0.5))<0.5){
			FragColor.rgb = vec3(1.0);
			return;
		}
		discard;
	case 6:
		FragColor = texture(font,TextCrd);
		if(FragColor.r > 0.7){
			discard;
		}	
		break;
	case 7:{
		vec2 tpos;
		float tx  = TextCrd.x * cos(0.785) - TextCrd.y * sin(0.785);
   		tpos.y = TextCrd.x * sin(0.785) + TextCrd.y * cos(0.785);
    	tpos.x = tx;
	    if((tpos.x > -0.1 && tpos.x < 0.1 && tpos.y < 1.2 && tpos.y > 0.2) 
	    || (tpos.y >  0.6 && tpos.y < 0.8 && tpos.x < 0.5 && tpos.x > -0.5)){
	        FragColor.rgb = vec3(1.0);
	    }
		break;
	}
	case 8:{
		FragColor.rgb = Data;
		break;
	}
	case 9:{
		if((TextCrd.x < 0.05 || TextCrd.x > 0.95 || TextCrd.y < 0.05 || TextCrd.y > 0.95) && toolSel == 0){
			FragColor.rgb = vec3(1.0);
			return;
		}
		if(sdSegment(TextCrd,vec2(0.2,0.3),vec2(0.8,0.7))<0.1){
			FragColor.rgb = vec3(1.0);
		}
		break;
	}
	case 10:
		if((TextCrd.x < 0.05 || TextCrd.x > 0.95 || TextCrd.y < 0.05 || TextCrd.y > 0.95) && toolSel == 1){
			FragColor.rgb = vec3(1.0);
			return;
		}
		if(distance(TextCrd,vec2(0.5))<0.25){
			FragColor.rgb = vec3(1.0);
		}
		break;
	case 11:
		if((TextCrd.x < 0.05 || TextCrd.x > 0.95 || TextCrd.y < 0.05 || TextCrd.y > 0.95) && toolSel == 2){
			FragColor.rgb = vec3(1.0);
			return;
		}
		if(sdTrapezoid(TextCrd-vec2(0.5,0.45),0.2,0.26,0.2)<0.0){
	     	FragColor.rgb = vec3(1.0);
	    }
	    if(distance(TextCrd*vec2(1.0,3.0),vec2(0.5,0.75))<0.2){
	    		FragColor.rgb = vec3(1.0);
	    }
	    if(distance(TextCrd*vec2(1.0,3.0),vec2(0.5,1.95))<0.26){
	    		FragColor.rgb = vec3(1.0);
	    }
	    if(distance(TextCrd*vec2(1.0,3.0),vec2(0.5,1.95))<0.2){
	    		FragColor.rgb = vec3(0.0);
	    }
		break;
	case 12:
		if((TextCrd.x < 0.05 || TextCrd.x > 0.95 || TextCrd.y < 0.05 || TextCrd.y > 0.95) && toolSel == 3){
			FragColor.rgb = vec3(1.0);
			return;
		}
		if(TextCrd.x < 0.7 && TextCrd.x > 0.3 && TextCrd.y < 0.7 && TextCrd.y > 0.3){
			FragColor.rgb = vec3(1.0);
		}
		break;
	case 13:
		if(float(toolSize)/12.0 < TextCrd.x+0.01 && float(toolSize)/12.0 > TextCrd.x-0.01){
			FragColor.rgb = vec3(0.5);
		}
		else if(TextCrd.y < 0.5+TextCrd.x*0.5 && TextCrd.y > 0.5-TextCrd.x*0.5){
			FragColor.rgb = vec3(1.0);
		}
		break;
	}
}







