#version 400
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

mat4 Transform(vec3 Offset)
{
	//vec3 is column, does not row;  
	return mat4(
	vec4(1.0,0.0,0.0,0.0), 
	vec4(0.0,1.0,0.0,0.0), 
	vec4(0.0,0.0,1.0,0.0),
	vec4(Offset.x, Offset.y,Offset.z,1.0));
}
layout(shared , column_major ) uniform CameraBlock
{
	mat4 Proj;
	vec4 Quaternion;
	mat4 Pos;
}Camera;

uniform int Frame;
uniform int MaxFrame;

double GetOffset() 
{
	return  (1./MaxFrame)*Frame;
};
double GetStep()
{
	return 1./MaxFrame;
};
layout(points) in;

in vec2 Size[];
in mat4 Rotations[];
in vec3 Position[];

layout(triangle_strip , max_vertices=4) out;
out vec2 UV;
void main(void)
{
	mat4 CamRot=Quat2Rot(Camera.Quaternion);
	vec2 Half=Size[0].xy/2.; 
	UV=vec2(GetOffset(),1.0);
	gl_Position=
	Camera.Proj*CamRot*
	Camera.Pos*Transform(Position[0])*Rotations[0]*vec4(-Half.x,-Half.y,0.,1.);
	EmitVertex();
	UV=vec2(GetOffset(),0.0);
	gl_Position=
	Camera.Proj*CamRot*
	Camera.Pos*Transform(Position[0])*Rotations[0]*vec4(-Half.x,Half.y,0.,1.);
	EmitVertex();
	UV=vec2(GetOffset()+GetStep(), 1.0);
	gl_Position=
	Camera.Proj*CamRot*
	Camera.Pos*Transform(Position[0])*Rotations[0]*vec4(Half.x,-Half.y,0.,1.);
	EmitVertex();
	UV=vec2(GetOffset()+GetStep(), 0.0);;
	gl_Position=
	Camera.Proj*CamRot*
	Camera.Pos*Transform(Position[0])*Rotations[0]*vec4(Half.x,Half.y,0.,1.);
	EmitVertex();
	EndPrimitive();
}; 

