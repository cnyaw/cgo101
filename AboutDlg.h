#pragma once

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
  enum { IDD = IDD_ABOUTBOX };

  CHyperLink m_homepage;

  BEGIN_MSG_MAP(CAboutDlg)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
    COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
  END_MSG_MAP()

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    m_homepage.SubclassWindow(GetDlgItem(IDC_STATIC1));
    m_homepage.SetHyperLinkExtendedStyle(
                 HLINK_NOTOOLTIP | HLINK_UNDERLINEHOVER,
                 HLINK_NOTOOLTIP | HLINK_UNDERLINEHOVER);
    m_homepage.SetHyperLink(_T("http://www.smallworld.idv.tw/"));

    CenterWindow(GetParent());
    return TRUE;
  } // OnInitDialog

  LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
    EndDialog(wID);
    return 0;
  }
};
