/*==========================================================
 Copyright (C) 2005 YOKOGAWA ELECTRIC CORPORATION

    ALL RIGHTS RESERVED BY YOKOGAWA ELECTRIC CORPORATION.
THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT
WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND/OR FITNESS FOR A PARTICULAR PURPOSE.

                            YOKOGAWA ELECTRIC CORPORATION
==========================================================*/
// MainDialog.cpp

#include "stdafx.h"
#include "WT500Demo.h"
#include "MainDialog.h"
#include "CommDialog.h"
#include "Connection.h"
//#include <fstream.h>
using namespace std;
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//##########################################//
// CMainDialog
//##########################################//
const int MAX_ITEM    = 10;       //the max items count.
const int MAX_ELEMENT = 3;        //the max elements count.
const int MAX_LINES   = 100;      //the max monitor lines count.
const int TIMER_1     = 1;        //used when getting data by update rate.
const int TIMER_2     = 2;        //used when getting data by timer.
const char* MODEL     = "760203"; //used when testing instrument model.

const int MAX_FUNCTION          = 66;
const int NORMAL_FUNCTION_TOP   = 0;
const int NORMAL_FUNCTION_BOTTM = 41;
const int HARM_FUNCTION_TOP     = 42;
const int HARM_FUNCTION_BOTTOM  = 61;
const int DELTA_FUNCTION_TOP    = 62;
const int DELTA_FUNCTION_BOTTOM = 65;
const int MAX_ORDER             = 51;

CMainDialog::CMainDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDialog::IDD, pParent)
{
	HarmonicOption=-1;
	for(int i=0; i<9; i++)
		oldFuncName[i] = _T("U");
	//{{AFX_DATA_INIT(CMainDialog)
	m_sEditTest = _T("");
	m_sEditSend = _T("");
	m_sEditRcv = _T("");
	m_sEditError = _T("");
	m_sFileName = _T("");
	m_iRadioElement = -1;
	m_sComboItems = _T("");
	//}}AFX_DATA_INIT
}

void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDialog)
	DDX_Control(pDX, IDC_COMBO_F9, m_combo_f9);
	DDX_Control(pDX, IDC_COMBO_F8, m_combo_f8);
	DDX_Control(pDX, IDC_COMBO_F7, m_combo_f7);
	DDX_Control(pDX, IDC_COMBO_F6, m_combo_f6);
	DDX_Control(pDX, IDC_COMBO_F5, m_combo_f5);
	DDX_Control(pDX, IDC_COMBO_F4, m_combo_f4);
	DDX_Control(pDX, IDC_COMBO_F3, m_combo_f3);
	DDX_Control(pDX, IDC_COMBO_F10, m_combo_f10);
	DDX_Control(pDX, IDC_COMBO_F1, m_combo_f1);
	DDX_Control(pDX, IDC_COMBO_F2, m_combo_f2);
	DDX_Control(pDX, IDC_COMBOR, m_comboR);
	DDX_Control(pDX, IDC_COMBOV, m_comboU);
	DDX_Control(pDX, IDC_COMBOC, m_comboI);
	DDX_Text(pDX, IDC_EDIT_TEST, m_sEditTest);
	DDX_Text(pDX, IDC_EDIT_SEND, m_sEditSend);
	DDX_Text(pDX, IDC_EDIT_RCV, m_sEditRcv);
	DDX_Text(pDX, IDC_EDIT_ERR, m_sEditError);
	DDX_Text(pDX, IDC_EDIT_FILE, m_sFileName);
	DDX_Radio(pDX, IDC_RADIO_E1, m_iRadioElement);
	DDX_CBString(pDX, IDC_COMBO_ITEMS, m_sComboItems);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
	//{{AFX_MSG_MAP(CMainDialog)
	ON_BN_CLICKED(IDC_RADIO_E1, OnRadioE1)
	ON_BN_CLICKED(IDC_RADIO_E2, OnRadioE2)
	ON_BN_CLICKED(IDC_RADIO_E3, OnRadioE3)
	ON_BN_CLICKED(IDC_BUTTON_SETRANGE, OnButtonSetRange)
	ON_BN_CLICKED(IDC_BUTTON_SENDCMD, OnButtonSendCmd)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEMS, OnSelchangeComboItems)
	ON_BN_CLICKED(IDC_BUTTON_GETITEMS, OnButtonGetItems)
	ON_BN_CLICKED(IDC_BUTTON_SETRATE, OnButtonSetRate)
	ON_BN_CLICKED(IDC_BUTTON_SETITEMS, OnButtonSetItems)
	ON_BN_CLICKED(IDC_BUTTON_GETDATA_S, OnButtonGetDataS)
	ON_BN_CLICKED(IDC_BUTTON_GETDATA_R, OnButtonGetDataR)
	ON_BN_CLICKED(IDC_BUTTON_GETDATA_T, OnButtonGetDataT)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_SAVE, OnCheckSave)
	ON_BN_CLICKED(IDC_BUTTON_SCLR, OnButtonSclr)
	ON_BN_CLICKED(IDC_BUTTON_RCLR, OnButtonRclr)
	ON_BN_CLICKED(IDC_BUTTON_GETERR, OnButtonGetError)
	ON_BN_CLICKED(IDC_BUTTON_HEADER, OnButtonHeader)
	//ON_BN_CLICKED(IDC_BUTTON_R, OnButtonRst)
	ON_BN_CLICKED(IDCLOSE, OnDlgClose)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_VERBOSE, OnButtonVerbose)
	ON_CBN_SELCHANGE(IDC_COMBO_F2, OnSelchangeComboF2)
	ON_CBN_SELCHANGE(IDC_COMBO_F3, OnSelchangeComboF3)
	ON_CBN_SELCHANGE(IDC_COMBO_F4, OnSelchangeComboF4)
	ON_CBN_SELCHANGE(IDC_COMBO_F5, OnSelchangeComboF5)
	ON_CBN_SELCHANGE(IDC_COMBO_F6, OnSelchangeComboF6)
	ON_CBN_SELCHANGE(IDC_COMBO_F7, OnSelchangeComboF7)
	ON_CBN_SELCHANGE(IDC_COMBO_F8, OnSelchangeComboF8)
	ON_CBN_SELCHANGE(IDC_COMBO_F9, OnSelchangeComboF9)
	ON_CBN_SELCHANGE(IDC_COMBO_F10, OnSelchangeComboF10)
	ON_CBN_SELCHANGE(IDC_COMBO_F1, OnSelchangeComboF1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//==========================================//
///<summary>
///Function: ConstInit
///</summary>
//==========================================//
void CMainDialog::InitListItem()
{
    //===========================
    ///Error Code from Tmctl
    //===========================
    //Error Number  Problem                                  Solutions
	errorMsg[0]  = "Device not found";            //   2  1 Check the wiring.  
	errorMsg[1]  = "Connection to device failed"; //   4  2 Check the wiring.  
	errorMsg[2]  = "Device not connected";        //   8  3 Connect the device using the initialization function.  
	errorMsg[3]  = "Device already connected";    //  16  4 Two connections cannot be opened.  
	errorMsg[4]  = "Incompatible PC";             //  32  5 Check the hardware you are using.  
	errorMsg[5]  = "Illegal parameter";           //  64  6 Check parameter type etc. 
    errorMsg[6].Empty();
	errorMsg[7]  = "Send error";                  // 256  8 Check the wiring, address, and ID.
	errorMsg[8]  = "Receive error";               // 512  9 Check whether an error occurred on the device.  
	errorMsg[9]  = "Received data not block data";//1024 10
	errorMsg[10] = "System error";                //4096 11 There is a problem with the operating environment.  
	errorMsg[11] = "Illegal device ID";           //8192 12 Use the ID of the device acquired by the initialization function.  

    //===========================
    ///UpdateRate List
    //===========================
	updateRateList[0] = "100ms";
	updateRateList[1] = "200ms";
	updateRateList[2] = "500ms";
	updateRateList[3] = "1s";
	updateRateList[4] = "2s";
	updateRateList[5] = "5s";
	updateRateList[6].Empty();
	updateRateList[7].Empty();
	updateRateList[8].Empty();
	updateRateList[9].Empty();

    //AutoRange item
    rangeListAuto = "AUTO";
    //===========================
    ///Voltage Range List
    //===========================
    //CrestFactor = 3
	voltageList[0] [0] = "15V";
	voltageList[0] [1] = "30V";
	voltageList[0] [2] = "60V";
	voltageList[0] [3] = "100V";
	voltageList[0] [4] = "150V";
	voltageList[0] [5] = "300V";
	voltageList[0] [6] = "600V";
	voltageList[0] [7] = "1000V";
	voltageList[0] [8].Empty();
	voltageList[0] [9].Empty();
	voltageList[0] [10].Empty();
	voltageList[0] [11].Empty();

    //CrestFactor = 6
	voltageList[1] [0] = "7.5V";
	voltageList[1] [1] = "15V";
	voltageList[1] [2] = "30V";
	voltageList[1] [3] = "50V";
	voltageList[1] [4] = "75V";
	voltageList[1] [5] = "150V";
	voltageList[1] [6] = "300V";
	voltageList[1] [7] = "500V";
	voltageList[1] [8].Empty();
	voltageList[1] [9].Empty();
	voltageList[1] [10].Empty();
	voltageList[1] [11].Empty();
    //===========================
    ///Current Range List
    //===========================

    //CrestFactor = 3
	currentList[0] [0] = "500mA";
	currentList[0] [1] = "1A";
	currentList[0] [2] = "2A";
	currentList[0] [3] = "5A";
	currentList[0] [4] = "10A";
	currentList[0] [5] = "20A";
	currentList[0] [6] = "40A";

	//Selecting the External Current Sensor Range
	currentList[0] [7] = "50mV";
	currentList[0] [8] = "100mV";
	currentList[0] [9] = "200mV";
	currentList[0] [10] = "500mV";
	currentList[0] [11] = "1V";
	currentList[0] [12] = "2V";
	currentList[0] [13] = "5V";
	currentList[0] [14] = "10V";
	currentList[0] [15].Empty();
	currentList[0] [16].Empty();
	currentList[0] [17].Empty();
	currentList[0] [18].Empty();

    //CrestFactor = 6
	currentList[1] [0] = "250mA";
	currentList[1] [1] = "500mA";
	currentList[1] [2] = "1A";
	currentList[1] [3] = "2.5A";
	currentList[1] [4] = "5A";
	currentList[1] [5] = "10A";
	currentList[1] [6] = "20A";

	//Selecting the External Current Sensor Range
	currentList[1] [7] = "25mV";
	currentList[1] [8] = "50mV";
	currentList[1] [9] = "100mV";
	currentList[1] [10] = "250mV";
	currentList[1] [11] = "500mV";
	currentList[1] [13] = "1V";
	currentList[1] [14] = "2.5V";
	currentList[1] [15] = "5V";
	currentList[1] [16].Empty();
	currentList[1] [17].Empty();
	currentList[1] [18].Empty();

    //===========================
    ///Element List
    //===========================
    eList[0] = "1";
    eList[1] = "2";
    eList[2] = "3";
    eList[3] = "SIGMA";
    eList[4].Empty();
    eList[5].Empty();
    eList[6].Empty();

    //===========================
    ///Order List
    //===========================
	oList[0] = "Total";
    oList[1] = "DC";
    oList[2] = "1";
    oList[3] = "2";
    oList[4] = "3";
    oList[5] = "4";
    oList[6] = "5";
    oList[7] = "6";
    oList[8] = "7";
    oList[9] = "8";
    oList[10] = "9";
    oList[11] = "10";
    oList[12] = "11";
    oList[13] = "12";
    oList[14] = "13";
    oList[15] = "14";
    oList[16] = "15";
    oList[17] = "16";
    oList[18] = "17";
    oList[19] = "18";
    oList[20] = "19";
    oList[21] = "20";
    oList[22] = "21";
    oList[23] = "22";
    oList[24] = "23";
    oList[25] = "24";
    oList[26] = "25";
    oList[27] = "26";
    oList[28] = "27";
    oList[29] = "28";
    oList[30] = "29";
    oList[31] = "30";
    oList[32] = "31";
    oList[33] = "32";
    oList[34] = "33";
    oList[35] = "34";
    oList[36] = "35";
    oList[37] = "36";
    oList[38] = "37";
    oList[39] = "38";
    oList[40] = "39";
    oList[41] = "40";
    oList[42] = "41";
    oList[43] = "42";
    oList[44] = "43";
    oList[45] = "44";
    oList[46] = "45";
    oList[47] = "46";
    oList[48] = "47";
    oList[49] = "48";
    oList[50] = "49";
    oList[51] = "50";
    oList[52].Empty();
    oList[53].Empty();
    oList[54].Empty();
    oList[55].Empty();
    oList[56].Empty();
    oList[57].Empty();
    oList[58].Empty();
    oList[59].Empty();
    oList[60].Empty();
    oList[61].Empty();
    oList[62].Empty();
    oList[63].Empty();
    oList[64].Empty();
    oList[65].Empty();
    oList[66].Empty();
    oList[67].Empty();
    oList[68].Empty();
    oList[69].Empty();
    oList[70].Empty();
    oList[71].Empty();
    oList[72].Empty();
    oList[73].Empty();
    oList[74].Empty();
    oList[75].Empty();
    oList[76].Empty();
    oList[77].Empty();
    oList[78].Empty();
    oList[79].Empty();
    oList[80].Empty();
    oList[81].Empty();
    oList[82].Empty();
    oList[83].Empty();
    oList[84].Empty();
    oList[85].Empty();
    oList[86].Empty();
    oList[87].Empty();
    oList[88].Empty();
    oList[89].Empty();
    oList[90].Empty();
    oList[91].Empty();
    oList[92].Empty();
    oList[93].Empty();
    oList[94].Empty();
    oList[95].Empty();
    oList[96].Empty();
    oList[97].Empty();
    oList[98].Empty();
    oList[99].Empty();
    oList[100].Empty();
    oList[101].Empty();
    oList[102].Empty();

	//===========================
    //     init Struct
    //===========================
    List[0].FunctionName = "URMS";
    List[0].ElementFlag  = TRUE;
    List[0].OrderFlag    = FALSE;

    List[1].FunctionName = "UMN";
    List[1].ElementFlag  = TRUE;
    List[1].OrderFlag    = FALSE;

    List[2].FunctionName = "UDC";
    List[2].ElementFlag  = TRUE;
    List[2].OrderFlag    = FALSE;

    List[3].FunctionName = "URMN";
    List[3].ElementFlag  = TRUE;
    List[3].OrderFlag    = FALSE;

    List[4].FunctionName = "UAC";
    List[4].ElementFlag  = TRUE;
    List[4].OrderFlag    = FALSE;

    List[5].FunctionName = "IRMS";
    List[5].ElementFlag  = TRUE;
    List[5].OrderFlag    = FALSE;

    List[6].FunctionName = "IMN";
    List[6].ElementFlag  = TRUE;
    List[6].OrderFlag    = FALSE;

    List[7].FunctionName = "IDC";
    List[7].ElementFlag  = TRUE;
    List[7].OrderFlag    = FALSE;

    List[8].FunctionName = "IRMN";
    List[8].ElementFlag  = TRUE;
    List[8].OrderFlag    = FALSE;

    List[9].FunctionName = "IAC";
    List[9].ElementFlag  = TRUE;
    List[9].OrderFlag    = FALSE;

    List[10].FunctionName = "P";
    List[10].ElementFlag  = TRUE;
    List[10].OrderFlag    = FALSE;

    List[11].FunctionName = "S";
    List[11].ElementFlag  = TRUE;
    List[11].OrderFlag    = FALSE;

    List[12].FunctionName = "Q";
    List[12].ElementFlag  = TRUE;
    List[12].OrderFlag    = FALSE;

    List[13].FunctionName = "LAMBDA"; //"LAMBda"
    List[13].ElementFlag  = TRUE;
    List[13].OrderFlag    = FALSE;

    List[14].FunctionName = "PHI";
    List[14].ElementFlag  = TRUE;
    List[14].OrderFlag    = FALSE;

    List[15].FunctionName = "FU"; //"FreqU[fU]"
    List[15].ElementFlag  = TRUE;
    List[15].OrderFlag    = FALSE;

    List[16].FunctionName = "FI"; //"FreqI[fI]"
    List[16].ElementFlag  = TRUE;
    List[16].OrderFlag    = FALSE;

	List[17].FunctionName = "UPPEAK"; //"U+peak[U+peak]"
    List[17].ElementFlag  = TRUE;
    List[17].OrderFlag    = FALSE;

	List[18].FunctionName = "UMPEAK"; //"U-peak[U-peak]"
    List[18].ElementFlag  = TRUE;
    List[18].OrderFlag    = FALSE;

	List[19].FunctionName = "IPPEAK"; //"i +peak"
    List[19].ElementFlag  = TRUE;
    List[19].OrderFlag    = FALSE;

    List[20].FunctionName = "IMPEAK"; //"i -peak"
    List[20].ElementFlag  = TRUE;
    List[20].OrderFlag    = FALSE;

    List[21].FunctionName = "CFU"; //"CfU"
    List[21].ElementFlag  = TRUE;
    List[21].OrderFlag    = FALSE;

    List[22].FunctionName = "CFI"; //"CfI"
    List[22].ElementFlag  = TRUE;
    List[22].OrderFlag    = FALSE;

    List[23].FunctionName = "TIME"; //"i-Time"
    List[23].ElementFlag  = TRUE;
    List[23].OrderFlag    = FALSE;

    List[24].FunctionName = "WH"; //"Wp"
    List[24].ElementFlag  = TRUE;
    List[24].OrderFlag    = FALSE;

    List[25].FunctionName = "WHP"; //"Wp+"
    List[25].ElementFlag  = TRUE;
    List[25].OrderFlag    = FALSE;

    List[26].FunctionName = "WHM"; //"Wp-"
    List[26].ElementFlag  = TRUE;
    List[26].OrderFlag    = FALSE;

    List[27].FunctionName = "AH"; //"q"
    List[27].ElementFlag  = TRUE;
    List[27].OrderFlag    = FALSE;

    List[28].FunctionName = "AHP"; //"q+"
    List[28].ElementFlag  = TRUE;
    List[28].OrderFlag    = FALSE;

    List[29].FunctionName = "AHM"; //"q-"
    List[29].ElementFlag  = TRUE;
    List[29].OrderFlag    = FALSE;

    List[30].FunctionName = "WS"; //"WS"
    List[30].ElementFlag  = TRUE;
    List[30].OrderFlag    = FALSE;

    List[31].FunctionName = "WQ"; //"WQ"
    List[31].ElementFlag  = TRUE;
    List[31].OrderFlag    = FALSE;

    List[32].FunctionName = "ETA1"; //"ETA1"
    List[32].ElementFlag  = FALSE;
    List[32].OrderFlag    = FALSE;

    List[33].FunctionName = "ETA2"; //"ETA2"
    List[33].ElementFlag  = FALSE;
    List[33].OrderFlag    = FALSE;

    //-------------F1~F8----------------
    List[34].FunctionName = "F1";
    List[34].ElementFlag  = FALSE;
    List[34].OrderFlag    = FALSE;

    List[35].FunctionName = "F2";
    List[35].ElementFlag  = FALSE;
    List[35].OrderFlag    = FALSE;

    List[36].FunctionName = "F3";
    List[36].ElementFlag  = FALSE;
    List[36].OrderFlag    = FALSE;

    List[37].FunctionName = "F4";
    List[37].ElementFlag  = FALSE;
    List[37].OrderFlag    = FALSE;

    List[38].FunctionName = "F5";
    List[38].ElementFlag  = FALSE;
    List[38].OrderFlag    = FALSE;

    List[39].FunctionName = "F6";
    List[39].ElementFlag  = FALSE;
    List[39].OrderFlag    = FALSE;

    List[40].FunctionName = "F7";
    List[40].ElementFlag  = FALSE;
    List[40].OrderFlag    = FALSE;

    List[41].FunctionName = "F8";
    List[41].ElementFlag  = FALSE;
    List[41].OrderFlag    = FALSE;

    //-------------'/G5 optional--------------

    List[42].FunctionName = "UK";
    List[42].ElementFlag  = TRUE;
    List[42].OrderFlag    = TRUE;

    List[43].FunctionName = "IK";
    List[43].ElementFlag  = TRUE;
    List[43].OrderFlag    = TRUE;

    List[44].FunctionName = "PK";
    List[44].ElementFlag  = TRUE;
    List[44].OrderFlag    = TRUE;

    List[45].FunctionName = "SK";
    List[45].ElementFlag  = TRUE;
    List[45].OrderFlag    = TRUE;

    List[46].FunctionName = "QK";
    List[46].ElementFlag  = TRUE;
    List[46].OrderFlag    = TRUE;

    List[47].FunctionName = "LAMBDAK"; //"LAMBda"
    List[47].ElementFlag  = TRUE;
    List[47].OrderFlag    = TRUE;

    List[48].FunctionName = "PHIK";
    List[48].ElementFlag  = TRUE;
    List[48].OrderFlag    = TRUE;

    List[49].FunctionName = "PHIUK";
    List[49].ElementFlag  = TRUE;
    List[49].OrderFlag    = TRUE;

    List[50].FunctionName = "PHIIK";
    List[50].ElementFlag  = TRUE;
    List[50].OrderFlag    = TRUE;

    List[51].FunctionName = "UHDF";
    List[51].ElementFlag  = TRUE;
    List[51].OrderFlag    = TRUE;

    List[52].FunctionName = "IHDF";
    List[52].ElementFlag  = TRUE;
    List[52].OrderFlag    = TRUE;

    List[53].FunctionName = "PHDF";
    List[53].ElementFlag  = TRUE;
    List[53].OrderFlag    = TRUE;

    List[54].FunctionName = "UTHD"; //"Uthd"
    List[54].ElementFlag  = TRUE;
    List[54].OrderFlag    = FALSE;

    List[55].FunctionName = "ITHD"; //"Ithd"
    List[55].ElementFlag  = TRUE;
    List[55].OrderFlag    = FALSE;

    List[56].FunctionName = "PTHD"; //"Pthd"
    List[56].ElementFlag  = TRUE;
    List[56].OrderFlag    = FALSE;

    List[57].FunctionName = "PHI_U1U2"; //"PHIU1-U2"
    List[57].ElementFlag  = FALSE;
    List[57].OrderFlag    = FALSE;

    List[58].FunctionName = "PHI_U1U3"; //"PHIU1-U3"
    List[58].ElementFlag  = FALSE;
    List[58].OrderFlag    = FALSE;

    List[59].FunctionName = "PHI_U1I1"; //"PHIU1-I1"
    List[59].ElementFlag  = FALSE;
    List[59].OrderFlag    = FALSE;

    List[60].FunctionName = "PHI_U1I2"; //"PHIU1-I2"
    List[60].ElementFlag  = FALSE;
    List[60].OrderFlag    = FALSE;

    List[61].FunctionName = "PHI_U1I3"; //"PHIU1-I3
    List[61].ElementFlag  = FALSE;
    List[61].OrderFlag    = FALSE;

    //---------/DT delta computation version-------

    List[62].FunctionName = "DELTA1"; //"DELTAF1"
    List[62].ElementFlag  = FALSE;
    List[62].OrderFlag    = FALSE;

    List[63].FunctionName = "DELTA2"; //"DELTAF2"
    List[63].ElementFlag  = FALSE;
    List[63].OrderFlag    = FALSE;

    List[64].FunctionName = "DELTA3"; //"DELTAF3"
    List[64].ElementFlag  = FALSE;
    List[64].OrderFlag    = FALSE;

    List[65].FunctionName = "DELTA4"; //"DELTAF4"
    List[65].ElementFlag  = FALSE;
    List[65].OrderFlag    = FALSE;
}

// find function position in the List[] arrary
int CMainDialog::FindFunPos(CString fun)
{
	int position;
	position = -1;

	for(int i=0; i<MAX_FUNCTION; i++)
	{
		if (List[i].FunctionName == fun)
		{
			position = i;
			break;	
		}
	}
	return position;
}

//==========================================//
///Dialog Init
//==========================================//
BOOL CMainDialog::OnInitDialog() 
{
    m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON);
	CDialog::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    InitListItem();
    CString Type;
	CString msg_temp;

	//Get Instrument IDN
    msg_temp="*IDN?";
    SetSendMonitor(msg_temp);
    if (!QueriesData(60, msg_temp, &Type))
    {
        return FALSE;
    }
    SetReceiveMonitor(Type);

    CWnd* pWnd;
    CString model = CConnection::m_sName;
    model = CutLeft((char)10, &model);

    //--------#set ModelType display#
    pWnd = GetDlgItem(IDC_MODEL_EDIT);
    pWnd->SetWindowText(model);

    //check model.
    CutLeft(',', &model);
    int symbol = model.Find(",");
    //if (model.Left(symbol) != MODEL)
	if (model.Left(symbol) != "760201" && model.Left(symbol) != "760202" && model.Left(symbol) != "760203" )
    {
        DispError("it seems not WT500, program may run incorrectly!");
    }

    //----------#set unuse element RadioBox disable#

    lastElement = atoi(model.Mid(5, 1));

    int elementIndex;
    for (elementIndex=0; elementIndex<MAX_ELEMENT; elementIndex++)
    {
        pWnd = GetDlgItem(IDC_RADIO_E1 + elementIndex);
        if (elementIndex > lastElement - 1)
        {
            pWnd->EnableWindow(FALSE);
        }
    }

    //----------------------#Queries the CrestFactor#
    CString value;
    CString msg = ":INPUT:CFACTOR?";
	SetSendMonitor(msg);
    if (!QueriesData(20, msg, &value))
    {
        return FALSE;
    }
	SetReceiveMonitor(value);
    crestFactor = CutLeft((char)10, &value);//cut left with LF.

	GetUpdateRate();
    //get current item settings from instrument.

	GetOption();
	ReadItemSettings();

	//Set the first RadioBox to be checked.
    if (lastElement >0)
    {
        CheckDlgButton(IDC_RADIO_E1, BST_CHECKED);
    }
    OnRadioE1();

    pWnd = GetDlgItem(IDC_EDIT_FILE);
    pWnd->SetWindowText("MeasuredData.csv");
    pWnd->EnableWindow(FALSE);
    pWnd = GetDlgItem(IDC_EDIT_TMR);
    pWnd->SetWindowText("500");
    return TRUE;
}

//==========================================//
///Dialog Close
//==========================================//
void CMainDialog::OnClose()
{
	//close connection when exit window by system menu.
	m_connection.Finish();
	CDialog::OnClose();
}
void CMainDialog::OnDlgClose()
{
	//close connection when exit window by system menu.
	m_connection.Finish();
    CMainDialog::OnOK();
}

//==========================================//
///to get and display the given item in the list
//==========================================//
void CMainDialog::ComboListSel(CComboBox* pCombo, CString content)
{
    int n;
    CString listText;
    for (n=0; n<pCombo->GetCount(); n++)
    {
        pCombo->GetLBText(n, listText);
        if (listText == content)
        {
            pCombo->SetCurSel(n);
            break;
        }
    }
}

//==========================================//
/// <summary> Function: CutLeft </summary>
///cut the left half to outData,
///and the right portion remain in inData.
//example:
///symbol:"2", in:"12345" => out:"1", in:"345"
//==========================================//
CString CMainDialog::CutLeft(char symbol, CString* inData)
{
    CString outData = *inData;
    int pos = (*inData).Find(symbol);
    if (pos == -1)
    {
        //if no symbol, cut with LF.
        pos = (*inData).Find(10);
    }
    if (pos != -1)
    {
        outData = (*inData).Left(pos);
        *inData = (*inData).Mid(pos + 1);
    }
    //cut data when harmonics mode
    pos = outData.Find(" ");
    if (pos != -1)
    {
        outData = outData.Mid(pos + 1);
    }
    return outData;
}

//==========================================//
///<summary>Function: QueriesData</summary>
//==========================================//
bool CMainDialog::QueriesData(int maxLength, CString sMsg, CString* data)
{
    int rtn;
	//Send Command.
    rtn = m_connection.Send(sMsg);
    if (rtn != 0)
    {
	    DispError(m_connection.GetLastError());
	    return FALSE;
    }

	//Queries Data.
    int realLength;
    char* buf;
    if ((buf = new char[maxLength]) == NULL)
    {
        return FALSE;
    }
    memset(buf,0,maxLength);
    rtn = m_connection.Receive(buf, maxLength, &realLength);
    (*data).Format(_T("%s"), buf);
    delete[] buf;
    if (rtn != 0)
    {
	    DispError(m_connection.GetLastError());
	    return FALSE;
    }
	return TRUE;
}

//==========================================//
///<summary>Function: SetSendMonitor</summary>
//==========================================//
void CMainDialog::SetSendMonitor(CString msg)
{
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SEND);
    CString sendMsg;
    pEdit->GetWindowText(sendMsg);
    //add return symbol into msg.
    if (sendMsg != "")
    {
        sendMsg += "\r\n";
    }
    sendMsg += msg;

    //get the count of rows(return symbol),
    //cut off the old row when rows count > 100.
    int lineCount = 0;
    int lF        = 0;
    while (sendMsg.Find(0x0A, lF) != -1)
    {
        lF = sendMsg.Find(0x0A, lF) + 1;
        lineCount ++;
    }
    if (lineCount >= MAX_LINES)
    {
        CutLeft((char)10, &sendMsg);
    }
    //set to display.
    pEdit->SetWindowText(sendMsg);
    pEdit->LineScroll(lineCount);
}
//==========================================//
/// Function: SetReceiveMonitor
//==========================================//
void CMainDialog::SetReceiveMonitor(CString data)
{
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RCV);
    CString rcvData;
    pEdit->GetWindowText(rcvData);
    //cut off the terminator into data.
    data.Replace(10, 0);
    data.Format(_T("%s"), (LPSTR)(LPCTSTR)data);
    //add return in data.
    if (rcvData != "")
    {
        rcvData += "\r\n";
    }
    rcvData += data;

    //get the count of return symbol,
    //cut off the old row when rows count > 100.
    int lineCount = 0;
    int lF = 0;
    while (rcvData.Find(0x0A, lF) != -1)
    {
        lF = rcvData.Find(0x0A, lF) + 1;
        lineCount ++;
    }
    if (lineCount >= MAX_LINES)
    {
        CutLeft((char)10, &rcvData);
    }
    //set to display.
    pEdit->SetWindowText(rcvData);
    pEdit->LineScroll(lineCount);
}
//==========================================//
/// <summary> Function: DispError </summary>
///the errorID is received from tmctl.
///get the errorMsg corresponding the errorID.
//==========================================//
void CMainDialog::DispError(int errorID)
{
    CWnd* pWnd = GetDlgItem(IDC_EDIT_ERR);
    if (errorID == 0)
    {
        pWnd->SetWindowText("getting detail error failed.");
        return;
    }
    int n = 0;
    while (2 << n != errorID)
    {
        n ++;
    }
    //set errorMsg to display.
    pWnd->SetWindowText(errorMsg[n]);
}
void CMainDialog::DispError(CString errorInfo)
{
    //set errorMsg to display.
    CWnd* pWnd = GetDlgItem(IDC_EDIT_ERR);
    pWnd->SetWindowText(errorInfo);    
}

//==========================================//
/// <summary> Function: GetData </summary>
//==========================================//
void CMainDialog::GetItemData()
{
    int n;
    int rtn;
    CString msg;

    UpdateData();
    //----------------------#print title(Function) when getting data once#
    CWnd* pWnd = GetDlgItem(IDC_BUTTON_GETDATA_S);
    if (IsDlgButtonChecked(IDC_CHECK_SAVE) 
        && !m_sFileName.IsEmpty()//when file name is null, do not save.
        && pWnd->IsWindowEnabled())
    {
        //print titles(function) when saveBox is checked.
        ofstream csvFile(m_sFileName, ios::app);
        if (csvFile.fail())
        {
            MessageBox("file can not open!", "Error", MB_ICONERROR | MB_OK);
            return;
        }

        //out put current time.
        SYSTEMTIME CurTime;
        GetLocalTime(&CurTime);
        csvFile << "Date: " << CurTime.wYear << "/" << CurTime.wMonth;
        csvFile << "/" << CurTime.wDay << ",";

        int fLoop = IDC_COMBO_F1;
        CString function;
        CComboBox* pCombo;
        for (n = 0; n < atoi(m_sComboItems); n ++)
        {
	        pCombo = (CComboBox*)GetDlgItem(fLoop ++);
            pCombo->GetWindowText(function);
            csvFile << function << ",";
        }
        csvFile << "\n";
        csvFile.close();
    }

    msg = ":NUMERIC:NORMAL:VALUE?";

    //----------------------#send message#
    SetSendMonitor(msg);

    //###ASCII:TmcSend(); FLOAT:TmcSendBuLength()###
    if (IsDlgButtonChecked(IDC_RADIO_ASC))
    {
        rtn = m_connection.Send(msg);
    }
    else
    {
        rtn = m_connection.SendByLength(msg, msg.GetLength());
    }
    if (rtn != 0)
    {
	    DispError(m_connection.GetLastError());
	    return;
    }

    //----------------------#receive values#
    char* buf;
    int maxLength;
    int realLength;
    CString value;
    CEdit* pEdit;
    int dLoop = IDC_EDIT_D1;
    CString data;

    if (IsDlgButtonChecked(IDC_RADIO_ASC))
    {
        //----------------------#receive values by ASCII#
        //###ASCII:TmcReceive()###
        UpdateData(TRUE);
        maxLength = 15 * atoi(m_sComboItems);
        if ((buf = new char[maxLength]) == NULL)
        {
            return;
        }
        memset(buf, 0, maxLength);
        rtn = m_connection.Receive(buf, maxLength, &realLength);
        value.Format(_T("%s"), buf);
        delete[] buf;
        if (rtn != 0)
        {
	        DispError(m_connection.GetLastError());
	        return;
        }
        SetReceiveMonitor(value);
    }
    else
    {
        //----------------------#receive values by Float#
        //###FLOAT:TmcReceiveBlock()###
        rtn = m_connection.ReceiveBlockHeader(&maxLength);
        if (maxLength < 1)
        {
            return;
        }
        maxLength ++;//see tmctl's help
        if ((buf = new char[maxLength]) == NULL)
        {
            return;
        }

        int isEnd = 0;
        char bytes[sizeof(float)];
        float floatBuf;
		CString outputValue;
        while (isEnd == 0)
        {
            memset(buf, 0, maxLength);
            rtn = m_connection.ReceiveBlockData(buf, maxLength, &realLength, &isEnd);
            if (rtn != 0)
            {
                delete[] buf;
	            DispError(m_connection.GetLastError());
	            return;
            }
			for (n=0; n<realLength-1; n++)
			{
                data.Format(_T("%02X"), (unsigned char)buf[n]);
				outputValue = outputValue + data;
			}
            //converse the char format and copy as float.
            for (n=0; n<realLength/4; n++)
            {
                bytes[3] = buf[n * 4];
                bytes[2] = buf[n * 4 + 1];
                bytes[1] = buf[n * 4 + 2];
                bytes[0] = buf[n * 4 + 3];
                memcpy(&floatBuf, bytes, sizeof(float));
                data.Format(_T("%e"), floatBuf);
                value += data + ',';
            }
        }
        delete[] buf;
		SetReceiveMonitor(outputValue);
    }

    //----------------------#display data and output when saveBox checked#
    if (IsDlgButtonChecked(IDC_CHECK_SAVE) && !m_sFileName.IsEmpty())
    {
        ofstream csvFile(m_sFileName, ios::app);
        if (csvFile.fail())
        {
            MessageBox("file can not open!", "Error", MB_ICONERROR | MB_OK);
            return;
        }

        //out put current time.
        SYSTEMTIME CurTime;
        GetLocalTime(&CurTime);
        csvFile << "Time: " << CurTime.wHour << ":" <<CurTime.wMinute << ":";

        //csvFile << second.Format("%5.3lf",msec);
		CString msec;
		msec.Format("%d.%03d", CurTime.wSecond, CurTime.wMilliseconds);
        csvFile << (LPCSTR)msec << ",";

        for (n=0; n<atoi(m_sComboItems); n++)
        {
            //set display
            data = CutLeft(',', &value);
	        pEdit = (CEdit*)GetDlgItem(dLoop++);
            pEdit->SetWindowText(data);
            //output data
            csvFile << data << ",";
        }
        csvFile << "\n";
        csvFile.close();
    }
    else
    {
        for (n=0; n<atoi(m_sComboItems); n++)
        {
            //set display
            data = CutLeft(',', &value);
	        pEdit = (CEdit*)GetDlgItem(dLoop++);
            pEdit->SetWindowText(data);
        }
    }
}

//==========================================//
/// <summary> Function: GetRanges </summary>
//==========================================//
bool CMainDialog::GetRanges(int eNo)
{
    int n;
    CString msg;
    CString terminal;

    m_comboU.ResetContent();
    m_comboI.ResetContent();
    //UpdateData(FALSE); //used for DDX refresh

    //----------------------#Set the Range Lists#
    //when crest factor == 3, set ranges.
    if (crestFactor == "3")
    {
        ///#set voltage list#
		n = 0;
        while (!voltageList[0][n].IsEmpty())
        {
            m_comboU.AddString(voltageList[0][n]);
            n++;
        }
        m_comboU.AddString(rangeListAuto);
        ///#set current list#

		n=0;
		while (!currentList[0][n].IsEmpty())
        {
            m_comboI.AddString(currentList[0][n]);
            n++;
        }
        m_comboI.AddString(rangeListAuto);
    }
    //when crest factor == 6, set ranges.
    else if (crestFactor == "6")
    {
        ///#set voltage list#
        n = 0;
        while (!voltageList[1][n].IsEmpty())
        {
            m_comboU.AddString(voltageList[1][n]);
            n++;
        }
        m_comboU.AddString(rangeListAuto);
        ///#set current list#
		n=0;
		while (!currentList[1][n].IsEmpty())
        {
            m_comboI.AddString(currentList[1][n]);
            n++;
        }
        m_comboI.AddString(rangeListAuto);
    }

    //--------------------#Get Voltage Range Settings#
    //###":VOLT:RANG:ELEM1 3.00E+00;ELEM2 1.00E+03"###
    CString range;
    double doubleRange;

    msg.Format(_T("%d"),eNo);
    msg = ":INPUT:VOLTAGE:RANGE:ELEMENT" + msg + "?";
    SetSendMonitor(msg);
    if (!QueriesData(60, msg, &range))
    {
        return FALSE;
    }
    SetReceiveMonitor(range);
    range = CutLeft((char)10, &range);//cut left with LF.
    /*************************************************************/
    CString auto_tem;
    //double doubleRange;
    auto_tem="";
    msg.Format(_T("%d"),eNo);
    msg = ":INPUT:VOLTAGE:AUTO:ELEMENT" + msg + "?";
    SetSendMonitor(msg);
    if (!QueriesData(60, msg, &auto_tem))
    {
        return FALSE;
    }
    SetReceiveMonitor(auto_tem);
    auto_tem = CutLeft((char)10, &auto_tem);//cut left with LF.
	/*************************************************************/

    if (auto_tem == "1")
    {
        m_comboU.SetWindowText(rangeListAuto);
    }
    else
    {
        doubleRange = atof(range);
        range.Format(_T("%f"), doubleRange);
        range.TrimRight("0");
        range.TrimRight(".");
        m_comboU.SetWindowText(range + "V");
    }

    //----------------------#Get Current Range Settings#
    //###":CURR:RANG:ELEM5 3.00E+00;ELEM6 1.00E-03"###
    msg.Format(_T("%d"),eNo);
    msg = ":INPUT:CURRENT:RANGE:ELEMENT" + msg + "?";
    SetSendMonitor(msg);
    if (!QueriesData(60, msg, &range))
    {
        return FALSE;
    }
    SetReceiveMonitor(range);
    range = CutLeft((char)10, &range);//cut left with LF.

	//wether have external current sensor input
	CString external;
	external.Empty();
	external=CutLeft(',',&range);  //cut left with ",".

    /* * * * ****************************************************/
    CString tem_auto;
    //double doubleRange;
    tem_auto="";
    msg.Format(_T("%d"),eNo);
    msg = ":INPUT:CURRENT:AUTO:ELEMENT" + msg + "?";
    SetSendMonitor(msg);
    if (!QueriesData(60, msg, &tem_auto))
    {
        return FALSE;
    }
    SetReceiveMonitor(tem_auto);
    tem_auto = CutLeft((char)10, &tem_auto);//cut left with LF.
	/* * * * ****************************************************/

    if (tem_auto == "1")
	{
        m_comboI.SetWindowText(rangeListAuto);
    }
	else
	{
        doubleRange = atof(range);
        if (doubleRange < 1)
        {
		    range.Format(_T("%f"), doubleRange * 1000);
		    range.TrimRight("0");
            range.TrimRight(".");
		    if(external=="EXT"||external=="EXTERNAL")
			{
			     m_comboI.SetWindowText(range + "mV");
			}
		     else
			 {
			     m_comboI.SetWindowText(range + "mA");
			 }
        }
        else
        { 
			range.Format(_T("%f"), doubleRange);
            range.TrimRight("0");
            range.TrimRight(".");

			if(external=="EXT" || external=="EXTERNAL")
			{
				m_comboI.SetWindowText(range + "V");
			}
			else
			{
				m_comboI.SetWindowText(range + "A");
			}
        }
    }
    return TRUE;
}

//==========================================//
///E1~E3 Radio Check
//==========================================//
void CMainDialog::OnRadioE1()
{
	GetRanges(1);
	Currentelement=1;
}

void CMainDialog::OnRadioE2()
{
	GetRanges(2);
	Currentelement=2;
}

void CMainDialog::OnRadioE3()
{
	GetRanges(3);
	Currentelement=3;
}

//==========================================//
///Set Range
//==========================================//
void CMainDialog::OnButtonSetRange() 
{
	int rtn_tmp;
	rtn_tmp=-1;
	//----------------------#Get the Selected Element#
    UpdateData(TRUE);
    CString sENo;           //format element Number to CString.
    sENo.Format(_T("%d"),m_iRadioElement + 1);
	//----------------------#Send Voltage Range#
	CString msg;
	m_comboU.GetWindowText(msg);
	if(msg != "AUTO")
	{
		msg = ":INPUT:VOLTAGE:RANGE:ELEMENT" + sENo + " " + msg;
	}
    else 
	{
		msg = ":INPUT:VOLT:AUTO:ELEMENT" + sENo + " " + "ON";
	}
	SetSendMonitor(msg);
    int rtn = m_connection.Send(msg);
    if(rtn != 0)
    {
	    DispError(m_connection.GetLastError());
	    //when setting failed, resume the original value.
	    GetRanges(m_iRadioElement + 1);
    }

	//----------------------#Send Current Range#
	CString Current_Sen;
	m_comboI.GetWindowText(msg);
    Current_Sen=msg;
    rtn_tmp=Current_Sen.Find('V');

    if(msg != "AUTO")
	{
		if(rtn_tmp > 0)
		{
			msg = ":INPUT:CURRENT:RANGE:ELEMENT" + sENo + " " + "EXTERNAL,"+" "+ msg;
		}
		else
		{
			msg = ":INPUT:CURRENT:RANGE:ELEMENT" + sENo + " " + msg;
		}
	}
	else
	{
       msg = ":INPUT:CURRENT:AUTO:ELEMENT"+ sENo + " " + "ON";
	}
	SetSendMonitor(msg);
    rtn = m_connection.Send(msg);
    if(rtn != 0)
    {
	    DispError(m_connection.GetLastError());
	    //when setting failed, resume the original value.
	    GetRanges(m_iRadioElement + 1);
    }
    GetRanges(Currentelement);
}

//===========================================//
/// <summary>
/// Function: Get Update Rate
/// </summary>
//===========================================//
void CMainDialog::GetUpdateRate() 
{
    ///#get harmonics state#
    CString msg;
	CString value;
	value == "";
    ///#set updateRate combo list#
    m_comboR.ResetContent();
    CWnd* pWnd;

    pWnd = GetDlgItem(IDC_BUTTON_SETRATE);
    pWnd->EnableWindow(TRUE);
    pWnd = GetDlgItem(IDC_COMBOR);
    pWnd->EnableWindow(TRUE);
	int n = 0;
    while(!updateRateList[n].IsEmpty())
    {
        m_comboR.AddString(updateRateList[n]);
        n++;
    }

    msg = ":RATE?";
    SetSendMonitor(msg);
    if (!QueriesData(30, msg, &value))
    {
        return;
    }
    SetReceiveMonitor(value);
    value = CutLeft((char)10, &value); //cut left with LF.

    double doubleRate = atof(value);
    if(doubleRate < 1)
    {
        //when "ms" unit, multiply 1k.
        value.Format(_T("%.0f"), doubleRate * 1000);
        m_comboR.SetWindowText(value + "ms");
    }
    else
    {
        value.Format(_T("%.0f"), doubleRate);
        m_comboR.SetWindowText(value + "s");
    }
}

//==========================================//
///Set UpdateRate
//==========================================//
void CMainDialog::OnButtonSetRate() 
{
    CString value;
    CString msg;

	//Send Command.
    m_comboR.GetWindowText(value);
    msg = ":RATE " + value;
	SetSendMonitor(msg);
	int rtn = m_connection.Send(msg);
    if(rtn != 0)
    {
	    DispError(m_connection.GetLastError());
        //when setting failed, resume the original value.
        GetUpdateRate();
    }
	GetUpdateRate();
}

//==========================================//
///Test Command Send
//==========================================//
void CMainDialog::OnButtonSendCmd() 
{
    UpdateData(TRUE);
    if(m_sEditTest == "")
    {
        return;
    }
	SetSendMonitor(m_sEditTest);
	if(m_sEditTest.Find("?") == -1)
    {
        int rtn;
	    //Send Command.
	    rtn = m_connection.Send(m_sEditTest);
        if(rtn != 0)
        {
	        DispError(m_connection.GetLastError());
        }
    }
    else
    {
        CString data;
        if (!QueriesData(1000, m_sEditTest, &data))
        {
            return;
        }
	    SetReceiveMonitor(data);
    }
}

//==========================================//
///Getting Error Information
//==========================================//
void CMainDialog::OnButtonGetError() 
{
    //----------------------#queries error from instrument#
    CString msg;
    CString errInfo;
    msg = ":STATUS:ERROR?";
    SetSendMonitor(msg);
    if(!QueriesData(150, msg, &errInfo))
    {
        return;
    }
    //cut off the return.
    errInfo = errInfo.Left(errInfo.Find((char)10));
    SetReceiveMonitor(errInfo);
    DispError(errInfo);
}

//==========================================//
///Set Header On/Off
//==========================================//
void CMainDialog::OnButtonHeader() 
{
    CString header;
    CString msg;
    //----------------------#queries header status#
    msg = ":COMMUNICATE:HEADER?";
    SetSendMonitor(msg);
    if(!QueriesData(25, msg, &header))
    {
        return;
    }
    SetReceiveMonitor(header);
    header = CutLeft((char)10, &header);//cut left with LF.

    //----------------------#set header status#
    if(header == "1")
    {
        msg = ":COMMUNICATE:HEADER OFF";
    }
    else
    {
        msg = ":COMMUNICATE:HEADER ON";
    }
    SetSendMonitor(msg);
    int rtn = m_connection.Send(msg);
    if(rtn != 0)
    {
	    DispError(m_connection.GetLastError());
	    return;
    }
}

//==========================================//
///Set Verbose On/Off
//==========================================//
void CMainDialog::OnButtonVerbose() 
{
    CString verbose;
    CString msg;
    //----------------------#queries verbose status#
    msg = ":COMMUNICATE:VERBOSE?";
    SetSendMonitor(msg);
    if(!QueriesData(25, msg, &verbose))
    {
        return;
    }
    SetReceiveMonitor(verbose);
    verbose = CutLeft((char)10, &verbose);//cut left with LF.

    //----------------------#set verbose status#
    if(verbose == "1")
    {
        msg = ":COMMUNICATE:VERBOSE OFF";
    }
    else
    {
        msg = ":COMMUNICATE:VERBOSE ON";
    }
    SetSendMonitor(msg);
    int rtn = m_connection.Send(msg);
    if(rtn != 0)
    {
	    DispError(m_connection.GetLastError());
	    return;
    }
}

//==========================================//
///System Reset
//==========================================//
void CMainDialog::OnButtonRst() 
{
    int reset = MessageBox("System will be All Reset, continue?", "Confirm", MB_OKCANCEL|MB_ICONWARNING);
    if(reset == IDCANCEL)
    {
        return;
    }
    else
    {
        CString msg = "*RST";
	    SetSendMonitor(msg);
	    int rtn = m_connection.Send(msg);
        if(rtn != 0)
        {
	        DispError(m_connection.GetLastError());
	        return;
        }
        UpdateData();
		GetUpdateRate();
        GetRanges(m_iRadioElement + 1);
		ReadItemSettings();
    }
}

//=============================================//
///Function: ReadItemSettings
//=============================================//
void CMainDialog::ReadItemSettings() 
{
	CString msg;
    CString buf;
    CComboBox* pCombo;
	CString fun_msg;
	int pos;
	pos=-1;

    //----------------------#get ASCII/BINARY#
    msg = ":NUMERIC:FORMAT?";
    SetSendMonitor(msg);
    if(!QueriesData(30, msg, &buf))
    {
        return;
    }
    SetReceiveMonitor(buf);
    buf = CutLeft((char)10, &buf);  //cut left with LF.
    //----------------------#set ASCII/BINARY option#
    if(buf.Find("ASC") != -1)
    {
        CheckDlgButton(IDC_RADIO_ASC, BST_CHECKED);
        CheckDlgButton(IDC_RADIO_BIN, BST_UNCHECKED);
    }
    else
    {
        CheckDlgButton(IDC_RADIO_ASC, BST_UNCHECKED);
        CheckDlgButton(IDC_RADIO_BIN, BST_CHECKED);
    }
    //----------------------#get item count#

    msg = ":NUMERIC:NORMAL:NUMBER?";
    SetSendMonitor(msg);
    if(!QueriesData(30, msg, &buf))
    {
        return;
    }
    SetReceiveMonitor(buf);
    buf = CutLeft((char)10, &buf);   //cut left with LF.

    //----------------------#set item count combo#
    pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ITEMS);
    if(atoi(buf) > MAX_ITEM)
    {
        CString maxItem;
        maxItem.Format(_T("%d"), MAX_ITEM);
        ComboListSel(pCombo, maxItem);
    }
    else
    {
        ComboListSel(pCombo, buf);
    }
    OnSelchangeComboItems();

    msg = ":NUMERIC:NORMAL?";
 
    SetSendMonitor(msg);
    if(!QueriesData(30 + 30*atoi(buf), msg, &buf))
    {
        return;
    }
    SetReceiveMonitor(buf); 
    buf = CutLeft((char)10, &buf);      //cut left with LF.
    //cut off the item number(item count) portion.
    CutLeft(';', &buf);

    //----------------------#set item settings to be display#
    UpdateData(TRUE);
    CString itemBuf;
    CString element;
    int fLoop = IDC_COMBO_F1;
    int eLoop = IDC_COMBO_E1;
    int oLoop = IDC_COMBO_O1;
    int n;

    for(n=0; n<atoi(m_sComboItems); n++)
    {
		itemBuf = CutLeft(';', &buf);
		fun_msg = CutLeft(',', &itemBuf);
		/************ Replace *************/
		if(fun_msg == "LAMB")
		   {fun_msg = "LAMBDA";}
		else if(fun_msg == "UPP")
		   {fun_msg = "UPPEAK";}
		else if(fun_msg == "UMP")
		   {fun_msg = "UMPEAK";}
		else if(fun_msg == "IPP")
		   {fun_msg = "IPPEAK";}
		else if(fun_msg == "IMP")
		   {fun_msg = "IMPEAK";}
		/***********************************/
		pCombo = (CComboBox*)GetDlgItem(fLoop++);
        pCombo->SetWindowText(fun_msg);
		oldFuncName[n] = fun_msg;

		//#set element.#  
		pCombo = (CComboBox*)GetDlgItem(eLoop++);
		pos = -1;
		pos = FindFunPos(fun_msg);
		if(List[pos].ElementFlag)
		{
            pCombo->EnableWindow(TRUE);
			element = CutLeft(',', &itemBuf);
			if (element == "SIGM")
			{
				element = "SIGMA";
			}
			pCombo->SetWindowText(element);
		}
		else
		{  
			pCombo->SetWindowText("");
			pCombo->EnableWindow(FALSE);
		}

		//set orders
        pos = -1;
	    pCombo = (CComboBox*)GetDlgItem(oLoop ++);
		if(HarmonicOption >= 0)
		{
			pos=FindFunPos(fun_msg);
			if(List[pos].OrderFlag)
			{
				CString temp_order;
				temp_order=CutLeft(',',&itemBuf);
				if (temp_order=="TOTAL"||temp_order=="TOT")
				{temp_order="Total";}
				pCombo->EnableWindow(TRUE);
				pCombo->SetWindowText(temp_order);
			}
			else 
			{   
				pCombo->SetWindowText("");
				pCombo->EnableWindow(FALSE);
			}
		}
		else
		{
			pCombo->SetWindowText("");
		    pCombo->EnableWindow(FALSE);
		}
	}//end for
}

//==========================================//
//Function: SendItemSettings
//==========================================//
void CMainDialog::SendItemSettings() 
{
    CString msg;
    CString state;
    int rtn;

    //----------------------#set ASCII/Float(Binary)#
    if(IsDlgButtonChecked(IDC_RADIO_ASC))
    {
        msg = ":NUMERIC:FORMAT ASCII";
    }
    else
    {
        msg = ":NUMERIC:FORMAT FLOAT";
    }
    SetSendMonitor(msg);
    rtn = m_connection.Send(msg);
    if(rtn != 0)
    {
	    DispError(m_connection.GetLastError());
	    return;
    }

    //----------------------#set items number#
    msg = ":NUMERIC:NORMAL:NUMBER " + m_sComboItems;

    SetSendMonitor(msg);
    rtn = m_connection.Send(msg);
    if(rtn != 0)
    {
	    DispError(m_connection.GetLastError());
	    return;
    }

    //----------------------#send message detail#
    int n;
    CString itemIndex;
    CString param;
    CComboBox* pCombo;
    int fLoop = IDC_COMBO_F1;
    int eLoop = IDC_COMBO_E1;
    int oLoop = IDC_COMBO_O1;

    msg = ":NUMERIC:NORMAL:";
    for(n=0; n<atoi(m_sComboItems); n++)
    {
        itemIndex.Format(_T("%d"), n + 1);

        //set function parameter into message.
        pCombo = (CComboBox*)GetDlgItem(fLoop++);
        pCombo->GetWindowText(param);
        msg = msg + "ITEM" + itemIndex + " " + param;

        //set element parameter into message.
        pCombo = (CComboBox*)GetDlgItem(eLoop++);
        pCombo->GetWindowText(param);
		if(pCombo->IsWindowEnabled() == TRUE)
		{
        msg = msg + "," + param;
		}

        //set order parameter into message, if have.
        pCombo = (CComboBox*)GetDlgItem(oLoop++);
        pCombo->GetWindowText(param);
		if(pCombo->IsWindowEnabled() == TRUE)
        {
            msg = msg + "," + param;
        }

        //set separator into message.
        if (n != atoi(m_sComboItems) - 1)
        {
            msg = msg + ";";
        }
    }
    msg.Replace("SGM", "SIGM");//sending as "SIGM" when display as "SGM".
 
	SetSendMonitor(msg);
    rtn = m_connection.Send(msg);
    if(rtn != 0)
    {
	    DispError(m_connection.GetLastError());
	    return;
    }
}

//=============================================//
///Read Item Settings
//=============================================//
void CMainDialog::OnButtonGetItems() 
{
	ReadItemSettings();
}

//==========================================//
///Send Item Settings
//==========================================//
void CMainDialog::OnButtonSetItems() 
{
    SendItemSettings();
}

//==========================================//
//Function: EnableItems
//==========================================//
void CMainDialog::EnableItems(bool isEnable)
{
    GetDlgItem(IDC_BUTTON_SETRATE)->EnableWindow(isEnable);
    GetDlgItem(IDC_COMBOR)->EnableWindow(isEnable);
    GetDlgItem(IDC_BUTTON_SETRANGE)->EnableWindow(isEnable);
    GetDlgItem(IDC_BUTTON_SENDCMD)->EnableWindow(isEnable);
    GetDlgItem(IDC_BUTTON_GETERR)->EnableWindow(isEnable);
    GetDlgItem(IDC_BUTTON_RST)->EnableWindow(isEnable);
    GetDlgItem(IDC_BUTTON_HEADER)->EnableWindow(isEnable);
    GetDlgItem(IDC_BUTTON_VERBOSE)->EnableWindow(isEnable);

    GetDlgItem(IDC_BUTTON_GETITEMS)->EnableWindow(isEnable);
    GetDlgItem(IDC_BUTTON_SETITEMS)->EnableWindow(isEnable);
    GetDlgItem(IDC_COMBO_ITEMS)->EnableWindow(isEnable);
    GetDlgItem(IDC_RADIO_ASC)->EnableWindow(isEnable);
    GetDlgItem(IDC_RADIO_BIN)->EnableWindow(isEnable);

    GetDlgItem(IDC_BUTTON_GETDATA_S)->EnableWindow(isEnable);
    GetDlgItem(IDC_BUTTON_GETDATA_R)->EnableWindow(isEnable);
    GetDlgItem(IDC_BUTTON_GETDATA_T)->EnableWindow(isEnable);
    GetDlgItem(IDC_EDIT_TMR)->EnableWindow(isEnable);
    GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(isEnable);
    GetDlgItem(IDC_EDIT_FILE)->EnableWindow(isEnable);
}

//==========================================//
//Timer Event
//==========================================//
void CMainDialog::OnTimer(UINT nIDEvent)
{
    if(nIDEvent == 1)   //get data by update rate.
    {
        CString eesr;
        CString msg = ":STATUS:EESR?";
        SetSendMonitor(msg);
        if(!QueriesData(20, msg, &eesr))
        {
            return;
        }
        SetReceiveMonitor(eesr);
        eesr = CutLeft((char)10, &eesr);  //cut left with LF.

        if((atoi(eesr) & 0X01) == 1)
        {
            GetItemData();
        }
    }
    else if(nIDEvent == 2)  //get data by timer.
    {
        GetItemData();
        return;
    }
}

//==========================================//
///GetData Single
//==========================================//
void CMainDialog::OnButtonGetDataS() 
{
    SendItemSettings();
    //GetItemData();
}

//==========================================//
///GetData by UpdateRate
//==========================================//
void CMainDialog::OnButtonGetDataR() 
{
    CString caption;
    CWnd* pWnd;
    pWnd = GetDlgItem(IDC_BUTTON_GETDATA_R);
    pWnd->GetWindowText(caption);
    //----------------------#resume all#
    if(caption == "STOP")
    {
        KillTimer(TIMER_1);
        pWnd->SetWindowText("    Get Data      (Update Rate)");
        EnableItems(TRUE);
        //resume fileNameInputText.
        if(!IsDlgButtonChecked(IDC_CHECK_SAVE))
        {
            pWnd = (CButton*)GetDlgItem(IDC_EDIT_FILE);
            pWnd->EnableWindow(FALSE);
        }
    }
    //----------------------#getting datas#
    else
    {
        UpdateData(TRUE);
        if(IsDlgButtonChecked(IDC_CHECK_SAVE) && !m_sFileName.IsEmpty())
        {
            //print titles(function) once when saveBox is checked
            ofstream csvFile(m_sFileName, ios::app);
            if(csvFile.fail())
            {
                MessageBox("file can not open!", "Error", MB_ICONERROR | MB_OK);
                return;
            }

            //out put current time.
            SYSTEMTIME CurTime;
            GetLocalTime(&CurTime);
            csvFile << "Date: " << CurTime.wYear << "/" << CurTime.wMonth;
            csvFile << "/" << CurTime.wDay << ",";

            int fLoop = IDC_COMBO_F1;
            int n;
            CString function;
            CComboBox* pCombo;
            for(n=0; n<atoi(m_sComboItems); n++)
            {
	            pCombo = (CComboBox*)GetDlgItem(fLoop++);
                pCombo->GetWindowText(function);
                csvFile << function << ",";
            }
            csvFile << "\n";
            csvFile.close();
        }
        SendItemSettings();

        //reset filter1.
        CString msg = ":STATUS:FILTER1 FALL";
        SetSendMonitor(msg);
	    int rtn = m_connection.Send(msg);
        if(rtn != 0)
        {
	        DispError(m_connection.GetLastError());
	        return;
        }
        //*******************************************
		CString eesr;
        msg = ":STATUS:EESR?";
        SetSendMonitor(msg);
        if(!QueriesData(20, msg, &eesr))
        {
            return;
        }
        SetReceiveMonitor(eesr);
		//*******************************************
        //reset other controls' display.
        EnableItems(FALSE);
        pWnd->EnableWindow(TRUE);
        pWnd->SetWindowText("STOP");
        
        //set timer interval and start getting data.
        CWnd::SetTimer(TIMER_1,20,NULL);
    }
}

//==========================================//
///GetData by Timer
//==========================================//
void CMainDialog::OnButtonGetDataT() 
{
    CString text;
    CWnd* pWnd;
    pWnd = GetDlgItem(IDC_BUTTON_GETDATA_T);
    pWnd->GetWindowText(text);
    //----------------------#resume all#
    if(text == "STOP")
    {
        KillTimer(TIMER_2);
        pWnd->SetWindowText("Get Data (Timer)");
        EnableItems(TRUE);
        //resume fileNameInputText.
        if(!IsDlgButtonChecked(IDC_CHECK_SAVE))
        {
            pWnd = (CButton*)GetDlgItem(IDC_EDIT_FILE);
            pWnd->EnableWindow(FALSE);
        }
    }
    //----------------------#getting datas#
    else
    {
        UpdateData(TRUE);
        if(IsDlgButtonChecked(IDC_CHECK_SAVE) && !m_sFileName.IsEmpty())
        {
            //print titles(function) once while saveBox is checked.
            ofstream csvFile(m_sFileName, ios::app);
            if(csvFile.fail())
            {
                MessageBox("file can not open!", "Error", MB_ICONERROR | MB_OK);
                return;
            }

            //out put current time.
            SYSTEMTIME CurTime;
            GetLocalTime(&CurTime);
            csvFile << "Date: " << CurTime.wYear << "/" << CurTime.wMonth;
            csvFile << "/" << CurTime.wDay << ",";

            int fLoop = IDC_COMBO_F1;
            int n;
            CString function;
            CComboBox* pCombo;
            for(n=0; n<atoi(m_sComboItems); n++)
            {
	            pCombo = (CComboBox*)GetDlgItem(fLoop ++);
                pCombo->GetWindowText(function);
                csvFile << function << ",";
            }
            csvFile << "\n";
            csvFile.close();
        }

        SendItemSettings();
        //reset other controls' display.
        EnableItems(FALSE);
        pWnd->EnableWindow(TRUE);
        pWnd->SetWindowText("STOP");

        //set timer interval and start getting data.
        pWnd = GetDlgItem(IDC_EDIT_TMR);
        pWnd->GetWindowText(text);
        int timer = atoi(text);
        if(timer < 20)
        {
            timer = 20;
            pWnd->SetWindowText("20");
        }
        CWnd::SetTimer(TIMER_2, timer, NULL);
    }
}

//##########################################//
//DISPLAY CONTROL
//##########################################//

//==========================================//
///Normal Checked
//==========================================//

//GetOption
void CMainDialog::GetOption(void)
{
    int itemIndex;
    int n;
	CComboBox* pCombo;
	CEdit* pEdit;
    int fLoop = IDC_COMBO_F1;
    int eLoop = IDC_COMBO_E1;
    int oLoop = IDC_COMBO_O1;
    int dLoop = IDC_EDIT_D1;
	int G5G6;
	int DT;

    CString opt;
    CString msg = "*OPT?";

    SetSendMonitor(msg);
    if(!QueriesData(50, msg, &opt))
    {
        return;
    }
    SetReceiveMonitor(opt);

    G5G6 = -1;
	DT   = -1;
    //Get Harmonics option and Delta option
	G5G6=opt.Find("G5");
	if(G5G6 == (-1)) {
		G5G6 = opt.Find("G6");
	}

	HarmonicOption = G5G6;
	//get /DT option 
	DT = opt.Find("DT");

    for(itemIndex=0; itemIndex<MAX_ITEM; itemIndex++)
    {
        //----------------------#set function comboBoxes#
	    pCombo = (CComboBox*)GetDlgItem(fLoop ++);
        pCombo->ResetContent();
		for(n=NORMAL_FUNCTION_TOP; n<=NORMAL_FUNCTION_BOTTM; n++)
		{
			pCombo->AddString(List[n].FunctionName);
		}

		//Add Harmonics funcion
		if(G5G6 >= 0)
		{
			for(n=HARM_FUNCTION_TOP; n<=HARM_FUNCTION_BOTTOM; n++)
				pCombo->AddString(List[n].FunctionName);
		}

		//Add Delta funcion
		if(DT >= 0)
		{
			for(n=DELTA_FUNCTION_TOP; n<=DELTA_FUNCTION_BOTTOM; n++)
				pCombo->AddString(List[n].FunctionName);
		}

        pCombo->SetCurSel(0);

        //-------------#set elememt comboBoxes#
	    pCombo = (CComboBox*)GetDlgItem(eLoop ++);
        pCombo->ResetContent();
        for(n=0; n<lastElement; n++)
        {
            pCombo->AddString(eList[n]);
        }
        if (lastElement != 1)
        {
            pCombo->AddString(eList[3]);
        }
        pCombo->SetCurSel(0);

        //----------------------#set order comboBoxes#
	    pCombo = (CComboBox*)GetDlgItem(oLoop ++);
		if(HarmonicOption >= 0)
		{
			pCombo->ResetContent();
			pCombo->EnableWindow(TRUE);

			for(n=0; n<=MAX_ORDER; n++)
			{
				pCombo->AddString(oList[n]);

			}
			pCombo->SetCurSel(0);
		}
		else
		{
		    pCombo->EnableWindow(FALSE);
		}
        //----------------------#set data editBoxes#
	    pEdit = (CEdit*)GetDlgItem(dLoop ++);
        pEdit->SetWindowText("");
    }
    //----------------------#get settings from instrument#
}

//==========================================//
///DataSave Box Checked
//==========================================//
void CMainDialog::OnCheckSave() 
{
    CWnd* pWnd = GetDlgItem(IDC_EDIT_FILE);
    if(IsDlgButtonChecked(IDC_CHECK_SAVE))
    {
        pWnd->EnableWindow(TRUE);
    }
    else
    {
        pWnd->EnableWindow(FALSE);
    }
}

//==========================================//
///Items Count Changed
//==========================================//
void CMainDialog::OnSelchangeComboItems()
{
    int n;
	CWnd* pWndCtrl;
	CString fun_msg;
	int pos;
	UpdateData();
	//--------#set to ReadOnly(TRUE/FALSE)#
    int fLoop = IDC_COMBO_F1;
    int eLoop = IDC_COMBO_E1;
    int oLoop = IDC_COMBO_O1;
    int dLoop = IDC_EDIT_D1;
    fun_msg="";
	for(n=0; n<atoi(m_sComboItems); n++)
	{
		GetDlgItem(fLoop)->EnableWindow(TRUE);
		GetDlgItemText(fLoop,fun_msg);
		fLoop++;
		//#set element.#  
		pos = -1;
		pos = FindFunPos(fun_msg);
		if(List[pos].ElementFlag)
		{
			GetDlgItem(eLoop++)->EnableWindow(TRUE);
		}
		else 
		{
		    GetDlgItem(eLoop++)->EnableWindow(FALSE);
		}
		//#set Order.# 
		if(HarmonicOption >= 0)
		{
			if(List[pos].OrderFlag) 
			{
			pWndCtrl = GetDlgItem(oLoop++);
			pWndCtrl->EnableWindow(TRUE);
			}
			else 
			{
				pWndCtrl = GetDlgItem(oLoop++);
				pWndCtrl->EnableWindow(FALSE);
			}
		}
		else  
		{
			pWndCtrl = GetDlgItem(oLoop++);
			pWndCtrl->EnableWindow(FALSE);
		}

	    GetDlgItem(dLoop++)->EnableWindow(TRUE);
	}
	while(n < MAX_ITEM)
	{
	    pWndCtrl = GetDlgItem(fLoop++);
		pWndCtrl->EnableWindow(FALSE);
	    pWndCtrl = GetDlgItem(eLoop++);
		pWndCtrl->EnableWindow(FALSE);
	    pWndCtrl = GetDlgItem(oLoop++);
		pWndCtrl->EnableWindow(FALSE);
	    pWndCtrl = GetDlgItem(dLoop++);
		pWndCtrl->EnableWindow(FALSE);
		n++;
	}
}

//==========================================//
///Clear Send Monitor
//==========================================//
void CMainDialog::OnButtonSclr() 
{
    CWnd* pWnd = GetDlgItem(IDC_EDIT_SEND);
    pWnd->SetWindowText("");
}

//==========================================//
///Clear Receive Monitor
//==========================================//
void CMainDialog::OnButtonRclr() 
{
    CWnd* pWnd = GetDlgItem(IDC_EDIT_RCV);
    pWnd->SetWindowText("");
}

//==========================================//
///Change element combobox and order combobox 
///state base on Function_selected_ change
//==========================================//
void CMainDialog::FCombo_SelectedIndexChanged(int fcomb, int ecomb, int ocomb, CString Funbuf)
{
	if(oldFuncName[fcomb - IDC_COMBO_F1] == Funbuf)
		return;
	else
		oldFuncName[fcomb - IDC_COMBO_F1] = Funbuf;

	CComboBox* pCombo_f;
	CComboBox* pCombo_e;
	CComboBox* pCombo_o;
	int pos = -1;

	pCombo_f = (CComboBox*)GetDlgItem(fcomb);
	pCombo_e = (CComboBox*)GetDlgItem(ecomb);
	pCombo_o = (CComboBox*)GetDlgItem(ocomb);
   
	pos = FindFunPos(Funbuf); 
	if(pos >= 0)
	{
		if(List[pos].ElementFlag == true)
		{
		    pCombo_e->SetWindowText("1");
		    pCombo_e->EnableWindow(TRUE);
		}
		else
		{
			pCombo_e->SetWindowText("");
			pCombo_e->EnableWindow(FALSE);
		}
        
		if(HarmonicOption >= 0)
		{
			if(List[pos].OrderFlag == true)
			{
                pCombo_o->SetWindowText("Total");
				pCombo_o->EnableWindow(TRUE);
			}
			else
			{
				pCombo_o->SetWindowText("");
				pCombo_o->EnableWindow(FALSE);
			}
		}
		else
		{
			pCombo_o->SetWindowText("");
            pCombo_o->EnableWindow(FALSE);
		}
	}//end if(pos>=0)

}

void CMainDialog::OnSelchangeComboF1() 
{
    int fcomb = IDC_COMBO_F1;
    int ecomb = IDC_COMBO_E1;
    int ocomb = IDC_COMBO_O1;
	int nIndex = m_combo_f1.GetCurSel();

    CString str;
	if(nIndex != CB_ERR)
	{
		m_combo_f1.GetLBText(nIndex, str);	
	}
	FCombo_SelectedIndexChanged(fcomb, ecomb, ocomb, str);
}

void CMainDialog::OnSelchangeComboF2() 
{
    int fcomb = IDC_COMBO_F2;
    int ecomb = IDC_COMBO_E2;
    int ocomb = IDC_COMBO_O2;
	int nIndex = m_combo_f2.GetCurSel(); 

	CString str;
	if(nIndex != CB_ERR)
	{
		m_combo_f2.GetLBText(nIndex, str);	
	}
	FCombo_SelectedIndexChanged(fcomb, ecomb, ocomb, str);	
}

void CMainDialog::OnSelchangeComboF3() 
{
    int fcomb = IDC_COMBO_F3;
    int ecomb = IDC_COMBO_E3;
    int ocomb = IDC_COMBO_O3;	
	int nIndex;
	nIndex= m_combo_f3.GetCurSel(); 

	CString str;
	if(nIndex != CB_ERR)
	{
		m_combo_f3.GetLBText(nIndex, str);	
	}
	FCombo_SelectedIndexChanged(fcomb, ecomb, ocomb, str);	
}

void CMainDialog::OnSelchangeComboF4() 
{
    int fcomb = IDC_COMBO_F4;
    int ecomb = IDC_COMBO_E4;
    int ocomb = IDC_COMBO_O4;
	int nIndex = m_combo_f4.GetCurSel();  

	CString str;
	if(nIndex != CB_ERR)
	{
		m_combo_f4.GetLBText(nIndex, str);	
	}
	FCombo_SelectedIndexChanged(fcomb, ecomb, ocomb, str);	
}

void CMainDialog::OnSelchangeComboF5() 
{
    int fcomb = IDC_COMBO_F5;
    int ecomb = IDC_COMBO_E5;
    int ocomb = IDC_COMBO_O5;
	int nIndex = m_combo_f5.GetCurSel();  

	CString str;
	if(nIndex != CB_ERR)
	{
		m_combo_f5.GetLBText(nIndex, str);	
	}
	FCombo_SelectedIndexChanged(fcomb, ecomb, ocomb, str);	
}

void CMainDialog::OnSelchangeComboF6() 
{
    int fcomb = IDC_COMBO_F6;
    int ecomb = IDC_COMBO_E6;
    int ocomb = IDC_COMBO_O6;
	int nIndex = m_combo_f6.GetCurSel(); 

	CString str;
	if(nIndex != CB_ERR)
	{
		m_combo_f6.GetLBText(nIndex, str);	
	}
	FCombo_SelectedIndexChanged(fcomb, ecomb, ocomb, str);	
}

void CMainDialog::OnSelchangeComboF7() 
{
    int fcomb = IDC_COMBO_F7;
    int ecomb = IDC_COMBO_E7;
    int ocomb = IDC_COMBO_O7;
	int nIndex = m_combo_f7.GetCurSel(); 

	CString str;
	if(nIndex != CB_ERR)
	{
		m_combo_f7.GetLBText(nIndex, str);	
	}
	FCombo_SelectedIndexChanged(fcomb, ecomb, ocomb, str);	
}

void CMainDialog::OnSelchangeComboF8() 
{
    int fcomb = IDC_COMBO_F8;
    int ecomb = IDC_COMBO_E8;
    int ocomb = IDC_COMBO_O8;
	int nIndex = m_combo_f8.GetCurSel(); 

	CString str;
	if(nIndex != CB_ERR)
	{
		m_combo_f8.GetLBText(nIndex, str);	
	}
	FCombo_SelectedIndexChanged(fcomb, ecomb, ocomb, str);	
}

void CMainDialog::OnSelchangeComboF9() 
{
    int fcomb = IDC_COMBO_F9;
    int ecomb = IDC_COMBO_E9;
    int ocomb = IDC_COMBO_O9;
	int nIndex = m_combo_f9.GetCurSel();  

	CString str;
	if(nIndex != CB_ERR)
	{
		m_combo_f9.GetLBText(nIndex, str);	
	}
	FCombo_SelectedIndexChanged(fcomb, ecomb, ocomb, str);	
}

void CMainDialog::OnSelchangeComboF10() 
{
    int fcomb = IDC_COMBO_F10;
    int ecomb = IDC_COMBO_E10;
    int ocomb = IDC_COMBO_O10;
	int nIndex = m_combo_f10.GetCurSel(); 

	CString str;
	if(nIndex != CB_ERR)
	{
		m_combo_f10.GetLBText(nIndex, str);	
	}
	FCombo_SelectedIndexChanged(fcomb, ecomb, ocomb, str);	
}
