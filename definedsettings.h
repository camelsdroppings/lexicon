
#define SID_SPLASH_DELAY "Delay"
#define SID_SPLASH_ENABLED "Enabled"
#define SID_SPLASH_LOCATION "Location"
#define SID_SYSTEM_ARABIC_FONT "Arabic font"
/*

These are the keys to access values in the INI file.

Format is SID_<section name>_<unique id>

They should be used whenever qsetttingsvalue() or qsettings.setValue() is called
They should not be translated.

 */
#define SID_ENTRY_BACK "Back"
#define SID_ENTRY_CSS  "CSS"
#define SID_ENTRY_CLEAN "Clean"
#define SID_ENTRY_CLEAR_SCENE "Clear scene"
#define SID_ENTRY_DEBUG "Debug"
#define SID_ENTRY_FIND  "Find"
#define SID_ENTRY_FIND_NEXT "Find next"
#define SID_ENTRY_FORWARD  "Forward"
#define SID_ENTRY_FONT_SIZE "Font size"
#define SID_ENTRY_HOME       "Home"
#define SID_ENTRY_MARGIN     "Margin"
#define SID_ENTRY_MARK     "Mark"
#define SID_ENTRY_MOVE_FOCUS_UP "Move focus up"
#define SID_ENTRY_MOVE_FOCUS_DOWN "Move focus down"
#define SID_ENTRY_NARROW  "Narrow"
#define SID_ENTRY_OFF_PAGE "Off page"
#define SID_ENTRY_RELOAD    "Reload"
#define SID_ENTRY_SCALE   "Scale"
#define SID_ENTRY_SHOW    "Show"
#define SID_ENTRY_STEP    "Step"
#define SID_ENTRY_HIGHLIGHT_COLOR "Highlight color"
#define SID_ENTRY_TEXT_WIDTH "Text width"
#define SID_ENTRY_WIDEN   "Widen"
#define SID_ENTRY_KEY_HELP "Key help"
#define SID_ENTRY_PRINT_NOTES  "Print Notes"
#define SID_ENTRY_PRINT_NODES  "Print Nodes"
#define SID_ENTRY_PRINT_INFO   "Print Info"
#define SID_ENTRY_ZOOM_IN "Zoom in"
#define SID_ENTRY_ZOOM_OUT "Zoom out"
#define SID_ENTRY_PRINT_CSS "Print Css"
#define SID_ENTRY_DUMP_HTML "Save Html"
#define SID_ENTRY_DUMP_XML  "Save Xml"
#define SID_ENTRY_DUMP_OUTPUT_HTML "Save output Html"
#define SID_ENTRY_SHOW_LINK_WARNING  "Show link warning"
#define SID_ENTRY_XML_VIEWER_SIZE "Xml viewer size"
#define SID_ENTRY_XML_VIEWER_POS "Xml viewer pos"
#define SID_ENTRY_OUTPUT_PATH "Debug directory"
#define SID_ENTRY_NODEINFO_CLOSE "Node info close"

#define SID_XSLT_ENTRY   "Entry"
#define SID_XSLT_PRINT   "Print"

#define SID_NOTES_DEBUG "Debug"
#define SID_NOTES_ENABLED "Enabled"
#define SID_NOTES_DATABASE "Database"
#define SID_NOTES_SHOW_KEYMAPS  "Show keymaps"
#define SID_NOTES_SHOW_PRINT  "Show print"
#define SID_NOTES_COLUMNS "Column widths"
#define SID_NOTES_DEFAULT_COLUMN_WIDTH "Default width"
#define SID_NOTES_CONTEXT_STYLE "Context style"
#define SID_NOTES_SUBSTR_LENGTH "Note substr length"
#define SID_NOTES_WORD_COLUMN "Word column"
#define SID_NOTES_SUBJECT_COLUMN "Subject column"
#define SID_NOTES_COLUMN_STATE   "Column state"
//#define SID_SHORTCUT_COLLAPSE_ALL "Collapse all"

#define SID_HISTORY_ENABLED          "Enabled"
#define SID_HISTORY_DATABASE         "Database"
#define SID_HISTORY_MENU_ARABIC_FONT "Menu Arabic font"
#define SID_HISTORY_NEW_TAB          "New tab"
#define SID_HISTORY_GO_TAB           "Activate tab"
#define SID_HISTORY_DUPLICATE_DEPTH  "Duplicate depth"
#define SID_HISTORY_SIZE             "Size"
#define SID_HISTORY_LIST_ARABIC_FONT "List Arabic font"
#define SID_HISTORY_VIEWER_SIZE      "Viewer size"
#define SID_HISTORY_VIEWER_POS       "Viewer pos"
#define SID_HISTORY_LIST_STATE       "List state"

#define SID_ROOTS_DEBUG "Debug"
#define SID_ROOTS_EXPAND "Expand"
#define SID_ROOTS_KEEP_FOCUS "Keep focus"
#define SID_ROOTS_MOVE_UP "Move up"
#define SID_ROOTS_MOVE_DOWN "Move down"
#define SID_ROOTS_BACKGROUND_COLOR "Background"
#define SID_ROOTS_ITYPE_FONT "Itype font"


#define SID_PRINTER_NAME "Name"
#define SID_PRINTER_ORIENTATION "Orientation"
#define SID_PRINTER_RESOLUTION "Resolution"
#define SID_PRINTER_PAPER_SIZE "Paper size"
#define SID_PRINTER_COPIES "Copies"
#define SID_PRINTER_FULL_PAGE "Full page"
#define SID_PRINTER_USE "Use"
#define SID_PRINTER_OUTPUT_PDF "Output to PDF"
#define SID_PRINTER_PDF_DIRECTORY "PDF directory"
#define SID_PRINTER_AUTONAME_PDF "Autoname PDF"
#define SID_PRINTER_AUTONAME_METHOD "Naming method"




#define SID_SHORTCUT_BOOKMARK_ADD              "Bookmark add"
#define SID_SHORTCUT_CONTENTS_COLLAPSE_ALL     "Contents collapse all"
#define SID_SHORTCUT_CONTENTS_COLLAPSE_LETTER  "Contents collapse letter"
#define SID_SHORTCUT_CONTENTS_SHOW             "Contents show"
#define SID_SHORTCUT_CONVERT_TO_ENTRY          "Convert to entry"
#define SID_SHORTCUT_DELETE_TAB                "Delete tab"
#define SID_SHORTCUT_DELETE_OTHER_TABS         "Delete other tabs"
#define SID_SHORTCUT_FOCUS_CONTENT             "Focus content"
#define SID_SHORTCUT_FOCUS_TREE                "Focus tree"
#define SID_SHORTCUT_GO_TAB                    "Go Tab"
#define SID_SHORTCUT_HISTORY_BACK              "History back"
#define SID_SHORTCUT_HISTORY_NEXT              "History next"
#define SID_SHORTCUT_HISTORY_LIST              "History list"
#define SID_SHORTCUT_HISTORY_CLEAR             "History clear"
#define SID_SHORTCUT_HISTORY_ENABLE            "History enable"
#define SID_SHORTCUT_HISTORY_DISABLE           "History disable"
#define SID_SHORTCUT_KEYMAPS_DISABLE           "Keymaps disable"
#define SID_SHORTCUT_KEYMAPS_ENABLE            "Keymaps enable"
#define SID_SHORTCUT_KEYMAPS_HELP              "Keymaps help"
#define SID_SHORTCUT_LOCAL_SEARCH              "Local search"
#define SID_SHORTCUT_LOCAL_SEARCH_CLEAR        "Local search clear"
#define SID_SHORTCUT_LOCAL_SEARCH_FIND         "Local search find"
#define SID_SHORTCUT_LOCAL_SEARCH_NEXT         "Local search next"
#define SID_SHORTCUT_LOCAL_SEARCH_SHOW         "Local search show"
#define SID_SHORTCUT_NAV_PAGE_MODE             "Nav Page mode"
#define SID_SHORTCUT_NAV_ROOT_MODE             "Nav Root mode"
#define SID_SHORTCUT_NAV_FIRST                 "Go first"
#define SID_SHORTCUT_NAV_NEXT                  "Go next"
#define SID_SHORTCUT_NAV_PREV                  "Go prev"
#define SID_SHORTCUT_NAV_LAST                  "Go last"

/*
#define SID_SHORTCUT_PAGE_FIRST                "Page first"
#define SID_SHORTCUT_PAGE_LAST                 "Page last"
#define SID_SHORTCUT_PAGE_NEXT                 "Page next"
#define SID_SHORTCUT_PAGE_PREV                 "Page prev"
#define SID_SHORTCUT_ROOT_FIRST                "Root first"
#define SID_SHORTCUT_ROOT_LAST                 "Root last"
#define SID_SHORTCUT_ROOT_NEXT                 "Root next"
#define SID_SHORTCUT_ROOT_PREV                 "Root prev"
*/
#define SID_SHORTCUT_OPTIONS                   "Options"
#define SID_SHORTCUT_PAGE_SEARCH               "Page search"
#define SID_SHORTCUT_QUIT                      "Quit"
#define SID_SHORTCUT_SEARCH_DELETE             "Search delete"
#define SID_SHORTCUT_SEARCH_ENTRY              "Search entry"
#define SID_SHORTCUT_SEARCH_HEAD               "Search head"
#define SID_SHORTCUT_SEARCH_NODE               "Search node"
#define SID_SHORTCUT_SEARCH_ROOT               "Search root"
#define SID_SHORTCUT_SEARCH_WORD               "Search word"
#define SID_SHORTCUT_SEARCH_PAGE               "Search page"
#define SID_SHORTCUT_SELECT_ALL                "Select all"
#define SID_SHORTCUT_SELECT_ENTRY              "Select entry"
#define SID_SHORTCUT_SHOW_NOTES                "Show notes"
#define SID_SHORTCUT_KEEP_SYNCED               "Toggle sync always"
#define SID_SHORTCUT_SYNC_PAGE_WITH_CONTENTS   "Sync page with contents"
#define SID_SHORTCUT_SYNC_CONTENTS_WITH_PAGE   "Sync contents with page"
#define SID_SHORTCUT_TOGGLE_INTERFACE          "Toggle interface"
#define SID_SHORTCUT_SHOW_LOGS                 "Show logs"
#define SID_SHORTCUT_ARABIC_FONT               "Change Arabic font"
#define SID_SHORTCUT_MENU_SHOW                 "Menu show"
#define SID_SHORTCUT_XREF_MODE                  "Xref mode"

#define SID_SEARCH_IGNORE_DIACRITICS           "Ignore diacritics"
#define SID_SEARCH_TYPE_REGEX                  "Regex"
#define SID_SEARCH_WHOLE_WORD                  "Whole word"

#define SID_SEARCH_DIACRITICS                  "Diacritics"

#define SID_HEADSEARCH_STEP                    "Step"
#define SID_HEADSEARCH_FOCUS_TABLE             "Focus table"
#define SID_HEADSEARCH_NEW_TAB                 "New tab"
#define SID_HEADSEARCH_GO_TAB                  "Activate tab"
#define SID_HEADSEARCH_WHOLE_WORD              "Whole word"
#define SID_HEADSEARCH_DIACRITICS              "Ignore diacritics"
#define SID_HEADSEARCH_TYPE_REGEX              "Regex"
#define SID_HEADSEARCH_FORCE                   "Force LTR"
#define SID_HEADSEARCH_STATE                   "State"
#define SID_HEADSEARCH_USE_PHRASE               "Use phrase"
#define SID_HEADSEARCH_COLUMNS                  "HeadSearch Export columns"
#define SID_HEADSEARCH_NODEINFO_CLOSE           "Node info close"

#define SID_LOCALSEARCH_SHOW_ALL               "Show all results"
#define SID_LOCALSEARCH_DIACRITICS             "Ignore diacritics"
#define SID_LOCALSEARCH_WHOLE_WORD             "Whole word"
#define SID_LOCALSEARCH_FORCE_LTR              "Force LTR"
#define SID_LOCALSEARCH_TYPE_REGEX             "Regex"
#define SID_LOCALSEARCH_FORCE                  "Force LTR"
#define SID_LOCALSEARCH_IGNORE_CASE            "Ignore case"

#define SID_FULLSEARCH_ONE_ROW                 "One row"
#define SID_FULLSEARCH_RESULTS_FONT            "Results Arabic font"
#define SID_FULLSEARCH_DEBUG                   "Debug"
#define SID_FULLSEARCH_FRAGMENT_SIZE           "Fragment size"
#define SID_FULLSEARCH_INCLUDE_HEADS           "Include heads"
#define SID_FULLSEARCH_HEAD_BACKGROUND         "Head word background"
#define SID_FULLSEARCH_HEAD_TEXT               "Head text"

#define SID_FULLSEARCH_VIEWER_POSITION         "Viewer position"
#define SID_FULLSEARCH_STEP                    "Step"
#define SID_FULLSEARCH_WHOLE_WORD              "Whole word"
#define SID_FULLSEARCH_DIACRITICS              "Ignore diacritics"
#define SID_FULLSEARCH_TYPE_REGEX              "Regex"
#define SID_FULLSEARCH_NEW_TAB                 "New tab"
#define SID_FULLSEARCH_GO_TAB                  "Activate tab"
#define SID_FULLSEARCH_FORCE                   "Force LTR"


#define SID_FULLSEARCH_MAIN_CONTEXT            "Main context"
#define SID_FULLSEARCH_ARABIC_CONTEXT          "Arabic context"

#define SID_ROOTSEARCH_NEW_TAB                 "Root new tab"
#define SID_ROOTSEARCH_GO_TAB                  "Root activate tab"
#define SID_PAGESEARCH_NEW_TAB                 "Page new tab"
#define SID_PAGESEARCH_GO_TAB                  "Page activate tab"
#define SID_NODESEARCH_NEW_TAB                 "Node new tab"
#define SID_NODESEARCH_GO_TAB                  "Node activate tab"

#define SID_ARABIC_FONT_NAME                   "Font"
#define SID_ARABIC_FONT_SIZE                   "Size"

#define SID_NODE_VIEWER_SIZE                   "Viewer size"
#define SID_NODE_ARABIC_FONT                   "Arabic font"

#define SID_CONTENTS_BACKGROUND                "Background"
#define SID_CONTENTS_DEBUG                     "Debug"
#define SID_CONTENTS_EXPAND                    "Expand"
#define SID_CONTENTS_MOVE_DOWN                 "Move down"
#define SID_CONTENTS_MOVE_UP                   "Move up"
#define SID_CONTENTS_STANDARD_FONT             "Standard font"
#define SID_CONTENTS_ARABIC_FONT               "Arabic font"
#define SID_CONTENTS_KEEP_FOCUS                "Keep focus"
#define SID_CONTENTS_SHOWHEAD                  "Show head"
#define SID_CONTENTS_SHOWENTRY                 "Show entry"
#define SID_CONTENTS_SHOWNODE                  "Show node"
#define SID_CONTENTS_SHOWSUPPLEMENT            "Show supplement"
#define SID_CONTENTS_ROMAN_ITYPES              "Verb form number Roman"
#define SID_CONTENTS_ITYPES                    "Itype"

#define SID_LOGGING_FILE                       "Logfile"
#define SID_LOGGING_MAXSIZE                    "Maximum size"
#define SID_LOGGING_LEVEL                      "Level"
#define SID_LOGGING_ARCHIVES                   "Archive"
#define SID_LOGGING_VIEWER_MAXLINES            "Viewer lines"
#define SID_LOGGING_VIEWER_INTERVAL            "Viewer refresh interval"
#define SID_LOGGING_ROTATE                     "Rotate"
#define SID_LOGGING_DEBUG                       "Debug"
#define SID_LOGGING_SIZE                        "Size"
#define SID_LOGGING_POS                         "Pos"
//#define SID_LOGGING_LOGFILE                     "Logfile"
#define SID_LOGGING_MAXIMUM_SIZE                "Maximum size"


#define SID_BOOKMARK_NEW_TAB                   "New tab"
#define SID_BOOKMARK_GO_TAB                    "Activate tab"
#define SID_BOOKMARK_DEBUG                     "Debug"
#define SID_BOOKMARK_ADD                       "Add"
#define SID_BOOKMARK_LIST                      "List"
#define SID_BOOKMARK_JUMP                      "Jump"
#define SID_BOOKMARK_REVERT                    "Revert"
#define SID_BOOKMARK_CLEAR                     "Clear"
#define SID_BOOKMARK_ID                        "Id"
#define SID_BOOKMARK_LIST_STATE                "Book marklist state"
#define SID_BOOKMARK_MENU_FORMAT               "Menu format"
#define SID_SYSTEM_DATABASE                    "Database"
#define SID_SYSTEM_STYLESHEET                  "Stylesheet"
#define SID_SYSTEM_TOOLBAR_TEXT                "Toolbar text"
#define SID_SYSTEM_INTERFACE_WARNING           "Show interface warning"
#define SID_SYSTEM_MENU_WARNING                "Show menu warning"
#define SID_SYSTEM_SAVE_SETTINGS               "Save settings"
#define SID_SYSTEM_SAVE_TABS                   "Save tabs"
#define SID_SYSTEM_RESTORE_TABS                "Restore tabs"
#define SID_SYSTEM_DEBUG                       "Debug"
#define SID_SYSTEM_SAVE_BOOKMARKS              "Save bookmarks"
#define SID_SYSTEM_RESTORE_BOOKMARKS           "Restore bookmarks"
#define SID_SYSTEM_DOCKED                      "Docked"
#define SID_SYSTEM_MINIMAL                     "Minimal interface"
#define SID_SYSTEM_BY_ROOT                     "By root"
#define SID_SYSTEM_TITLE                       "Title"
#define SID_SYSTEM_ICON_SIZE                   "Icon size"
#define SID_SYSTEM_CONTENTS_LINKED             "Contents linked"
#define SID_SYSTEM_TABBAR_NUMBER               "Tabs number"
#define SID_SYSTEM_TABBAR_MOVEABLE             "Tabs moveable"
#define SID_SYSTEM_CURRENT_TAB                 "Current tab"
#define SID_SYSTEM_RUN_DATE                    "Run date"
#define SID_SYSTEM_SIZE                        "Size"
#define SID_SYSTEM_POS                         "Pos"
#define SID_SYSTEM_STATE                       "State"

#define SID_SYSTEM_OPEN_LINK                   "Open links in current tab"
#define SID_SYSTEM_ACTIVATE_LINK               "Always Make link active"

#define SID_SYSTEM_EXIT                        "Exit"
#define SID_SYSTEM_MESSAGE_TIMEOUT             "Message time"
#define SID_SYSTEM_HELP                         "Help"
#define SID_SYSTEM_APPEND_NEW_TABS              "Append new tab"
#define SID_SYSTEM_LINK_CHECK_MODE              "Link check mode"
#define SID_SYSTEM_ALLOW_DUPLICATES             "Allow duplicates"
#define SID_MAPS_CURRENT_MAP                    "Current map"
#define SID_MAPS_ENABLED                        "Enabled"
#define SID_MAPS_DEBUG                          "Debug"


#define SID_HELP_CURRENT_PAGE                   "Current page"
#define SID_HELP_SIZE                           "Size"
#define SID_HELP_POS                            "Pos"
#define SID_HELP_COLLECTION                     "Help collection"

#define SID_HELP_LOCAL_URL                      "Current local URL"
#define SID_HELP_LOCAL_PREFIX                   "Local prefix"
#define SID_HELP_LOCAL_LOCATION                 "Local location"

#define SID_HELP_ONLINE_URL                     "Current online URL"
#define SID_HELP_ONLINE_PREFIX                  "Online prefix"
#define SID_HELP_ONLINE_LOCATION                "Online location"

#define SID_HELP_LOCAL                          "Local docs"

#define SID_EDITVIEW_SIZE                       "Size"
#define SID_EDITVIEW_POS                       "Pos"
#define SID_EDITVIEW_FILE_STYLESHEET           "Filename style"

#define SID_TABS_TYPE                           "type"
#define SID_TABS_PLACE                          "place"
#define SID_TABS_ZOOM                           "zoom"
#define SID_TABS_TEXTWIDTH                      "textwidth"
#define SID_TABS_HOME                           "home"
#define SID_TABS_TITLE                          "title"

#define SID_KEYBOARDS_CONFIG                    "Config"


#define SID_ICON_ABOUT  "About"
#define SID_ICON_BACK  "Back"
#define SID_ICON_DIRECTORY "Directory"
#define SID_ICON_BOOKMARKS "Bookmarks"
#define SID_ICON_CLEAR "Clear"
#define SID_ICON_DOCS "Docs"
#define SID_ICON_EXIT "Exit"
#define SID_ICON_FIRST "First"
#define SID_ICON_HISTORY "History"
#define SID_ICON_INSERT_LINK "Insert link"
#define SID_ICON_KEYMAPS "Keymaps"
#define SID_ICON_KEYMAPS_DISABLED "Keymaps-disabled"
#define SID_ICON_LAST "Last"
#define SID_ICON_LINK "Link"
#define SID_ICON_LOCAL_SEARCH "Local search"
#define SID_ICON_LOCAL_SEARCH_NEXT "Local search next"
#define SID_ICON_LOGS "Logs"
#define SID_ICON_NARROW "Narrow"
#define SID_ICON_NEXT "Next"
#define SID_ICON_NOTES "Notes"
#define SID_ICON_PREFERENCES "Preferences"
#define SID_ICON_PRINT "Print"
#define SID_ICON_SEARCH "Search"
#define SID_ICON_SYNC_LEFT "Sync left"
#define SID_ICON_SYNC_RIGHT "Sync right"
#define SID_ICON_UNLINK "Unlink"
#define SID_ICON_WIDEN "Widen"
#define SID_ICON_ZOOM "Zoom"
#define SID_ICON_ZOOM_IN "ZoomIn"
#define SID_ICON_ZOOM_OUT "ZoomOut"



#define SID_RESOURCES_IMAGES  "Image"


#define SID_EXPORT_SEARCH_COLUMNS     "Columns"
#define SID_EXPORT_SEARCH_SEPARATOR   "Separator"
#define SID_EXPORT_SEARCH_ROWS        "All Rows"
#define SID_EXPORT_SEARCH_FILENAME    "File"
#define SID_EXPORT_SEARCH_SIZE    "Size"
#define SID_EXPORT_SEARCH_POS    "Pos"
#define SID_EXPORT_SEARCH_SAVE    "Save settings"
#define SID_EXPORT_SEARCH_HEADERS "Headers"

#define SID_ALWAYS 0
#define SID_NEVER  1
#define SID_PROMPT 2


//#define SID_NOTES
#define SID_OPTIONS_CLOSE "Close warning"
// pagesetdialog
#define SID_PAGESET_MAIN_CONTEXT            "Main context"
#define SID_PAGESET_ARABIC_CONTEXT          "Arabic context"
#define SID_PAGESET_OVERWRITE_EXISTING      "Overwrite"
#define SID_PAGESET_SELECT_ALL              "Select all"
#define SID_PAGESET_SAVE_DIALOG_POS         "Save dialog pos"
#define SID_PAGESET_SAVE_DIALOG_SIZE        "Save dialog size"
#define SID_PAGESET_LOAD_DIALOG_POS         "Load dialog pos"
#define SID_PAGESET_LOAD_DIALOG_SIZE        "Load dialog size"
#define SID_PAGESET_PAGE_DIALOG_POS         "Page dialog pos"
#define SID_PAGESET_PAGE_DIALOG_SIZE        "Page dialog size"
#define SID_PAGESET_EDIT_DIALOG_POS         "Edit dialog pos"
#define SID_PAGESET_EDIT_DIALOG_SIZE        "Edit dialog size"
#define SID_PAGESET_TABLIST_STATE           "Tab list state"
#define SID_PAGESET_SETLIST_STATE           "Set list state"
#define SID_PAGESET_EDIT_TAB_LIST_STATE     "Edit tablist state"
#define SID_PAGESET_LOADSETLIST_STATE       "Load setlist state"
#define SID_PAGESET_PAGEENTRY_STATE         "Page entry state"
