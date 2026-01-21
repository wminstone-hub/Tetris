
// TetrisDoc.cpp: CTetrisDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tetris.h"
#endif

#include "TetrisDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTetrisDoc

IMPLEMENT_DYNCREATE(CTetrisDoc, CDocument)

BEGIN_MESSAGE_MAP(CTetrisDoc, CDocument)
END_MESSAGE_MAP()


// CTetrisDoc 생성/소멸

CTetrisDoc::CTetrisDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CTetrisDoc::~CTetrisDoc()
{
}

BOOL CTetrisDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CTetrisDoc serialization

void CTetrisDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CTetrisDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CTetrisDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CTetrisDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CTetrisDoc 진단

#ifdef _DEBUG
void CTetrisDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTetrisDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTetrisDoc 명령

int CTetrisDoc::myadd()
{
	// TODO: 여기에 구현 코드 추가.
	return 0;
}

int CTetrisDoc::Render(CDC *pDC)
{
	// TODO: 여기에 구현 코드 추가.
	
	CBrush brush(RGB(180, 180, 180));
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&brush);
	pDC->SelectObject(&pen);
	
	for (mTileX = 0; mTileX < BOARD_WIDTH; mTileX++) {
		for (mTileY = 0; mTileY < BOARD_HEIGHT; mTileY++) {
			pDC->Rectangle(
				BOARD_WIDTH_OFFSET + mTileX * BLOCK_SIZE,
				BOARD_HIEGHT_OFFSET + (mTileY +1) * BLOCK_SIZE,
				BOARD_WIDTH_OFFSET + (mTileX +1) * BLOCK_SIZE,
				BOARD_HIEGHT_OFFSET + mTileY * BLOCK_SIZE
			);
		}
	}
	

	return 0;
}

void CTetrisDoc::OnCreateBoard(CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.
	//보드 외곽선 및 구분선 그리기 펜
	CPen boardPen(PS_SOLID, 3, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&boardPen);

	pDC->SelectObject(&boardPen);
	// 게임 보드 구분 세로선(게임보드 픽셀 185~685)
	pDC->MoveTo(182, 0);
	pDC->LineTo(182, 1000);
	pDC->MoveTo(588, 0);
	pDC->LineTo(588, 1000);
	// 다음 블록 표시 칸
	pDC->Rectangle(10, 40, 168, 198);
	pDC->TextOutW(50, 20, _T("NEXT BRICK"));
	// 점수판 칸
	pDC->Rectangle(10, 233, 168, 295);
	pDC->TextOutW(65, 210, _T("SCORE"));
	// 타이머 칸
	pDC->Rectangle(600, 100, 775, 160);
	pDC->TextOutW(672, 78, _T("TIMER"));

	//게임 보드 생성
	int rectStartX = 185;
	int rectStartY = 0;
	int rectEndX = 225;
	int rectEndY = 40;
	CRect square(rectStartX + 1, rectStartY + 1, rectEndX - 1, rectEndY - 1);

	pDC->SelectObject(pOldPen);

	//한 칸 그리기 펜
	CPen squarePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&squarePen);
	//보드 색상 브러시
	CBrush boardBrush(boardColor);
	CBrush floorBrush(floorColor);
	CBrush* pOldBrush = pDC->SelectObject(&boardBrush);

	for (int curHight = 0; curHight <= BOARD_HEIGHT; curHight++) {
		for (int curWidth = 0; curWidth < BOARD_WIDTH; curWidth++) {
			if (curHight == BOARD_HEIGHT) {
				pDC->SelectObject(&floorBrush);
			}
			pDC->Rectangle(rectStartX, rectStartY, rectEndX, rectEndY);
			rectStartX += BLOCK_SIZE;
			rectEndX += BLOCK_SIZE;
		}
		rectStartX = 185;
		rectEndX = 225;
		rectStartY += BLOCK_SIZE;
		rectEndY += BLOCK_SIZE;
	}

	pDC->SelectObject(pOldPen);
	
	DeleteObject(&boardPen);
	DeleteObject(&squarePen);

	pDC->SelectObject(pOldBrush);

	DeleteObject(&boardBrush);
	DeleteObject(&floorBrush);
	return;
}
