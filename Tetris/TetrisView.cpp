
// TetrisView.cpp: CTetrisView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include <string>
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
	ON_BN_CLICKED(103, &CTetrisView::OnPauseBtnClicked)
	ON_BN_CLICKED(104, &CTetrisView::OnResumeBtnClicked)
	ON_BN_CLICKED(105, &CTetrisView::OnRestartBtnClicked)
	ON_WM_TIMER()
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
	
	if (pDoc->mGameStaus == 0) {
		pDoc->CreateUI(pDC);
		pDoc->DrawBoard(pDC);
	}

	if (pDoc->mGameStaus == 1) {
		mTimerStr.Format(_T("%d"), pDoc->mTimer);
		pDC->TextOutW(680, 125, mTimerStr);
	}
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
	
	// 버튼 생성
	mGameStartBtn.Create(_T("Game Start"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(240, 400, 540, 500), this, 101);
	mExitBtn.Create(_T("Exit"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(240, 520, 540, 620), this, 102);
	mPauseBtn.Create(_T("||"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(720, 10, 770, 60), this, 103);
	mResumeBtn.Create(_T("Resume"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(240, 400, 540, 500), this, 104);
	mRestartBtn.Create(_T("Restart"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(240, 400, 540, 500), this, 105);

	GetDlgItem(103)->ShowWindow(SW_HIDE);
	GetDlgItem(104)->ShowWindow(SW_HIDE);
	GetDlgItem(105)->ShowWindow(SW_HIDE);
	GetDlgItem(103)->EnableWindow(FALSE);
	GetDlgItem(104)->EnableWindow(FALSE);
	GetDlgItem(105)->EnableWindow(FALSE);

	return 0;
}

void CTetrisView::OnStartBtnClicked()
{
	// TODO: 여기에 구현 코드 추가.

	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDC* pDC = GetDC();

	GetDlgItem(101)->ShowWindow(SW_HIDE);
	GetDlgItem(102)->ShowWindow(SW_HIDE);
	GetDlgItem(103)->ShowWindow(SW_SHOWNORMAL);
	GetDlgItem(101)->EnableWindow(FALSE);
	GetDlgItem(102)->EnableWindow(FALSE);
	GetDlgItem(103)->EnableWindow(TRUE);

	pDoc->mGameStaus = 1; //게임 시작 상태로 변경
	pDoc -> mTimer = 0;
	SetTimer(1, 1000, NULL); //1초 간격 타이머 시작
	pDoc->DrawBoard(pDC);
	pDoc->CreateBlock(pDoc->blockType0, pDC);
}

void CTetrisView::OnExitBtnClicked()
{
	// TODO: 여기에 구현 코드 추가.

	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}

void CTetrisView::OnPauseBtnClicked()
{
	// TODO: 여기에 구현 코드 추가.

	GetDlgItem(104)->ShowWindow(SW_SHOWNORMAL);
	GetDlgItem(102)->ShowWindow(SW_SHOWNORMAL);
	GetDlgItem(104)->EnableWindow(TRUE);
	GetDlgItem(102)->EnableWindow(TRUE);
}

void CTetrisView::OnResumeBtnClicked()
{
	// TODO: 여기에 구현 코드 추가.
	GetDlgItem(104)->ShowWindow(SW_HIDE);
	GetDlgItem(102)->ShowWindow(SW_HIDE);
	GetDlgItem(104)->EnableWindow(FALSE);
	GetDlgItem(102)->EnableWindow(FALSE);

}

void CTetrisView::OnRestartBtnClicked()
{
	// TODO: 여기에 구현 코드 추가.
}

void CTetrisView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDC* pDC = GetDC();
	int status = 0;

	if (pDoc->mGameStaus == 1) {
		pDoc->mTimer += 1;
		status = pDoc->DropBlock(&pDoc->curBlock, pDC);
		if (status == 1) {
			status = 0;
			pDoc->CreateBlock(pDoc->blockType1, pDC);
		}
		Invalidate(FALSE);
	}
	else {
		KillTimer(1);
	}

	CView::OnTimer(nIDEvent);
}
