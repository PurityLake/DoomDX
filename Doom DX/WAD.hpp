#pragma once

#include "DoomDX.hpp"

struct Header {
	char type[4];
	int32 numLumps, infoOffset;
};

// Object that holds data that points to a lump and what type it is
struct LumpInfo {
	int32 filepos, size;
	char name[8];
};

// Object that golds data from THINGS section of WAD
struct Thing {
	int16 x, y, angle, doomed, flags;
};

// Object that holds data from LINEDEF section of WAD
struct LineDef {
	int16 startVertex, endVertex;
	int16 flags, specialType, sectorTag;
	int16 frontSideDef, backSideDef;
};

// Object that holds data from SIDEDEF section of WAD
struct SideDef {
	int16 x, y;
	int8 upperTexture[8], lowerTexture[8], middleTexture[8];
	int16 sectorNum;
};

// Object that holds data from VERTEX section of WAD
struct Vertex {
	int16 x, y;
};

// Object that holds data from SEGMENT section of WAD
struct Seg {
	int16 startnum, endnum;
	int16 angle, linedefnum;
	int16 direction;
	int16 offset;
};

// Object that holds data from SSECTOR sections of WAD
struct SubSector {
	int16 segCount, firstSegNum;
};

// Object that holds data from NODE sections of WAD
struct Node {
	int16 x, y;
	int16 dx, dy;
	int16 rightBBox[4], leftBBox[4];
	int16 rightChild, leftChild;
};

// Object that holds data from SECTOR sections of WAD
struct Sector {
	int16 floorHeight, ceilingHeight;
	int64 floorTexture, ceilingTexture;
	int16 lightLevel, type, tagNum;
};