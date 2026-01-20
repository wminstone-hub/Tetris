
// TetrisDoc.h: CTetrisDoc 클래스의 인터페이스
//


#pragma once

//게임보드 상수
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
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
	int mBoard[BOARD_HEIGHT][BOARD_WIDTH] = {0, }; // 게임 보드
	int mBlockX, mBlockY;           // 현재 블록 위치
	int x, y;						// 그리기 위치
	int mBlockType;                  // 현재 블록 종류
	int mNextBlockType;              // 다음 블록 종류
	int mCurrentRotation;          // 현재 블록 회전 상태
	int mScore;                      // 점수
	int mLevel;                      // 현재 레벨
	int mGameStaus;                 // 게임 상태 (타이틀, 진행중, 일시정지, 게임오버)


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
};
