/*
** soundemitter.h
**
** This file is part of mkxp.
**
** Copyright (C) 2014 - 2021 Amaryllis Kulla <ancurio@mapleshrine.eu>
**
** mkxp is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 2 of the License, or
** (at your option) any later version.
**
** mkxp is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with mkxp.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SOUNDEMITTER_H
#define SOUNDEMITTER_H

#include "intrulist.h"
#include "al-util.h"
#include "boost-hash.h"
#include "sharedstate.h"
#include "filesystem.h"
#include "exception.h"
#include "config.h"
#include "util.h"
#include "debugwriter.h"

#include <string>
#include <vector>
#include <SDL_sound.h>
#include <algorithm>

#define SE_CACHE_MEM (10*1024*1024)

struct SoundBuffer;
struct Config;

struct SoundEmitter
{
	typedef BoostHash<std::string, SoundBuffer*> BufferHash;

	IntruList<SoundBuffer> buffers;
	BufferHash bufferHash;

	/* Byte count sum of all cached / playing buffers */
	uint32_t bufferBytes;

	const size_t srcCount;
	std::vector<AL::Source::ID> alSrcs;
	std::vector<SoundBuffer*> atchBufs;

	/* Indices of sources, sorted by priority (lowest first) */
	std::vector<size_t> srcPrio;

	float volumeRatio;  // Global volume ratio (0.0-1.0)

	SoundEmitter(const Config &conf);
	~SoundEmitter();

	void play(const std::string &filename,
	          int volume,
	          int pitch);

	void stop();

	float getVolume() const { return volumeRatio * 100; }
	void setVolume(float value) { volumeRatio = clamp(value / 100.0f, 0.0f, 1.0f); }

private:
	static float clamp(float value, float min, float max)
	{
		return std::max(min, std::min(value, max));
	}

	SoundBuffer *allocateBuffer(const std::string &filename);
};

#endif // SOUNDEMITTER_H
