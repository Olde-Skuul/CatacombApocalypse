/* Catacomb Apocalypse Source Code
 * Copyright (C) 1993-2014 Flat Rock Software
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

// C3_GAME.C

#include <stdlib.h>

#include "DEF.H"
#include "gelib.h"
#pragma hdrstop

#ifdef PROFILE
#include "TIME.H"
#endif


/*
=============================================================================

						 LOCAL CONSTANTS

=============================================================================
*/

#define NUMLUMPS				45

#define EYESTALKLUMP			0
#define BLOBLUMP				1
#define BOLTLUMP				2
#define NUKELUMP				3
#define POTIONLUMP			4
#define RKEYLUMP				5
#define YKEYLUMP				6
#define GKEYLUMP				7
#define BKEYLUMP				8
#define RGEMLUMP				9
#define GGEMLUMP				10
#define BGEMLUMP				11
#define YGEMLUMP				12
#define PGEMLUMP				13
#define CHESTLUMP				14
#define PLAYERLUMP			15
#define FTIMELUMP				16
#define PORTALLUMP			17
#define COLUMN1LUMP			18
#define FIREPOTLUMP			19
#define COLUMN2LUMP			20
#define EYELUMP				21
#define FUTUREMAGELUMP		22
#define FORCEFIELDLUMP		23
#define ROBOTANKLUMP			24
#define RAMBONELUMP			25
#define STOMPYLUMP			26
#define TROLLLUMP				27
#define WIZARDLUMP			28
#define HEADLUMP				29
#define INVISDUDELUMP		30
#define BUGLUMP				31
#define CYBORGLUMP			32
#define WATERCHESTLUMP		33
#define GRELLUMP				34
#define RAYLUMP				35
#define COLUMN3LUMP			36
#define OLDCHESTLUMP			37
#define OLDFIREPOTLUMP		38
#define COLUMN4LUMP			39
#define TOMB1LUMP				40
#define TOMB2LUMP				41
#define DEMONLUMP				42
#define COLUMN5LUMP			43

int     lumpstart[NUMLUMPS] = {
EYESTALK_LUMP_START,
BLOB_LUMP_START,
BOLT_LUMP_START,
NUKE_LUMP_START,
POTION_LUMP_START,
RKEY_LUMP_START,
YKEY_LUMP_START,
GKEY_LUMP_START,
BKEY_LUMP_START,
RGEM_LUMP_START,
GGEM_LUMP_START,
BGEM_LUMP_START,
YGEM_LUMP_START,
PGEM_LUMP_START,
CHEST_LUMP_START,
PLAYER_LUMP_START,
TIME_LUMP_START,
PORTAL_LUMP_START,
COLUMN1_LUMP_START,
FFIREPOT_LUMP_START,
COLUMN2_LUMP_START,
EYE_LUMP_START,
FUTUREMAGE_LUMP_START,
FORCEFIELD_LUMP_START,
ROBOTANK_LUMP_START,
RAMBONE_LUMP_START,
STOMPY_LUMP_START,
TROLL_LUMP_START,
WIZARD_LUMP_START,
HEAD_LUMP_START,
INVISDUDE_LUMP_START,
BUG_LUMP_START,
CYBORG_LUMP_START,
O_WATER_CHEST_LUMP_START,
GREL_LUMP_START,
RAY_LUMP_START,
COLUMN3_LUMP_START,
OLD_CHEST_LUMP_START,
OFIREPOT_LUMP_START,
COLUMN4_LUMP_START,
TOMB1_LUMP_START,
TOMB2_LUMP_START,
DEMON_LUMP_START,
COLUMN5_LUMP_START,
};


int     lumpend[NUMLUMPS] = {
EYESTALK_LUMP_END,
BLOB_LUMP_END,
BOLT_LUMP_END,
NUKE_LUMP_END,
POTION_LUMP_END,
RKEY_LUMP_END,
YKEY_LUMP_END,
GKEY_LUMP_END,
BKEY_LUMP_END,
RGEM_LUMP_END,
GGEM_LUMP_END,
BGEM_LUMP_END,
YGEM_LUMP_END,
PGEM_LUMP_END,
CHEST_LUMP_END,
PLAYER_LUMP_END,
TIME_LUMP_END,
PORTAL_LUMP_END,
COLUMN1_LUMP_END,
FFIREPOT_LUMP_END,
COLUMN2_LUMP_END,
EYE_LUMP_END,
FUTUREMAGE_LUMP_END,
FORCEFIELD_LUMP_END,
ROBOTANK_LUMP_END,
RAMBONE_LUMP_END,
STOMPY_LUMP_END,
TROLL_LUMP_END,
WIZARD_LUMP_END,
HEAD_LUMP_END,
INVISDUDE_LUMP_END,
BUG_LUMP_END,
CYBORG_LUMP_END,
O_WATER_CHEST_LUMP_END,
GREL_LUMP_END,
RAY_LUMP_END,
COLUMN3_LUMP_END,
OLD_CHEST_LUMP_END,
OFIREPOT_LUMP_END,
COLUMN4_LUMP_END,
TOMB1_LUMP_END,
TOMB2_LUMP_END,
DEMON_LUMP_END,
COLUMN5_LUMP_END,
};



/*
=============================================================================

						 GLOBAL VARIABLES

=============================================================================
*/

unsigned        latchpics[NUMLATCHPICS];
unsigned        tileoffsets[NUMTILE16];
unsigned        textstarts[27];

boolean splitscreen=false;
/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/

boolean lumpneeded[NUMLUMPS];


//===========================================================================



/*
==========================
=
= ScanInfoPlane
=
= Spawn all actors and mark down special places
=
==========================
*/

void ScanInfoPlane (void)
{
	unsigned char hibyte;
	unsigned        x,y,i,j;
	unsigned int tile;
	unsigned        far     *start;

	InitObjList();                  // start spawning things with a clean slate

	scolor = gcolor = 0;
	skycolor = &scolor;
	groundcolor = &gcolor;


	memset (lumpneeded,0,sizeof(lumpneeded));

	start = mapsegs[2];
	for (y=0;y<mapheight;y++)
		for (x=0;x<mapwidth;x++)
		{
			tile = *start++;
			hibyte = tile >> 8;
			tile &= 0xff;

			switch (hibyte)
			{
				char hi;

				case 0xFB:
					wall_anim_time = tile;
					tile = 0;
					break;

				case 0xfa:								// sky/ground color
					x++;
					tile = *start++;
					hi = tile >> 8;
					tile &= 0xff;
					switch (hibyte)
					{
						case 0xfa:			// sky / ground color
							scolor = ((hi)|(hi<<8));
							gcolor = ((tile)|(tile<<8));
							skycolor = &scolor;
							groundcolor = &gcolor;
						break;

					}
				break;
			}

			if ((!tile) || (hibyte))
				continue;

			switch (tile)
			{
			case 1:
			case 2:
			case 3:
			case 4:
				lumpneeded[PLAYERLUMP] = true;
				SpawnPlayer(x,y,NORTH+tile-1);
			break;

			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
				lumpneeded[tile-5+BOLTLUMP] = true;
				SpawnBonus(x,y,tile-5);
			break;

			case 12:
				lumpneeded[EYESTALKLUMP] = true;
				SpawnAquaMan(x, y);
			break;


			case 13:
				lumpneeded[BLOBLUMP] = true;
				SpawnBlob(x, y);
			break;


			case 14:
				lumpneeded[BUGLUMP] = true;
				SpawnBug(x, y);
			break;

			case 15:
				lumpneeded[CYBORGLUMP] = true;
				SpawnCyborgDemon(x, y);
			break;

			case 16:
				lumpneeded[EYELUMP] = true;
				SpawnShooterEye(x, y);
			break;

			case 17:
				lumpneeded[FUTUREMAGELUMP] = true;
				SpawnFutureMage(x, y);
			break;

			case 18:
				lumpneeded[INVISDUDELUMP] = true;
				SpawnInvisDude(x, y);
			break;

			case 19:
				lumpneeded[ROBOTANKLUMP] = true;
				SpawnRoboTank(x, y);
			break;

			case 20:
				lumpneeded[RAMBONELUMP] = true;
				SpawnRamBone(x, y);
			break;

			case 21:
				lumpneeded[STOMPYLUMP] = true;
				SpawnStompy(x, y);
			break;

			case 22:
				lumpneeded[TROLLLUMP] = true;
				SpawnTroll(x, y);
			break;

			case 23:
				lumpneeded[WIZARDLUMP] = true;
				SpawnWizard(x, y);
			break;

			case 24:
				SpawnBounce(x, y, 0);
			break;

			case 25:
				SpawnBounce(x, y, 1);
			break;

			case 26:
				lumpneeded[RKEYLUMP] = lumpneeded[GRELLUMP] = true;
				SpawnGrelminar (x,y);
			break;

			case 27:
				lumpneeded[EYELUMP] = true;
				SpawnRunningEye(x,y);
			break;

			case 28:
				lumpneeded[RAYLUMP] = true;
				SpawnRay(x, y);
			break;

			case 29:
				lumpneeded[HEADLUMP] = true;
				SpawnEgyptianHead(x, y);
			break;

			case 30:
				lumpneeded[DEMONLUMP] = true;
				SpawnDemon(x, y);
			break;

			case 31:
				lumpneeded[COLUMN5LUMP] = true;
				SpawnMiscObjects(x, y, 9);
			break;

			case 32:
				SpawnInvisWallCntroller(x, y);
			break;

			case 33:
			break;

			case 34:
			break;

			case 35:
			break;

			case 36:
				lumpneeded[COLUMN1LUMP] = true;
				SpawnMiscObjects(x, y, 1);
			break;

			case 37:
				lumpneeded[FIREPOTLUMP] = true;
				SpawnMiscObjects(x, y, 4);
			break;

			case 38:
				lumpneeded[PORTALLUMP] = true;
				SpawnWarp(x, y);
			break;

			case 39:
				lumpneeded[FTIMELUMP] = true;
				SpawnFTime(x,y);
			break;


			case 40:
			case 41:
			case 42:
			case 43:
			case 44:
				lumpneeded[tile-40+RGEMLUMP] = true;
				SpawnBonus(x,y,tile-40+B_RGEM);
			break;

			case 45:
				lumpneeded[COLUMN2LUMP] = true;
				SpawnMiscObjects(x, y, 2);
			break;

			case 46:
				lumpneeded[COLUMN3LUMP] = true;
				SpawnMiscObjects(x, y, 3);
			break;

			case 47:
				lumpneeded[FORCEFIELDLUMP] = true;
				SpawnForceField(x, y);
			break;

			case 48:
				lumpneeded[OLDCHESTLUMP] = true;
				SpawnBonus(x, y, B_OLDCHEST);
			break;

			case 49:        // chest
				if (gcolor == 0x0101)
					lumpneeded[WATERCHESTLUMP] = true;
				else
					lumpneeded[CHESTLUMP] = true;
				SpawnBonus(x,y,B_CHEST);
			break;

			case 50:
				lumpneeded[COLUMN4LUMP] = true;
				SpawnMiscObjects(x, y, 5);
			break;

			case 51:
				lumpneeded[OLDFIREPOTLUMP] = true;
				SpawnMiscObjects(x, y, 6);
			break;

			case 52:
				lumpneeded[TOMB1LUMP] = true;
				SpawnMiscObjects(x, y, 7);
			break;

			case 53:
				lumpneeded[TOMB2LUMP] = true;
				SpawnMiscObjects(x, y, 8);
			break;

			case 54:
			break;

			case 55:
			break;

			case 56:
			break;

			case 57:
			break;

			case 58:
			break;

			case 59:
			break;

			case 60:
			break;

			case 61:
			break;

			case 62:
			break;

			case 63:
			break;

			case 64:
			break;

			case 65:
			break;

			case 66:
			break;

			case 67:
			break;

			case 68:
			break;

			case 69:
			break;

			case 70:
			break;

			case 71:
			break;

			}
		}

}

//==========================================================================

/*
==================
=
= ScanText
=
==================
*/

void ScanText (void)
{
	int     i;
	char far *text;

	text = (char _seg *)grsegs[LEVEL1TEXT+mapon];

	textstarts[0] = 0;

	for (i=1;i<=26;i++)
	{
		while (*text != '\n')
		{
			if (*text == '\r')
				*text = 0;
			text++;
		}
		text++;
		textstarts[i] = FP_OFF(text);
	}

}

//==========================================================================

/*
==================
=
= DrawEnterScreen
=
==================
*/
#if 0
static  char    *levelnames[] =
				{
					"Programmers Test Map",
					"The Garden of Tears",
					"The Den of Zombies",
					"The Mausoleum Grounds",
					"The Main Floor of the Mausoleum",
					"Mike's Blastable Passage",
					"The Crypt of Nemesis the Undead",
					"The Subterranean Vault",
					"The Ancient Aqueduct",
					"The Orc Mines",
					"The Lair of the Troll",
					"The Demon's Inferno",
					"The Battleground of the Titans",
					"The Coven of Mages",
					"The Inner Sanctum",
					"The Haunt of Nemesis",
					"The Passage to the Surface",
					"Big Jim's Domain",
					"Nolan",
					"19",
					"20",
					"21",
					"22",
					"23",
					"24",
					"25",
					"26",
					"27",
					"28",
					"29",
					"30",
					"31",
					"32",
					"33",
					"34",
					"35",
					"36",
					"37",
					"38",
					"39",
				};
#endif

void DrawEnterScreen ()
{
	int width;

	bufferofs = displayofs = screenloc[screenpage];
	VW_Bar(0,0,VIEWWIDTH,VIEWHEIGHT,0);

//	width = strlen(levelnames[gamestate.mapon]);
	width = strlen("A new challenge awaits you.");
	if (width < 20)
		width = 20;
	CenterWindow(width,3);
	US_CPrint("\nA new challenge awaits you.\n");
//	US_CPrint(levelnames[gamestate.mapon]);
}

//==========================================================================

boolean tileneeded[NUMFLOORS];


/*
==================
=
= CacheScaleds
=
==================
*/

void CacheScaleds (void)
{
	int     i,j;
	unsigned        source,dest;

	FreeUpMemory ();
	CA_CacheGrChunk(LEVEL1TEXT+mapon);
	ScanText ();

//
// make sure we are displaying screenpage 0
//
	if (screenpage)
	{
		source = screenloc[screenpage];
		dest = screenloc[0];
		VW_ScreenToScreen (source,dest,40,VIEWHEIGHT);
		screenpage = 0;
		VW_SetScreen (dest,0);
		displayofs = dest;
	}

//
// cache wall pictures
//
	for (i=1;i<NUMFLOORS;i++)
		if (tileneeded[i])
		{
			SetupScaleWall (walllight1[i]);
			SetupScaleWall (walldark1[i]);
		}

//
// cache the actor pictures
//
	for (i=0;i<NUMLUMPS;i++)
		if (lumpneeded[i])
			for (j=lumpstart[i];j<=lumpend[i];j++)
				SetupScalePic(j);

	source = screenloc[0];
	for (i=1;i<=2;i++)
	{
		dest = screenloc[i];
		VW_ScreenToScreen (source,dest,40,VIEWHEIGHT);
	}

	screenpage = 1;
}

//==========================================================================


/*
==================
=
= SetupGameLevel
=
==================
*/

void SetupGameLevel ()
{
	int     x,y,i,loop;
	unsigned        far *map,tile,far *spotptr,spot;
	unsigned                search_tile;
	boolean		exploding_walls_present = false;

	memset (tileneeded,0,sizeof(tileneeded));
//
// randomize if not a demo
//
#if 0
	if (DemoMode)
	{
		US_InitRndT(false);
		gamestate.difficulty = gd_Normal;
	}
	else
#endif
		US_InitRndT(true);

//
// load the level
//
	CA_CacheMap (gamestate.mapon);

	mapwidth = mapheaderseg[mapon]->width;
	mapheight = mapheaderseg[mapon]->height;

//
// make a lookup table for the maps left edge
//
	spot = 0;
	for (y=0;y<mapheight;y++)
	{
	  farmapylookup[y] = spot;
	  spot += mapwidth;
	}


//
// copy the wall data to a data segment array
//
	memset (tilemap,0,sizeof(tilemap));
	memset (actorat,0,sizeof(actorat));
	map = mapsegs[0];
	spotptr = mapsegs[2];
	for (y=0;y<mapheight;y++)
		for (x=0;x<mapwidth;x++)
		{
			tile = *map++;

			if (((*spotptr)>>8) == EXP_WALL_CODE)
			{
				exploding_walls_present = true;
			}

			if (tile<NUMFLOORS)
			{
#if 0
				if (tile == WALL_SKELETON_CODE)
				{
					tileneeded[tile+1] = tileneeded[tile+2] = true;
					tilemap[x][y] = tile;
				}
#endif
				if ((tile == 66) || (tile == 67) || (tile == 68) || (tile == 69))
				{
					if ((tile == 66) || (tile == 67))
						tileneeded[tile+2] = true;
					tileneeded[21] = tileneeded[tile] = true;
					tilemap[x][y] = tile;
				}
				else
				if (tile != INVISIBLEWALL)
				{
					tileneeded[tile] = true;
					tilemap[x][y] = tile;
					if (ANIM_FLAGS(tile))
					{
						search_tile = tile+(char signed)ANIM_FLAGS(tile);

						if (!tileneeded[search_tile])
							while (search_tile != tile)
							{
								tileneeded[search_tile] = true;
								if (ANIM_FLAGS(search_tile))
									search_tile += (char signed)ANIM_FLAGS(search_tile);
								else
									TrashProg("Unending Tile Animation!");
							}
					}

				}
				if (tile>0)
					(unsigned)actorat[x][y] = tile;
			}
			spotptr++;
		}


	//
	// Mark any gfx chunks needed
	//

//      CA_MarkGrChunk(NORTHICONSPR);
//      CA_CacheMarks(NULL);


//
// decide which graphics are needed and spawn actors
//
	head_base_delay = 0;  // (1*60) + random(1*60);
	ScanInfoPlane ();
	_fmemset(wall_anim_pos,0,sizeof(wall_anim_pos));


//
// mark which exploding walls are needed ---- the check for floor color
// is preformed in ScanInfoPlane.
//

	if (exploding_walls_present)
	{
				extern unsigned gnd_colors[];

				if (gcolor == 0x0101)
					tileneeded[WATEREXP] = tileneeded[WATEREXP+1] = tileneeded[WATEREXP+2] = true;
				else
					tileneeded[WALLEXP] = tileneeded[WALLEXP+1] = tileneeded[WALLEXP+2] = true;

	}


//
// have the caching manager load and purge stuff to make sure all marks
// are in memory
//
	CA_LoadAllSounds ();
}

//==========================================================================

/*
=====================
=
= LatchDrawPic
=
=====================
*/

void LatchDrawPic (unsigned x, unsigned y, unsigned picnum)
{
	unsigned height, source, dest;
	unsigned wide;

	wide = pictable[picnum-STARTPICS].width;
	height = pictable[picnum-STARTPICS].height;
	dest = bufferofs + ylookup[y]+x;
	source = latchpics[picnum-FIRSTLATCHPIC];

	EGAWRITEMODE(1);
	EGAMAPMASK(15);

asm     mov     bx,[linewidth]
asm     sub     bx,[wide]

asm     mov     ax,[screenseg]
asm     mov     es,ax
asm     mov     ds,ax

asm     mov     si,[source]
asm     mov     di,[dest]
asm     mov     dx,[height]                             // scan lines to draw
asm     mov     ax,[wide]

lineloop:
asm     mov     cx,ax
asm     rep     movsb
asm     add     di,bx

asm     dec     dx
asm     jnz     lineloop

asm     mov     ax,ss
asm     mov     ds,ax                                   // restore turbo's data segment

	EGAWRITEMODE(0);
}

#if USE_STRIPS

//--------------------------------------------------------------------------
// LatchDrawPicStrip() - srcoff is distance into source file (in PIXELS!)
//--------------------------------------------------------------------------
void LatchDrawPicStrip (unsigned x, unsigned y, unsigned picnum, unsigned srcoff)
{
	unsigned wide, height, source, dest, shift, srcmod;

	shift = (srcoff & 7) >> 1;
	srcoff >>= 3;
	wide = pictable[picnum-STARTPICS].width;
	srcmod = wide - linewidth + (shift != 3);
	if (wide > linewidth)
		wide = linewidth;
	height = pictable[picnum-STARTPICS].height;
	dest = bufferofs + ylookup[y]+x;

	picnum = ((picnum - (FIRSTSTRIPPIC+1)) >> 2) + (shift);
	source = latchpics[(FIRSTSTRIPPIC-FIRSTLATCHPIC+1)+picnum];

	EGAWRITEMODE(1);
	EGAMAPMASK(15);

asm     mov     bx,[linewidth]
asm     sub     bx,[wide]

asm     mov     ax,[screenseg]
asm     mov     es,ax
asm     mov     ds,ax

asm     mov     si,[source]
asm	  add		 si,[srcoff]
asm     mov     di,[dest]
asm     mov     dx,[height]                             // scan lines to draw
asm     mov     ax,[wide]

lineloop:
asm     mov     cx,ax
asm     rep     movsb
asm     add     di,bx
asm	  add     si,[srcmod]

asm     dec     dx
asm     jnz     lineloop

asm     mov     ax,ss
asm     mov     ds,ax                                   // restore turbo's data segment

	EGAWRITEMODE(0);
}

#endif


//==========================================================================

/*
=====================
=
= Victory
=
=====================
*/

void Victory (boolean playsounds)
{
	struct Shape shape;

	if (playsounds)
	{
		SD_PlaySound (GETBOLTSND);
		SD_WaitSoundDone ();
		SD_PlaySound (GETNUKESND);
		SD_WaitSoundDone ();
		SD_PlaySound (GETPOTIONSND);
		SD_WaitSoundDone ();
		SD_PlaySound (GETKEYSND);
		SD_WaitSoundDone ();
//		SD_PlaySound (GETSCROLLSND);
//		SD_WaitSoundDone ();
		SD_PlaySound (GETPOINTSSND);
	}

	FreeUpMemory();

	if (!screenfaded)
		VW_FadeOut();

	NormalScreen ();

	screenpage = bufferofs = 0;

	CA_CacheGrChunk (FINALEPIC);
	UNMARKGRCHUNK(FINALEPIC);
	VW_DrawPic(0, 0, FINALEPIC);

	VW_FadeIn();

}

//==========================================================================

#if 0
/*
===================
=
= Died
=
===================
*/

void Died (void)
{
	unsigned page1,page2;
//
// fizzle fade screen to grey
//
	FreeUpMemory ();
	SD_PlaySound (GAMEOVERSND);
	bufferofs = screenloc[(screenpage+1)%3];
	DisplayMsg("Though fallen, your Spirit ...",NULL);
//      LatchDrawPic(0,0,DEADPIC);
//      FizzleFade(bufferofs,displayofs,VIEWWIDTH,VIEWHEIGHT,false);
	IN_ClearKeysDown();
	while (!Keyboard[sc_Enter]);
//      IN_Ack();
	VW_SetScreen (bufferofs,0);
	VW_ColorBorder(0);
}
#endif

//==========================================================================

/*
===================
=
= NormalScreen
=
===================
*/

void NormalScreen (void)
{
	 VW_SetSplitScreen (200);
	 bufferofs = displayofs = SCREEN1START;
	 VW_Bar(0,0,320,200,0);
	 bufferofs = SCREEN2START;
	 VW_Bar(0,0,320,200,0);
	 VW_SetScreen (displayofs,0);
	 splitscreen = false;
}

//==========================================================================

/*
===================
=
= DrawPlayScreen
=
===================
*/

void DrawPlayScreen (void)
{
	int     i,j,p,m;

	screenpage = 0;

	bufferofs = 0;
	VW_Bar (0,0,320,STATUSLINES,0);
	for (i=0;i<3;i++)
	{
		bufferofs = screenloc[i];
		VW_Bar (0,0,320,VIEWHEIGHT,0);
	}

	splitscreen = true;
	VW_SetSplitScreen(120);
	VW_SetScreen(screenloc[0],0);

	CA_CacheGrChunk (STATUSPIC);

	bufferofs = 0;
	VW_DrawPic (0,0,STATUSPIC);

	grneeded[STATUSPIC] &= ~ca_levelbit;
	MM_SetPurge(&grsegs[STATUSPIC],3);

//	RedrawStatusWindow ();
	bufferofs = displayofs = screenloc[0];
}


//==========================================================================

/*
===================
=
= LoadLatchMem
=
===================
*/

unsigned latchmemavail;

void LoadLatchMem (void)
{
	static unsigned base_destoff=0;
	static int base_numpics=0;
	int     i,j,p,m,numpics;
	byte    far *src, far *dest;
	unsigned        destoff;

	EGAWRITEMODE(0);

//
// draw some pics into latch memory
//

  if (!base_numpics)
  {

//
// tile 8s
//
	latchpics[0] = freelatch;
	src = (byte _seg *)grsegs[STARTTILE8];
	dest = MK_FP(0xa000,freelatch);

	for (i=0;i<NUMTILE8;i++)
	{
		for (p=0;p<4;p++)
		{
			m = 1<<p;
			asm     mov     dx,SC_INDEX
			asm     mov     al,SC_MAPMASK
			asm     mov     ah,[BYTE PTR m]
			asm     out     dx,ax
			for (j=0;j<8;j++)
				*(dest+j)=*src++;
		}
		dest+=8;
	}

//
// tile 16s
//
	src = (byte _seg *)grsegs[STARTTILE16];

	for (i=0;i<NUMTILE16;i++)
	{
		CA_CacheGrChunk (STARTTILE16+i);
		src = (byte _seg *)grsegs[STARTTILE16+i];
		if (src)
		{
			tileoffsets[i] = FP_OFF(dest);
			for (p=0;p<4;p++)
			{
				m = 1<<p;
				asm     mov     dx,SC_INDEX
				asm     mov     al,SC_MAPMASK
				asm     mov     ah,[BYTE PTR m]
				asm     out     dx,ax
				for (j=0;j<32;j++)
					*(dest+j)=*src++;
			}
			dest+=32;
			MM_FreePtr (&grsegs[STARTTILE16+i]);
			UNMARKGRCHUNK(STARTTILE16+i);
		}
		else
			tileoffsets[i] = 0;
	}


//
// pics
//
	numpics=1;
	destoff = FP_OFF(dest);
	for (i=FIRSTLATCHPIC+1;i<FIRSTGROUNDPIC;i++)
	{
		latchpics[numpics++] = destoff;
		CA_CacheGrChunk (i);
		j = pictable[i-STARTPICS].width * pictable[i-STARTPICS].height;
		VW_MemToScreen (grsegs[i],destoff,j,1);
		destoff+=j;
		MM_FreePtr (&grsegs[i]);
		UNMARKGRCHUNK(i);
	}

	base_numpics = numpics;
	base_destoff = destoff;

  }

	numpics = base_numpics;
	destoff = base_destoff;

#if USE_STRIPS
//
// ground pics
//
	numpics++;
	for (i=FIRSTGROUNDPIC+1;i<FIRSTSTRIPPIC;i++)
	{
		int shape = (*groundcolor & 0xf0) - 16;

	// Is current shape needed?
	//
		if (shape != (i-(FIRSTGROUNDPIC+1)))
		{
			numpics++;
			continue;
		}

		latchpics[numpics++] = destoff;
		CA_CacheGrChunk (i);
		j = pictable[i-STARTPICS].width * pictable[i-STARTPICS].height;
		VW_MemToScreen (grsegs[i],destoff,j,1);
		destoff+=j;
		MM_FreePtr (&grsegs[i]);
		UNMARKGRCHUNK(i);
	}


//
// 'parallax' strips - used in place of a sky color
//
// Under current setup, each strip takes about 7k in latch memory.
// To create 2 pixel scrolling, 4 strips are needed, that's 28k of
// latch memory needed to produce this effect.
//
	numpics++;
	for (i=FIRSTSTRIPPIC+1;i<FIRSTSCALEPIC;i++)
	{
		memptr work;
		unsigned workdest,stripsize,planesize;
		short loop,pic=i-STARTPICS;
		int shape = (*skycolor & 0xf0) - 16;

	// Is current shape needed?
	//
		if (shape != (i-(FIRSTSTRIPPIC+1)))
		{
			numpics++;
			continue;
		}

	// CAL_ShiftSprite() works with the SRC and DST in the same
	// segment. So we must allocate memory for two strips, and
	// move the base strip into that segment. Then we can use the
	// 2nd half of that memory for each shifted strip.
	//
		CA_CacheGrChunk (i);
		planesize = (pictable[pic].width+1) * pictable[pic].height;
		stripsize = planesize * 4;
//		MM_GetPtr(&work,(stripsize*2)+0000);
		MM_GetPtr(&work,65536);
		movedata((unsigned)grsegs[i],0,(unsigned)work,0,stripsize);
		workdest = 32768; //(stripsize+15) & 0xFFF0;

	// Free base strip
	//
		MM_FreePtr (&grsegs[i]);
		UNMARKGRCHUNK(i);

	// Create three shifted strips and move 'em to latch!
	//
		for (loop=3; loop; loop--)
		{
		// Produce current shift for this strip
		//
			latchpics[numpics++] = destoff;
			CAL_ShiftSprite ((unsigned)work,0,workdest,pictable[pic].width,
								  pictable[pic].height,loop*2,false);

		// Copy this shift to latch memory
		//
			VW_MemToScreen ((memptr)((unsigned)work+(workdest>>4)),destoff,planesize,1);
			destoff+=planesize;
		}

	// Copy unshifted strip to latch
	//
		latchpics[numpics++] = destoff;
		planesize = pictable[pic].width * pictable[pic].height;
		VW_MemToScreen (work,destoff,planesize,1);
		destoff+=planesize;

	// Free work buffer
	//
		MM_FreePtr(&work);
	}
#endif

// Keep track of how much latch memory we have...
//
	latchmemavail = 65535-destoff;

	EGAMAPMASK(15);
}

//==========================================================================

/*
===================
=
= FizzleOut
=
===================
*/

void FizzleOut (int showlevel)
{
	unsigned page1,page2;
//
// fizzle fade screen to grey
//
	bufferofs = screenloc[(screenpage+1)%3];
	if (showlevel)
		DrawEnterScreen ();
	FizzleFade(bufferofs,displayofs,VIEWWIDTH,VIEWHEIGHT,false);
}

//==========================================================================

/*
====================
=
= FreeUpMemory
=
====================
*/

void FreeUpMemory (void)
{
	int     i;

	for (i=0;i<NUMSCALEPICS;i++)
		if (shapedirectory[i])
			MM_SetPurge (&(memptr)shapedirectory[i],3);

	for (i=0;i<NUMSCALEWALLS;i++)
		if (walldirectory[i])
			MM_SetPurge (&(memptr)walldirectory[i],3);
}

//==========================================================================

#if 0

/*
==================
=
= DrawHighScores
=
==================
*/

void    DrawHighScores(void)
{
	char            buffer[16],*str;
	word            i,j,
				w,h,
				x,y;
	HighScore       *s;


	CA_CacheGrChunk (HIGHSCORESPIC);
	VWB_DrawPic (0,0,HIGHSCORESPIC);
	MM_SetPurge (&grsegs[HIGHSCORESPIC],3);
	UNMARKGRCHUNK(HIGHSCORESPIC);

	for (i = 0,s = Scores;i < MaxScores;i++,s++)
	{
		PrintY = 68 + (16 * i);

		//
		// name
		//
		PrintX = 60;
		US_Print(s->name);

		//
		// level
		//
		ultoa(s->completed,buffer,10);
		for (str = buffer;*str;str++)
			*str = *str + (129 - '0');      // Used fixed-width numbers (129...)
		USL_MeasureString(buffer,&w,&h);
		PrintX = (25 * 8) - 8 - w;
		US_Print(buffer);

		//
		// score
		//
		ultoa(s->score,buffer,10);
		for (str = buffer;*str;str++)
			*str = *str + (129 - '0');      // Used fixed-width numbers (129...)
		USL_MeasureString(buffer,&w,&h);
		PrintX = (34 * 8) - 8 - w;
		US_Print(buffer);
	}

	fontcolor = F_BLACK;
}



/*
=======================
=
= CheckHighScore
=
=======================
*/

void    CheckHighScore (long score,word other)
{
	word            i,j;
	int                     n;
	HighScore       myscore;

	strcpy(myscore.name,"");
	myscore.score = score;
	myscore.completed = other;

	for (i = 0,n = -1;i < MaxScores;i++)
	{
		if
		(
			(myscore.score > Scores[i].score)
		||      (
				(myscore.score == Scores[i].score)
			&&      (myscore.completed > Scores[i].completed)
			)
		)
		{
			for (j = MaxScores;--j > i;)
				Scores[j] = Scores[j - 1];
			Scores[i] = myscore;
			n = i;
			HighScoresDirty = true;
			break;
		}
	}

	if (n != -1)
	{
	//
	// got a high score
	//
		DrawHighScores ();
		PrintY = 68 + (16 * n);
		PrintX = 60;
		US_LineInput(PrintX,PrintY,Scores[n].name,nil,true,MaxHighName,100);
	}
}

#endif


//==========================================================================

/*
===================
=
= GameLoop
=
===================
*/

void GameLoop (void)
{
	boolean wait = false;
	int i,xl,yl,xh,yh;
	char num[20];
#ifdef PROFILE
	clock_t start,end;
#endif

	DrawPlayScreen ();
	IN_ClearKeysDown();

restart:
	if (!loadedgame)
	{
		gamestate.difficulty = restartgame;
		restartgame = gd_Continue;
		DrawEnterScreen ();
		if (gamestate.mapon != 8)
			fizzlein = true;
		wait = true;
	}

	do
	{
		playstate = gd_Continue;
		if (!loadedgame)
			SetupGameLevel ();
		else
			loadedgame = false;

		FreeUpMemory();
		LoadLatchMem();
		CacheScaleds ();

		if (EASYMODEON)
			DisplaySMsg("*** NOVICE ***", NULL);
		else
			DisplaySMsg("*** WARRIOR ***", NULL);

		status_delay = 250;

		RedrawStatusWindow();
		if (wait)
		{
			VW_WaitVBL(120);
			wait = false;
		}

#ifdef PROFILE
start = clock();
while (start == clock());
start++;
#endif

		PlayLoop ();

#ifdef PROFILE
end = clock();
itoa(end-start,str,10);
		Quit (str);
#endif


		switch (playstate)
		{
		case ex_abort:
			FreeUpMemory ();
			return;
		case ex_resetgame:
			NewGame();
		case ex_loadedgame:
		case ex_warped:
			FreeUpMemory();
			if (playstate != ex_resetgame)
				DisplayMsg("                                      ", NULL);
			DisplaySMsg("                  ", NULL);
			goto restart;
		case ex_victorious:
			screenpage = 0;
			bufferofs = 0;
			status_flag = 0;
			return;
		}

	} while (1);

}


#if 0
//
// make wall pictures purgable
//
	for (i=0;i<NUMSCALEWALLS;i++)
		if (walldirectory[i])
			MM_SetPurge (&(memptr)walldirectory[i],3);


//
// cache wall pictures back in
//
	for (i=1;i<NUMFLOORS;i++)
		if (tileneeded[i])
		{
			SetupScaleWall (walllight1[i]);
			SetupScaleWall (walllight2[i]);
			SetupScaleWall (walldark1[i]);
			SetupScaleWall (walldark2[i]);
		}
#endif
