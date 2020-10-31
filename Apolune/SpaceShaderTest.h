#pragma once

#include "StarNestShader.h"
#include "SpaceShaders.h"
#include "GLWindow.h"



class SpaceShaderTest : public GLWindow {
public:
 Zp<GLImage> img;
 ElectronCloudsProfile profile;
 EmissionCloudsProfile profile2;
 WaterAndGroundProfile profile3;
 Zdouble mouseFactor;
 void OnLoad() {
  background.Pick(indigo);
  Fullscreen();
  profile2.noiseTex=img=library.Load("data/images/noise1.png",tllto_Tiling);
 }
 void Render() {
  FBOColor *out=&fbo512;
  profile.time+=FRAMETIME/100.0;
  profile3.time+=FRAMETIME;
  profile.pos2d.Set(1.0+input->mxd/10000.0,1.0+input->myd/10000.0);
//  profile.resolution.Set(out->w,out->h);
  profile2.time+=FRAMETIME;
  profile2.pos2d.Set(input->mxi,input->myi);
  if ( input->KeyDown(DX_Q) ) profile.move2d.Set(input->mxd,input->myd);
//  profile3.pos2d.Set(input->mxd*5.0,input->myd*5.0);
//  ShadedWaterAndGround(out,&profile3);
//  ShadedElectronClouds(out,&profile);
  //else
  //if ( input->KeyDown(DX_2) )
  ShadedEmissionClouds(out,&profile2);
//  out->Bind();
//  ShadedStarNestSimple2(&img->texture,FRAMETIME,input->mxd,input->myd,1.0,1.0);
//  out->Unbind();
  out->RenderUpsideDown(none,0,0,display->w,display->h);
 }
 void Between() {
 }
};