#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <time.h>

#include "gdPdf.h"

jmp_buf env;

const int   TitlePaddingH =   2;
const int   TitlePaddingV =   2;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
 gdPdf_error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void* user_data)
 {
  fprintf(stderr, "gdcore::gdPdf_error_handler : ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT) error_no, (HPDF_UINT) detail_no);
  longjmp(env, 1);
 }

gdStPdf* gdPdf_openPdf(const char* pPdfFile, int withBookmarks)
 {
  gdStPdf*   pPdf = (gdStPdf*) malloc(sizeof(gdStPdf));
  if ( !pPdf ) return 0;
  memset(pPdf, 0, sizeof(gdStPdf));

  pPdf->withBookmarks  = withBookmarks;
  pPdf->pdfHandle      = 0;
  pPdf->pdfRootOutline = 0;
  pPdf->pdfFile        = pPdfFile;
  pPdf->pageProperties.nOptimized = 0;

  pPdf->pdfHandle = HPDF_New(gdPdf_error_handler, NULL);
  if (!pPdf->pdfHandle)
    {
     free(pPdf);
     return 0;
    }
  if (setjmp(env))
    {
     if ( pPdf->pdfHandle )
        HPDF_Free (pPdf->pdfHandle);
     pPdf->pdfHandle = 0;
     free(pPdf);
     return 0;
    }

  HPDF_UseUTFEncodings(pPdf->pdfHandle);
  HPDF_AddPageLabel(pPdf->pdfHandle, 0, HPDF_PAGE_NUM_STYLE_DECIMAL, 1, "");
  HPDF_SetCompressionMode(pPdf->pdfHandle, HPDF_COMP_ALL);

// TODO : Set the font for the bookmarks
  if ( pPdf->withBookmarks )
    {
     /* Set page mode to use outlines. */
     HPDF_SetPageMode(pPdf->pdfHandle, HPDF_PAGE_MODE_USE_OUTLINE);

     /* create outline root. */
     pPdf->pdfRootOutline = HPDF_CreateOutline(pPdf->pdfHandle, NULL, "Report", NULL);
     HPDF_Outline_SetOpened(pPdf->pdfRootOutline, HPDF_TRUE);
    }
  return pPdf;
 }

void gdPdf_closePdf(gdStPdf* pPdf)
 {
  gdPdf_clearPageProperties(pPdf);
  HPDF_Free (pPdf->pdfHandle);
  pPdf->pdfHandle = 0;
  pPdf->pdfRootOutline = 0;
  free(pPdf);
 }

int gdPdf_savePdf(gdStPdf* pPdf)
 {
  HPDF_SaveToFile(pPdf->pdfHandle, pPdf->pdfFile);
 }

ST_GDPDF_COLS_SHEET*   gdPdf_getNewColsSheet()
 {
  ST_GDPDF_COLS_SHEET* pSheet = (ST_GDPDF_COLS_SHEET*) malloc(sizeof(ST_GDPDF_COLS_SHEET));
  if ( pSheet )
     memset(pSheet, 0, sizeof(ST_GDPDF_COLS_SHEET));
  return pSheet;
 }

ST_GDPDF_ROWS_SHEET*   gdPdf_getNewRowsSheet()
 {
  ST_GDPDF_ROWS_SHEET* pSheet = (ST_GDPDF_ROWS_SHEET*) malloc(sizeof(ST_GDPDF_ROWS_SHEET));
  if ( pSheet )
     memset(pSheet, 0, sizeof(ST_GDPDF_ROWS_SHEET));
  return pSheet;
 }

void gdPdf_clearPageProperties(gdStPdf* pPdf)
 {
  ST_GDPDF_COLS_SHEET* curColsSheet = pPdf->pageProperties.rootSheetColumns;
  while(curColsSheet)
   {
    ST_GDPDF_COLS_SHEET* nextSheet = curColsSheet->nextSheet;
    free(curColsSheet);
    curColsSheet = nextSheet;
   }
  pPdf->pageProperties.rootSheetColumns = 0;

  ST_GDPDF_ROWS_SHEET* curRowsSheet = pPdf->pageProperties.rootSheetRows;
  while(curRowsSheet)
   {
    ST_GDPDF_ROWS_SHEET* nextSheet = curRowsSheet->nextSheet;
    free(curRowsSheet);
    curRowsSheet = nextSheet;
   }
  pPdf->pageProperties.rootSheetRows = 0;

  free(pPdf->pageProperties.headerColsWidth);
  free(pPdf->pageProperties.headerNbRowsOfEachColumn);
  free(pPdf->pageProperties.nbRowsOfRowLabel);

  memset(&pPdf->pageProperties, 0, sizeof(ST_GDPDF_PAGE));
 }

void gdPdf_setFontTitle(gdStPdf* pPdf, HPDF_Page page)
 {
  HPDF_Font       pdfFont;

  pdfFont = HPDF_GetFont(pPdf->pdfHandle, "Helvetica-Bold", NULL);
  HPDF_Page_SetFontAndSize(page, pdfFont, pPdf->pageProperties.fontTitlesSize);
//  if ( !pPdf->pageProperties.pageTitlesHeight )
//     pPdf->pageProperties.pageTitlesHeight = HPDF_Page_GetCurrentFontSize(page);
 }

void gdPdf_setFontColTitle(gdStPdf* pPdf, HPDF_Page page)
 {
  HPDF_Font       pdfFont;

//  const char* font_name = HPDF_LoadTTFontFromFile (pPdf->pdfHandle, "/usr/share/fonts/truetype/msttcorefonts/Arial_Bold.ttf", HPDF_TRUE);
//  pdfFont = HPDF_GetFont(pPdf->pdfHandle, font_name, NULL);
  pdfFont = HPDF_GetFont(pPdf->pdfHandle, "Helvetica-Bold", NULL);
  HPDF_Page_SetFontAndSize(page, pdfFont,  pPdf->pageProperties.fontHeaderSize);
  if ( !pPdf->pageProperties.headerHeightOfRow )
    {
/*
     HPDF_Page_BeginText(page);
     pPdf->pageProperties.headerHeightOfRow = HPDF_Font_GetCapHeight(pdfFont);
fprintf(stderr, "pPdf->pageProperties.headerHeightOfRow %d\n",pPdf->pageProperties.headerHeightOfRow );
     HPDF_Box pdfBox = HPDF_Font_GetBBox(pdfFont);
     pPdf->pageProperties.headerHeightOfRow = pdfBox.top - pdfBox.bottom;
fprintf(stderr, "pPdf->pageProperties.headerHeightOfRow %d\n",pPdf->pageProperties.headerHeightOfRow );
     pPdf->pageProperties.headerHeightOfRow = HPDF_Font_GetAscent(pdfFont) / 100;
fprintf(stderr, "pPdf->pageProperties.headerHeightOfRow %d\n",pPdf->pageProperties.headerHeightOfRow );
*/
//     pPdf->pageProperties.headerHeightOfRow = (HPDF_Page_GetCurrentFontSize(page) * 2);
//     HPDF_Page_EndText(page);
    }
 }

void gdPdf_setFontRowTitle(gdStPdf* pPdf, HPDF_Page page)
 {
  HPDF_Font       pdfFont;

  pdfFont = HPDF_GetFont(pPdf->pdfHandle, "Helvetica", NULL);
  HPDF_Page_SetFontAndSize(page, pdfFont, pPdf->pageProperties.fontRowLabelSize);
 }

void gdPdf_setFontQte(gdStPdf* pPdf, HPDF_Page page)
 {
  HPDF_Font       pdfFont;

  pdfFont = HPDF_GetFont(pPdf->pdfHandle, "Helvetica", NULL);
  HPDF_Page_SetFontAndSize(page, pdfFont, pPdf->pageProperties.fontRowQteSize);
 }

void gdPdf_printTitle(HPDF_Page page, const char* pTitle, int align, HPDF_Rect m_workingRect)
 {
//  HPDF_Page_SetRGBFill(page, 0, 0, 0);       // Caractere noir
//  HPDF_Page_SetRGBFill(page, 1.0, 1.0, 1.0); // Fond blanc
//  HPDF_Page_Fill(page);

  HPDF_Page_BeginText(page);
  HPDF_Page_TextRect(page, m_workingRect.left, m_workingRect.top, m_workingRect.right, m_workingRect.bottom, pTitle, align, NULL);
  HPDF_Page_EndText(page);
 }

void gdPdf_printTitleA(gdStPdf* pPdf, HPDF_Page page, const char* pTitle)
 {
  HPDF_Rect       m_workingRect;
  m_workingRect.left         = pPdf->pageProperties.pageLeftMargin;
  m_workingRect.top          = pPdf->pageProperties.pageHeight - pPdf->pageProperties.pageTopMargin;
  m_workingRect.right        = pPdf->pageProperties.pageWidth / 2;
  m_workingRect.bottom       = m_workingRect.top - pPdf->pageProperties.pageTitlesHeight;
  gdPdf_printTitle(page, pTitle, HPDF_TALIGN_LEFT, m_workingRect);
 }

void gdPdf_printTitleB(gdStPdf* pPdf, HPDF_Page page, const char* pTitle)
 {
  HPDF_Rect       m_workingRect;
  m_workingRect.left         = pPdf->pageProperties.pageWidth  / 2;
  m_workingRect.top          = pPdf->pageProperties.pageHeight - pPdf->pageProperties.pageTopMargin;
  m_workingRect.right        = pPdf->pageProperties.pageWidth  - pPdf->pageProperties.pageRightMargin;
  m_workingRect.bottom       = m_workingRect.top - pPdf->pageProperties.pageTitlesHeight;
//  gdPdf_printTitle(page, pTitle, HPDF_TALIGN_RIGHT, m_workingRect);
  gdPdf_printTitle(page, pTitle, HPDF_TALIGN_LEFT, m_workingRect);
 }

void gdPdf_printTitleC(gdStPdf* pPdf, HPDF_Page page, const char* pTitle)
 {
  HPDF_Rect       m_workingRect;
  m_workingRect.left         = pPdf->pageProperties.pageLeftMargin;
  m_workingRect.top          = pPdf->pageProperties.pageHeight - pPdf->pageProperties.pageTopMargin - pPdf->pageProperties.pageTitlesHeight;
  m_workingRect.right        = pPdf->pageProperties.pageWidth  / 2;
  m_workingRect.bottom       = m_workingRect.top - pPdf->pageProperties.pageTitlesHeight;
  gdPdf_printTitle(page, pTitle, HPDF_TALIGN_LEFT, m_workingRect);
 }

void gdPdf_printTitleD(gdStPdf* pPdf, HPDF_Page page, const char* pTitle)
 {
  HPDF_Rect       m_workingRect;
  m_workingRect.left         = pPdf->pageProperties.pageWidth / 2;
  m_workingRect.top          = pPdf->pageProperties.pageHeight - pPdf->pageProperties.pageTopMargin - pPdf->pageProperties.pageTitlesHeight;
  m_workingRect.right        = pPdf->pageProperties.pageWidth  - pPdf->pageProperties.pageRightMargin;
  m_workingRect.bottom       = m_workingRect.top - pPdf->pageProperties.pageTitlesHeight;
//  gdPdf_printTitle(page, pTitle, HPDF_TALIGN_RIGHT, m_workingRect);
  gdPdf_printTitle(page, pTitle, HPDF_TALIGN_LEFT, m_workingRect);
 }

int gdPdf_printColTitleByRow(gdStPdf* pPdf, HPDF_Page page, int startCol, int curCol, int curRow, const char* pTitle, int colSep)
 {
  if ( colSep ) return;
  HPDF_Rect       m_workingRect;
  m_workingRect.top      = pPdf->pageProperties.pageHeight - pPdf->pageProperties.pageTopMargin - pPdf->pageProperties.startHeight
                         - (pPdf->pageProperties.pageTitlesHeight * 2) - pPdf->pageProperties.pageHeightAfterTitles;
  m_workingRect.bottom   = m_workingRect.top - (pPdf->pageProperties.headerHeightOfRow * pPdf->pageProperties.headerMaxNbRows);
  m_workingRect.left     = pPdf->pageProperties.pageLeftMargin + pPdf->pageProperties.startWidth;
  if ( curCol )
     m_workingRect.left +=  (pPdf->pageProperties.headerColsWidth[0] * pPdf->pageProperties.em2Point);
  int iCol;
  for(iCol = startCol; iCol < curCol; iCol++)
     m_workingRect.left += (pPdf->pageProperties.headerColsWidth[iCol] * pPdf->pageProperties.em2Point);
  m_workingRect.right    = m_workingRect.left + (pPdf->pageProperties.headerColsWidth[curCol] * pPdf->pageProperties.em2Point);

  if ( !curRow )
    {
     HPDF_Page_SetRGBFill(page, 0.0, 0.0, 0.4); // Fond bleu
     HPDF_Page_Rectangle(page, m_workingRect.left, m_workingRect.bottom, m_workingRect.right - m_workingRect.left, m_workingRect.top - m_workingRect.bottom);
//     HPDF_Page_Rectangle(page, m_workingRect.left + 1, m_workingRect.bottom - 1, m_workingRect.right - m_workingRect.left - 1, m_workingRect.top - m_workingRect.bottom + 1);
     HPDF_Page_Fill(page);
     HPDF_Page_SetRGBFill(page, 1.0, 1.0, 1.0);   // Caracteres blanc
     HPDF_Page_SetRGBStroke(page, 0.27, 0.47, 0.6); // Traits blanc
     gdPdf_draw_column(m_workingRect, page, 0);
    }

  m_workingRect.top -= (pPdf->pageProperties.headerHeightOfRow * curRow);
  m_workingRect.bottom   = m_workingRect.top - pPdf->pageProperties.headerHeightOfRow;
  return gdPdf_draw_label(m_workingRect, page, pTitle, HPDF_TALIGN_CENTER, 0);
 }

int gdPdf_printColTitle(gdStPdf* pPdf, HPDF_Page page, int startCol, int curCol, const char* pTitle, int maxTitleRow, int rowTitleNeeded, int bTest, int colSep)
 {
//  fprintf(stderr, "startcol = %d, curcol = %d, label = <%s>, maxtitlerow = %d, rowneeded = %d\n", startCol, curCol, pTitle, maxTitleRow, rowTitleNeeded);
  if ( colSep ) return;
  HPDF_Rect       m_workingRect;
  m_workingRect.top      = pPdf->pageProperties.pageHeight - pPdf->pageProperties.pageTopMargin - pPdf->pageProperties.startHeight
                         - (pPdf->pageProperties.pageTitlesHeight * 2) - pPdf->pageProperties.pageHeightAfterTitles;
  m_workingRect.bottom   = m_workingRect.top - (pPdf->pageProperties.headerHeightOfRow * maxTitleRow);
  m_workingRect.left     = pPdf->pageProperties.pageLeftMargin + pPdf->pageProperties.startWidth;
  if ( curCol )
     m_workingRect.left +=  (pPdf->pageProperties.headerColsWidth[0] * pPdf->pageProperties.em2Point);
  int iCol;
  for(iCol = startCol; iCol < curCol; iCol++)
     m_workingRect.left += (pPdf->pageProperties.headerColsWidth[iCol] * pPdf->pageProperties.em2Point);
  m_workingRect.right    = m_workingRect.left + (pPdf->pageProperties.headerColsWidth[curCol] * pPdf->pageProperties.em2Point);

  if ( !bTest )
    {
     HPDF_Page_SetRGBFill(page, 0, 0.0, 0.4); // Fond bleu
     HPDF_Page_Rectangle(page, m_workingRect.left, m_workingRect.bottom, m_workingRect.right - m_workingRect.left, m_workingRect.top - m_workingRect.bottom);
     HPDF_Page_Fill(page);
     HPDF_Page_SetRGBFill(page, 1.0, 1.0, 1.0); // Caracteres blanc
     gdPdf_draw_column(m_workingRect, page, 0);
    }
//  m_workingRect.bottom   = m_workingRect.top - (pPdf->pageProperties.headerHeightOfRow * rowTitleNeeded);
//  return gdPdf_draw_label(m_workingRect, page, pTitle, HPDF_TALIGN_CENTER, bTest);
//  fprintf(stderr, "top=%f,bottom=%f,left=%f,right=%f, pTitle=<%s>, bTest=%d\n", m_workingRect.top, m_workingRect.bottom, m_workingRect.left, m_workingRect.right, pTitle, bTest);
  return gdPdf_draw_label(m_workingRect, page, pTitle, HPDF_TALIGN_LEFT, bTest);
 }

int gdPdf_printRowTitle(gdStPdf* pPdf, HPDF_Page page, int startRow, int curRow, const char* pTitle, int rowTitleNeeded, int bTest, int rowSep, float fWidth, int align)
 {
  HPDF_Rect       m_workingRect;
  m_workingRect.top    = pPdf->pageProperties.pageHeight            - pPdf->pageProperties.pageTopMargin - pPdf->pageProperties.startHeight - (pPdf->pageProperties.pageTitlesHeight * 2)
                       - pPdf->pageProperties.pageHeightAfterTitles - (pPdf->pageProperties.headerHeightOfRow * pPdf->pageProperties.headerMaxNbRows);
  int iRow;
  for(iRow = startRow; iRow < curRow; iRow++)
    m_workingRect.top -= (pPdf->pageProperties.headerHeightOfRow *  pPdf->pageProperties.nbRowsOfRowLabel[iRow]);
  m_workingRect.bottom = m_workingRect.top - (pPdf->pageProperties.headerHeightOfRow * rowTitleNeeded);
  m_workingRect.left   = pPdf->pageProperties.pageLeftMargin + pPdf->pageProperties.startWidth;
  if ( rowSep )
    {
     m_workingRect.right  = m_workingRect.left + fWidth;
     if ( curRow == startRow )
        HPDF_Page_SetRGBFill(page, 0.0, 0.0, 0.0); // Caractere noir
//        HPDF_Page_SetRGBFill(page, 0.0, 0.0, 0.4); // Caractere bleu
    }
   else
     m_workingRect.right  = m_workingRect.left + (pPdf->pageProperties.headerColsWidth[0] * pPdf->pageProperties.em2Point);

  if ( !bTest && !rowSep)
    {
     if ( !(curRow % 2) )
        HPDF_Page_SetRGBFill(page, 0.941, 0.949, 1.0 ); // Fond bleu pale
      else
        HPDF_Page_SetRGBFill(page, 1.0, 1.0, 1.0);      // Fond blanc
     HPDF_Page_Rectangle(page, m_workingRect.left, m_workingRect.bottom, m_workingRect.right - m_workingRect.left, m_workingRect.top - m_workingRect.bottom);
     HPDF_Page_Fill(page);
     HPDF_Page_SetRGBFill(page, 0, 0.0, 0.0);     // Caractere noir
     HPDF_Page_SetRGBStroke(page, 0, 0.2, 0.6); // Traits bleu
     gdPdf_draw_column(m_workingRect, page, 0);
    }
  return gdPdf_draw_label(m_workingRect, page, pTitle, (align ? HPDF_TALIGN_RIGHT : HPDF_TALIGN_LEFT), bTest);
 }

int gdPdf_printQte(gdStPdf* pPdf, HPDF_Page page, int startRow, int curRow, int startCol, int curCol, const char* pQte, int colSep, int align)
 {
  if ( colSep ) return;
  HPDF_Rect       m_workingRect;
  m_workingRect.top    = pPdf->pageProperties.pageHeight - pPdf->pageProperties.pageTopMargin - pPdf->pageProperties.startHeight - (pPdf->pageProperties.pageTitlesHeight * 2)
                       - pPdf->pageProperties.pageHeightAfterTitles - (pPdf->pageProperties.headerHeightOfRow * pPdf->pageProperties.headerMaxNbRows);
  int iRow;
  for(iRow = startRow; iRow < curRow; iRow++)
    m_workingRect.top -= (pPdf->pageProperties.headerHeightOfRow * pPdf->pageProperties.nbRowsOfRowLabel[iRow]);

//  if ( row ) m_workingRect.top -= (pPdf->pageProperties.headerHeightOfRow * row);
  m_workingRect.bottom = (m_workingRect.top - pPdf->pageProperties.headerHeightOfRow * pPdf->pageProperties.nbRowsOfRowLabel[curRow]);
  m_workingRect.left   = pPdf->pageProperties.pageLeftMargin + pPdf->pageProperties.startWidth + (pPdf->pageProperties.headerColsWidth[0] * pPdf->pageProperties.em2Point);
  int iCol;
  for(iCol = startCol; iCol < curCol; iCol++)
     m_workingRect.left += (pPdf->pageProperties.headerColsWidth[iCol] * pPdf->pageProperties.em2Point);
  m_workingRect.right  = m_workingRect.left + (pPdf->pageProperties.headerColsWidth[curCol] * pPdf->pageProperties.em2Point);

  if ( !(curRow % 2) )     HPDF_Page_SetRGBFill(page, 0.941, 0.949, 1.0); // Fond bleu pale
   else                    HPDF_Page_SetRGBFill(page,   1.0,   1.0, 1.0); // Fond blanc
  HPDF_Page_Rectangle(page, m_workingRect.left, m_workingRect.bottom, m_workingRect.right - m_workingRect.left, m_workingRect.top - m_workingRect.bottom);
  HPDF_Page_Fill(page);
  HPDF_Page_SetRGBFill(page, 0, 0, 0);       // Caractere noir
  HPDF_Page_SetRGBStroke(page, 0, 0.2, 0.6); // Traits bleu
  gdPdf_draw_column(m_workingRect, page, 0);
  if ( pPdf->pageProperties.nbRowsOfRowLabel[curRow] > 1 )
     m_workingRect.top -= ((pPdf->pageProperties.headerHeightOfRow * pPdf->pageProperties.nbRowsOfRowLabel[curRow]) - pPdf->pageProperties.headerHeightOfRow);
  return gdPdf_draw_label(m_workingRect, page, pQte, (align ? HPDF_TALIGN_RIGHT : HPDF_TALIGN_LEFT), 0);
 }

void gdPdf_print_pageNumber(gdStPdf* pPdf, HPDF_Page page, char* pnText)
 {
  HPDF_Rect       m_workingRect;

  m_workingRect.left         = pPdf->pageProperties.pageWidth / 2;
  m_workingRect.top          = pPdf->pageProperties.pageBottomMargin + pPdf->pageProperties.footerHeight;
  m_workingRect.right        = pPdf->pageProperties.pageWidth  - pPdf->pageProperties.pageRightMargin;
  m_workingRect.bottom       = pPdf->pageProperties.pageBottomMargin;
  HPDF_Page_BeginText(page);
  HPDF_Page_TextRect(page, m_workingRect.left, m_workingRect.top, m_workingRect.right, m_workingRect.bottom, pnText, HPDF_TALIGN_RIGHT, NULL);
  HPDF_Page_EndText(page);
 }

void gdPdf_print_date(gdStPdf* pPdf, HPDF_Page page)
 {
  HPDF_Rect       m_workingRect;
  char            buf[128];

  struct tm  *gd_tm;
  time_t      timesys;

  timesys = time( 0L );
  gd_tm = localtime( &timesys );
  gd_tm->tm_mon++;
  gd_tm->tm_year+=1900;
  gd_tm->tm_wday++;
  gd_tm->tm_yday++;

  sprintf(buf, "Date : %02d/%02d/%04d", gd_tm->tm_mday, gd_tm->tm_mon, gd_tm->tm_year);

  m_workingRect.left         = pPdf->pageProperties.pageLeftMargin;
  m_workingRect.top          = pPdf->pageProperties.pageBottomMargin + pPdf->pageProperties.footerHeight;
  m_workingRect.right        = pPdf->pageProperties.pageWidth  / 2;
  m_workingRect.bottom       = pPdf->pageProperties.pageBottomMargin;
  HPDF_Page_BeginText(page);
  HPDF_Page_TextRect(page, m_workingRect.left, m_workingRect.top, m_workingRect.right, m_workingRect.bottom, buf, HPDF_TALIGN_LEFT, NULL);
  HPDF_Page_EndText(page);
 }

void gdPdf_draw_column(HPDF_Rect m_workingRect, HPDF_Page page, const char* label)
 {
  gdPdf_draw_column2(m_workingRect, page, label, HPDF_TALIGN_LEFT, 1, 1, 1, 1);
 }

void gdPdf_draw_column2(HPDF_Rect m_workingRect, HPDF_Page page, const char* label, int align, int top, int bottom, int left, int right)
 {
  HPDF_Page_SetLineWidth(page, 0.5);
// Draw horizontal line top
  if ( top ) {
     HPDF_Page_MoveTo(page, m_workingRect.left,  m_workingRect.top);
     HPDF_Page_LineTo(page, m_workingRect.right, m_workingRect.top);
     HPDF_Page_Stroke(page);
    }

// Draw horizontal line bottom
  if ( bottom ) {
     HPDF_Page_MoveTo(page, m_workingRect.left,  m_workingRect.bottom);
     HPDF_Page_LineTo(page, m_workingRect.right, m_workingRect.bottom);
     HPDF_Page_Stroke(page);
    }

// Draw vertical line left
  if ( left ) {
     HPDF_Page_MoveTo(page, m_workingRect.left, m_workingRect.top);
     HPDF_Page_LineTo(page, m_workingRect.left, m_workingRect.bottom);
     HPDF_Page_Stroke(page);
    }

// Draw vertical line right
  if ( right ) {
     HPDF_Page_MoveTo(page, m_workingRect.right, m_workingRect.top);
     HPDF_Page_LineTo(page, m_workingRect.right, m_workingRect.bottom);
     HPDF_Page_Stroke(page);
    }

  if ( !label ) return;
  HPDF_PageAttr attr = (HPDF_PageAttr) page->attr;
  HPDF_Box bbox = HPDF_Font_GetBBox(attr->gstate->font);
  HPDF_Rect rect;
  rect.top     = m_workingRect.top    + (bbox.top    / 1000);
  rect.bottom  = m_workingRect.bottom - (bbox.bottom / 1000);
  rect.left    = m_workingRect.left   + (bbox.left   / 1000);
  rect.right   = m_workingRect.right  - (bbox.right  / 1000);
  gdPdf_draw_label(rect, page, label, align, 0);
 }

int gdPdf_draw_label(HPDF_Rect m_workingRect, HPDF_Page page, const char* label, int align, int bTest)
 {
  HPDF_RGBColor rgbCol;
  if ( bTest )
    {
     rgbCol = HPDF_Page_GetRGBFill(page);
     HPDF_Page_SetRGBFill(page, 1.0, 1.0, 1.0); // Caractere blanc
    }

  int len = 0;
  HPDF_Page_BeginText(page);
  HPDF_STATUS hStatus =
     HPDF_Page_TextRect(page, m_workingRect.left + TitlePaddingH,  m_workingRect.top - TitlePaddingV,
                              m_workingRect.right - TitlePaddingH, m_workingRect.bottom + TitlePaddingV, label, align, &len);
  HPDF_Page_EndText(page);

  if ( bTest )
     HPDF_Page_SetRGBFill(page, rgbCol.r, rgbCol.g, rgbCol.b);

  if ( hStatus == HPDF_OK ) return 1;
  return 0;
 }

/*
   Guy :
      How can I retrieve the maximum height needed by a font ?
   mikitad@gmail.com :
      i had this problem to, so I wrote a quick function as I didn't find
      antyhihg that let me do this as well - here is what I came up with
      hope it helps ever one out there
      obviously on an error we are returning -1, I haven't tested this all
      that well, and there is an infinite loop in there so be carfull while
      using it!
*/
double gdPdf_measureText(const char* aStr, HPDF_REAL aWidth, HPDF_Page aPage)
 {
/*
  J'ai commente ces lignes et modifié le return car je ne veut que la hauteur d'une seule ligne 

  string str = aStr;
  HPDF_REAL count = 0;
  for(;;)
   {
    count++;
    HPDF_UINT unit = HPDF_Page_MeasureText(aPage, str.c_str(), aWidth, HPDF_TRUE, NULL);
    if ( unit == str.length() )
       break;
    else if ( !unit )
       return -1;
    str = str.substr(unit);
  }

  if ( count > 0 )
   {
*/
    HPDF_PageAttr attr = (HPDF_PageAttr) aPage->attr;

    HPDF_Box bbox = HPDF_Font_GetBBox(attr->gstate->font);
    // fprintf(stderr, "bbox - top = %f, bottom =%f, left = %f, tight = %f\n", bbox.top, bbox.bottom, bbox.left, bbox.right); 

    if ( attr->gstate->text_leading == 0 )
       HPDF_Page_SetTextLeading (aPage, (bbox.top - bbox.bottom) / 1000 * attr->gstate->font_size);

    HPDF_REAL lineSize = HPDF_Page_GetTextLeading(aPage);

    return lineSize;
/*
    return count*lineSize;
   }

  return -1;
*/
 }

