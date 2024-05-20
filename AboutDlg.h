#pragma once

class CAboutDlg : public CSimpleDialog<IDD_ABOUTBOX>
{
public:
  CHyperLink m_homepage;

  BEGIN_MSG_MAP(CAboutDlg)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    CHAIN_MSG_MAP(CSimpleDialog)
  END_MSG_MAP()

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    m_homepage.SubclassWindow(GetDlgItem(IDC_STATIC1));
    m_homepage.SetHyperLinkExtendedStyle(
                 HLINK_NOTOOLTIP | HLINK_UNDERLINEHOVER,
                 HLINK_NOTOOLTIP | HLINK_UNDERLINEHOVER);
    m_homepage.SetHyperLink(_T("https://smallworld.idv.tw/"));

    CenterWindow(GetParent());
    return TRUE;
  } // OnInitDialog
};
