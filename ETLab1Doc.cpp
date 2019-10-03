
// ETLab1Doc.cpp: реализация класса CETLab1Doc 
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "ETLab1.h"
#endif

#include "ETLab1Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CETLab1Doc

IMPLEMENT_DYNCREATE(CETLab1Doc, CDocument)

BEGIN_MESSAGE_MAP(CETLab1Doc, CDocument)
	ON_COMMAND(ID_EDIT_CLEAR_ALL, &CETLab1Doc::OnEditClearAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL, &CETLab1Doc::OnUpdateEditClearAll)
	ON_COMMAND(ID_EDIT_UNDO, &CETLab1Doc::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CETLab1Doc::OnUpdateEditUndo)
END_MESSAGE_MAP()


// Создание или уничтожение CETLab1Doc

CETLab1Doc::CETLab1Doc() noexcept
{
	// TODO: добавьте код для одноразового вызова конструктора

}

CETLab1Doc::~CETLab1Doc()
{
}

BOOL CETLab1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	return TRUE;
}




// Сериализация CETLab1Doc

void CETLab1Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
		m_LineArray.Serialize(ar);
	}
	else
	{
		// TODO: добавьте код загрузки
		m_LineArray.Serialize(ar);
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CETLab1Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
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

// Поддержка обработчиков поиска
void CETLab1Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CETLab1Doc::SetSearchContent(const CString& value)
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

// Диагностика CETLab1Doc

#ifdef _DEBUG
void CETLab1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CETLab1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CETLab1Doc
IMPLEMENT_SERIAL(CLine, CObject, 1)
void CLine::Draw(CDC *PDC)
{
	/*PDC->MoveTo(m_X1, m_Y1);
	PDC->LineTo(m_X2, m_Y1);
	PDC->LineTo(m_X2, m_Y2);
	PDC->LineTo(m_X2, m_Y1);
	PDC->LineTo(m_X1, m_Y1);*/
	PDC->Rectangle(m_X1, m_Y1, m_X2, m_Y2);
	
}
void CLine::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
		ar<<m_X1<< m_Y1<< m_X2 << m_Y2;
	else
		ar>>m_X1>>m_Y1>>m_X2>>m_Y2;
}


void CETLab1Doc::AddLine(int X1, int Y1, int X2, int Y2)
{
	/*CLine *pLine = new CLine(X1, Y1, X2, Y1);
	m_LineArray.Add(pLine);
	CLine *tLine = new CLine(X2, Y1, X2, Y2);
	m_LineArray.Add(tLine);
	CLine *kLine = new CLine(X2, Y2, X1, Y2);
	m_LineArray.Add(kLine);
	CLine *lLine = new CLine(X1, Y2, X1, Y1);
	m_LineArray.Add(lLine);*/


	CLine *pLine = new CLine(X1, Y1, X2, Y2);
	m_LineArray.Add(pLine);
	SetModifiedFlag();
}

CLine* CETLab1Doc::GetLine(int Index)
{
	if (Index<0 || Index>m_LineArray.GetUpperBound())
		return 0;
	return m_LineArray.GetAt(Index);
}

int CETLab1Doc::GetNumLines()
{
	return (int)m_LineArray.GetSize();
}


void CETLab1Doc::DeleteContents()
{
	// TODO: добавьте специализированный код или вызов базового класса
	int Index = (int)m_LineArray.GetSize();
	while (Index--)
		delete m_LineArray.GetAt(Index);
	m_LineArray.RemoveAll();

	CDocument::DeleteContents();
}


void CETLab1Doc::OnEditClearAll()
{
	// TODO: добавьте свой код обработчика команд
	DeleteContents();
	UpdateAllViews(0);
	SetModifiedFlag();

}


void CETLab1Doc::OnUpdateEditClearAll(CCmdUI *pCmdUI)
{
	// TODO: добавьте свой код обработчика ИП обновления команд
	pCmdUI->Enable((int)m_LineArray.GetSize());
}


void CETLab1Doc::OnEditUndo()
{
	// TODO: добавьте свой код обработчика команд
	int Index = (int)m_LineArray.GetUpperBound();
	if (Index > -1)
	{
		delete m_LineArray.GetAt(Index);
		m_LineArray.RemoveAt(Index);
	}
	UpdateAllViews(0);
	SetModifiedFlag();

}


void CETLab1Doc::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	// TODO: добавьте свой код обработчика ИП обновления команд
	pCmdUI->Enable((int)m_LineArray.GetSize());
}
