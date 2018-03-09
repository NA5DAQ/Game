//Geometry Shader
#version 400
layout(points) in;
layout(triangle_strip , max_vertices=4) out;
out vec2 UV; //

in float ASCII[]; //
in float Order[]; //

uniform double Size; //Length of a snapshot that it is a symvol
uniform dvec2 Screen; //Screen resolution
uniform vec2 Bounds;
uniform dvec2 ScreenPos; //Anchor position
uniform dvec2 TextureSize; //Texture resolution
uniform double CountSymvols; //Count of syvols in texture
uniform int Depth;//Depth symvol \n
uniform int NewLine;// curent VABenum pos
double Ratio=17. / 7.;
double GetOffset() 
{
	return  trunc((TextureSize.x/CountSymvols)*(ASCII[0]-32))/TextureSize.x;
};
double GetStep()
{
	return (TextureSize.x/CountSymvols)/TextureSize.x;
};
dvec2 NormalyzeCoord(dvec2 Coord)
{
	return highp dvec2(2.*Coord.x-1.,(Coord.y*2.-1.));
};
dvec2 Screen2UV(dvec2 Coord)
{
	return dvec2(Coord.x/Screen.x,Coord.y/Screen.y);
};
mat3 Transform(dvec2 Offset)
{
	//vec3 is column, does not row;  
	return mat3(vec3(1.0,0.0,0.0), vec3(0.0,1.0,0.0), vec3(Offset.x,Offset.y,1.0));
}
vec2 CalcPosition(void)
{
	mat3 FinalMatrix=Transform(-Bounds)*Transform(vec2(0,-Size*Ratio))*Transform(ScreenPos);
	return (FinalMatrix*vec3(0.0,0.0,1.0)).xy;
};
void main()
{
	//double RatioScreen=Screen.x/Screen.y;
	double RatioScreen=1;
	int Offset=3;	
	// UV#1
	UV=vec2(GetOffset(),1.0);
	// VERTEX
	gl_Position=vec4(NormalyzeCoord(Screen2UV(vec2((CalcPosition().x+((Offset+Size)*(Order[0]-NewLine))),CalcPosition().y-(Size*Ratio*Depth) ) )),0.,1.0 );
	EmitVertex();
	// UV#2
	UV=vec2(GetOffset(),0.0);
	// VERTEX
	gl_Position=vec4(NormalyzeCoord(Screen2UV(vec2((CalcPosition().x+((Offset+Size)*(Order[0]-NewLine))),
	CalcPosition().y+Size*Ratio*RatioScreen-(Size*Ratio*Depth)) ) ),0., 1.0 );
	EmitVertex();
	// UV#3
	UV=vec2(GetOffset()+GetStep(), 1.0);
	// VERTEX
	gl_Position=vec4(NormalyzeCoord(Screen2UV(vec2((CalcPosition().x+((Offset+Size)*(Order[0]-NewLine))+Size),CalcPosition().y-(Size*Ratio*Depth)) ) ),0.,1.0 );
	EmitVertex();
	// UV#4
	UV=vec2(GetOffset()+GetStep(), 0.0);
	// VERTEX
	gl_Position=vec4(NormalyzeCoord(Screen2UV(vec2((CalcPosition().x+((Offset+Size)*(Order[0]-NewLine))+Size),CalcPosition().y+Size*Ratio*RatioScreen-(Size*Ratio*Depth)))  ),0.,1.0 );
	EmitVertex();
	EndPrimitive();
};