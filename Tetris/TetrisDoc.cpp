
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
	for (int i = 0; i < BOARD_HEIGHT + CEILING_HEIGHT; i++) {
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
	pDC->TextOutW(672, 78, _T("TIMER"));
	pDC->TextOutW(680, 125, TimerFormet(mTimer));

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
	int rectEndX = BOARD_WIDTH_OFFSET + tile.x * BLOCK_SIZE;
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

//블록 색상 지우기 오버로드
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


int CTetrisDoc::RotateBlock(Block* curBlock, CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.
	switch (CheckCollision(boardStatus, *curBlock, 3)) {
	case 0: // 회전가능하면~
		PaintBlock(*curBlock, pDC); //이전 블록 지우기
		*curBlock = bufferBlock; //버퍼에 저장된 회전된 블록 정보로 교체
		PaintBlock(*curBlock, curBlock->blockColor, pDC); //새로운 블록 그리기
		return 0;

	case 1: // 충돌 발생하면~
		return 1;

	default:
		return -1; //오류
	}
}



// 충돌 검사 함수 (dir == 0 : 아래, dir == 1 : 왼쪽, dir == 2 : 오른쪽)
// curBlock 변수명 왜이래 지역변수잖아!!!!!!!!!!!!!!!
// curBlock 변수명 왜이래 지역변수잖아!!!!!!!!!!!!!!!
// curBlock 변수명 왜이래 지역변수잖아!!!!!!!!!!!!!!!
// curBlock 변수명 왜이래 지역변수잖아!!!!!!!!!!!!!!!
// curBlock 변수명 왜이래 지역변수잖아!!!!!!!!!!!!!!!
// curBlock 변수명 왜이래 지역변수잖아!!!!!!!!!!!!!!!
// curBlock 변수명 왜이래 지역변수잖아!!!!!!!!!!!!!!!
int CTetrisDoc::CheckCollision(int boardStatus[][BOARD_WIDTH + WALL_WIDTH], Block curBlock, int mod)
{
	// TODO: 여기에 구현 코드 추가
	switch (mod) {
	case 0: //아래 충돌 검사
		for (int i = 0; i < 4; i++) {
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

	case 3: // 시계방향 회전 충돌 검사

		switch (curBlock.blockType) {

		case 0: // I형블록
			switch (curBlock.blockRotateState) {
			case '0'://0->R
				curBlock.tile[0].x += 1; curBlock.tile[0].y -= 1;
				curBlock.tile[1].x += 0; curBlock.tile[1].y += 0;
				curBlock.tile[2].x -= 1; curBlock.tile[2].y += 1;
				curBlock.tile[3].x -= 2; curBlock.tile[3].y += 2;
				curBlock.blockRotateState = 'R';
				break;
			case 'R'://R->2
				curBlock.tile[0].x -= 2; curBlock.tile[0].y += 1;
				curBlock.tile[1].x -= 1; curBlock.tile[1].y += 0;
				curBlock.tile[2].x += 0; curBlock.tile[2].y -= 1;
				curBlock.tile[3].x += 1; curBlock.tile[3].y -= 2;
				curBlock.blockRotateState = '2';
				break;
			case '2'://2->L
				curBlock.tile[0].x += 2; curBlock.tile[0].y -= 2;
				curBlock.tile[1].x += 1; curBlock.tile[1].y -= 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x -= 1; curBlock.tile[3].y += 1;
				curBlock.blockRotateState = 'L';
				break;
			case 'L'://L->0
				curBlock.tile[0].x -= 1; curBlock.tile[0].y += 2;
				curBlock.tile[1].x -= 0; curBlock.tile[1].y += 1;
				curBlock.tile[2].x += 1; curBlock.tile[2].y += 0;
				curBlock.tile[3].x += 2; curBlock.tile[3].y -= 1;
				curBlock.blockRotateState = '0';
				break;
			}
			break;

			
		case 1: // O형블록
			//O형블록은 회전해도 모양이 변하지 않음
			break;


		case 2: // T형블록
			switch (curBlock.blockRotateState) {
			case '0': //0->R
				curBlock.tile[0].x += 1; curBlock.tile[0].y += 1;
				curBlock.tile[1].x += 1; curBlock.tile[1].y -= 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x -= 1; curBlock.tile[3].y += 1;
				curBlock.blockRotateState = 'R';
				break;
			case 'R': //R->2
				curBlock.tile[0].x -= 1; curBlock.tile[0].y += 1;
				curBlock.tile[1].x += 1; curBlock.tile[1].y += 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x -= 1; curBlock.tile[3].y -= 1;
				curBlock.blockRotateState = '2';
				break;
			case '2': //2->L
				curBlock.tile[0].x -= 1; curBlock.tile[0].y -= 1;
				curBlock.tile[1].x -= 1; curBlock.tile[1].y += 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x += 1; curBlock.tile[3].y -= 1;
				curBlock.blockRotateState = 'L';
				break;
			case 'L': //L->0
				curBlock.tile[0].x += 1; curBlock.tile[0].y -= 1;
				curBlock.tile[1].x -= 1; curBlock.tile[1].y -= 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x += 1; curBlock.tile[3].y += 1;
				curBlock.blockRotateState = '0';
				break;
			}
			break;

			
		case 3: // J형블록
			switch (curBlock.blockRotateState) {
			case '0': //0->R
				curBlock.tile[0].x += 2; curBlock.tile[0].y += 0;
				curBlock.tile[1].x += 1; curBlock.tile[1].y -= 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x -= 1; curBlock.tile[3].y += 1;
				curBlock.blockRotateState = 'R';
				break;
			case 'R': //R->2
				curBlock.tile[0].x += 0; curBlock.tile[0].y += 2;
				curBlock.tile[1].x += 1; curBlock.tile[1].y += 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x -= 1; curBlock.tile[3].y -= 1;
				curBlock.blockRotateState = '2';
				break;
			case '2': //2->L
				curBlock.tile[0].x -= 2; curBlock.tile[0].y += 0;
				curBlock.tile[1].x -= 1; curBlock.tile[1].y += 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x += 1; curBlock.tile[3].y -= 1;
				curBlock.blockRotateState = 'L';
				break;
			case 'L': //L->0
				curBlock.tile[0].x += 0; curBlock.tile[0].y -= 2;
				curBlock.tile[1].x -= 1; curBlock.tile[1].y -= 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x += 1; curBlock.tile[3].y += 1;
				curBlock.blockRotateState = '0';
				break;
			}
			break;

			
		case 4: // L형블록
			switch (curBlock.blockRotateState) {
			case '0': //0->R
				curBlock.tile[0].x += 0; curBlock.tile[0].y += 2;
				curBlock.tile[1].x += 1; curBlock.tile[1].y -= 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x -= 1; curBlock.tile[3].y += 1;
				curBlock.blockRotateState = 'R';
				break;
			case 'R': //R->2
				curBlock.tile[0].x -= 2; curBlock.tile[0].y += 0;
				curBlock.tile[1].x += 1; curBlock.tile[1].y += 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x -= 1; curBlock.tile[3].y -= 1;
				curBlock.blockRotateState = '2';
				break;
			case '2': //2->L
				curBlock.tile[0].x += 0; curBlock.tile[0].y -= 2;
				curBlock.tile[1].x -= 1; curBlock.tile[1].y += 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x += 1; curBlock.tile[3].y -= 1;
				curBlock.blockRotateState = 'L';
				break;
			case 'L': //L->0
				curBlock.tile[0].x += 2; curBlock.tile[0].y += 0;
				curBlock.tile[1].x -= 1; curBlock.tile[1].y -= 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x += 1; curBlock.tile[3].y += 1;
				curBlock.blockRotateState = '0';
				break;
			}
			break;



			
		case 5: // Z형블록
			switch (curBlock.blockRotateState) {
			case '0': //0->R
				curBlock.tile[0].x += 2; curBlock.tile[0].y += 0;
				curBlock.tile[1].x += 1; curBlock.tile[1].y += 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x -= 1; curBlock.tile[3].y += 1;
				curBlock.blockRotateState = 'R';
				break;
			case 'R': //R->2
				curBlock.tile[0].x += 0; curBlock.tile[0].y += 2;
				curBlock.tile[1].x -= 1; curBlock.tile[1].y += 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x -= 1; curBlock.tile[3].y -= 1;
				curBlock.blockRotateState = '2';
				break;
			case '2': //2->L
				curBlock.tile[0].x -= 2; curBlock.tile[0].y += 0;
				curBlock.tile[1].x -= 1; curBlock.tile[1].y -= 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x += 1; curBlock.tile[3].y -= 1;
				curBlock.blockRotateState = 'L';
				break;
			case 'L': //L->0
				curBlock.tile[0].x += 0; curBlock.tile[0].y -= 2;
				curBlock.tile[1].x += 1; curBlock.tile[1].y -= 1;
				curBlock.tile[2].x += 0; curBlock.tile[2].y += 0;
				curBlock.tile[3].x += 1; curBlock.tile[3].y += 1;
				curBlock.blockRotateState = '0';
				break;
			}
			break;

		
		case 6: // S형블록
			switch (curBlock.blockRotateState) {
			case '0': //0->R
				curBlock.tile[0].x += 0; curBlock.tile[0].y += 0;
				curBlock.tile[1].x -= 1; curBlock.tile[1].y += 1;
				curBlock.tile[2].x += 2; curBlock.tile[2].y += 0;
				curBlock.tile[3].x += 1; curBlock.tile[3].y += 1;
				curBlock.blockRotateState = 'R';
				break;
			case 'R': //R->2
				curBlock.tile[0].x += 0; curBlock.tile[0].y += 1;
				curBlock.tile[1].x += 1; curBlock.tile[1].y += 0;
				curBlock.tile[2].x -= 2; curBlock.tile[2].y += 1;
				curBlock.tile[3].x -= 1; curBlock.tile[3].y += 0;
				curBlock.blockRotateState = '2';
				break;
			case '2': //2->L
				curBlock.tile[0].x -= 1; curBlock.tile[0].y -= 1;
				curBlock.tile[1].x -= 2; curBlock.tile[1].y += 0;
				curBlock.tile[2].x += 1; curBlock.tile[2].y -= 1;
				curBlock.tile[3].x += 0; curBlock.tile[3].y += 0;
				curBlock.blockRotateState = 'L';
				break;
			case 'L': //L->0
				curBlock.tile[0].x += 1; curBlock.tile[0].y += 0;
				curBlock.tile[1].x += 2; curBlock.tile[1].y -= 1;
				curBlock.tile[2].x -= 1; curBlock.tile[2].y += 0;
				curBlock.tile[3].x += 0; curBlock.tile[3].y -= 1;
				curBlock.blockRotateState = '0';
				break;
			}
			break;
			
		default:
			return -1;
		}
		return CheckWallKick(boardStatus, curBlock, &bufferBlock); //월킥 포합 회전가능여부 체크


	case 4: // 기본충돌 검사 // 오... 결합도 좆됫는데?
		for (int i = 0; i < 4; i++) {
			if (boardStatus[curBlock.tile[i].y][curBlock.tile[i].x] != 0) {
				return 1; //충돌 발생
			}
		}
		return 0; //충돌 없음

	default:
		return -1; //오류
	}

}


int CTetrisDoc::CheckWallKick(int boardStatus[][BOARD_WIDTH + WALL_WIDTH], Block block, Block* bufferBlock)
{
	// TODO: 여기에 구현 코드 추가.

	Block orgBlock = block; //원본위치저장

	switch (block.blockType) {
	case 0: //I형블록
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 4; j++) {
				block.tile[j].x = orgBlock.tile[j].x + wallKickData_I[block.blockRotateState][i].x;
				block.tile[j].y = orgBlock.tile[j].y + wallKickData_I[block.blockRotateState][i].y;
			}
			if (CheckCollision(boardStatus, block, 4) == 0) { // 충돌없으면
				*bufferBlock = block; // 전역변수에 지역변수 저장
				return 0;
			}
		}
		return 1; // 월킥 실패 회전불가
		break;

	case 1: //O형블록
		// O형 블록은 Wall Kick 불필요
		break;

	case 2: //T형블록
	case 3: //J형블록
	case 4: //L형블록
	case 5: //Z형블록
	case 6: //S형블록
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 4; j++) {
				block.tile[j].x = orgBlock.tile[j].x + wallKickData_JLSTZ[block.blockRotateState][i].x;
				block.tile[j].y = orgBlock.tile[j].y + wallKickData_JLSTZ[block.blockRotateState][i].y;
			}
			if (CheckCollision(boardStatus, block, 4) == 0) { // 충돌없으면
				*bufferBlock = block; // 전역변수에 지역변수 저장
				return 0;
			}
		}
		return 1; // 월킥 실패 회전불가
		break;

	default:
		return -1;
	}
}




void CTetrisDoc::EmbedBlock(int boardStatus[][BOARD_WIDTH + WALL_WIDTH], Block curBlock, CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.
	for (int i = 0; i < 4; i++) {
		boardStatus[curBlock.tile[i].y][curBlock.tile[i].x] = curBlock.blockType + 1; //보드 상태에 블록 정보 삽입
	}
	PaintBlock(curBlock, curBlock.blockColor, pDC); // 블록 그리기
	EraseOneLine(pDC); //라인 지우기 검사 및 처리
	return;
}

void CTetrisDoc::EraseOneLine(CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.
	std::vector<int> line; // 확인해야 하는 라인의 y좌표
	Tile tile;
	//확인해야 하는 중복 제거된 라인 벡터 생성
	for (int i = 0; i < sizeof(curBlock.tile)/sizeof(curBlock.tile[0]); i++) {
		if (line.size() == 0) {
			line.push_back(curBlock.tile[i].y); //처음에는 무조건 추가
			continue;
		}
		for (int y : line) {
			if (curBlock.tile[i].y == y) {
				break; //이미 벡터에 들어있는 라인인 경우 건너뜀
			}
			line.push_back(curBlock.tile[i].y); //확인해야 하는 라인 추가
		}
	}
	//라인 검사
	for (int y : line) {
		bool isFullLine = true;
		for (int x = 1; x <= BOARD_WIDTH; x++) {
			if (boardStatus[y][x] == 0) {
				isFullLine = false; //빈 칸이 있으면 꽉 찬 라인이 아님
				break;
			}
		}
		if (isFullLine) {
			//라인 지우기
			for (int x = 1; x <= BOARD_WIDTH; x++) {
				boardStatus[y][x] = 0; //라인 상태 초기화
				tile.x = x; tile.y = y;
				PaintTile(tile, boardColor, pDC); //라인 지우기 출력
			}
			//위의 라인들을 한 칸씩 내리기
			for (int row = y; row > 0; row--) {
				for (int x = 1; x <= BOARD_WIDTH; x++) {
					boardStatus[row][x] = boardStatus[row - 1][x];
					tile.x = x; tile.y = row;
					//한 칸 아래로 칠하기
					switch (boardStatus[row][x]) {
					case 0:
						if (row == 0 || row == 1)
							PaintTile(tile, ceilingColor, pDC);
						else
							PaintTile(tile, boardColor, pDC);
						break;
					case 1:
						PaintTile(tile, blockColor0, pDC);
						break;
					case 2:
						PaintTile(tile, blockColor1, pDC);
						break;
					case 3:
						PaintTile(tile, blockColor2, pDC);
						break;
					case 4:
						PaintTile(tile, blockColor3, pDC);
						break;
					case 5:
						PaintTile(tile, blockColor4, pDC);
						break;
					case 6:
						PaintTile(tile, blockColor5, pDC);
						break;
					case 7:
						PaintTile(tile, blockColor6, pDC);
						break;
					default:
						break;
					}
				}
			}
			//맨 위 라인 초기화
			for (int x = 1; x <= BOARD_WIDTH; x++) {
				boardStatus[0][x] = 0;
				tile.x = x; tile.y = 0;
				PaintTile(tile, ceilingColor, pDC);
			}
			//점수 증가
			mScore += 100;
		}
	}
}




void CTetrisDoc::RenderBoard(CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.

}
// 분:초 형식 변환 함수
CString CTetrisDoc::TimerFormet(int m_Timer)
{
	// TODO: 여기에 구현 코드 추가.
	CString mMin, mSec;			// 변환된 분,초
	mMin.Format(_T("%02d"), m_Timer / 60);
	mSec.Format(_T("%02d"), m_Timer % 60);


	return mMin + _T(" : ") + mSec;
}
