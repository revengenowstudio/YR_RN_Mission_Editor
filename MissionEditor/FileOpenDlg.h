#pragma once

class CFileDialogClsid
{
public:
	enum class DialogMode
	{
		SaveFile,
		OpenFile,
	};

	CFileDialogClsid(DialogMode dialogMode,
		const CString& extension = {},
		FILEOPENDIALOGOPTIONS dwFlags = FOS_FILEMUSTEXIST,
		const CString& searchFilter = {},
		HWND hParentWnd = nullptr)  :
		m_dialogMode(dialogMode),
		m_defExt(extension),
		m_filter(searchFilter),
		m_saveFileName(),
		m_path(),
		m_parent(hParentWnd),
		m_flags(dwFlags)
	{
		m_filter.TrimRight("|");
	}

	INT_PTR DoModal();

	void SetSaveFileName(CString input);

	CString GetFilePath();
	CString GetFileName();
	CString GetFileExt();
	CString GetFolderPath();

private:
	void SetFileFilter(CString filter);

	DialogMode m_dialogMode;
	CString m_defExt;
	CString m_filter;
	CString m_saveFileName;
	HWND m_parent;
	DWORD m_flags;
	CString m_path;
	CComPtr<IFileDialog> m_pDlg;
};
