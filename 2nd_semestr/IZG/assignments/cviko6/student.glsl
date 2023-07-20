#ifdef VERTEX_SHADER

uniform mat4 view;
uniform mat4 proj;
#line 5

out highp vec2 vCoord;

void main(){
  if(gl_VertexID>=32){
    gl_Position = vec4(0.f,0.f,0.f,1.f);
    return;
  }

  mat4 mvp = proj*view;

//1=x dopredu/dozadu
//2=y nahoru/dolu
//3=z doprava/doleva
//4= scale

//podlaha
if(gl_VertexID==0){
  vCoord = vec2(0.f,0.f);
  gl_Position = mvp*vec4(-2.f,0.f,-2.f,1.f);
}
if(gl_VertexID==1){
  vCoord = vec2(1.f,0.f);
  gl_Position = mvp*vec4(+2.f,0.f,-2.f,1.f);
}
if(gl_VertexID==2){
  vCoord = vec2(0.f,1.f);
  gl_Position = mvp*vec4(-2.f,0.f,+2.f,1.f);
}
if(gl_VertexID==3){
  vCoord = vec2(0.f,1.f);
  gl_Position = mvp*vec4(-2.f,0.f,+2.f,1.f);
}
if(gl_VertexID==4){
  vCoord = vec2(1.f,0.f);
  gl_Position = mvp*vec4(+2.f,0.f,-2.f,1.f);
}
if(gl_VertexID==5){
  vCoord = vec2(1.f,1.f);
  gl_Position = mvp*vec4(+2.f,0.f,+2.f,1.f);
}


}//end main

#endif



#ifdef FRAGMENT_SHADER
in highp vec2 vCoord;
out highp vec4 fColor;

void main(){
  vec2 u = fract(vCoord*4);
  float dist = distance(u, vec2(0.5));
  float a = float(dist < 0.5f);

  vec3 col = mix(vec3(0.f,0.5f,0.f),vec3(0.f,1.f,0.f),a);

  fColor = vec4(col,1);

}
#endif
