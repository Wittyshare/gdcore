 /*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
 */
#if defined GD_HAVE_GNUMERIC

//#include <gnumeric-config.h>
#include <glib/gi18n.h>
#include "gnumeric.h"
#include "position.h"
#include "parse-util.h"
#include "workbook.h"
#include "workbook-priv.h"
#include "workbook-control.h"
#include "sheet.h"
#include "cell.h"
#include "value.h"
#include "dependent.h"
#include "expr-name.h"
#include "libgnumeric.h"
#include "gutils.h"
//#include "gnumeric-paths.h"
#include "gnm-plugin.h"
#include "command-context.h"
#include "command-context-stderr.h"
#include "workbook-view.h"
#include "goffice/goffice.h"
#include "gsf/gsf-utils.h"

#endif

#include "gdSpreadSheet.h"

void gdSpreadSheet_init()
 { 
#if defined GD_HAVE_GNUMERIC
  /* initialization */
  gnm_init();
  GOCmdContext* cc = cmd_context_stderr_new();
  gnm_plugins_init(GO_CMD_CONTEXT(cc));
  GOErrorInfo*   plugin_errs;
  char* outfile = go_shell_arg_to_uri("/tmp/test.gnumeric");
  GOFileSaver*  fileSaver = go_file_saver_for_file_name(outfile);
  
  go_plugin_db_activate_plugin_list(go_plugins_get_available_plugins(), &plugin_errs);
  if ( plugin_errs ) {
     // FIXME: What do we want to do here? 
     go_error_info_free(plugin_errs);
     fprintf(stderr, "could not activate plugins\n");
     g_object_unref(cc);
     gnm_pre_parse_shutdown();
     gnm_shutdown();
     return;
  }
  // create a workbookwith two sheets 
  Workbook *wb = workbook_new_with_sheets(2);
  // retrieve the first sheet 
  Sheet *sheet = workbook_sheet_by_index(wb, 0);

  WorkbookView* wbv = workbook_view_new(wb);

  int res = wb_view_save_as(wbv, fileSaver, outfile, cc);
  
  g_object_unref(cc);
  gnm_shutdown();
  gnm_pre_parse_shutdown();
  return;
#endif
 }

