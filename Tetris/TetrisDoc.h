
// TetrisDoc.h: CTetrisDoc 클래스의 인터페이스
//


#pragma once

//게임보드 상수
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define WALL_WIDTH 2	//벽 두께
#define FLOOR_HEIGHT 1 //바닥 두께
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
	int mScore;                 // 점수
	int mLevel;                 // 현재 레벨
	int mGameStaus;             // 게임 상태 (타이틀, 진행중, 일시정지, 게임오버)

	//보드 상태 표현 2차원 배열
	int mCurBoardState[BOARD_HEIGHT + FLOOR_HEIGHT][BOARD_WIDTH + WALL_WIDTH] = { 0, };
	int mOldBoardState[BOARD_HEIGHT + FLOOR_HEIGHT][BOARD_WIDTH + WALL_WIDTH] = { 0, };

	//보드 색상 정의
	const COLORREF boardColor = 0x00dce6ee; // 아이보리
	const COLORREF floorColor = 0x00808080; // 짙은 회색

	// 블록 색상 정의
	const COLORREF blockColor1 = 0x00fce0d1; // 하늘색
	const COLORREF blockColor2 = 0x0000d0dd; // 노란색
	const COLORREF blockColor3 = 0x00792646; // 보라색
	const COLORREF blockColor4 = 0x00007fff; // 주황색
	const COLORREF blockColor5 = 0x00ffcf00; // 파란색
	const COLORREF blockColor6 = 0x000000ff; // 빨간색
	const COLORREF blockColor7 = 0x0000ff00; // 초록색

	struct Tile {
		int x;
		int y;
	};

	struct Block {
		Tile tile1;
		Tile tile2;
		Tile tile3;
		Tile tile4;
	};


	Block blockType1;
	Block blockType2;
	Block blockType3;
	Block blockType4;
	Block blockType5;
	Block blockType6;
	Block blockType7;

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
	int myadd();
	int Render(CDC *pDC);
	void OnCreateBoard(CDC* pDC);
};
