
// ETLab1View.cpp: реализация класса CETLab1View
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "ETLab1.h"
#endif

#include "ETLab1Doc.h"
#include "ETLab1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CETLab1View

IMPLEMENT_DYNCREATE(CETLab1View, CView)

BEGIN_MESSAGE_MAP(CETLab1View, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CETLab1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
//	ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// Создание или уничтожение CETLab1View

CETLab1View::CETLab1View() noexcept
{
	// TODO: добавьте код создания
	m_Dragging = 0;
	m_HCross = AfxGetApp()->LoadStandardCursor(IDC_CROSS);

}

CETLab1View::~CETLab1View()
{
}

BOOL CETLab1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs
	m_ClassName = AfxRegisterWndClass(
		CS_HREDRAW | CS_VREDRAW, // стили окна
		0,					  // без указателя;
		(HBRUSH)::GetStockObject(WHITE_BRUSH),
		// задать чисто белый фон;
		0);					  //без значка
	cs.lpszClass = m_ClassName;


	return CView::PreCreateWindow(cs);
}

// Рисование CETLab1View

void CETLab1View::OnDraw(CDC* pDC)
{
	CETLab1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	int Index = pDoc->GetNumLines();
	while (Index--)
		pDoc->GetLine(Index)->Draw(pDC);


	// TODO: добавьте здесь код отрисовки для собственных данных
}


// Печать CETLab1View


void CETLab1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CETLab1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CETLab1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CETLab1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}

void CETLab1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CETLab1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Диагностика CETLab1View

#ifdef _DEBUG
void CETLab1View::AssertValid() const
{
	CView::AssertValid();
}

void CETLab1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CETLab1Doc* CETLab1View::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CETLab1Doc)));
	return (CETLab1Doc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CETLab1View





void CETLab1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	m_PointOld = point;
	m_PointOrigin = point;

	SetCapture();
	m_Dragging = 1;

	RECT Rect;
	GetClientRect(&Rect);
	ClientToScreen(&Rect);
	::ClipCursor(&Rect);


	CView::OnLButtonDown(nFlags, point);
}


void CETLab1View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	::SetCursor(m_HCross);
	if (m_Dragging)
	{
		CClientDC ClientDC(this);
		
		ClientDC.MoveTo(m_PointOrigin);
		ClientDC.Rectangle(m_PointOrigin.x, m_PointOrigin.y, point.x, point.y);
		m_PointOld = point;

	}

	CView::OnMouseMove(nFlags, point);
}


void CETLab1View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (m_Dragging)
	{
		m_Dragging = 0;
		::ReleaseCapture();
		::ClipCursor(NULL);
		CClientDC ClientDC(this);
		ClientDC.SetROP2(R2_NOT);
		//ClientDC.MoveTo(m_PointOrigin);
		//ClientDC.LineTo(m_PointOld);
		ClientDC.SetROP2(R2_COPYPEN);
		//ClientDC.MoveTo(m_PointOrigin);
		//ClientDC.LineTo(point);
		CETLab1Doc* pDoc = GetDocument();
		pDoc->AddLine(m_PointOrigin.x, m_PointOrigin.y, point.x, point.y);
	
	}

	CView::OnLButtonUp(nFlags, point);
}
