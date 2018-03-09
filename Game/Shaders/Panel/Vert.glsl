#version 400
layout(location=0) in vec3 P;
layout(location=1) in vec2 S;
layout(location=2) in vec4 Q;

out mat4 Rotations;
out vec2 Size;
out vec3 Position;

//Quaternion to Rot Matrix
mat4 Quat2Rot(vec4 Q)
{
	//								     Q.w,Q.x,Q.y,Q.z
	//vec4 is column, does not row;  Vec(  x,  y,  z,  w) <-- Q.w,Q.x,Q.y,Q.z
	mat4 Rot;
	Q=Q.yzwx;
	//first column
	Rot[0][0] = 1. - 2. * Q.y *Q.y - 2. * Q.z*Q.z;
	Rot[1][0] = 2. * Q.x*Q.y - 2. * Q.z*Q.w;
	Rot[2][0] = 2. * Q.x*Q.z + 2. * Q.y*Q.w;
	Rot[3][0] = 0.;
	//second column
	Rot[0][1] = 2. * Q.x*Q.y + 2. * Q.z*Q.w;
	Rot[1][1] = 1. - 2. * Q.x*Q.x - 2. * Q.z*Q.z;
	Rot[2][1] = 2. * Q.y*Q.z - 2. * Q.x*Q.w;
	Rot[3][1] = 0.;
	//third column
	Rot[0][2] = 2. * Q.x*Q.z - 2. * Q.y*Q.w;
	Rot[1][2] = 2. * Q.y*Q.z + 2. * Q.x*Q.w;
	Rot[2][2] = 1. - 2. * Q.x*Q.x - 2. * Q.y*Q.y;
	Rot[3][2] = 0.;
	//last column
	Rot[0][3] = 0.0;Rot[1][3] = 0.0;Rot[2][3] = 0.0;Rot[3][3] = 1.0;
	return Rot;
}
void main(void)
{
	Rotations=Quat2Rot(Q);
	Position=P;
	Size=S;
};