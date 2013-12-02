#ifndef __GD_HEADER_gdPdf__
#define __GD_HEADER_gdPdf__            1

/*! \file gdPdf.h */

#ifdef __cplusplus
extern "C" {
#endif

#include <hpdf.h>

/*!
    \struct ST_GDPDF_COLS_SHEET
    \brief Describe the width sheet properties.
*/
typedef struct ST_GDPDF_COLS_SHEET {
  struct ST_GDPDF_COLS_SHEET*              prevSheet;                              //!< Previous sheet.
  struct ST_GDPDF_COLS_SHEET*              nextSheet;                              //!< Next sheet.
  float                                    sheetWidth;                             //!< Width of the sheet.
  int                                      nbColumns;                              //!< Number of columns in this sheet.
  int                                      colStart;                               //!< Index of the starting column.
  int                                      colEnd;                                 //!< Index of the ending column.
  int                                      pageBreak;                              //!< Skip page after this page are printed ?
} __attribute((packed)) ST_GDPDF_COLS_SHEET;

/*! \struct ST_GDPDF_ROWS_SHEET
    \brief Describe the height sheet properties.
*/
typedef struct ST_GDPDF_ROWS_SHEET {
  struct ST_GDPDF_ROWS_SHEET*              prevSheet;                              //!< Previous sheet.
  struct ST_GDPDF_ROWS_SHEET*              nextSheet;                              //!< Next sheet.
  float                                    sheetHeight;                            //!< Height of the sheet.
  int                                      nbRows;                                 //!< Number of rows in this sheet.
  int                                      rowStart;                               //!< Index of the staring row.
  int                                      rowEnd;                                 //!< Index of the ending row.
  int                                      pageBreak;                              //!< Skip page after this page are printed ?
} __attribute((packed)) ST_GDPDF_ROWS_SHEET;

/*! \struct ST_GDPDF_PAGE
    \brief Describe the page properties.
*/
typedef struct ST_GDPDF_PAGE { // Real = specified value * GDPDF_EM2PIXELS or value in point
  int                  nOptimized;               //!< Is the optimization are made ?
  HPDF_PageDirection   pageOrientation;          //!< Portrait or Landscape
  float                em2Point;                 //!< Conersion unit from em to point.
  int                  maxRowsInHeader;          //!< The maximum number of rows in the header.
  int                  maxRowsInRowLabel;        //!< The maximum number of rows for the row label.
  float                fontTitlesSize;           //!< The font size used for the titles.
  float                fontHeaderSize;           //!< The font size used for the header.
  float                fontRowLabelSize;         //!< The font size used for the row label.
  float                fontRowQteSize;           //!< The font size used for the quantity;.
  float                pageWidth;                //!< Real width of a page
  float                pageHeight;               //!< Real height of a page
  float                pageTopMargin;            //!< Real Top page margin
  float                pageBottomMargin;         //!< Real Bottom page margin
  float                pageLeftMargin;           //!< Real Left page margin
  float                pageRightMargin;          //!< Real Right page margin
  float                startHeight;              //!< Start drawn from here.
  float                startWidth;               //!< Start drawn from here.
  float                pageTitlesHeight;         //!< Height of the 4 titles of a page
  float                pageHeightAfterTitles;    //!< Height between the title and the header of the sheet.
  int                  pageNbRows;               //!< Number of rows by page.
  int                  pageNbColumns;            //!< Number of columns by page
  float                headerHeightOfRow;        //!< Real Height of each Row in the header, given by a call on setFontColTitle() function.
  int*                 headerColsWidth;          //!< Width of each column header.
  int                  headerMaxNbRows;          //!< Maximum rows needed for the biggest label in columns titles.
  int*                 headerNbRowsOfEachColumn; //!< Rows in sheet header needed for each column.
  int                  nbRowsOfLabel;            //!< Maximum rows needed for the biggest label in rows titles.
  int*                 nbRowsOfRowLabel;         //!< Rows in sheet line label needed for each row.
  float                footerHeight;             //!< Height of footer (contain the page number, ect...) given by setFontColTitle() function.
  int                  nbColumnsSheets;          //!< Number of width sheets when the virtual size of a page are bigger that the real page size.
  int                  nbRowsSheets;             //!< Number of height sheets when the virtual size of a page are bigger that the real page size.
  ST_GDPDF_COLS_SHEET* rootSheetColumns;         //!< Columns Sheets array.
  ST_GDPDF_ROWS_SHEET* rootSheetRows;            //!< Rows Sheets array.
} __attribute((packed)) ST_GDPDF_PAGE;

/*! \struct gdStPdf
    \brief The application handle.
*/
typedef struct st_gdpdf_def {
  HPDF_Doc        pdfHandle;                 //!< hpdf handle.
  HPDF_Outline    pdfRootOutline;            //!< hpdf main outline.
  const char*     pdfFile;                   //!< The path to the pdf file.
  int             withBookmarks;             //!< With outline ?
  ST_GDPDF_PAGE   pageProperties;            //!< Application handle page properties.
} __attribute((packed)) gdStPdf;


/*! \brief Open a pdf file.

 */
extern gdStPdf*               gdPdf_openPdf(const char* pPdfFile, int withBookmarks);

/*! \brief Close a pdf handle.

 */
extern void                   gdPdf_closePdf(gdStPdf* pPdf);

/*! \brief Save the pdf document.

 */
extern int                    gdPdf_savePdf(gdStPdf* pPdf);

/*! \brief Obtain a new virtual sheet by columns optimization.

 */
extern ST_GDPDF_COLS_SHEET*   gdPdf_getNewColsSheet();

/*! \brief Obtain a new virtual sheet by rows optimization.

 */
extern ST_GDPDF_ROWS_SHEET*   gdPdf_getNewRowsSheet();

/*! \brief Cleanup memory resources.

 */
extern void                   gdPdf_clearPageProperties(gdStPdf* pPdf);

/*! \brief Set the font used by page titles.

 */
extern void                   gdPdf_setFontTitle(gdStPdf* pPdf, HPDF_Page page);

/*! \brief Set the font used by sheet columns titles (header data).

 */
extern void                   gdPdf_setFontColTitle(gdStPdf* pPdf, HPDF_Page page);

/*! \brief Set the font used by sheet rows titles (column 0 data).

 */
extern void                   gdPdf_setFontRowTitle(gdStPdf* pPdf, HPDF_Page page);

/*! \brief Set the font used by sheet cells quantity data).

 */
extern void                   gdPdf_setFontQte(gdStPdf* pPdf, HPDF_Page page);

/*! \brief Draw a title.

 */
extern void                   gdPdf_printTitle (HPDF_Page page, const char* pTitle, int align, HPDF_Rect m_workingRect);

/*! \brief Compute the rectangle for a top left title.

 */
extern void                   gdPdf_printTitleA(gdStPdf* pPdf, HPDF_Page page, const char* pTitle);

/*! \brief Compute the rectangle for a top right title.

 */
extern void                   gdPdf_printTitleB(gdStPdf* pPdf, HPDF_Page page, const char* pTitle);

/*! \brief Compute the rectangle for a bottom left title.

 */
extern void                   gdPdf_printTitleC(gdStPdf* pPdf, HPDF_Page page, const char* pTitle);

/*! \brief Compute the rectangle for a bottom right title.

 */
extern void                   gdPdf_printTitleD(gdStPdf* pPdf, HPDF_Page page, const char* pTitle);

/*! \brief Compute the rectangle for a column label splitted by row.

 */
extern int                    gdPdf_printColTitleByRow(gdStPdf* pPdf, HPDF_Page page, int startCol, int curCol, int curRow, const char* pTitle, int colSep);

/*! \brief Compute the rectangle for a column label.

 */
extern int                    gdPdf_printColTitle(gdStPdf* pPdf, HPDF_Page page, int startCol, int curCol, const char* pTitle, int maxTitleRow, int rowTitleNeeded, int bTest, int colSep);

/*! \brief Compute the rectangle for a row label.

 */
extern int                    gdPdf_printRowTitle(gdStPdf* pPdf, HPDF_Page page, int startRow, int curRow, const char* pTitle, int rowTitleNeeded, int bTest, int rowSep, float fWidth, int align);

/*! \brief Compute the rectangle for a quantity (cell) value.

 */
extern int                    gdPdf_printQte(gdStPdf* pPdf, HPDF_Page page, int startRow, int curRow, int startCol, int curCol, const char* pQte, int colSep, int align);

/*! \brief Compute the rectangle for printing a date in the bottom right corner.

 */
extern void                   gdPdf_print_date(gdStPdf* pPdf, HPDF_Page page);

/*! \brief Compute the rectangle for printing a page number in the bottom right corner.

 */
extern void                   gdPdf_print_pageNumber(gdStPdf* pPdf, HPDF_Page page, char* pnText);

/*! \brief Draw a column background and foreground.

 */
extern void                   gdPdf_draw_column(HPDF_Rect rect, HPDF_Page page, const char* label);
extern void                   gdPdf_draw_column2(HPDF_Rect rect, HPDF_Page page, const char* label, int align, int top, int bottom, int left, int right);

/*! \brief Draw a column label.

 */
extern int                    gdPdf_draw_label(HPDF_Rect rect, HPDF_Page page, const char* label, int align, int bTest);

/*! \brief retrieve the maximum height needed by a font

 */
extern double                 gdPdf_measureText(const char* aStr, HPDF_REAL aWidth, HPDF_Page aPage);

#ifdef __cplusplus
}
#endif

#endif // __GD_HEADER_gdPdf__

