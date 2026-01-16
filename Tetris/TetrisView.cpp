
// TetrisView.cpp: CTetrisView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tetris.h"
#endif

#include "TetrisDoc.h"
#include "TetrisView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTetrisView

IMPLEMENT_DYNCREATE(CTetrisView, CView)

BEGIN_MESSAGE_MAP(CTetrisView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_BN_CLICKED(101, &CTetrisView::OnStartBtnClicked)
	ON_BN_CLICKED(102, &CTetrisView::OnExitBtnClicked)
END_MESSAGE_MAP()

// CTetrisView 생성/소멸

CTetrisView::CTetrisView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	





}

CTetrisView::~CTetrisView()
{
}

BOOL CTetrisView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CTetrisView 그리기

void CTetrisView::OnDraw(CDC* pDC)
{
	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	/*pDoc->myadd();

	CClientDC dc(this);
	LOGBRUSH lb;

	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(100, 100, 100);

	CPen NewPen;
	// 여기서 PS_JOIN_MITER가 추가 되었는데
	// 이 외에도, ROUND, BEVEL이 존재.
	NewPen.CreatePen(PS_GEOMETRIC | PS_ENDCAP_FLAT | PS_JOIN_MITER, 20, &lb);
	CPen* pOldPen = dc.SelectObject(&NewPen);

	CBrush NewBrush(RGB(0, 0, 192));
	CBrush* pOldBrush = dc.SelectObject(&NewBrush);

	// POINT 변수를 생성해 각각의 주소를 기억하게 한 후,
	POINT arPt[5] = { {20, 20}, {140, 20}, {140, 140}, {70, 180}, {20, 140} };

	POINT arPt[5] = {  };
	Block_Type_1;
	// Polygon 함수를 사용해 이전과 같이 도형을 그려 주고 있다.
	dc.Polygon(arPt, 5);

	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
	*/

	CClientDC dc(this);
	CPen pen, * curpen;
	pen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	curpen = dc.SelectObject(&pen);
	// 게임 보드 구분 세로선(게임보드 픽셀 185~685)
	dc.MoveTo(182, 0);
	dc.LineTo(182, 1000);
	dc.MoveTo(588, 0);
	dc.LineTo(588, 1000);
	// 다음 블록 표시 칸
	dc.Rectangle(10, 40, 168, 198);
	dc.TextOutW(50, 20, _T("NEXT BRICK"));
	// 점수판 칸
	dc.Rectangle(10, 233, 168, 295);
	dc.TextOutW(65, 210, _T("SCORE"));
	// 타이머 칸
	dc.Rectangle(600, 100, 775, 160);
	dc.TextOutW(672, 78, _T("TIMER"));

	CPen pen2;
	pen2.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	curpen = dc.SelectObject(&pen2);
	dc.Rectangle(185, 0, 225, 40);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CTetrisView 인쇄

BOOL CTetrisView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CTetrisView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CTetrisView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CTetrisView 진단

#ifdef _DEBUG
void CTetrisView::AssertValid() const
{
	CView::AssertValid();
}

void CTetrisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTetrisDoc* CTetrisView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTetrisDoc)));
	return (CTetrisDoc*)m_pDocument;
}
#endif //_DEBUG


// CTetrisView 메시지 처리기

int CTetrisView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	mGameStartBtn.Create(_T("Game Start"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(240, 400, 540, 500), this, 101);
	mExitBtn.Create(_T("Exit"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(240, 520, 540, 620), this, 102);
	mPauseBtn.Create(_T("||"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(720, 10, 770, 60), this, 103);
	mResumeBtn.Create(_T("Resume"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(240, 400, 540, 500), this, 104);
	mResetBtn.Create(_T("Reset"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(240, 400, 540, 500), this, 105);

	GetDlgItem(104)->ShowWindow(SW_HIDE);
	GetDlgItem(105)->ShowWindow(SW_HIDE);
	GetDlgItem(104)->EnableWindow(FALSE);
	GetDlgItem(105)->EnableWindow(FALSE);

	return 0;
}

void CTetrisView::OnStartBtnClicked()
{
	// TODO: 여기에 구현 코드 추가.

	GetDlgItem(101)->ShowWindow(SW_HIDE);
	GetDlgItem(102)->ShowWindow(SW_HIDE);
	GetDlgItem(101)->EnableWindow(FALSE);
	GetDlgItem(102)->EnableWindow(FALSE);
	Sleep(1000);
	GetDlgItem(104)->ShowWindow(SW_SHOWNORMAL);
	GetDlgItem(102)->ShowWindow(SW_SHOWNORMAL);
	GetDlgItem(104)->EnableWindow(TRUE);
	GetDlgItem(102)->EnableWindow(TRUE);
}

void CTetrisView::OnExitBtnClicked()
{
	// TODO: 여기에 구현 코드 추가.
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}

void CTetrisView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	// 상하좌우 키를 구분해 메시지를 출력합니다.

}
