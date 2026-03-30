#include "stdafx.h"
#include "IniContentEditor.h"
#include "functions.h"
#include <sstream>

BEGIN_MESSAGE_MAP(CIniContentEditor, CDialog)
	ON_EN_KILLFOCUS(IDC_INI_E_CUR_SEC_VAL, onSectionChanged)
END_MESSAGE_MAP()

CIniContentEditor::CIniContentEditor(CWnd* pParent)
	: CDialog(IDD, pParent)
{
}

BOOL CIniContentEditor::OnInitDialog()
{
	if (!CDialog::OnInitDialog()) {
		return FALSE;
	}

	translateUI();

	m_sectionValue.SetWindowText(m_sectionID);

	CFont font;
	font.CreatePointFont(100, _T("Tahoma"));
	m_contentText.SetFont(&font);
	m_contentText.SetWindowText(m_content);

    return TRUE;
}

void CIniContentEditor::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_INI_E_CUR_SEC_VAL, m_sectionValue);
	DDX_Control(pDX, IDC_SECTION_TEXT, m_contentText);
}

void CIniContentEditor::translateUI()
{
	SetWindowText(GetLanguageStringACP("IniContentCaption"));

	TranslateDlgItem(*this, IDC_INI_E_CUR_SEC, "IniContentCurrentSection");
	TranslateDlgItem(*this, IDC_INI_E_DISABLE_VALIDATION, "IniContentDisableValidation");
	TranslateDlgItem(*this, IDOK, "OK");
	TranslateDlgItem(*this, IDCANCEL, "Cancel");
}

BOOL CIniContentEditor::PreTranslateMessage(MSG* pMsg)
{
	int ret = -1;
	if (pMsg->message == WM_KEYDOWN) {
		ret = onMessageKeyDown(pMsg);
	}
	return ret < 0 ? this->CDialog::PreTranslateMessage(pMsg) : ret;
}

void CIniContentEditor::OnOK()
{
	auto const pCheckBox = reinterpret_cast<CButton*>(GetDlgItem(IDC_INI_E_DISABLE_VALIDATION));
	auto const validationOff = pCheckBox && pCheckBox->GetCheck() == BST_CHECKED;

	CString curIniContent;
	m_contentText.GetWindowText(curIniContent);

	if (!validationOff) {
		// TODO: validate ini text
		// 
		//if (fail) {
		//	return;
		//}
	}
	m_content = curIniContent;

	std::string line;
	std::istringstream parseStream(m_content.operator LPCSTR());
	while (std::getline(parseStream, line)) {
		auto const eqlPos = line.find('=');
		// TODO: handle ; and trim
		if (eqlPos == line.npos) {
			continue;
		}
		auto key = CString(line.data(), eqlPos);
		auto value = CString(line.data() + eqlPos + 1);
		key.Trim();
		value.Trim();
		m_section.SetString(key, value);
	}

	EndDialog(IDOK);
}

BOOL CIniContentEditor::onMessageKeyDown(MSG* pMsg)
{
	switch (pMsg->wParam) {
		default:
			return -1;
		case VK_RETURN:
		{
			switch (::GetDlgCtrlID(pMsg->hwnd)) {
			default:
				break;// never exist window (default -1) even nothing did
			case IDC_SECTION_TEXT: 
				this->onTextInsertLineEnd();
				break;
			}
		}
	}
	return TRUE;
}

void CIniContentEditor::onTextInsertLineEnd()
{
	m_contentText.ReplaceSel(_T("\r\n"), TRUE);
}

void CIniContentEditor::onSectionChanged()
{
	CString newSecName;
	m_sectionValue.GetWindowText(newSecName);
	newSecName.Trim();

	if (newSecName == m_sectionID) {
		return;
	}
	// msg box to hint sure to change section ID ?
	auto const allow = MessageBox(GetLanguageStringACP("You just changed section ID, are you sure?"),
		GetLanguageStringACP("Warning"), MB_YESNO) == IDYES;
	if (!allow) {
		m_sectionValue.SetWindowText(m_sectionID);
	}
	m_sectionID = newSecName;
}
