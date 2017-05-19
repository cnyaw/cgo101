
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/09/27 Waync.
//

#pragma once

class CHelpDlg : public CDialogImpl<CHelpDlg>
{
public:

  enum { IDD = IDD_DIALOG2 };

  int mSel;
  CListBox mList;
  CEdit mDesc;

  CHelpDlg(int sel) : mSel(sel)
  {
  } // CHelpDlg

  BEGIN_MSG_MAP_EX(CHelpDlg)
    MSG_WM_INITDIALOG(OnInitDialog)
    COMMAND_ID_HANDLER_EX(IDOK, OnCloseCmd)
    COMMAND_ID_HANDLER_EX(IDCANCEL, OnCloseCmd)
    COMMAND_CODE_HANDLER_EX(LBN_SELCHANGE, OnSelChange)
  END_MSG_MAP()

  BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
  {
    CenterWindow(GetParent());

    mList = GetDlgItem(IDC_LIST1);
    mDesc = GetDlgItem(IDC_EDIT1);

    cgo::CardGameManager& cgm = cgo::CardGameManager::inst();

    CString str;
    for (int i = 0; i < (int)cgm.mGames.size(); ++i) {
      CString strName;
      strName.LoadString(cgm.mGames[i]->GetName());
      str.Format(TEXT("%03d %s"), i + 1, strName);
      mList.AddString(str);
    }

    mList.SetCurSel(mSel);

    CString strDesc;
    strDesc.LoadString(cgm.mGames[mSel]->GetDesc());
    mDesc.SetWindowText(strDesc);

    return TRUE;
  } // OnInitDialog

  void OnCloseCmd(UINT uNotifyCode, int nID, CWindow wndCtl)
  {
    EndDialog(nID);
  } // OnCloseCmd

  void OnSelChange(UINT uNotifyCode, int nID, CWindow wndCtl)
  {
    mSel = mList.GetCurSel();
    CString strDesc;
    strDesc.LoadString(cgo::CardGameManager::inst().mGames[mSel]->GetDesc());
    mDesc.SetWindowText(strDesc);
  } // OnSelChange
};

// end of HelpDlg.h
