#include "stdafx.h"
#include "MapData.h"

void CMapData::CreateSlopesAt(DWORD dwPos)
{
	//OutputDebugString("CreateSlopes()\n");

	auto& m = *GetFielddataAt(dwPos);
	if (m.wGround == 0xFFFF) {
		m.wGround = 0;
	}

	TILEDATA& d = (*tiledata)[m.wGround];

	int ns = -1;
	int i, e, p = 0;
	int h[3][3];
	for (i = 0; i < 3; i++) {
		for (e = 0; e < 3; e++) {
			int pos = dwPos + (i - 1) + (e - 1) * m_IsoSize;
			if (pos < 0 || pos >= m_IsoSize * m_IsoSize) {
				h[i][e] = 0;
			} else {
				auto const& m2 = *GetFielddataAt(pos);
				h[i][e] = m.bHeight - m2.bHeight;
			}
		}
	}

	// check if the current tile must be heightened anyway
	if (!theApp.m_Options.bDisableSlopeCorrection && d.bMorphable) {
		if ((h[0][1] < 0 && h[2][1] < 0) || (h[1][0] < 0 && h[1][2] < 0)
			|| (h[1][0] < 0 && h[0][2] < 0 && h[0][1] >= 0)
			|| (h[1][0] < 0 && h[2][2] < 0 && h[2][1] >= 0)

			|| (h[0][1] < 0 && h[2][0] < 0 && h[1][0] >= 0)
			|| (h[0][1] < 0 && h[2][2] < 0 && h[1][2] >= 0)

			|| (h[1][2] < 0 && h[0][0] < 0 && h[0][1] >= 0)
			|| (h[1][2] < 0 && h[2][0] < 0 && h[2][1] >= 0)

			|| (h[2][1] < 0 && h[0][0] < 0 && h[1][0] >= 0)
			|| (h[2][1] < 0 && h[0][2] < 0 && h[1][2] >= 0)

			|| (h[1][0] < 0 && h[0][1] < 0 && h[0][0] >= 0)
			|| (h[0][1] < 0 && h[1][2] < 0 && h[0][2] >= 0)
			|| (h[1][2] < 0 && h[2][1] < 0 && h[2][2] >= 0)
			|| (h[2][1] < 0 && h[1][0] < 0 && h[2][0] >= 0)

			) {
			SetHeightAt(dwPos, m.bHeight + 1);
			for (i = -1; i < 2; i++) {
				for (e = -1; e < 2; e++) {
					CreateSlopesAt(dwPos + i + e * m_IsoSize);
				}
			}
			return;
		}
	}

	BOOL checkOtherSlopes = FALSE;

	if (h[0][0] == -1 && h[2][2] == -1 && h[2][0] >= 0 && h[0][2] >= 0 && h[1][0] >= 0 && h[1][2] >= 0 && h[0][1] >= 0 && h[2][1] >= 0) {
		ns = SLOPE_UP_LEFTTOP_AND_RIGHTBOTTOM;
	}
	if (h[0][2] == -1 && h[2][0] == -1 && h[0][0] >= 0 && h[2][2] >= 0 && h[0][1] >= 0 && h[1][0] >= 0 && h[1][2] >= 0 && h[2][1] >= 0) {
		ns = SLOPE_UP_LEFTBOTTOM_AND_RIGHTTOP;
	}

	if (ns == -1) {
		if (h[1][0] == -1 && h[0][1] != -1 && h[1][2] != -1 && h[2][1] != -1) {
			ns = SLOPE_UP_LEFT;
		} else if (h[0][1] == -1 && h[1][0] != -1 && h[2][1] != -1 && h[1][2] != -1) {
			ns = SLOPE_UP_TOP;
		} else if (h[1][2] == -1 && h[0][1] != -1 && h[1][0] != -1 && h[2][1] != -1) {
			ns = SLOPE_UP_RIGHT;
		} else if (h[2][1] == -1 && h[0][1] != -1 && h[1][0] != -1 && h[1][2] != -1) {
			ns = SLOPE_UP_BOTTOM;
		}

		if (ns == -1) {
			if (h[0][0] == -2) {
				ns = SLOPE_DOWN_BOTTOM;
			}
			if (h[2][0] == -2) {
				ns = SLOPE_DOWN_RIGHT;
			}
			if (h[0][2] == -2) {
				ns = SLOPE_DOWN_LEFT;
			}
			if (h[2][2] == -2) {
				ns = SLOPE_DOWN_TOP;
			}
		}

		if (ns == -1 && h[0][0] == -1) {
			if (h[1][0] == -1 && h[0][1] == -1) {
				ns = SLOPE_DOWN_RIGHTBOTTOM;
			} else if (h[1][0] == 0 && h[0][1] == 0) {
				ns = SLOPE_UP_LEFTTOP;
			}
			//else if(h[2][2]==1) ns=SLOPE_DOWN_BOTTOM;
		}

		if (ns == -1 && h[2][0] == -1) {
			if (h[1][0] == -1 && h[2][1] == -1) {
				ns = SLOPE_DOWN_RIGHTTOP;
			} else if (h[1][0] == 0 && h[2][1] == 0) {
				ns = SLOPE_UP_LEFTBOTTOM;
			}
			//else if(h[0][2]==1) ns=SLOPE_DOWN_RIGHT;
		}
		if (ns == -1 && h[0][2] == -1) {
			if (h[1][2] == -1 && h[0][1] == -1) {
				ns = SLOPE_DOWN_LEFTBOTTOM;
			} else if (h[1][2] == 0 && h[0][1] == 0) {
				ns = SLOPE_UP_RIGHTTOP;
			}
			//else if(h[2][0]==1) ns=SLOPE_DOWN_LEFT;
		}
		if (ns == -1 && h[2][2] == -1) {
			if (h[1][2] == -1 && h[2][1] == -1) {
				ns = SLOPE_DOWN_LEFTTOP;
			} else if (h[1][2] == 0 && h[2][1] == 0) {
				ns = SLOPE_UP_RIGHTBOTTOM;
			}
			//else if(h[0][0]==1) ns=SLOPE_DOWN_TOP;
		}

		if (ns == -1 && h[1][0] == -1 && h[2][1] == -1) {
			ns = SLOPE_DOWN_RIGHTTOP;
		}
		if (ns == -1 && h[1][2] == -1 && h[2][1] == -1) {
			ns = SLOPE_DOWN_LEFTTOP;
		}
		if (ns == -1 && h[1][0] == -1 && h[0][1] == -1) {
			ns = SLOPE_DOWN_RIGHTBOTTOM;
		}
		if (ns == -1 && h[1][2] == -1 && h[0][1] == -1) {
			ns = SLOPE_DOWN_LEFTBOTTOM;
		}

		int rampbase = rampset_start;//atoi((*tiles).sections["General"].values["RampBase"]);
		int rampsmooth = tiles->GetInteger("General", "RampSmooth");

		if (ns == -1 && (d.wTileSet == rampset || d.wTileSet == rampsmooth) && d.bMorphable) {
			SetTileAt(dwPos, 0, 0);
		}
		if (tiledata == &un_tiledata) {
			int r = ramp2set;
			int m = pave2set;

			if (ns == -1 && (d.wTileSet == r || d.wTileSet == m) && d.bMorphable) {
				SetTileAt(dwPos, pave2set_start/*GetTileID(m,0)*/, 0);
			}
		}
		if (d.bMorphable && ns != -1) {
			// NEW URBAN FIX FOR URBAN PAVEMENT
			if (tiledata == &un_tiledata) {
				//[NewUrbanInfo]
				//Morphable2=114
				//Ramps2=117
				int r = ramp2set;
				int m = pave2set;
				if (d.wTileSet == r || d.wTileSet == m) {
					rampbase = ramp2set_start;
				}
			}
			SetTileAt(dwPos, rampbase + ns - 1, 0);

		}
	}
}