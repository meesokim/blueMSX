/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32Avi.h,v $
**
** $Revision: 1.4 $
**
** $Date: 2008-03-30 18:38:48 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
******************************************************************************
*/
#ifndef WIN32_AVI
#define WIN32_AVI

#include <windows.h>
#include "MsxTypes.h"
#include "AudioMixer.h"
#include "VideoRender.h"
#include "Properties.h"

void aviStartRender(HWND hwnd, Properties* properties, Video* video);
void aviStopRender();

typedef struct AviSound AviSound;

AviSound* aviSoundCreate(HWND hwnd, Mixer* mixer, UInt32 sampleRate, UInt32 bufferSize, Int16 channels);
void aviSoundDestroy(AviSound* aviSound);
void aviSoundSuspend(AviSound* aviSound);
void aviSoundResume(AviSound* aviSound);

#endif //WIN32_AVI