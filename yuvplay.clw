; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CmpDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "yuvplay.h"

ClassCount=19
Class1=CYuvplayApp
Class2=CYuvplayDlg
Class3=CAboutDlg

ResourceCount=24
Resource1=IDD_DLG_ASTRING
Resource2=IDR_MAINFRAME
Resource3=IDD_DLG_COMPARE
Resource4=IDD_DLG_VALUES2
Class4=CmpDlg
Class5=ImageWnd
Class6=ValuesDlg
Class7=PosWnd
Resource5=IDD_DIALOG_PIXEL
Class8=DlgTranscode
Resource6=IDR_MENU_FAVOR (Chinese (P.R.C.))
Resource7=IDD_YUVPLAY_DIALOG (Chinese (P.R.C.))
Resource8=IDD_DLG_VALUES (Chinese (P.R.C.))
Resource9=IDD_DLG_COMPARE (Chinese (P.R.C.))
Resource10=IDD_DLG_TRANSCODE (Chinese (P.R.C.))
Resource11=IDD_ABOUTBOX (Chinese (P.R.C.))
Resource12=IDD_DLG_FILTER_GAIN
Class9=CDlgPixel
Resource13=IDD_DLG_FILT_Y8
Class10=DLGFormater
Resource14=IDD_DLG_VALUES
Class11=CDlgFilter
Resource15=IDD_ABOUTBOX
Class12=DLGProperty
Resource16=IDD_DLG_FILTER
Class13=DlgFilterGain
Resource17=IDD_DLG_FILTERS
Resource18=IDD_DLG_TRANSCODE
Class14=CDlgSimpleCfg
Resource19=IDD_DLG_MSG
Class15=CDlgMsg
Resource20=IDD_DIALOG_FORMATER
Class16=CDlgFilter_Y8
Resource21=IDD_DLG_PROPERTY
Class17=CDlgValues2
Resource22=IDD_YUVPLAY_DIALOG
Class18=CDlgAString
Resource23=IDD_DLG_SIMPLE_CFG
Class19=CDlgValues2Set
Resource24=IDD_DLG_VALUES2_SETTING

[CLS:CYuvplayApp]
Type=0
HeaderFile=yuvplay.h
ImplementationFile=yuvplay.cpp
Filter=N

[CLS:CYuvplayDlg]
Type=0
HeaderFile=yuvplayDlg.h
ImplementationFile=yuvplayDlg.cpp
Filter=D
LastObject=CYuvplayDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=yuvplayDlg.h
ImplementationFile=yuvplayDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC1,static,1342308480
Control3=IDC_STATIC3,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC2,static,1342308352

[DLG:IDD_DLG_VALUES]
Type=1
Class=ValuesDlg
ControlCount=8
Control1=IDC_RADIO_HEX,button,1342177289
Control2=IDC_RADIO_DEC,button,1342177289
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_GRID_POS,edit,1342244992
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_START_POINT,edit,1342244992
Control7=IDC_EDIT_VALUES,edit,1345325060
Control8=IDC_BUTTON_SET,button,1342242816

[DLG:IDD_YUVPLAY_DIALOG]
Type=1
Class=CYuvplayDlg
ControlCount=41
Control1=IDC_SLIDER_PLAY,msctls_trackbar32,1342242840
Control2=IDC_STATIC_FRAMENUM,edit,1342242945
Control3=IDC_BUTTON_OPEN,button,1342242816
Control4=IDC_BUTTON_BOOKMARK,button,1342242816
Control5=IDC_BUTTON_PLAY,button,1342242816
Control6=IDC_BUTTON_PAUSE,button,1342242816
Control7=IDC_BUTTON_STOP,button,1342242816
Control8=IDC_BUTTON_PREV,button,1342242816
Control9=IDC_BUTTON_NEXT,button,1342242816
Control10=IDC_BUTTON_SAVE,button,1342242816
Control11=IDC_BUTTON_MORE,button,1342242816
Control12=IDC_STATIC_FRAME,static,1342177287
Control13=IDC_STATIC_SIZE,static,1342308352
Control14=IDC_COMBO_SIZE,combobox,1344340995
Control15=IDC_BUTTON_DELSIZE,button,1342242816
Control16=IDC_EDIT_WIDTH,edit,1350639745
Control17=IDC_STATIC_SIZE_X,static,1342308352
Control18=IDC_EDIT_HEIGHT,edit,1350639745
Control19=IDC_BUTTON_ADDSIZE,button,1342242816
Control20=IDC_STATIC_FORMAT,static,1342308352
Control21=IDC_COMBO_FORMAT,combobox,1344341059
Control22=IDC_STATIC_SHOW,static,1342308352
Control23=IDC_COMBO_SHOWYUV,combobox,1344341059
Control24=IDC_STATIC_FPS,static,1342308352
Control25=IDC_COMBO_FPS,combobox,1344341058
Control26=IDC_CHECK_LOOP,button,1342242819
Control27=IDC_BUTTON_APPLY,button,1342242817
Control28=IDC_CHECK_EXT,button,1342242819
Control29=IDC_STATIC_EXT,button,1342177287
Control30=IDC_CHECK_GRID,button,1342242819
Control31=IDC_COMBO_GRIDXN,combobox,1344340035
Control32=IDC_BUTTON_COLOR,button,1342242816
Control33=IDC_CHECK_COMPARE,button,1342242819
Control34=IDC_BUTTON_TRANSCODE,button,1342242816
Control35=IDC_BUTTON_FORMAT_INFO,button,1342242816
Control36=IDC_STATIC,static,1342308352
Control37=IDC_COMBO_FIELD,combobox,1344339971
Control38=IDC_CHECK_FILTER,button,1342242819
Control39=IDC_BUTTON_FILTER_CFG,button,1342242816
Control40=IDC_STATIC_HELP,static,1342308352
Control41=IDC_CHECK_CONF,button,1342242819

[DLG:IDD_DLG_COMPARE]
Type=1
Class=CmpDlg
ControlCount=30
Control1=IDC_SLIDER_PLAY,msctls_trackbar32,1342242840
Control2=IDC_STATIC_FRAMENUM,static,1342308352
Control3=IDC_BUTTON_OPEN,button,1342242816
Control4=IDC_BUTTON_PREV,button,1342242816
Control5=IDC_BUTTON_NEXT,button,1342242816
Control6=IDC_BUTTON_STOP,button,1342242816
Control7=IDC_BUTTON_SAVE,button,1342242816
Control8=IDC_BUTTON_SAVE2,button,1342242816
Control9=IDC_CHECK_LOCKSYNC,button,1342242819
Control10=IDC_RADIO_BI,button,1342177289
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT_THRESHOLD,edit,1350631552
Control13=IDC_RADIO_DIFF,button,1342177289
Control14=IDC_BUTTON_OK,button,1342242816
Control15=IDC_STATIC,static,1342308352
Control16=IDC_COMBO_FORMAT,combobox,1344339971
Control17=IDC_STATIC,static,1342308352
Control18=IDC_COMBO_FIELD,combobox,1344339971
Control19=IDC_CHECK_FILTER,button,1342242819
Control20=IDC_BUTTON_FILTER_CFG,button,1342242816
Control21=IDC_STATIC,button,1342177287
Control22=IDC_BUTTON_FORMAT_INFO,button,1342242816
Control23=IDC_STATIC,static,1342308352
Control24=IDC_EDIT_WIDTH,edit,1350639745
Control25=IDC_STATIC,static,1342308352
Control26=IDC_EDIT_HEIGHT,edit,1350639745
Control27=IDC_STATIC_FRAME,static,1342177287
Control28=IDC_STATIC,static,1342308352
Control29=IDC_COMBO_CMP_COMP,combobox,1344339971
Control30=IDC_STATIC_HELP,static,1342308352

[CLS:CmpDlg]
Type=0
HeaderFile=CmpDlg.h
ImplementationFile=CmpDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CmpDlg
VirtualFilter=dWC

[CLS:ImageWnd]
Type=0
HeaderFile=ImageWnd.h
ImplementationFile=ImageWnd.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=IDM_BK_ADD

[CLS:ValuesDlg]
Type=0
HeaderFile=ValuesDlg.h
ImplementationFile=ValuesDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_GRID_POS
VirtualFilter=dWC

[CLS:PosWnd]
Type=0
HeaderFile=PosWnd.h
ImplementationFile=PosWnd.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=PosWnd

[DLG:IDD_DLG_TRANSCODE]
Type=1
Class=DlgTranscode
ControlCount=17
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_FILE,edit,1350631552
Control3=IDC_BUTTON_OPEN,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_COMBO_FORMAT,combobox,1344340035
Control6=IDC_STATIC,static,1342308352
Control7=IDC_COMBO_START,combobox,1344339970
Control8=IDC_STATIC,static,1342308352
Control9=IDC_COMBO_FRAMENUM,combobox,1344339970
Control10=IDC_BUTTON_TRANS,button,1342242816
Control11=IDC_BUTTON_QUIT,button,1342242816
Control12=IDC_STATIC_INFO,static,1342308865
Control13=IDC_STATIC,static,1342308352
Control14=IDC_COMBO_SKIP_FRAME,combobox,1344339970
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC_SRC_INFO,static,1342308352
Control17=IDC_STATIC_HELP,static,1342308352

[CLS:DlgTranscode]
Type=0
HeaderFile=DlgTranscode.h
ImplementationFile=DlgTranscode.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COMBO_FORMAT
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX (Chinese (P.R.C.))]
Type=1
Class=CAboutDlg
ControlCount=6
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT1,edit,1342244996

[DLG:IDD_YUVPLAY_DIALOG (Chinese (P.R.C.))]
Type=1
Class=CYuvplayDlg
ControlCount=33
Control1=IDC_BUTTON_MORE,button,1342242816
Control2=IDC_BUTTON_OPEN,button,1342242816
Control3=IDC_BUTTON_PREV,button,1342242816
Control4=IDC_BUTTON_NEXT,button,1342242816
Control5=IDC_SLIDER1,msctls_trackbar32,1342242840
Control6=IDC_STATIC_FRAMENUM,static,1342308352
Control7=IDC_BUTTON_PLAY,button,1342242816
Control8=IDC_BUTTON_PAUSE,button,1342242816
Control9=IDC_BUTTON_STOP,button,1342242816
Control10=IDC_BUTTON_SAVE,button,1342242816
Control11=IDC_EDIT_WIDTH,edit,1350639744
Control12=IDC_EDIT_HEIGHT,edit,1350639744
Control13=IDC_COMBO_SIZE,combobox,1344340995
Control14=IDC_COMBO_SHOWYUV,combobox,1344341059
Control15=IDC_COMBO_FORMAT,combobox,1344341059
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_COMBO_FPS,combobox,1344341058
Control19=IDC_STATIC,static,1342308352
Control20=IDC_BUTTON_ADDSIZE,button,1342242816
Control21=IDC_BUTTON_DELSIZE,button,1342242816
Control22=IDC_STATIC,static,1342308352
Control23=IDC_CHECK_GRID,button,1342242819
Control24=IDC_COMBO_GRIDXN,combobox,1344340034
Control25=IDC_STATIC1,static,1342177287
Control26=IDC_STATIC2,static,1342177287
Control27=IDC_BUTTON_APPLY,button,1342242817
Control28=IDC_CHECK_COMPARE,button,1342242819
Control29=IDC_CHECK_LOOP,button,1342242819
Control30=IDC_BUTTON_COLOR,button,1342242816
Control31=IDC_STATIC,static,1342308353
Control32=IDC_BUTTON_TRANSCODE,button,1342242816
Control33=IDC_BUTTON_BOOKMARK,button,1342242816

[DLG:IDD_DLG_VALUES (Chinese (P.R.C.))]
Type=1
Class=ValuesDlg
ControlCount=9
Control1=IDC_EDIT_VALUES,edit,1345325060
Control2=IDC_RADIO_HEX,button,1342177289
Control3=IDC_RADIO_DEC,button,1342177289
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_GRID_POS,edit,1342244992
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_GRID_SIZE,edit,1342244992
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_START_POINT,edit,1342244992

[DLG:IDD_DLG_COMPARE (Chinese (P.R.C.))]
Type=1
Class=CmpDlg
ControlCount=14
Control1=IDC_BUTTON_OPEN,button,1342242816
Control2=IDC_BUTTON_PREV,button,1342242816
Control3=IDC_BUTTON_NEXT,button,1342242816
Control4=IDC_SLIDER1,msctls_trackbar32,1342242840
Control5=IDC_STATIC_FRAMENUM,static,1342308352
Control6=IDC_BUTTON_SAVE,button,1342242816
Control7=IDC_CHECK_LOCKSYNC,button,1342242819
Control8=IDC_BUTTON_STOP,button,1342242816
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_THRESHOLD,edit,1350631552
Control11=IDC_BUTTON_OK,button,1342242816
Control12=IDC_RADIO_BI,button,1342177289
Control13=IDC_RADIO_DIFF,button,1342177289
Control14=IDC_BUTTON_SAVE2,button,1342242816

[DLG:IDD_DLG_TRANSCODE (Chinese (P.R.C.))]
Type=1
Class=DlgTranscode
ControlCount=12
Control1=IDC_STATIC,static,1342308352
Control2=IDC_COMBO_FORMAT,combobox,1344340035
Control3=IDC_STATIC,static,1342308352
Control4=IDC_COMBO_START,combobox,1344339970
Control5=IDC_STATIC,static,1342308352
Control6=IDC_COMBO_FRAMENUM,combobox,1344339970
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_FILE,edit,1350631552
Control9=IDC_BUTTON_OPEN,button,1342242816
Control10=IDC_STATIC_INFO,static,1342308865
Control11=IDC_BUTTON_TRANS,button,1342242816
Control12=IDC_BUTTON_QUIT,button,1342242816

[MNU:IDR_MENU_FAVOR (Chinese (P.R.C.))]
Type=1
Class=CYuvplayDlg
Command1=IDM_BK_ADD
Command2=IDM_BK_MAN
Command3=IDM_RECENT
CommandCount=3

[DLG:IDD_DIALOG_PIXEL]
Type=1
Class=CDlgPixel
ControlCount=11
Control1=IDC_STATIC_YUV0,static,1350696961
Control2=IDC_STATIC_YUV1,static,1350696961
Control3=IDC_STATIC_YUV2,static,1350696961
Control4=IDC_STATIC_YUV3,static,1350696961
Control5=IDC_STATIC_YUV4,static,1350696961
Control6=IDC_STATIC_YUV5,static,1350696961
Control7=IDC_STATIC_YUV6,static,1350696961
Control8=IDC_STATIC_YUV7,static,1350696961
Control9=IDC_STATIC_YUV8,static,1350696961
Control10=IDC_CHECK_HEX,button,1342242819
Control11=IDC_STATIC,static,1342308352

[CLS:CDlgPixel]
Type=0
HeaderFile=DlgPixel.h
ImplementationFile=DlgPixel.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgPixel

[DLG:IDD_DIALOG_FORMATER]
Type=1
Class=DLGFormater
ControlCount=15
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_NAME,edit,1342244992
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_VERSION,edit,1342244992
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_AUTHOR,edit,1342244992
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_PATH,edit,1342244992
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT_COMMENT,edit,1352730692
Control12=IDC_STATIC,static,1342308352
Control13=IDC_BUTTON_CFG1,button,1342242816
Control14=IDC_BUTTON_CFG2,button,1342242816
Control15=IDC_BUTTON_QUERY,button,1342242816

[CLS:DLGFormater]
Type=0
HeaderFile=DLGFormater.h
ImplementationFile=DLGFormater.cpp
BaseClass=CDialog
Filter=D
LastObject=DLGFormater
VirtualFilter=dWC

[DLG:IDD_DLG_FILTERS]
Type=1
Class=?
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_FILTERS,SysListView32,1350631437
Control4=IDC_BUTTON_PROPERTY,button,1342242816
Control5=IDC_BUTTON_CFG,button,1342242816
Control6=IDC_BUTTON_UP,button,1342242816
Control7=IDC_BUTTON_DOWN,button,1342242816

[CLS:DLGProperty]
Type=0
HeaderFile=DLGProperty.h
ImplementationFile=DLGProperty.cpp
BaseClass=CDialog
Filter=D
LastObject=DLGProperty
VirtualFilter=dWC

[DLG:IDD_DLG_FILTER_GAIN]
Type=1
Class=DlgFilterGain
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_VAL1,edit,1350631552
Control7=IDC_EDIT_VAL2,edit,1350631552
Control8=IDC_EDIT_VAL3,edit,1350631552

[CLS:DlgFilterGain]
Type=0
HeaderFile=DlgFilterGain.h
ImplementationFile=DlgFilterGain.cpp
BaseClass=CDialog
Filter=D
LastObject=DlgFilterGain
VirtualFilter=dWC

[DLG:IDD_DLG_PROPERTY]
Type=1
Class=DLGProperty
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_NAME,edit,1342244992
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_VERSION,edit,1342244992
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_AUTHOR,edit,1342244992
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_PATH,edit,1342244992
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT_COMMENT,edit,1352730692

[DLG:IDD_DLG_FILTER]
Type=1
Class=CDlgFilter
ControlCount=15
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_NAME,edit,1342244992
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_VERSION,edit,1342244992
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_AUTHOR,edit,1342244992
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_PATH,edit,1342244992
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT_COMMENT,edit,1352730692
Control12=IDC_STATIC,static,1342308352
Control13=IDC_BUTTON_CFG1,button,1342242816
Control14=IDC_BUTTON_CFG2,button,1342242816
Control15=IDC_BUTTON_QUERY,button,1342242816

[CLS:CDlgFilter]
Type=0
HeaderFile=DlgFilter.h
ImplementationFile=DlgFilter.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgFilter
VirtualFilter=dWC

[DLG:IDD_DLG_SIMPLE_CFG]
Type=1
Class=CDlgSimpleCfg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_VAL,edit,1350631552
Control6=IDC_EDIT_STR,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_VAL2,edit,1350631552

[CLS:CDlgSimpleCfg]
Type=0
HeaderFile=DlgSimpleCfg.h
ImplementationFile=DlgSimpleCfg.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgSimpleCfg
VirtualFilter=dWC

[DLG:IDD_DLG_MSG]
Type=1
Class=CDlgMsg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_EDIT_MSG,edit,1352730692

[CLS:CDlgMsg]
Type=0
HeaderFile=DlgMsg.h
ImplementationFile=DlgMsg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgMsg

[DLG:IDD_DLG_FILT_Y8]
Type=1
Class=CDlgFilter_Y8
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CHECK_Y8_EMU,button,1342242819

[CLS:CDlgFilter_Y8]
Type=0
HeaderFile=DlgFilter_Y8.h
ImplementationFile=DlgFilter_Y8.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgFilter_Y8
VirtualFilter=dWC

[DLG:IDD_DLG_VALUES2]
Type=1
Class=CDlgValues2
ControlCount=8
Control1=IDC_RADIO_HEX,button,1342177289
Control2=IDC_RADIO_DEC,button,1342177289
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_GRID_SIZE,edit,1350631553
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_START_POINT,edit,1350631553
Control7=IDC_EDIT_VALUES,edit,1345325060
Control8=IDC_BUTTON_SET,button,1342242816

[CLS:CDlgValues2]
Type=0
HeaderFile=DlgValues2.h
ImplementationFile=DlgValues2.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgValues2

[DLG:IDD_DLG_ASTRING]
Type=1
Class=CDlgAString
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552

[CLS:CDlgAString]
Type=0
HeaderFile=DlgAString.h
ImplementationFile=DlgAString.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgAString

[DLG:IDD_DLG_VALUES2_SETTING]
Type=1
Class=CDlgValues2Set
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CHECK_Y,button,1342242819
Control4=IDC_CHECK_U,button,1342242819
Control5=IDC_CHECK_V,button,1342242819
Control6=IDC_STATIC,button,1342177287

[CLS:CDlgValues2Set]
Type=0
HeaderFile=DlgValues2Set.h
ImplementationFile=DlgValues2Set.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgValues2Set

