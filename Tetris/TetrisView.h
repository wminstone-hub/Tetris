
// TetrisView.h: CTetrisView 클래스의 인터페이스
//

#pragma once


class CTetrisView : public CView
{
protected: // serialization에서만 만들어집니다.
	CTetrisView() noexcept;
	DECLARE_DYNCREATE(CTetrisView)

// 특성입니다.
public:
	CTetrisDoc* GetDocument() const;
	
	CButton mGameStartBtn;
	CButton mExitBtn;
	CButton mPauseBtn;
	CButton mResumeBtn;
	CButton mRestartBtn;



// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CTetrisView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnStartBtnClicked();
	void OnExitBtnClicked();
	void OnPauseBtnClicked();
	void OnResumeBtnClicked();
	void OnRestartBtnClicked();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // TetrisView.cpp의 디버그 버전
inline CTetrisDoc* CTetrisView::GetDocument() const
   { return reinterpret_cast<CTetrisDoc*>(m_pDocument); }
#endif

