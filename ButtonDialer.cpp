/*
 * Copyright (C) 2011-2024 MicroSIP (http://www.microsip.org)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "stdafx.h"
#include "ButtonDialer.h"
#include "Strsafe.h"
#include "const.h"

/////////////////////////////////////////////////////////////////////////////
// CButtonDialer

CButtonDialer::CButtonDialer()
{
	forceNumeric = false;
	m_map.SetAt(_T("1"), _T(""));
	m_map.SetAt(_T("2"), _T("ABC"));
	m_map.SetAt(_T("3"), _T("DEF"));
	m_map.SetAt(_T("4"), _T("GHI"));
	m_map.SetAt(_T("5"), _T("JKL"));
	m_map.SetAt(_T("6"), _T("MNO"));
	m_map.SetAt(_T("7"), _T("PQRS"));
	m_map.SetAt(_T("8"), _T("TUV"));
	m_map.SetAt(_T("9"), _T("WXYZ"));
	m_map.SetAt(_T("0"), _T(""));
	m_map.SetAt(_T("*"), _T(""));
	m_map.SetAt(_T("#"), _T(""));
}

CButtonDialer::~CButtonDialer()
{
	CloseTheme();
}


BEGIN_MESSAGE_MAP(CButtonDialer, CButton)
	ON_WM_THEMECHANGED()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonDialer message handlers

void CButtonDialer::PreSubclassWindow()
{
	OpenTheme();

	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	GetObject(hFont, sizeof(LOGFONT), &lf);
	lf.lfHeight = 12;
	CDC *pDC = GetDC();
	if (pDC) {
		dpiY = GetDeviceCaps(pDC->m_hDC, LOGPIXELSY);
		lf.lfHeight = MulDiv(lf.lfHeight, dpiY, 96);
		ReleaseDC(pDC);
	}
	else {
		dpiY = 96;
	}
	StringCchCopy(lf.lfFaceName, LF_FACESIZE, _T("Microsoft Sans Serif"));
	m_FontLetters.CreateFontIndirect(&lf);

	DWORD dwStyle = ::GetClassLong(m_hWnd, GCL_STYLE);
	dwStyle &= ~CS_DBLCLKS;
	::SetClassLong(m_hWnd, GCL_STYLE, dwStyle);
}

LRESULT CButtonDialer::OnThemeChanged()
{
	CloseTheme();
	OpenTheme();
	return 0L;
}

void CButtonDialer::OnSize(UINT type, int w, int h)
{
	CButton::OnSize(type, w, h);
}

void CButtonDialer::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point)) {
		if (GetCapture() != this) {
			SetCapture();
			Invalidate();
		}
	}
	else {
		ReleaseCapture();
		Invalidate();
	}
}

void CButtonDialer::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// ---- Windows 11 calculator style rendering (rounded flat buttons, no phone letters) ----
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	CRect rt = lpDrawItemStruct->rcItem;
	UINT state = lpDrawItemStruct->itemState;

	// clear to parent background
	dc.FillSolidRect(rt, dc.GetBkColor());
	dc.SetBkMode(TRANSPARENT);

	// light-theme calculator colors
	COLORREF fill   = RGB(0xF9, 0xF9, 0xF9);
	COLORREF border = RGB(0xE5, 0xE5, 0xE5);
	COLORREF text   = RGB(0x1A, 0x1A, 0x1A);
	if (state & ODS_SELECTED)      { fill = RGB(0xE6, 0xE6, 0xE6); }   // pressed
	else if (GetCapture() == this) { fill = RGB(0xEF, 0xEF, 0xEF); }   // hover

	// rounded button body
	CRect rr = rt;
	rr.DeflateRect(2, 2);
	int radius = MulDiv(8, dpiY, 96);
	CBrush brFill(fill);
	CPen   penBorder(PS_SOLID, 1, border);
	CBrush* pOldBrush = dc.SelectObject(&brFill);
	CPen*   pOldPen   = dc.SelectObject(&penBorder);
	dc.RoundRect(rr, CPoint(radius, radius));
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);

	// caption centered — digits/symbols only (no ABC/DEF phone letters)
	CString strTemp;
	GetWindowText(strTemp);
	COLORREF crOldColor = dc.SetTextColor(text);
	dc.DrawText(strTemp, rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	dc.SetTextColor(crOldColor);

	dc.Detach();
}
