
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

	InitBoardStatus();
	InitBlockBox(blockBox);
	

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

//////////////////////////////////////////////초기화 함수들

void CTetrisDoc::InitBoardStatus()
{
	// TODO: 여기에 구현 코드 추가.
	for (int i = 0; i < BOARD_HEIGHT + FLOOR_HEIGHT; i++) {
		boardStatus[i][0] = -1; //왼쪽 벽
		boardStatus[i][BOARD_WIDTH + WALL_WIDTH - 1] = -1; //오른쪽 벽
	}
	for (int j = 0; j < BOARD_WIDTH + WALL_WIDTH; j++) {
		boardStatus[BOARD_HEIGHT + CEILING_HEIGHT][j] = -1; //바닥
	}
}

void CTetrisDoc::InitBlockBox(std::vector<int>& blockBox)
{
	// TODO: 여기에 구현 코드 추가.
	curIdx = 0;

	for (int i = 0; i <= 6; ++i) {
		blockBox.push_back(i);
	}

	std::random_device rd;
	engine.seed(rd());

	shuffleBox();
}

//////////////////////////////////////////////게임 상태 관련 함수들

int CTetrisDoc::IsGameOvered(CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.
	for (int i = 1; i <= BOARD_WIDTH; i++) {
		if (boardStatus[1][i] != 0) {
			//게임 오버 처리
			mGameStatus = 3; //게임오버 상태로 변경
			pDC->TextOutW(300, 300, _T("Game Over"));
			return 1;
		}
	}
	return 0;
}

//////////////////////////////////////////////랜덤 블록 생성 함수 (7-bag)

void CTetrisDoc::shuffleBox()
{
	// TODO: 여기에 구현 코드 추가.
	shuffle(blockBox.begin(), blockBox.end(), engine);	//7-bag 섞기
	curIdx = 0;											//인덱스 초기화
}

int CTetrisDoc::nextBlock()
{
	// TODO: 여기에 구현 코드 추가.
	if (curIdx >= blockBox.size()) { //끝까지 도달했으면 다시 섞기
		shuffleBox();
	}
	return blockBox[curIdx++]; //다음 블록 반환 후 인덱스 증가
}

//////////////////////////////////////////////블록 그리기 관련 함수들

void CTetrisDoc::CreateUI(CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.
	//보드 외곽선 및 구분선 그리기 펜
	CPen boardPen(PS_SOLID, 3, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&boardPen);

	pDC->SelectObject(&boardPen);
	// 게임 보드 구분 세로선(게임보드 픽셀 185~588)
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
	pDC->TextOutW(667, 78, _T("TIMER"));

	pDC->SelectObject(pOldPen);

	DeleteObject(&boardPen);
}


void CTetrisDoc::DrawBoard(CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.
	
	//한 칸 그리기 펜
	CPen squarePen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&squarePen);

	//게임 보드 생성
	int rectStartX = BOARD_WIDTH_OFFSET;
	int rectStartY = BOARD_HIEGHT_OFFSET;
	int rectEndX = BOARD_WIDTH_OFFSET + BLOCK_SIZE;
	int rectEndY = BLOCK_SIZE;

	//보드 색상 브러시
	CBrush boardBrush(boardColor);
	CBrush ceilingBrush(ceilingColor);
	CBrush floorBrush(floorColor);
	CBrush* pOldBrush = pDC->SelectObject(&boardBrush);

	//보드 그리기
	for (int curHeight = 0; curHeight < BOARD_HEIGHT + CEILING_HEIGHT + FLOOR_HEIGHT; curHeight++) {
		for (int curWidth = 0; curWidth < BOARD_WIDTH; curWidth++) {
			if (curHeight == 0 || curHeight == 1) {
				pDC->SelectObject(&ceilingBrush);
			}
			else if (curHeight == BOARD_HEIGHT + CEILING_HEIGHT) {
				pDC->SelectObject(&floorBrush);
			}
			else {
				pDC->SelectObject(&boardBrush);
			}
				pDC->Rectangle(rectStartX, rectStartY, rectEndX, rectEndY);
				rectStartX += BLOCK_SIZE;
				rectEndX += BLOCK_SIZE;
		}
		rectStartX = BOARD_WIDTH_OFFSET;
		rectEndX = BOARD_WIDTH_OFFSET + BLOCK_SIZE;
		rectStartY += BLOCK_SIZE;
		rectEndY += BLOCK_SIZE;
	}

	//펜, 브러시 원상복구
	pDC->SelectObject(pOldPen);
	DeleteObject(&squarePen);

	pDC->SelectObject(pOldBrush);
	DeleteObject(&boardBrush);
	DeleteObject(&floorBrush);
	return;
}

void CTetrisDoc::PaintTile(Tile tile, COLORREF color, CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.

	//색상 칠 할 부분 Rect 계산
	int rectStartX = BOARD_WIDTH_OFFSET + (tile.x - 1) * BLOCK_SIZE;
	int rectStartY = BOARD_HIEGHT_OFFSET + tile.y * BLOCK_SIZE;
	int rectEndX = BOARD_WIDTH_OFFSET + (tile.x) * BLOCK_SIZE;
	int rectEndY = BOARD_HIEGHT_OFFSET + (tile.y + 1) * BLOCK_SIZE;
	//테두리 제외한 내부 영역
	CRect paintSquare(rectStartX + 1, rectStartY + 1, rectEndX - 1, rectEndY - 1);

	CBrush NewBrush(color);
	CBrush* pOldBrush = pDC->SelectObject(&NewBrush);

	pDC->FillRect(paintSquare, &NewBrush);

	pDC->SelectObject(pOldBrush);
	DeleteObject(&NewBrush);
}

void CTetrisDoc::PaintBlock(Block block, COLORREF blockColor, CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.

	for (Tile tile : block.tile) {
		PaintTile(tile, blockColor, pDC);
	}
}
void CTetrisDoc::PaintBlock(Block block, CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.

	for (Tile tile : block.tile) {
		if (tile.y == 0 || tile.y == 1) //천장 부분은 천장 색상으로 칠하기
			PaintTile(tile, ceilingColor, pDC);
		else
			PaintTile(tile, boardColor, pDC);
	}
}

//////////////////////////////////////////////블록 동작 관련 함수들

void CTetrisDoc::CreateBlock(CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.
	

	if (IsGameOvered(pDC)) {//게임 오버 검사
		return;
	}
	
	//현재 블록 설정
	switch (nextBlock()) {
		case 0:
			curBlock = blockType0;
			break;
		case 1:
			curBlock = blockType1;
			break;
		case 2:
			curBlock = blockType2;
			break;
		case 3:
			curBlock = blockType3;
			break;
		case 4:
			curBlock = blockType4;
			break;
		case 5:
			curBlock = blockType5;
			break;
		case 6:
			curBlock = blockType6;
			break;
		default:
			break;
	}
	PaintBlock(curBlock, curBlock.blockColor, pDC);
	return;
}
 
int CTetrisDoc::DropBlock(Block* curBlock, CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.
	if (CheckCollision(boardStatus, *curBlock, 0)) { //아래 충돌 검사
		EmbedBlock(boardStatus, *curBlock, pDC);
		CreateBlock(pDC);
		return 1;
	}
	PaintBlock(*curBlock, pDC); //이전 블록 지우기
	for (int i = 0; i < 4; i++) {
		curBlock->tile[i].y += 1; //블록 한 칸 아래로 이동
	}
	PaintBlock(*curBlock, curBlock->blockColor, pDC); //새로운 블록 그리기

	return 0;
}

int CTetrisDoc::MoveBlockDirectionX(Block* curBlock, int direction, CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.
	if (CheckCollision(boardStatus, *curBlock, direction)) { //해당 방향 충돌 검사
		return 1; 
	}
	PaintBlock(*curBlock, pDC); //이전 블록 지우기
	if (direction == 1) { //왼쪽 이동
		for (int i = 0; i < 4; i++) {
			curBlock->tile[i].x -= 1; //블록 왼쪽으로 이동
		}
	}
	else if (direction == 2) { //오른쪽 이동
		for (int i = 0; i < 4; i++) {
			curBlock->tile[i].x += 1; //블록 오른쪽으로 이동
		}

	}
	else
		return -1; //오류
	PaintBlock(*curBlock, curBlock->blockColor, pDC); //새로운 블록 그리기
		
	return 0;
}

// 충돌 검사 함수 (dir == 0 : 아래, dir == 1 : 왼쪽, dir == 2 : 오른쪽)
int CTetrisDoc::CheckCollision(int boardStatus[][BOARD_WIDTH + WALL_WIDTH], Block curBlock, int direction)
{
	// TODO: 여기에 구현 코드 추가
	switch (direction) {
		case 0: //아래 충돌 검사
			for( int i = 0; i < 4; i++) {
				if (boardStatus[curBlock.tile[i].y + 1][curBlock.tile[i].x] != 0) {
					return 1; //충돌 발생
				}
			}
			return 0; //충돌 없음
		case 1: //왼쪽 충돌 검사
			for (int i = 0; i < 4; i++) {
				if (boardStatus[curBlock.tile[i].y][curBlock.tile[i].x - 1] != 0) {
					return 1; //충돌 발생
				}
			}
			return 0; //충돌 없음
		case 2: //오른쪽 충돌 검사
			for (int i = 0; i < 4; i++) {
				if (boardStatus[curBlock.tile[i].y][curBlock.tile[i].x + 1] != 0) {
					return 1; //충돌 발생
				}
			}
			return 0; //충돌 없음
		default:
			return -1; //오류
	}
}

void CTetrisDoc::EmbedBlock(int boardStatus[][BOARD_WIDTH + WALL_WIDTH], Block curBlock, CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.
	for (int i = 0; i < 4; i++) {
		boardStatus[curBlock.tile[i].y][curBlock.tile[i].x] = curBlock.blockType + 1; //보드 상태에 블록 정보 삽입
	}
	PaintBlock(curBlock, curBlock.blockColor, pDC); // 블록 그리기
	return;
}

void CTetrisDoc::RenderBoard(CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.

}