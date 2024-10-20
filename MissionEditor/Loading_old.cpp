

#if !defined(NOSURFACES)
// now standard version, with surfaces
BOOL CLoading::LoadTile(LPCSTR lpFilename, HMIXFILE hOwner, HTSPALETTE hPalette, DWORD dwID, BOOL bReplacement)
{
	last_succeeded_operation = 12;

	//errstream << "Loading " << lpFilename << " owned by " << hOwner << ", palette " << hPalette ;
	//errstream << lpFilename << endl;
	//errstream.flush();

	CIsoView& v = *((CFinalSunDlg*)theApp.m_pMainWnd)->m_view.m_isoview;


	//DeleteFile((CString)AppPath+(CString)"\\TmpTmp.tmp");
	//FSunPackLib::XCC_ExtractFile(lpFilename, (CString)AppPath+(CString)"\\TmpTmp.tmp" /*lpFilename*//*, hOwner);

	int tileCount;
	try {
		if (FSunPackLib::XCC_DoesFileExist(lpFilename, hOwner))
			//if(DoesFileExist((CString)AppPath+(CString)"\\TmpTmp.tmp"))
		{
			//if(
			FSunPackLib::SetCurrentTMP(lpFilename, hOwner);
			//)
			{

				//FSunPackLib::SetCurrentTMP((CString)AppPath+"\\TmpTmp.tmp"/* lpFilename*//*, NULL/*hOwner*//*);
				int tileWidth, tileHeight;
				RECT rect;
				FSunPackLib::XCC_GetTMPInfo(&rect, &tileCount, &tileWidth, &tileHeight);


				LPDIRECTDRAWSURFACE4* pics = new(LPDIRECTDRAWSURFACE4[tileCount]);
				if (FSunPackLib::LoadTMPImageInSurface(v.dd, 0, tileCount, pics, hPalette))
					//if(FSunPackLib::LoadTMPImageInSurface(v.dd,lpFilename, 0, tileCount, pics, hPalette, hOwner))
				{
					TILEDATA* td;
					if (!bReplacement) td = &(*tiledata)[dwID];
					else
					{

						TILEDATA* lpTmp = NULL;
						if ((*tiledata)[dwID].bReplacementCount)
						{
							lpTmp = new(TILEDATA[(*tiledata)[dwID].bReplacementCount]);
							memcpy(lpTmp, (*tiledata)[dwID].lpReplacements, sizeof(TILEDATA) * (*tiledata)[dwID].bReplacementCount);
						}

						(*tiledata)[dwID].lpReplacements = new(TILEDATA[(*tiledata)[dwID].bReplacementCount + 1]);

						if ((*tiledata)[dwID].bReplacementCount)
						{
							memcpy((*tiledata)[dwID].lpReplacements, lpTmp, sizeof(TILEDATA) * (*tiledata)[dwID].bReplacementCount);
							delete[] lpTmp;
						}

						td = &(*tiledata)[dwID].lpReplacements[(*tiledata)[dwID].bReplacementCount];
						(*tiledata)[dwID].bReplacementCount++;
					}


					td->tiles = new(SUBTILE[tileCount]);
					td->wTileCount = tileCount;
					td->cx = tileWidth;
					td->cy = tileHeight;
					td->rect = rect;

					int i;
					for (i = 0; i < tileCount; i++)
					{
						if (pics[i] != NULL)
						{

							int cx, cy;
							BYTE height, terraintype, direction;
							POINT p;
							FSunPackLib::XCC_GetTMPTileInfo(i, &p, &cx, &cy, &direction, &height, &terraintype, &td->tiles[i].rgbLeft, &td->tiles[i].rgbRight);
							td->tiles[i].pic = pics[i];
							td->tiles[i].sX = p.x;
							td->tiles[i].sY = p.y;
							td->tiles[i].wWidth = cx;
							td->tiles[i].wHeight = cy;
							td->tiles[i].bZHeight = height;
							td->tiles[i].bTerrainType = terraintype;
							td->tiles[i].bHackedTerrainType = terraintype;
							td->tiles[i].bDirection = direction;

							if (terraintype == 0xa)
							{
#ifdef RA2_MODE
								td->tiles[i].bHackedTerrainType = TERRAINTYPE_WATER;
#else
								td->tiles[i].bHackedTerrainType = TERRAINTYPE_WATER;
#endif
							}
							if (terraintype == TERRAINTYPE_ROUGH) td->tiles[i].bHackedTerrainType = TERRAINTYPE_GROUND;

							//if((*tiledata)[dwID].wTileSet==waterset) (*tiledata)[dwID].tiles[i].bHackedTerrainType=TERRAINTYPE_WATER;

							// shore hack: check fsdata.ini for new shore terrain
							if (td->wTileSet == shoreset)
							{
								int h;
								for (h = 0; h < (*tiledata_count); h++)
								{
									if ((*tiledata)[h].wTileSet == shoreset) break;
								}

								int pos = dwID - h;
								char c[50];
								itoa(pos, c, 10);
								CString hack = c;
								hack += "_";
								itoa(i, c, 10);
								hack += c;/*
								hack+="_";
								itoa(i/tileWidth, c, 10);
								hack+=c;*/

								CString section = "ShoreTerrainTS";
#ifdef RA2_MODE
								section = "ShoreTerrainRA2";
#endif

								if (g_data.sections[section].FindIndex(hack) >= 0)
								{
									int t = atoi(g_data.sections[section].values[hack]);
									if (t) td->tiles[i].bHackedTerrainType = TERRAINTYPE_WATER;
									else
										td->tiles[i].bHackedTerrainType = 0xe;
								}
							}
							if ((*tiledata)[dwID].wTileSet == waterset) (*tiledata)[dwID].tiles[i].bHackedTerrainType = TERRAINTYPE_WATER;
						} else
						{
							td->tiles[i].pic = NULL;
							td->tiles[i].sX = 0;
							td->tiles[i].sY = 0;
							td->tiles[i].wWidth = 0;
							td->tiles[i].wHeight = 0;
							td->tiles[i].bZHeight = 0;
							td->tiles[i].bTerrainType = 0;
							td->tiles[i].bDirection = 0;
						}
					}
				}

				if (tileCount > 0) delete[] pics;
			}
		} else
		{
			//errstream << " not found" << endl;
			return FALSE;
		}
	}
	catch (...)
	{

	}

	if ((*tiledata)[dwID].wTileCount == 0 || (*tiledata)[dwID].tiles[0].pic == NULL)
	{
		//errstream << " failed" << endl;
		//errstream.flush();

		return FALSE;
	} else
	{
		//errstream << " succeeded" << endl;
		//errstream.flush();
	}

	return TRUE;


}
#endif

#if !defined(NOSURFACES_OBJECTS) // surfaces
BOOL CLoading::LoadUnitGraphic(LPCTSTR lpUnittype)
{
	last_succeeded_operation = 10;

	CString _rules_image; // the image used
	CString filename; // filename of the image
	char theat = cur_theat; // standard theater char is t (Temperat). a is snow.

	BOOL bAlwaysSetChar; // second char is always theater, even if NewTheater not specified!
	WORD wStep = 1; // step is 1 for infantry, buildings, etc, and for shp vehicles it specifies the step rate between every direction
	WORD wStartWalkFrame = 0; // for examply cyborg reaper has another walk starting frame
	int iTurretOffset = 0; // used for centering y pos of turret (if existing)
	BOOL bStructure = rules.sections["BuildingTypes"].FindValue(lpUnittype) >= 0; // is this a structure?

	BOOL bPowerUp = rules.sections[lpUnittype].values["PowersUpBuilding"] != "";

	HTSPALETTE hPalette;
	if (theat == 'T') hPalette = m_hPalIsoTemp;
	if (theat == 'A') hPalette = m_hPalIsoSnow;
	if (theat == 'U') hPalette = m_hPalIsoUrb;

	CIsoView& v = *((CFinalSunDlg*)theApp.m_pMainWnd)->m_view.m_isoview;

	_rules_image = lpUnittype;
	if (rules.sections[lpUnittype].values.find("Image") != rules.sections[lpUnittype].values.end())
		_rules_image = rules.sections[lpUnittype].values["Image"];

	CString _art_image = _rules_image;
	if (art.sections[_rules_image].values.find("Image") != art.sections[_rules_image].values.end())
	{
		if (!isTrue(g_data.sections["IgnoreArtImage"].values[_rules_image]))
			_art_image = art.sections[_rules_image].values["Image"];
	}

	const CString& image = _art_image;
	const auto& rulesSection = rules.sections[lpUnittype];
	const auto& artSection = art.sections[image];

	if (!isTrue(art.sections[image].values["Voxel"])) // is it a shp graphic?
	{
		try
		{

			filename = image + ".shp";


			BYTE bTerrain = 0;



			BOOL isNewTerrain = FALSE;
			if (isTrue(art.sections[image].values["NewTheater"]))//&& isTrue(artSection.GetValueByName("TerrainPalette")))//(filename.GetAt(0)=='G' || filename.GetAt(0)=='N' || filename.GetAt(0)=='C') && filename.GetAt(1)=='A')
			{
				hPalette = m_hPalUnitTemp;
				if (theat == 'A') hPalette = m_hPalUnitSnow;
				if (theat == 'U') hPalette = m_hPalUnitUrb;
				filename.SetAt(1, theat);
				isNewTerrain = TRUE;
			}


			HMIXFILE hShpMix = FindFileInMix(filename, &bTerrain);

			BYTE bIgnoreTerrain = TRUE;

			if (hShpMix == NULL && isNewTerrain)
			{
				filename.SetAt(1, 'G');
				hShpMix = FindFileInMix(filename, &bTerrain);
				if (hShpMix) theat = 'G';

			}



			if (hShpMix == NULL && isNewTerrain)
			{
				filename.SetAt(1, 'A');
				hShpMix = FindFileInMix(filename, &bTerrain);
				if (hShpMix) theat = 'A';
			}

			if (hShpMix == NULL && isNewTerrain)
			{
				filename.SetAt(1, 'T');
				hShpMix = FindFileInMix(filename, &bTerrain);
				if (hShpMix) {
					theat = 'T';
					hPalette = m_hIsoTemp;
				}
			}


			if (isTrue(artSection.GetValueByName("TerrainPalette")))
			{
				bIgnoreTerrain = FALSE;

				if (cur_theat == 'T')
					hPalette = m_hPalIsoTemp;
				else if (cur_theat == 'A')
					hPalette = m_hPalIsoSnow;
				else if (cur_theat == 'U')
					hPalette = m_hPalIsoUrb;



			}



			if (hShpMix == 0)
			{
				filename = image;
				filename += ".shp";
				hShpMix = FindFileInMix(filename, &bTerrain);



				if (hShpMix == NULL)
				{
					filename = image;
					if (theat == 'T') filename += ".tem";
					if (theat == 'A') filename += ".sno";
					if (theat == 'U') filename += ".urb";
					filename.MakeLower();
					hShpMix = FindFileInMix(filename, &bTerrain);

					if (hShpMix == NULL)
					{
						filename = image;
						filename += ".tem";
						hShpMix = FindFileInMix(filename, &bTerrain);
						if (hShpMix)
						{
							hPalette = m_hPalIsoTemp;
						}
					}

					if (hShpMix != NULL)
					{



					} else
					{
						filename = image + ".shp";

						filename.SetAt(1, 'A');

						hShpMix = FindFileInMix(filename);

						if (hShpMix != NULL)
						{
							bAlwaysSetChar = TRUE;
						} else
						{
							filename.SetAt(1, 'A');
							hShpMix = FindFileInMix(filename);

							if (hShpMix != NULL)
							{
								theat = 'A';
								bAlwaysSetChar = TRUE;
							} else
							{
								filename.SetAt(1, 'U');
								hShpMix = FindFileInMix(filename);
								if (hShpMix) theat = 'U';
								else
								{
									filename.SetAt(1, 'T');
									hShpMix = FindFileInMix(filename);
									if (hShpMix) theat = 'T';
								}
							}
						}
					}
				} else
				{
					theat = 'T';
				}

			} else
			{

				// now we need to find out the palette

				if (isTrue(artSection.GetValueByName("TerrainPalette"))) // it´s a file in isotemp.mix/isosno.mix
				{

				} else // it´s a file in temperat.mix/snow.mix
				{
					if (cur_theat == 'T') hPalette = m_hPalUnitTemp;
					if (cur_theat == 'A') hPalette = m_hPalUnitSnow;
					if (cur_theat == 'U') hPalette = m_hPalUnitUrb;
				}

			}



			if (filename == "tibtre01.tem" || filename == "tibtre02.tem" || filename == "tibtre03.tem" || filename == "veinhole.tem")
			{
				hPalette = m_hPalUnitTemp;
			}


			SHPHEADER head;
			CString bibname;
			CString bibfilename;
			CString activeanim_name;
			CString activeanim_filename;
			CString idleanim_name;
			CString idleanim_filename;
			CString activeanim2_name;
			CString activeanim2_filename;
			CString activeanim3_name;
			CString activeanim3_filename;
			CString superanim1_name, superanim1_filename;
			CString superanim2_name, superanim2_filename;
			CString superanim3_name, superanim3_filename;
			CString superanim4_name, superanim4_filename;
			CString specialanim1_name, specialanim1_filename;
			CString specialanim2_name, specialanim2_filename;
			CString specialanim3_name, specialanim3_filename;
			CString specialanim4_name, specialanim4_filename;

			CString turretanim_name;
			CString turretanim_filename;
			LPDIRECTDRAWSURFACE4 bib = NULL;
			LPDIRECTDRAWSURFACE4 activeanim = NULL;
			LPDIRECTDRAWSURFACE4 idleanim = NULL;
			LPDIRECTDRAWSURFACE4 activeanim2 = NULL;
			LPDIRECTDRAWSURFACE4 activeanim3 = NULL;
			LPDIRECTDRAWSURFACE4 superanim1 = NULL;
			LPDIRECTDRAWSURFACE4 superanim2 = NULL;
			LPDIRECTDRAWSURFACE4 superanim3 = NULL;
			LPDIRECTDRAWSURFACE4 superanim4 = NULL;
			LPDIRECTDRAWSURFACE4 specialanim1 = NULL;
			LPDIRECTDRAWSURFACE4 specialanim2 = NULL;
			LPDIRECTDRAWSURFACE4 specialanim3 = NULL;
			LPDIRECTDRAWSURFACE4 specialanim4 = NULL;
			LPDIRECTDRAWSURFACE4* lpT = NULL;
			LPDIRECTDRAWSURFACE4 turrets[8] = { 0 };
			SHPIMAGEHEADER turretinfo[8];

			if (hShpMix > 0)
			{


				//errstream << (LPCTSTR)filename << " found " ;
				//errstream.flush();


				if (rules.sections[lpUnittype].values["Bib"] != "no") // seems to be ignored by TS, art.ini overwrites???
				{

					bibname = art.sections[image].values["BibShape"];
					if (bibname.GetLength() > 0)
					{
						bibfilename = bibname + ".shp";

						if (isTrue(art.sections[image].values["NewTheater"]))
							bibfilename.SetAt(1, theat);

						if (bAlwaysSetChar) bibfilename.SetAt(1, theat);

						if (FSunPackLib::XCC_DoesFileExist(bibfilename, hShpMix))
						{
							FSunPackLib::SetCurrentSHP(bibfilename, hShpMix);
							FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, 0, 1, &bib);

						}
					}
				}

				activeanim_name = art.sections[image].values["ActiveAnim"];
				if (activeanim_name.GetLength() > 0)
				{
					activeanim_filename = activeanim_name + ".shp";

					if (isTrue(art.sections[image].values["NewTheater"]))
						activeanim_filename.SetAt(1, theat);

					if (bAlwaysSetChar) activeanim_filename.SetAt(1, theat);

					if (FSunPackLib::XCC_DoesFileExist(activeanim_filename, hShpMix))
					{
						FSunPackLib::SetCurrentSHP(activeanim_filename, hShpMix);

						FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, 0, 1, &activeanim);


					}
				}

				idleanim_name = art.sections[image].values["IdleAnim"];
				if (idleanim_name.GetLength() > 0)
				{
					idleanim_filename = idleanim_name + ".shp";

					if (isTrue(art.sections[image].values["NewTheater"]))
						idleanim_filename.SetAt(1, theat);

					if (bAlwaysSetChar) idleanim_filename.SetAt(1, theat);

					if (FSunPackLib::XCC_DoesFileExist(idleanim_filename, hShpMix))
					{
						FSunPackLib::SetCurrentSHP(idleanim_filename, hShpMix);

						FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, 0, 1, &idleanim);
					}
				}


				activeanim2_name = art.sections[image].values["ActiveAnimTwo"];
				if (activeanim2_name.GetLength() > 0)
				{
					activeanim2_filename = activeanim2_name + ".shp";

					if (isTrue(art.sections[image].values["NewTheater"]))
						activeanim2_filename.SetAt(1, theat);

					if (bAlwaysSetChar) activeanim2_filename.SetAt(1, theat);

					if (FSunPackLib::XCC_DoesFileExist(activeanim2_filename, hShpMix))
					{
						FSunPackLib::SetCurrentSHP(activeanim2_filename, hShpMix);

						FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, 0, 1, &activeanim2);

					}
				}

				activeanim3_name = art.sections[image].values["ActiveAnimThree"];
				if (activeanim3_name.GetLength() > 0)
				{
					activeanim3_filename = activeanim3_name + ".shp";

					if (isTrue(art.sections[image].values["NewTheater"]))
						activeanim3_filename.SetAt(1, theat);

					if (bAlwaysSetChar) activeanim3_filename.SetAt(1, theat);

					if (FSunPackLib::XCC_DoesFileExist(activeanim3_filename, hShpMix))
					{
						FSunPackLib::SetCurrentSHP(activeanim3_filename, hShpMix);

						FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, 0, 1, &activeanim3);

					}
				}

				superanim1_name = art.sections[image].values["SuperAnim"];
				if (superanim1_name.GetLength() > 0)
				{
					superanim1_filename = superanim1_name + ".shp";

					if (isTrue(art.sections[image].values["NewTheater"]))
						superanim1_filename.SetAt(1, theat);

					if (bAlwaysSetChar) superanim1_filename.SetAt(1, theat);

					if (FSunPackLib::XCC_DoesFileExist(superanim1_filename, hShpMix))
					{
						FSunPackLib::SetCurrentSHP(superanim1_filename, hShpMix);
						FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, 0, 1, &superanim1);
					}
				}

				superanim2_name = art.sections[image].values["SuperAnimTwo"];
				if (superanim2_name.GetLength() > 0)
				{
					superanim2_filename = superanim2_name + ".shp";

					if (isTrue(art.sections[image].values["NewTheater"]))
						superanim2_filename.SetAt(1, theat);

					if (bAlwaysSetChar) superanim2_filename.SetAt(1, theat);

					if (FSunPackLib::XCC_DoesFileExist(superanim2_filename, hShpMix))
					{
						FSunPackLib::SetCurrentSHP(superanim2_filename, hShpMix);
						FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, 0, 1, &superanim2);
					}
				}

				superanim3_name = art.sections[image].values["SuperAnimThree"];
				if (superanim3_name.GetLength() > 0)
				{
					superanim3_filename = superanim3_name + ".shp";

					if (isTrue(art.sections[image].values["NewTheater"]))
						superanim3_filename.SetAt(1, theat);

					if (bAlwaysSetChar) superanim3_filename.SetAt(1, theat);

					if (FSunPackLib::XCC_DoesFileExist(superanim3_filename, hShpMix))
					{
						FSunPackLib::SetCurrentSHP(superanim3_filename, hShpMix);
						FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, 0, 1, &superanim3);
					}
				}

				superanim4_name = art.sections[image].values["SuperAnimFour"];
				if (superanim4_name.GetLength() > 0)
				{
					superanim4_filename = superanim4_name + ".shp";

					if (isTrue(art.sections[image].values["NewTheater"]))
						superanim4_filename.SetAt(1, theat);

					if (bAlwaysSetChar) superanim4_filename.SetAt(1, theat);

					if (FSunPackLib::XCC_DoesFileExist(superanim4_filename, hShpMix))
					{
						FSunPackLib::SetCurrentSHP(superanim4_filename, hShpMix);
						FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, 0, 1, &superanim4);
					}
				}

				specialanim1_name = art.sections[image].values["SpecialAnim"];
				if (specialanim1_name.GetLength() > 0)
				{
					specialanim1_filename = specialanim1_name + ".shp";

					if (isTrue(art.sections[image].values["NewTheater"]))
						specialanim1_filename.SetAt(1, theat);

					if (bAlwaysSetChar) specialanim1_filename.SetAt(1, theat);

					if (FSunPackLib::XCC_DoesFileExist(specialanim1_filename, hShpMix))
					{
						FSunPackLib::SetCurrentSHP(specialanim1_filename, hShpMix);
						FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, 0, 1, &specialanim1);
					}
				}

				specialanim2_name = art.sections[image].values["SpecialAnimTwo"];
				if (specialanim2_name.GetLength() > 0)
				{
					specialanim2_filename = specialanim2_name + ".shp";

					if (isTrue(art.sections[image].values["NewTheater"]))
						specialanim2_filename.SetAt(1, theat);

					if (bAlwaysSetChar) specialanim2_filename.SetAt(1, theat);

					if (FSunPackLib::XCC_DoesFileExist(specialanim2_filename, hShpMix))
					{
						FSunPackLib::SetCurrentSHP(specialanim2_filename, hShpMix);
						FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, 0, 1, &specialanim2);
					}
				}

				specialanim3_name = art.sections[image].values["SpecialAnimThree"];
				if (specialanim3_name.GetLength() > 0)
				{
					specialanim3_filename = specialanim3_name + ".shp";

					if (isTrue(art.sections[image].values["NewTheater"]))
						specialanim3_filename.SetAt(1, theat);

					if (bAlwaysSetChar) specialanim3_filename.SetAt(1, theat);

					if (FSunPackLib::XCC_DoesFileExist(specialanim3_filename, hShpMix))
					{
						FSunPackLib::SetCurrentSHP(specialanim3_filename, hShpMix);
						FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, 0, 1, &specialanim3);
					}
				}

				specialanim4_name = art.sections[image].values["SpecialAnimFour"];
				if (specialanim4_name.GetLength() > 0)
				{
					specialanim4_filename = specialanim4_name + ".shp";

					if (isTrue(art.sections[image].values["NewTheater"]))
						specialanim4_filename.SetAt(1, theat);

					if (bAlwaysSetChar) specialanim4_filename.SetAt(1, theat);

					if (FSunPackLib::XCC_DoesFileExist(specialanim4_filename, hShpMix))
					{
						FSunPackLib::SetCurrentSHP(specialanim4_filename, hShpMix);
						FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, 0, 1, &specialanim4);
					}
				}

				BOOL bVoxelTurret = FALSE;

				turretanim_name = rules.sections[image].values["TurretAnim"];
				if (bStructure && rules.sections[image].values["Turret"] == "yes" && turretanim_name.GetLength() > 0 && rules.sections[image].values["TurretAnimIsVoxel"] != "true")
				{
					turretanim_filename = turretanim_name + ".shp";
					if (art.sections[turretanim_name].values.find("Image") != art.sections[turretanim_name].values.end()) turretanim_filename = art.sections[turretanim_name].values["Image"] + ".shp";

					if (isTrue(art.sections[image].values["NewTheater"]))
						turretanim_filename.SetAt(1, theat);


					FSunPackLib::SetCurrentSHP(turretanim_filename, hShpMix);
					FSunPackLib::XCC_GetSHPHeader(&head);

					int iStartTurret = 0;
					const WORD wAnimCount = 4; // anims between each "normal" direction, seems to be hardcoded

					int i;

					for (i = 0; i < 8; i++)
					{
						if (iStartTurret + i * wAnimCount < head.c_images)
						{
							FSunPackLib::XCC_GetSHPImageHeader(iStartTurret + i * wAnimCount, &turretinfo[i]);
							FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, iStartTurret + i * wAnimCount, 1, &turrets[i]);
						}

					}
				} else if (bStructure && rules.sections[image].values["Turret"] == "yes" && turretanim_name.GetLength() > 0 && rules.sections[image].values["TurretAnimIsVoxel"] == "true")
				{
					turretanim_filename = turretanim_name + ".vxl";
					if (art.sections[turretanim_name].values.find("Image") != art.sections[turretanim_name].values.end()) turretanim_filename = art.sections[turretanim_name].values["Image"] + ".vxl";

					//if(isTrue(art.sections[image].values["NewTheater"]))
					//	turretanim_filename.SetAt(1, theat);

					HMIXFILE hVXL = FindFileInMix(turretanim_filename);

					if (hVXL)
					{
						bVoxelTurret = TRUE;

						if (
							FSunPackLib::SetCurrentVXL(turretanim_filename, hVXL)
							)
						{
							int i;

							for (i = 0; i < 8; i++)
							{
								float r_x, r_y, r_z;


								r_x = 300;
								r_y = 0;
								r_z = 45 * i + 90;

								// convert
								const double pi = 3.141592654;
								r_x = r_x / 180.0f * pi;
								r_y = r_y / 180.0f * pi;
								r_z = r_z / 180.0f * pi;

								int center_x, center_y;
								if (!
									FSunPackLib::LoadVXLImageInSurface(*m_voxelNormalTables, lightDirection, v.dd, 0, 1, r_x, r_y, r_z, &turrets[i], hPalette, &center_x, &center_y, atoi(rules.sections[image].values["TurretAnimZAdjust"]))
									)
								{

								} else
								{
									DDSURFACEDESC2 ddsd;
									memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
									ddsd.dwSize = sizeof(DDSURFACEDESC2);
									ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT;
									turrets[i]->GetSurfaceDesc(&ddsd);
									turretinfo[i].x = -center_x;
									turretinfo[i].y = -center_y;
									turretinfo[i].cx = ddsd.dwWidth;
									turretinfo[i].cy = ddsd.dwHeight;
								}

							}
						}
					}
				}


				if (art.sections[image].values.find("WalkFrames") != art.sections[image].values.end())
					wStep = atoi(art.sections[image].values["WalkFrames"]);
				if (art.sections[image].values.find("StartWalkFrame") != art.sections[image].values.end())
					wStartWalkFrame = atoi(art.sections[image].values["StartWalkFrame"]);
				if (art.sections[image].values.find("TurretOffset") != art.sections[image].values.end())
					iTurretOffset = atoi(art.sections[image].values["TurretOffset"]);


				if (art.sections[image].values["Palette"] == "lib")
					hPalette = m_hPalLib;

				BOOL bSuccess = FSunPackLib::SetCurrentSHP(filename, hShpMix);
				if (
					!bSuccess
					)
				{
					filename = image += ".sno";
					if (cur_theat == 'T' || cur_theat == 'U') hPalette = m_hPalIsoTemp;
					hShpMix = FindFileInMix(filename, &bTerrain);
					bSuccess = FSunPackLib::SetCurrentSHP(filename, hShpMix);

					if (!bSuccess)
					{
						missingimages[lpUnittype] = TRUE;
					}
				}

				if (bSuccess)
				{

					FSunPackLib::XCC_GetSHPHeader(&head);
					int i;
					int maxPics = head.c_images;
					if (maxPics > 8) maxPics = 8; // we only need 8 pictures for every direction!
					if (bStructure && !bPowerUp) maxPics = 1;
					if (bVoxelTurret) maxPics = 8;


					if (!bStructure && rules.sections[image].values["Turret"] == "yes")
					{
						int iStartTurret = wStartWalkFrame + 8 * wStep;
						const WORD wAnimCount = 4; // anims between each "normal" direction, seems to be hardcoded

						int i;

						for (i = 0; i < 8; i++)
						{
							if (!bStructure && iStartTurret + i * wAnimCount < head.c_images)
							{
								FSunPackLib::XCC_GetSHPImageHeader(iStartTurret + i * wAnimCount, &turretinfo[i]);
								FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, iStartTurret + i * wAnimCount, 1, &turrets[i]);
							}

						}
					}



					// create an array of pointers to directdraw surfaces
					lpT = new(LPDIRECTDRAWSURFACE4[maxPics]);
					memset(lpT, 0, sizeof(LPDIRECTDRAWSURFACE4) * maxPics);

					if (bVoxelTurret)
					{
						for (i = 0; i < maxPics; i++)
						{
							FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, 0, 1, &lpT[i]);
						}
					} else if (wStep == 1 && (rules.sections[lpUnittype].values["PowersUpBuilding"].GetLength() == 0 || !isTrue(rules.sections[lpUnittype].values["Turret"])))
					{ // standard case...

						FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, wStartWalkFrame, maxPics, lpT);

					} else if (rules.sections[lpUnittype].values["PowersUpBuilding"].GetLength() != 0 && isTrue(rules.sections[lpUnittype].values["Turret"]))
					{ // a "real" turret (vulcan cannon, etc...)
						for (i = 0; i < maxPics; i++)
						{
							FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, i * 4, 1, &lpT[i]);
						}
					} else
					{ // walk frames used
						for (i = 0; i < maxPics; i++)
						{
							int pic_in_file = i;
							FSunPackLib::LoadSHPImageInSurface(v.dd, hPalette, i * wStep + wStartWalkFrame, 1, &lpT[i]);
						}
					}

					for (i = 0; i < maxPics; i++)
					{
						int pic_in_file = i;
						if (bStructure && bVoxelTurret) pic_in_file = 0;
						SHPIMAGEHEADER imghead;
						FSunPackLib::XCC_GetSHPImageHeader(pic_in_file, &imghead);

						if (bib != NULL)
						{
							DDBLTFX fx;

							memset(&fx, 0, sizeof(DDBLTFX));
							fx.dwSize = sizeof(DDBLTFX);

							lpT[i]->Blt(NULL, bib, NULL, DDBLT_KEYSRC | DDBLT_WAIT, &fx);

							imghead.cx = head.cx - imghead.x; // update size of main graphic
							imghead.cy = head.cy - imghead.y;

						}

						if (activeanim != NULL)
						{
							DDBLTFX fx;

							memset(&fx, 0, sizeof(DDBLTFX));
							fx.dwSize = sizeof(DDBLTFX);

							lpT[i]->Blt(NULL, activeanim, NULL, DDBLT_KEYSRC | DDBLT_WAIT, &fx);


						}

						if (idleanim != NULL)
						{
							DDBLTFX fx;

							memset(&fx, 0, sizeof(DDBLTFX));
							fx.dwSize = sizeof(DDBLTFX);

							lpT[i]->Blt(NULL, idleanim, NULL, DDBLT_KEYSRC | DDBLT_WAIT, &fx);


						}

						if (activeanim2 != NULL)
						{
							DDBLTFX fx;

							memset(&fx, 0, sizeof(DDBLTFX));
							fx.dwSize = sizeof(DDBLTFX);

							lpT[i]->Blt(NULL, activeanim2, NULL, DDBLT_KEYSRC | DDBLT_WAIT, &fx);


						}

						if (activeanim3 != NULL)
						{
							DDBLTFX fx;

							memset(&fx, 0, sizeof(DDBLTFX));
							fx.dwSize = sizeof(DDBLTFX);

							lpT[i]->Blt(NULL, activeanim3, NULL, DDBLT_KEYSRC | DDBLT_WAIT, &fx);


						}

						if (superanim1 != NULL)
						{
							DDBLTFX fx;

							memset(&fx, 0, sizeof(DDBLTFX));
							fx.dwSize = sizeof(DDBLTFX);

							lpT[i]->Blt(NULL, superanim1, NULL, DDBLT_KEYSRC | DDBLT_WAIT, &fx);


						}

						if (superanim2 != NULL)
						{
							DDBLTFX fx;

							memset(&fx, 0, sizeof(DDBLTFX));
							fx.dwSize = sizeof(DDBLTFX);

							lpT[i]->Blt(NULL, superanim2, NULL, DDBLT_KEYSRC | DDBLT_WAIT, &fx);


						}

						if (superanim3 != NULL)
						{
							DDBLTFX fx;

							memset(&fx, 0, sizeof(DDBLTFX));
							fx.dwSize = sizeof(DDBLTFX);

							lpT[i]->Blt(NULL, superanim3, NULL, DDBLT_KEYSRC | DDBLT_WAIT, &fx);


						}

						if (superanim4 != NULL)
						{
							DDBLTFX fx;

							memset(&fx, 0, sizeof(DDBLTFX));
							fx.dwSize = sizeof(DDBLTFX);

							lpT[i]->Blt(NULL, superanim4, NULL, DDBLT_KEYSRC | DDBLT_WAIT, &fx);


						}

						if (specialanim1 != NULL)
						{
							DDBLTFX fx;

							memset(&fx, 0, sizeof(DDBLTFX));
							fx.dwSize = sizeof(DDBLTFX);

							lpT[i]->Blt(NULL, specialanim1, NULL, DDBLT_KEYSRC | DDBLT_WAIT, &fx);


						}

						if (specialanim2 != NULL)
						{
							DDBLTFX fx;

							memset(&fx, 0, sizeof(DDBLTFX));
							fx.dwSize = sizeof(DDBLTFX);

							lpT[i]->Blt(NULL, specialanim2, NULL, DDBLT_KEYSRC | DDBLT_WAIT, &fx);


						}

						if (specialanim3 != NULL)
						{
							DDBLTFX fx;

							memset(&fx, 0, sizeof(DDBLTFX));
							fx.dwSize = sizeof(DDBLTFX);

							lpT[i]->Blt(NULL, specialanim3, NULL, DDBLT_KEYSRC | DDBLT_WAIT, &fx);


						}

						if (specialanim4 != NULL)
						{
							DDBLTFX fx;

							memset(&fx, 0, sizeof(DDBLTFX));
							fx.dwSize = sizeof(DDBLTFX);

							lpT[i]->Blt(NULL, specialanim4, NULL, DDBLT_KEYSRC | DDBLT_WAIT, &fx);


						}



						if (turrets[i] != NULL)
						{
							DDBLTFX fx;
							int iMove = 0;

							DDSURFACEDESC2 ddsd;
							memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
							ddsd.dwSize = sizeof(DDSURFACEDESC2);
							ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT;
							turrets[i]->GetSurfaceDesc(&ddsd);


							memset(&fx, 0, sizeof(DDBLTFX));
							fx.dwSize = sizeof(DDBLTFX);

							RECT srcRect, destRect;
							srcRect.left = 0;
							srcRect.right = ddsd.dwWidth;
							destRect.left = (head.cx - ddsd.dwWidth) / 2;
							destRect.right = head.cx - destRect.left;

							if (iMove < 0)
							{
								srcRect.top = -iMove;
								srcRect.bottom = ddsd.dwHeight;
								destRect.top = 0;
								destRect.bottom = head.cy + iMove - (head.cy - ddsd.dwHeight);
							} else
							{
								int mx = imghead.x / 2 + imghead.cx / 2 + (-turretinfo[i].x / 2 - turretinfo[i].cx) + atoi(rules.sections[image].values["TurretAnimX"]);
								int my = imghead.y / 2 + imghead.cy / 2 + (-turretinfo[i].y / 2 - turretinfo[i].cy) + atoi(rules.sections[image].values["TurretAnimY"]);//+atoi(rules.sections[image].values["TurretAnimZAdjust"]);

								if (ddsd.dwWidth != head.cx || ddsd.dwHeight != head.cy)
								{
									// voxel turret
									//mx=head.cx/2-ddsd.dwWidth/2;//+atoi(rules.sections[image].values["TurretAnimX"]);
									//my=head.cy/2-ddsd.dwHeight/2+atoi(rules.sections[image].values["TurretAnimY"])+atoi(rules.sections[image].values["TurretAnimZAdjust"])/2;
									mx = imghead.x + imghead.cx / 2 + turretinfo[i].x + atoi(rules.sections[image].values["TurretAnimX"]);
									my = imghead.y + imghead.cy / 2 + turretinfo[i].y + atoi(rules.sections[image].values["TurretAnimY"]);//+atoi(rules.sections[image].values["TurretAnimZAdjust"])/2;

									errstream << turretinfo[i].x << " y:" << turretinfo[i].y << " mx:" << mx << " my:" << my << endl;
									errstream.flush();

									int XMover, YMover;
#ifdef RA2_MODE
									XMover = atoi(g_data.sections["BuildingVoxelTurretsRA2"].values[(CString)lpUnittype + "X"]);
									YMover = atoi(g_data.sections["BuildingVoxelTurretsRA2"].values[(CString)lpUnittype + "Y"]);
#else
									XMover = atoi(g_data.sections["BuildingVoxelTurrets"].values[(CString)lpUnittype + "X"]);
									YMover = atoi(g_data.sections["BuildingVoxelTurrets"].values[(CString)lpUnittype + "Y"]);
#endif

									mx += XMover;
									my += YMover;

									srcRect.top = 0;
									srcRect.left = 0;
									srcRect.right = ddsd.dwWidth;
									srcRect.bottom = ddsd.dwHeight;
									destRect.top = my;
									destRect.left = mx;
									destRect.right = destRect.left + ddsd.dwWidth;
									destRect.bottom = destRect.top + ddsd.dwHeight;
									if (destRect.top < 0)
									{
										int old = destRect.top;
										destRect.top = 0;
										srcRect.top -= old - destRect.top;

									}
									if (destRect.right >= head.cx)
									{
										int old = destRect.right;
										destRect.right = head.cx;
										srcRect.right -= old - destRect.right;
									}
									if (destRect.bottom >= head.cy)
									{
										int old = destRect.bottom;
										destRect.bottom = head.cy;
										srcRect.bottom -= old - destRect.bottom;
									}
								} else
								{

									if (mx < 0)mx = 0;
									if (my < 0)my = 0;
									srcRect.top = 0;
									srcRect.right = ddsd.dwWidth - mx;
									srcRect.bottom = ddsd.dwHeight - my;
									destRect.top = my;
									destRect.left = mx + (head.cx - ddsd.dwWidth) / 2;
									destRect.right = destRect.left + ddsd.dwWidth;;
									destRect.bottom = destRect.top + ddsd.dwHeight;
								}
							}




							if (lpT[i]->Blt(&destRect, turrets[i], &srcRect, DDBLT_KEYSRC | DDBLT_WAIT, &fx) != DD_OK)
							{

								errstream << "vxl turret: " << i << " size: " << ddsd.dwWidth << " " << ddsd.dwHeight << "  failed" << endl;
								errstream.flush();
								//exit(-99);
							}


						}

						if (!bPowerUp && i != 0 && imghead.unknown == 0 && bStructure)
						{
							if (lpT[i]) lpT[i]->Release();
						} else
						{
							char ic[50];
							itoa(i, ic, 10);

							PICDATA p;
							p.pic = lpT[i];
							p.x = imghead.x;
							p.y = imghead.y;
							p.wHeight = imghead.cy;
							p.wWidth = imghead.cx;
							p.wMaxWidth = head.cx;
							p.wMaxHeight = head.cy;
							p.bType = PICDATA_TYPE_SHP;
							p.bTerrain = bTerrain;
							if (bIgnoreTerrain) p.bTerrain = 0;


							pics[image + ic] = p;

							//errstream << " --> finished as " << (LPCSTR)(image+ic) << endl;
							//errstream.flush();
						}


					}

					delete[] lpT;


					if (bib) bib->Release();
					if (activeanim)activeanim->Release();
					if (idleanim)idleanim->Release();
					if (activeanim2)activeanim2->Release();
					if (activeanim3)activeanim3->Release();
					if (superanim1)superanim1->Release();
					if (superanim2)superanim2->Release();
					if (superanim3)superanim3->Release();
					if (superanim4)superanim4->Release();
					if (specialanim1)specialanim1->Release();
					if (specialanim2)specialanim2->Release();
					if (specialanim3)specialanim3->Release();
					if (specialanim4)specialanim4->Release();

					for (i = 0; i < 8; i++)
						if (turrets[i])turrets[i]->Release();

				}

				//errstream << " --> Finished" << endl;
				//errstream.flush();
			}

			else
			{
				errstream << "File in theater " << cur_theat << " not found: " << (LPCTSTR)filename << endl;
				errstream.flush();

				missingimages[lpUnittype] = TRUE;
			}

		}
		catch (...)
		{
			errstream << " exception " << endl;
			errstream.flush();
		}


	} else
	{
		filename = image + ".vxl";

		HMIXFILE hMix = FindFileInMix(filename);
		if (hMix == FALSE)
		{
			missingimages[lpUnittype] = TRUE;
			return FALSE;
		}


		int i;

		try
		{

			for (i = 0; i < 8; i++)
			{
				float r_x, r_y, r_z;


				r_x = 300;
				r_y = 0;
				r_z = 45 * i + 90;

				// convert
				const double pi = 3.141592654;
				r_x = r_x / 180.0f * pi;
				r_y = r_y / 180.0f * pi;
				r_z = r_z / 180.0f * pi;



				LPDIRECTDRAWSURFACE4 lpT;//=new(LPDIRECTDRAWSURFACE4[1]);
				LPDIRECTDRAWSURFACE4 lpTurret = NULL;
				LPDIRECTDRAWSURFACE4 lpBarrel = NULL;
				int turret_x, turret_y, turret_x_zmax, turret_y_zmax, barrel_x, barrel_y;

				if (isTrue(rules.sections[lpUnittype].values["Turret"]))
				{
					if (FSunPackLib::SetCurrentVXL(image + "tur.vxl", hMix))
					{
						FSunPackLib::LoadVXLImageInSurface(*m_voxelNormalTables, lightDirection, v.dd, 0, 1, r_x, r_y, r_z, &lpTurret, m_hPalUnitTemp, &turret_x, &turret_y, 0, &turret_x_zmax, &turret_y_zmax, -1, -1);
					}
					if (FSunPackLib::SetCurrentVXL(image + "barl.vxl", hMix))
					{
						FSunPackLib::LoadVXLImageInSurface(*m_voxelNormalTables, lightDirection, v.dd, 0, 1, r_x, r_y, r_z, &lpBarrel, m_hPalUnitTemp, &barrel_x, &barrel_y, 0, NULL, NULL, 0, 0);
					}
				}


				if (!FSunPackLib::SetCurrentVXL(filename, hMix))
				{
					return FALSE;
				}



				int xcenter, ycenter, xcenter_zmax, ycenter_zmax;

				if (!
					FSunPackLib::LoadVXLImageInSurface(*m_voxelNormalTables, lightDirection, v.dd, 0, 1, r_x, r_y, r_z, &lpT, m_hPalUnitTemp, &xcenter, &ycenter, 0, &xcenter_zmax, &ycenter_zmax)
					)
				{
					return FALSE;
				}

				DDSURFACEDESC2 ddsd;
				memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
				ddsd.dwSize = sizeof(DDSURFACEDESC2);
				ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT;
				lpT->GetSurfaceDesc(&ddsd);

				// turret
				if (lpTurret)
				{
					DDSURFACEDESC2 ddsdT;
					memset(&ddsdT, 0, sizeof(DDSURFACEDESC2));
					ddsdT.dwSize = sizeof(DDSURFACEDESC2);
					ddsdT.dwFlags = DDSD_WIDTH | DDSD_HEIGHT;
					lpTurret->GetSurfaceDesc(&ddsdT);

					DDBLTFX fx;
					memset(&fx, 0, sizeof(DDBLTFX));
					fx.dwSize = sizeof(DDBLTFX);

					RECT srcRect, destRect;
					srcRect.left = 0;
					srcRect.right = ddsdT.dwWidth;
					destRect.left = xcenter_zmax - turret_x;
					destRect.right = destRect.left + ddsdT.dwWidth;
					srcRect.top = 0;
					srcRect.bottom = ddsdT.dwHeight;
					destRect.top = ycenter_zmax - turret_y;
					destRect.bottom = destRect.top + ddsdT.dwHeight;

					lpT->Blt(&destRect, lpTurret, &srcRect, DDBLT_KEYSRC | DDBLT_WAIT, &fx);

				}

				// barrel
				if (lpBarrel)
				{
					DDSURFACEDESC2 ddsdB;
					memset(&ddsdB, 0, sizeof(DDSURFACEDESC2));
					ddsdB.dwSize = sizeof(DDSURFACEDESC2);
					ddsdB.dwFlags = DDSD_WIDTH | DDSD_HEIGHT;
					lpBarrel->GetSurfaceDesc(&ddsdB);

					DDSURFACEDESC2 ddsdT;
					memset(&ddsdT, 0, sizeof(DDSURFACEDESC2));
					ddsdT.dwSize = sizeof(DDSURFACEDESC2);
					ddsdT.dwFlags = DDSD_WIDTH | DDSD_HEIGHT;

					if (lpTurret) lpTurret->GetSurfaceDesc(&ddsdT);


					DDBLTFX fx;
					memset(&fx, 0, sizeof(DDBLTFX));
					fx.dwSize = sizeof(DDBLTFX);

					RECT srcRect, destRect;
					srcRect.left = 0;
					srcRect.right = ddsdB.dwWidth;
					destRect.left = xcenter_zmax - barrel_x + (turret_x_zmax - turret_x);
					destRect.right = destRect.left + ddsdB.dwWidth;
					srcRect.top = 0;
					srcRect.bottom = ddsdB.dwHeight;
					destRect.top = ycenter_zmax - barrel_y + (turret_y_zmax - turret_y);
					destRect.bottom = destRect.top + ddsdB.dwHeight;

					lpT->Blt(&destRect, lpBarrel, &srcRect, DDBLT_KEYSRC | DDBLT_WAIT, &fx);

				}

				char ic[50];
				itoa(7 - i, ic, 10);

				errstream << ddsd.dwWidth << " " << ddsd.dwHeight << "\n";
				PICDATA p;
				p.pic = lpT;
				p.x = -xcenter;
				p.y = -ycenter;
				p.wHeight = ddsd.dwHeight;
				p.wWidth = ddsd.dwWidth;
				p.wMaxWidth = ddsd.dwWidth;
				p.wMaxHeight = ddsd.dwHeight;
				p.bType = PICDATA_TYPE_VXL;
				p.bTerrain = 0;

				pics[image + ic] = p;

				errstream << "vxl saved as " << (LPCSTR)image << (LPCSTR)ic << endl;
				errstream.flush();

				if (lpBarrel) lpBarrel->Release();
				if (lpTurret) lpTurret->Release();

				//delete[] lpT;

			}
		}
		catch (...)
		{

		}

	}



	return FALSE;
}
#endif