#include "StdAfx.h"
#include "FileOpenDlg.h"
#include "functions.h"
#include "inlines.h"

void CFileDialogClsid::SetSaveFileName(CString input)
{
	m_saveFileName = input;
}

CString CFileDialogClsid::GetFilePath()
{
	return m_fileFullPath;
}

CString CFileDialogClsid::GetFileName()
{
	CString full = m_fileFullPath;
	int pos = full.ReverseFind(L'\\');
	return pos < 0 ? full : full.Mid(pos + 1);
}

CString CFileDialogClsid::GetFileExt()
{
	CString name = GetFileName();
	int pos = name.ReverseFind(L'.');
	return pos < 0 ? CString() : name.Mid(pos + 1);
}

CString CFileDialogClsid::GetFolderPath()
{
	CString full = m_fileFullPath;
	int pos = full.ReverseFind(L'\\');
	return pos < 0 ? CString() : full.Left(pos);
}

void CFileDialogClsid::SetFileFilter(CString filter)
{
	std::vector<COMDLG_FILTERSPEC> specs;
	std::vector<CString> tokens = Split(filter, '|');

	// m_filter trailing '|' is removed in CFileDialogClsid CTOR
	std::vector<std::wstring> storage;

	for (size_t i = 0; i + 1 < tokens.size(); i += 2) {
		auto name = utf8ToUtf16(tokens[i]);
		auto filter = utf8ToUtf16(tokens[i + 1]);
		storage.push_back(name);
		storage.push_back(filter);
		specs.push_back({
			storage[storage.size() - 2].c_str(),
			storage[storage.size() - 1].c_str()
			});
	}
	m_pDlg->SetFileTypes(static_cast<UINT>(specs.size()), specs.data());
}

INT_PTR CFileDialogClsid::DoModal()
{
	auto const isSaveDialogMode = m_dialogMode == DialogMode::SaveFile;
	const CLSID clsid = isSaveDialogMode
		? CLSID_FileSaveDialog : CLSID_FileOpenDialog;

	HRESULT hr = CoCreateInstance(clsid, nullptr,
		CLSCTX_ALL, IID_PPV_ARGS(&m_pDlg));

	if (FAILED(hr)) {
		return IDCANCEL;
	}

	if (isSaveDialogMode) {
		std::wstring fileName = utf8ToUtf16(m_saveFileName);
		m_pDlg->SetFileName(fileName.c_str());
	}

	SetFileFilter(m_filter);

	if (!m_defExt.IsEmpty()) {
		std::wstring defExt = utf8ToUtf16(m_defExt);
		m_pDlg->SetDefaultExtension(defExt.c_str());
	}

	DWORD dwOpt = 0;
	m_pDlg->GetOptions(&dwOpt);
	m_pDlg->SetOptions(dwOpt | m_flags);

	hr = m_pDlg->Show(m_parent);
	if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
		return IDCANCEL;
	}
	if (FAILED(hr)) {
		return IDCANCEL;
	}

	CComPtr<IShellItem> pItem;
	if (FAILED(m_pDlg->GetResult(&pItem))) {
		return IDCANCEL;
	}

	PWSTR psz = nullptr;
	if (FAILED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &psz))) {
		return IDCANCEL;
	}
	m_fileFullPath = psz;
	CoTaskMemFree(psz);
	return IDOK;
}
