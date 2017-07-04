#pragma once

class GdiCardGameRenderer : public cgo::CardGameRenderer
{
public:

  CDCHandle dc;

  CBitmap mBkgnd;
  CBitmap mCards;
  CBitmap mBmpDeck;
  CBitmap mBmpCover;

  GdiCardGameRenderer()
  {
    mBkgnd.LoadBitmap((LPCTSTR)IDB_BITMAP1);
    mCards.LoadBitmap((LPCTSTR)IDB_BITMAP2);
    mBmpDeck.LoadBitmap((LPCTSTR)IDB_BITMAP3);
    mBmpCover.LoadBitmap((LPCTSTR)IDB_BITMAP4);
  }

  void DrawCard(int x, int y, int card)
  {
    int color = GetColor(card);
    int number = GetNumber(card);
    CDC memdc;
    memdc.CreateCompatibleDC(dc);
    memdc.SelectBitmap(mCards);
    dc.BitBlt(x, y, CARD_WIDTH, CARD_HEIGHT, memdc, (number - 1) * CARD_WIDTH, color * CARD_HEIGHT, SRCCOPY);
  }

  void DrawCover(int x, int y)
  {
    CDC memdc;
    memdc.CreateCompatibleDC(dc);
    memdc.SelectBitmap(mBmpCover);
    dc.BitBlt(x, y, CARD_WIDTH, CARD_HEIGHT, memdc, 0, 0, SRCCOPY);
  }

  void DrawEmptyCard(int x, int y)
  {
    CDC memdc;
    memdc.CreateCompatibleDC(dc);
    memdc.SelectBitmap(mBmpDeck);
    dc.BitBlt(x, y, CARD_WIDTH, CARD_HEIGHT, memdc, 0, 0, SRCCOPY);
  }

  void EraseBkgnd()
  {
    RECT rc = {rcClient.left, rcClient.top, rcClient.right, rcClient.bottom};
    CBrush br;
    br.CreatePatternBrush(mBkgnd);
    dc.FillRect(&rc, br);
  }
};

class CCgoView : public CWindowImpl<CCgoView>
{
public:
  DECLARE_WND_CLASS(NULL)

  int mSel;
  cgo::CardGame* mGame;

  void NewGame()
  {
    mGame->NewGame();
    Invalidate(FALSE);
  } // NewGame

  BOOL PreTranslateMessage(MSG* pMsg)
  {
    pMsg;
    return FALSE;
  } // PreTranslateMessage

  BEGIN_MSG_MAP_EX(CCgoView)
    MSG_WM_CREATE(OnCreate)
    MSG_WM_ERASEBKGND(OnEraseBkgnd)
    MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
    MSG_WM_LBUTTONDOWN(OnLButtonDown)
    MSG_WM_MOUSEMOVE(OnMouseMove)
    MSG_WM_LBUTTONUP(OnLButtonUp)
    MSG_WM_PAINT(OnPaint)
  END_MSG_MAP()

  //
  // Message handler.
  //

  int OnCreate(LPCREATESTRUCT lpCreateStruct)
  {
    cgo::CardGameManager& cgm = cgo::CardGameManager::inst();

    mSel = rand() % cgm.mGames.size();
    mGame = cgm.mGames[mSel];

    SetMsgHandled(FALSE);
    return 0;
  } // OnCreate

  BOOL OnEraseBkgnd(CDCHandle dc)
  {
    return TRUE;
  } // OnEraseBkgnd

  void OnLButtonDblClk(UINT nFlags, CPoint point)
  {
    RECT rcClient;
    GetClientRect(&rcClient);

    cgo::POINT_t pt(point.x, point.y);
    cgo::RECT_t rc(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);

    if (mGame->OnMouseDown(pt, rc)) {
      Invalidate(FALSE);
    }
  } // OnLButtonDblClk

  void OnLButtonDown(UINT nFlags, CPoint point)
  {
    SetCapture();

    RECT rcClient;
    GetClientRect(&rcClient);

    cgo::POINT_t pt(point.x, point.y);
    cgo::RECT_t rc(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);

    if (mGame->OnMouseDown(pt, rc)) {
      Invalidate(FALSE);
    }
  } // OnLButtonDown

  void OnLButtonUp(UINT nFlags, CPoint point)
  {
    if (GetCapture() != m_hWnd) {
      return;
    }

    ReleaseCapture();

    RECT rcClient;
    GetClientRect(&rcClient);

    cgo::POINT_t pt(point.x, point.y);
    cgo::RECT_t rc(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);

    if (mGame->OnMouseUp(pt, rc)) {
      Invalidate(FALSE);
    }
  } // OnLButtonUp

  void OnMouseMove(UINT nFlags, CPoint point)
  {
    RECT rcClient;
    GetClientRect(&rcClient);

    cgo::POINT_t pt(point.x, point.y);
    cgo::RECT_t rc(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);

    if (mGame->OnMouseMove(pt, rc)) {
      Invalidate(FALSE);
    }
  } // OnMouseMove

  void OnPaint(CDCHandle hdc)
  {
    CPaintDC dc(m_hWnd);
    GdiCardGameRenderer g;
    RECT rcClient;
    GetClientRect(&rcClient);
    g.rcClient = cgo::RECT_t(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
    CMemoryDC memdc(dc, rcClient);
    g.dc = memdc;
    g.EraseBkgnd();
    mGame->DrawGame(g);
  } // OnPaint
};
