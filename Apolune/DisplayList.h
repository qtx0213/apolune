/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once

#include "ListItem.h"
#include "LinkedList.h"
#include "GLSetup.h"
#include "GLWindow.h"

/*
Certain commands are not compiled into the display list but are executed immediately,
regardless of the display-list mode. These commands are glAreTexturesResident,
glColorPointer, glDeleteLists, glDeleteTextures, glDisableClientState,
glEdgeFlagPointer, glEnableClientState, glFeedbackBuffer, glFinish, glFlush,
glGenLists, glGenTextures, glIndexPointer, glInterleavedArrays, glIsEnabled,
glIsList, glIsTexture, glNormalPointer, glPopClientAttrib, glPixelStore,
glPushClientAttrib, glReadPixels, glRenderMode, glSelectBuffer, glTexCoordPointer,
glVertexPointer, and all of the glGet commands.

(And apparently some of glEnable and probably glBindTexture even though its not documented,
but we've never gotten it to work.)

Similarly, glTexImage1D, glTexImage2D, and glTexImage3D are executed immediately and
not compiled into the display list when their first argument is GL_PROXY_TEXTURE_1D,
GL_PROXY_TEXTURE_1D, or GL_PROXY_TEXTURE_3D, respectively.

When the GL_ARB_imaging extension is supported, glHistogram executes immediately when
its argument is GL_PROXY_HISTOGRAM. Similarly, glColorTable executes immediately when
its first argument is glPROXY_COLOR_TABLE, glPROXY_POST_CONVOLUTION_COLOR_TABLE, or
glPROXY_POST_COLOR_MATRIX_COLOR_TABLE.

When the GL_ARB_multitexture extension is supported, glClientActiveTextureARB is
not compiled into display lists, but executed immediately.

When glEndList is encountered, the display-list definition is completed by associating
the list with the unique name list (specified in the glNewList command). If a display
list with name list already exists, it is replaced only when glEndList is called.

Notes

glCallList and glCallLists can be entered into display lists. 
Commands in the display list or lists executed by glCallList or glCallLists are
not included in the display list being created, even if the list creation mode
is GL_COMPILE_AND_EXECUTE.

A display list is just a group of commands and arguments, so errors generated by commands
in a display list must be generated when the list is executed. If the list is created in
GL_COMPILE mode, errors are not generated until the list is executed.
*/

class DisplayList : public ListItem {
public:
 GLuint index;
 GLint maxListNesting;
 bool series,uploaded;
 DisplayList(void) {
  series=false;
  uploaded=false;
  index=0;
  maxListNesting=0;
 }
 ~DisplayList(void) {
  if ( uploaded ) {
   GL_Report( "[entering ~DisplayList()]" );
   glDeleteLists(index,1);  // do not remove
   GL_Report( "~DisplayList()" );
  }
 }
 void GetMaxListNesting() { glGetIntegerv(GL_MAX_LIST_NESTING,&maxListNesting); }
 void StartRecording() {
  if ( uploaded ) Delete();
  GL_Report( "[entering DisplayList::StartRecording()]" );
  index=glGenLists(1);
  GL_Report( "DisplayList::StartRecording():glGenLists" );
  glNewList(index,GL_COMPILE);
 }
 void StopRecording() {
  glEndList();
  GL_Report( "DisplayList::StopRecording():glEndList" );
  uploaded=true;
 }
 void Upload() {
  GL_Report( "[entering DisplayList::Upload()]" );
  index=glGenLists(1);
  GL_Report( "DisplayList::Upload():glGenLists" );
  glNewList(index,GL_COMPILE);
  GL_Report( "DisplayList::Upload():glNewList" );
  Instructions();
  glEndList();
  GL_Report( "DisplayList::Upload():glEndList" );
  uploaded=true;
 }
 void Upload( GLuint base, unsigned int index ) {
  GL_Report( "[entering DisplayList::Upload(b,i)]" );
  this->index=base+(GLuint)index;
  GL_Report( "DisplayList::Upload():glNewList" );
  glNewList(this->index,GL_COMPILE);
  Instructions();
  glEndList();
  GL_Report( "DisplayList::Upload():glEndList" );
  series=true;
  uploaded=true;
 }
 void Delete() { 
   glDeleteLists(index,1);
   GL_Report( "DisplayList::Delete():glDeleteLists " );
   uploaded=false;
 }
 virtual void Instructions() {
  glNewList(index,GL_COMPILE);
  glEndList();
 }
 virtual void Before() {}
 void Call() {
  Before();
  GL_Report( "[entering DisplayList::Call()]" );
  glCallList(index);
  GL_Report( "DisplayList::Call() glCallList" );
  After();
 }
 virtual void After() {}
};

class DisplayLists : public LinkedList {
public:
 GLuint base;
 GLubyte *lists;
 bool uploaded;
 bool series;
 virtual void AssignAllocated() {}
 PROGRAMMABLE(DisplayLists,CallPtr,call,ActualCall);
 virtual void Before() {}
 void Call() {
  Before();
  ActualCall();
  After();
 }
 virtual void After() {}
 void UploadAsOne() {
  GL_Report( "[entering DisplayLists::UploadAsOne()]" );
  base=glGenLists(1);
  GL_Report( "DisplayLists::UploadAsOne():glGenLists" );
  glNewList(base,GL_COMPILE);
  FOREACH(DisplayList,d) {
   d->Instructions();
   d->series=false;
  }
  glEndList();
  GL_Report( "DisplayLists::UploadAsOne():glEndList" );
  uploaded=true;
  call=&DisplayLists::CallAsOne;
 }
 void CallAsOne() {
  GL_Report( "[entering DisplayLists::CallAsOne()]" );
  glCallList(base); 
  GL_Report( "DisplayLists::CallAsOne():glCallList" );
 }
 void UploadLists() {
  GL_Report( "[entering DisplayLists::UploadLists()]" );
  base=glGenLists((GLsizei) count);
  GL_Report( "DisplayLists::UploadLists():glGenLists" );
  lists=new GLubyte[count];
  unsigned int i=0;
  FOREACH(DisplayList,d) { d->Upload(base,i); lists[i]=(GLubyte) i; d->series=true; i++; }
  call=&DisplayLists::CallAsSeries;
 }
 inline void CallAsSeries() {
  GL_Report( "DisplayLists::Call" );
  glListBase(base);
  GL_Report( "DisplayLists::Call:glListBase" );
  glCallLists(count, GL_UNSIGNED_BYTE, lists);
  GL_Report( "DisplayLists::Call:glCallLists" );
 }
 void Clean() {
  GL_Report( "[entering DisplayLists::Clean()]" );
  if ( series ) {
   glDeleteLists(base,count);
   FOREACH(DisplayList,d) { d->uploaded=false; d->series=false; }
  } else glDeleteLists(base,1);
  GL_Report( "DisplayList::Clean() glDeleteLists" );
 }
 CLEARLISTRESET(DisplayList);
 DisplayLists() : LinkedList() {
  lists=null;
  series=false;
  uploaded=false;
  base=0;
 }
 void CleanAndClear() { 
  Clean();
  Clear();
 }
 ~DisplayLists() {
  Clean();
  Clear();
 }
};

class DisplayListses : public ListItem {
public:
 matrix2d<DisplayLists> lists;
 ~DisplayListses() { }
};

/* For calling lists out of sequence */
class DisplayListSet : public ListItem {
public:
};

class DisplayListSets : public LinkedList {
public:
};