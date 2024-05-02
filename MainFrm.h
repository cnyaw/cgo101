#pragma once

class CMainFrame :
  public CFrameWindowImpl<CMainFrame>, public CMessageFilter
{
public:
  DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

  CCgoView m_view;
  CCommandBarCtrl m_CmdBar;

  virtual BOOL PreTranslateMessage(MSG* pMsg)
  {
    return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
  } // PreTranslateMessage

  BEGIN_MSG_MAP_EX(CMainFrame)
    MSG_WM_CREATE(OnCreate)
    MSG_WM_DESTROY(OnDestroy)
    COMMAND_ID_HANDLER_EX(ID_APP_EXIT, OnFileExit)
    COMMAND_ID_HANDLER_EX(ID_FILE_NEW, OnFileNew)
    COMMAND_ID_HANDLER_EX(ID_APP_ABOUT, OnAppAbout)
    COMMAND_ID_HANDLER_EX(ID_FILE_NEWCARD, OnNewCard)
    COMMAND_ID_HANDLER_EX(ID_FILE_SELECTGAME, OnSelectGame)
    COMMAND_ID_HANDLER_EX(ID_FILE_SELECTGAMERANDOM, OnRandomSelectGame)
    CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
  END_MSG_MAP()

  //
  // Message handler.
  //

  int OnCreate(LPCREATESTRUCT lpCreateStruct)
  {
    ::srand((unsigned int)::time(NULL));

    //
    // Create command bar window.
    //

    HWND hWndCmdBar = m_CmdBar.Create(
                                 m_hWnd,
                                 rcDefault,
                                 NULL,
                                 ATL_SIMPLE_CMDBAR_PANE_STYLE);

    //
    // Attach menu.
    //

    m_CmdBar.AttachMenu(GetMenu());

    //
    // Load command bar images.
    //

    m_CmdBar.LoadImages(IDR_MAINFRAME);

    //
    // Remove old menu.
    //

    SetMenu(NULL);

    HWND hWndToolBar = CreateSimpleToolBarCtrl(
                         m_hWnd,
                         IDR_MAINFRAME,
                         FALSE,
                         ATL_SIMPLE_TOOLBAR_PANE_STYLE);

    CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
    AddSimpleReBarBand(hWndCmdBar);
    AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

    CreateSimpleStatusBar();

    m_hWndClient = m_view.Create(
                            m_hWnd,
                            rcDefault,
                            NULL,
                            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                            WS_EX_CLIENTEDGE);

    //
    // Register object for message filtering and idle updates.
    //

    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);

    m_view.mGame->NewGame();

    SetCaption();

    ResizeClient(1024, 600);

    SetMsgHandled(FALSE);
    return 0;
  } // OnCreate

  void OnDestroy()
  {
    //
    // Unregister message filtering and idle updates.
    //

    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);

    SetMsgHandled(FALSE);
  } // OnDestroy

  //
  // Command handler.
  //

  void OnAppAbout(UINT uNotifyCode, int nID, CWindow wndCtl)
  {
    CAboutDlg().DoModal();
  } // OnAppAbout

  void OnFileExit(UINT uNotifyCode, int nID, CWindow wndCtl)
  {
    PostMessage(WM_CLOSE);
  } // OnFileExit

  void OnFileNew(UINT uNotifyCode, int nID, CWindow wndCtl)
  {
    m_view.NewGame();
  } // OnFileNew

  void OnNewCard(UINT uNotifyCode, int nID, CWindow wndCtl)
  {
    if (m_view.mGame->NewCard()) {
      m_view.Invalidate(FALSE);
    }
  } // OnNewCard

  void OnRandomSelectGame(UINT uNotifyCode, int nID, CWindow wndCtl)
  {
    cgo::CardGameManager& cgm = cgo::CardGameManager::inst();

    int sel = m_view.mSel;
    while (true) {
      m_view.mSel = rand() % cgm.mGames.size();
      if (m_view.mSel != sel) {
        break;
      }
    }

    m_view.mGame = cgm.mGames[m_view.mSel];
    m_view.mGame->NewGame();
    m_view.Invalidate(FALSE);

    SetCaption();
  } // OnRandomSelectGame

  void OnSelectGame(UINT uNotifyCode, int nID, CWindow wndCtl)
  {
    CHelpDlg sel(m_view.mSel);
    if (IDOK != sel.DoModal()) {
      return;
    }

    if (sel.mSel == m_view.mSel) {
      return;
    }

    m_view.mSel = sel.mSel;
    m_view.mGame = cgo::CardGameManager::inst().mGames[sel.mSel];
    m_view.mGame->NewGame();
    m_view.Invalidate(FALSE);

    SetCaption();
  } // OnSelectGame

  //
  // Helper.
  //

  void SetCaption()
  {
    CString strName;
    strName.LoadString(m_view.mGame->GetName());
    CString strCaption;
    strCaption.Format(
                 TEXT("%s - ( %03d %s )"),
                 CString((LPCTSTR)IDR_MAINFRAME),
                 m_view.mSel + 1,
                 strName);
    SetWindowText(strCaption);
  } // SetCaption
};
