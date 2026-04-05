/*
	FinalSun/FinalAlert 2 Mission Editor

	Copyright (C) 1999-2024 Electronic Arts, Inc.
	Authored by Matthias Wagner

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "StdAfx.h"
#include "Structs.h"
#include "functions.h"

#include "MapTool.h"

ACTIONDATA::~ACTIONDATA()
{
}

void ACTIONDATA::reset() {
	mode = 0;
	type = 0;
	data = 0;
	data2 = 0;
	data3 = 0;
	z_data = 0;
	data_s = "";
	tool.reset();
}

void PICDATA::createVBorder()
{
	ASSERT(!vborder);
	ASSERT(pic && bType != PICDATA_TYPE_BMP);
	_vBorder = std::make_shared<std::vector<VBORDER>>(wMaxHeight);
	vborder = _vBorder->data();
	int k;
	for (k = 0; k < wMaxHeight; k++) {
		int l, r;
		GetDrawBorder(static_cast<BYTE*>(pic), wMaxWidth, k, l, r, 0);
		vborder[k].left = l;
		vborder[k].right = r;
	}
}

void XCString::SetString(const CHAR* inputStr)
{
	if (!inputStr || strlen(inputStr) == 0) {
		len = 0;
		if (auto str = std::exchange( wString, nullptr)) {
			delete[] str;
		}
		return;
	}
	auto const requiredWchars = MultiByteToWideChar(CP_ACP, 0, inputStr, -1, nullptr, 0);
	if (requiredWchars <= 0) {
		this->cString.Empty();
		return;
	}
	if (this->wString) {
		delete[] this->wString;
	}

	this->wString = new WCHAR[requiredWchars];

	MultiByteToWideChar(CP_ACP, 0, inputStr, -1, this->wString, requiredWchars);

	this->len = requiredWchars - 1; // no \0
	this->cString = inputStr;
}

void XCString::SetString(const WCHAR* wString, int len)
{
	this->len = len;

	if (this->wString) {
		delete[] this->wString;
	}

	bUsedDefault = FALSE;

	this->wString = new(WCHAR[len + 1]);
	memset(this->wString, 0, (len + 1) * 2);
	memcpy(this->wString, wString, len * 2);

	auto bufferSize = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, this->wString, len + 1, nullptr, 0, NULL, &bUsedDefault);
	if (bufferSize == 0) {
		cString = "";
		return; // failed
	}

	std::vector<BYTE> bByte(bufferSize + 4, 0);
	if (WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, this->wString, len + 1, (LPSTR)bByte.data(), bufferSize, NULL, &bUsedDefault) == 0) {
		cString = "";
		return; // failed
	}
	cString = bByte.data();
}
