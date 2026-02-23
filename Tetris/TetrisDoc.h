
// TetrisDoc.h: CTetrisDoc 클래스의 인터페이스
//


#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include <map>


//게임보드 상수
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define FLOOR_HEIGHT 1 //바닥 두께
#define CEILING_HEIGHT 2 //천장 두께
#define WALL_WIDTH 2  //벽 두께
#define BLOCK_SIZE 40	//픽셀 단위로 블록 한 칸 크기
#define BOARD_HIEGHT_OFFSET 0 // 게임보드위치
#define BOARD_WIDTH_OFFSET 185 // 게임보드위치

class CTetrisDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CTetrisDoc() noexcept;
	DECLARE_DYNCREATE(CTetrisDoc)

// 멤버 변수입니다.
public:
	int mBlockX, mBlockY;		// 현재 블록 위치
	int mTileX, mTileY;			// 타일 위치
	int mBlockType;				// 현재 블록 종류
	int mNextBlockType;         // 다음 블록 종류
	int mCurrentRotation;       // 현재 블록 회전 상태
	int mHoldFlag = 0;				// 홀드 사용 여부
	int mScore;                 // 점수
	CString mScoreStr;          // 점수 문자열
	int mLevel;                 // 현재 레벨
	int mTimer = 0;					// 게임진행시간
	CString mTimerStr;				// 게임진행시간 문자열
	int mGameStatus = 0;             // 게임 상태 (타이틀: 0, 진행중: 1, 일시정지: 2, 게임오버: 3)

	CImage mBlockImage;				//NEXT 블록 이미지를 저장할 CImage 객체

	//미노 블록 랜덤 생성 관련 변수
	std::vector<int> blockBox;
	size_t curIdx;
	std::mt19937 engine;

	//보드 상태 표현 2차원 배열
	int boardStatus[BOARD_HEIGHT + CEILING_HEIGHT + FLOOR_HEIGHT][BOARD_WIDTH + WALL_WIDTH] = { 0, };

	//보드 색상 정의
	const COLORREF boardColor = 0x00f0f0f0; // 아이보리
	const COLORREF ceilingColor = 0x00ccffff; // 옅은노랑
	const COLORREF floorColor = 0x00808080; // 짙은회색

	// 블록 색상 정의
	//const COLORREF blockColor0 = 0x00fce0d1; // 하늘색
	const COLORREF blockColor0 = 0x00ffcca0; // 하늘색
	const COLORREF blockColor1 = 0x0000d0dd; // 노란색
	const COLORREF blockColor2 = 0x00792646; // 보라색
	const COLORREF blockColor3 = 0x00007fff; // 주황색
	const COLORREF blockColor4 = 0x00ffcf00; // 파란색
	const COLORREF blockColor5 = 0x000000ff; // 빨간색
	const COLORREF blockColor6 = 0x0000ff00; // 초록색

	//타일과 블록 구조체 정의
	typedef struct {
		int x;
		int y;
	}Tile;

	typedef struct {
		char blockType; //블록 종류
		Tile tile[4]; //블록을 구성하는 4개의 타일
		char blockRotateState; //회전 상태
		COLORREF blockColor;
	}Block;

	//7가지 블록 타입 정의
	Block blockType0 = { 0, { { 4, 0 }, { 5, 0 }, { 6, 0 }, { 7, 0 } }, '0', blockColor0 }; //I형
	Block blockType1 = { 1, { { 5, 0 }, { 6, 0 }, { 5, 1 }, { 6, 1 } }, '0', blockColor1 }; //O형
	Block blockType2 = { 2, { { 5, 0 }, { 4, 1 }, { 5, 1 }, { 6, 1 } }, '0', blockColor2 }; //T형
	Block blockType3 = { 3, { { 5, 0 }, { 5, 1 }, { 6, 1 }, { 7, 1 } }, '0', blockColor3 }; //J형
	Block blockType4 = { 4, { { 6, 0 }, { 4, 1 }, { 5, 1 }, { 6, 1 } }, '0', blockColor4 }; //L형
	Block blockType5 = { 5, { { 5, 0 }, { 6, 0 }, { 6, 1 }, { 7, 1 } }, '0', blockColor5 }; //Z형
	Block blockType6 = { 6, { { 5, 0 }, { 6, 0 }, { 4, 1 }, { 5, 1 } }, '0', blockColor6 }; //S형

	//월킥 오프셋 구조체 정의
	struct WallKickOffset {
		int x;
		int y;
	};

	// I형 블록 Wall Kick 데이터
	std::map<char, std::vector<WallKickOffset>> wallKickData_I = {
		{'R', {{0, 0}, {-2, 0}, {+1, 0}, {-2, -1}, {+1, +2}}},  // 0->R
		{'2', {{0, 0}, {-1, 0}, {+2, 0}, {-1, +2}, {+2, -1}}},  // R->2
		{'L', {{0, 0}, {+2, 0}, {-1, 0}, {+2, +1}, {-1, -2}}},  // 2->L
		{'0', {{0, 0}, {+1, 0}, {-2, 0}, {+1, -2}, {-2, +1}}}   // L->0
	};

	// J, L, S, T, Z 블록 Wall Kick 데이터
	std::map<char, std::vector<WallKickOffset>> wallKickData_JLSTZ = {
		{'R', {{0, 0}, {-1, 0}, {-1, +1}, {0, -2}, {-1, -2}}},  // 0->R
		{'2', {{0, 0}, {+1, 0}, {+1, -1}, {0, +2}, {+1, +2}}},  // R->2
		{'L', {{0, 0}, {+1, 0}, {+1, +1}, {0, -2}, {+1, -2}}},  // 2->L
		{'0', {{0, 0}, {-1, 0}, {-1, -1}, {0, +2}, {-1, +2}}}   // L->0
	};



	//현재 블럭
	Block curBlock;

	//임시블럭
	Block bufferBlock;

	//홀드 블럭
	int holdBlock = -1;
		
// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CTetrisDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	//초기화 함수들
	void InitBoardStatus();
	void InitBlockBox(std::vector<int>& blockBox);
	void initGameStatus();
	//게임 상태 관련 함수들
	int IsGameOvered(CDC* pDC);
	CString TimerFormet(int m_Timer);
	//랜덤 블록 생성 함수 (7-bag)
	void shuffleBox();
	int nextBlock();
	//블록 그리기 관련 함수들
	void CreateUI(CDC* pDC);
	void DrawBoard(CDC* pDC);
	void PaintTile(Tile tile, COLORREF color, CDC* pDC);
	void PaintBlock(Block block, COLORREF blockColor, CDC* pDC);
	void PaintBlock(Block block, CDC* pDC);
	//블록 동작 관련 함수들
	void CreateBlock(CDC* pDC);
	void CTetrisDoc::CreateBlock(int blockType, CDC* pDC);
	int DropBlock(Block* curBlock, CDC* pDC);
	int MoveBlockDirectionX(Block* curBlock, int direction, CDC* pDC);
	int CheckCollision(int boardStatus[][BOARD_WIDTH + WALL_WIDTH], Block curBlock, int mod);
	void EmbedBlock(int boardStatus[][BOARD_WIDTH + WALL_WIDTH], Block curBlock, CDC* pDC);
	void EraseOneLine(CDC* pDC);
	void RenderBoard(CDC* pDC);
	int RotateBlock(Block* curBlock, CDC* pDC);
	int CheckWallKick(int boardStatus[][BOARD_WIDTH + WALL_WIDTH], Block curBlock, Block* bufferBlock);
	int HardDropBlock(Block* curBlock, CDC* pDC);
	void HoldBlock(CDC* pDC);
};
