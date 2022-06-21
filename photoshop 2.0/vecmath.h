#pragma once
#include <math.h>
typedef struct{
	float x;
	float y;
}VEC2;

typedef struct{
	unsigned int x;
	unsigned int y;
}IVEC2;

typedef struct{
	float x;
	float y;
	float z;
}VEC3;

void vec2div(VEC2 *p,float d){
	p->x /= d;
	p->y /= d;
}

inline VEC2 vec2divR(VEC2 p,float d){
	p.x /= d;
	p.y /= d;
	return p;
}

inline VEC2 vec2mul(VEC2 p,float d){
	p.x *= d;
	p.y *= d;
	return p;
}

inline VEC2 vec2add(VEC2 p,float d){
	p.x += d;
	p.y += d;
	return p;
}

void vec2addvec2(VEC2 *p,VEC2 p2){
	p->x += p2.x;
	p->y += p2.y;
}

void vec2sub(VEC2 *p,float d){
	p->x -= d;
	p->y -= d;
}

void vec2subvec2(VEC2 *p,VEC2 p2){
	p->x -= p2.x;
	p->y -= p2.y;
}

inline VEC2 vec2subvec2R(VEC2 p,VEC2 p2){
	p.x -= p2.x;
	p.y -= p2.y;
	return p;
}

inline IVEC2 ivec2subivec2R(IVEC2 p,IVEC2 p2){
	p.x -= p2.x;
	p.y -= p2.y;
	return p;
}

inline VEC2 vec2normR(VEC2 p){
	return vec2divR(p,fmaxf(fabsf(p.x),fabsf(p.y)));
}

void vec2norm(VEC2 *p){
	vec2div(p,fmaxf(fabsf(p->x),fabsf(p->y)));
}

inline float vec2distance(VEC2 p,VEC2 p2){
	VEC2 p3 = vec2subvec2R(p,p2);
	return sqrtf(p3.x*p3.x+p3.y*p3.y);
}

inline float ivec2distance(IVEC2 p,IVEC2 p2){
	IVEC2 p3 = ivec2subivec2R(p,p2);
	return sqrtf(p3.x*p3.x+p3.y*p3.y);
}
