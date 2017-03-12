/*==========================================================
 Copyright (C) 2005 YOKOGAWA ELECTRIC CORPORATION

    ALL RIGHTS RESERVED BY YOKOGAWA ELECTRIC CORPORATION.
THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT
WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND/OR FITNESS FOR A PARTICULAR PURPOSE.

                            YOKOGAWA ELECTRIC CORPORATION
==========================================================*/
// MainDialog.h
#if !defined(AFX_MAINDIALOG_H__45DE7D44_6716_494E_8A58_F6A26855DDBB__INCLUDED_)
#define AFX_MAINDIALOG_H__45DE7D44_6716_494E_8A58_F6A26855DDBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Connection.h"

//Define structure for function list 
struct Funstr
 {
    public:
	CString FunctionName;
    bool ElementFlag;
    bool OrderFlag;
 };

// CMainDialog
class CMainDialog : public CDialog
{
public:
	CMainDialog(CWnd* pParent = NULL);   //constructor
    int HarmonicOption;
	int Currentelement;
	//{{AFX_DATA(CMainDialog)
	enum { IDD = IDD_MAIN_DIALOG };
	CComboBox	m_combo_f9;
	CComboBox	m_combo_f8;
	CComboBox	m_combo_f7;
	CComboBox	m_combo_f6;
	CComboBox	m_combo_f5;
	CComboBox	m_combo_f4;
	CComboBox	m_combo_f3;
	CComboBox	m_combo_f10;
	CComboBox	m_combo_f1;
	CComboBox	m_combo_f2;
	CComboBox	m_comboR;      //update rate combo
	CComboBox	m_comboU;      //voltage combo
	CComboBox	m_comboI;      //current combo
	CString	m_sEditTest;       //test command edit
	CString	m_sEditSend;       //send command monitor
	CString	m_sEditRcv;        //receive data monitor
	CString	m_sEditError;      //error get edit
	CString	m_sFileName;       //file name for saving data
	int		m_iRadioElement;
	CString	m_sComboItems;
	//}}AFX_DATA

	// ClassWizard
	//{{AFX_VIRTUAL(CMainDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMainDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioE1();
	afx_msg void OnRadioE2();
	afx_msg void OnRadioE3();
	afx_msg void OnButtonSetRange();
	afx_msg void OnButtonSendCmd();
	afx_msg void OnSelchangeComboItems();
	afx_msg void OnButtonGetItems();
	afx_msg void OnButtonSetRate();
	afx_msg void OnButtonSetItems();
	afx_msg void OnButtonGetDataS();
	afx_msg void OnButtonGetDataR();
	afx_msg void OnButtonGetDataT();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCheckSave();
	afx_msg void OnButtonSclr();
	afx_msg void OnButtonRclr();
	afx_msg void OnButtonGetError();
	afx_msg void OnButtonHeader();
	afx_msg void OnButtonRst();
	afx_msg void OnDlgClose();
	afx_msg void OnClose();
	afx_msg void OnButtonVerbose();
	afx_msg void OnSelchangeComboF2();
	afx_msg void OnSelchangeComboF3();
	afx_msg void OnSelchangeComboF4();
	afx_msg void OnSelchangeComboF5();
	afx_msg void OnSelchangeComboF6();
	afx_msg void OnSelchangeComboF7();
	afx_msg void OnSelchangeComboF8();
	afx_msg void OnSelchangeComboF9();
	afx_msg void OnSelchangeComboF10();
	afx_msg void OnSelchangeComboF1();
	afx_msg void OnEditupdateComboF1();
	afx_msg void OnEditupdateComboF2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

    HICON m_hIcon;
    Funstr List[66];   //function structure list
    CString errorMsg[13];   //error message arrary
    CString updateRateList[10];  //update rate combo list
    CString voltageList[2][12];  //voltage combo list
    CString currentList[2][19];  //current combo list
    CString rangeListAuto;       //auto range list item
    CString eList[7];            //element combo list
    CString oList[103];          //order combo list
    int lastElement;
    CString crestFactor;
	CString	oldFuncName[10];

	CConnection m_connection;   //CConnection, use for communications.

    //FCombo_SelectedIndexChanged
    void FCombo_SelectedIndexChanged(int fcomb, int ecomb, int ocomb, CString Funbuf);

	//Find the function postion in the structure list
	int FindFunPos(CString fun);

    //GetOptions
	void GetOption(void);

    //value initialize.
    void InitListItem(void);

    //specify the current content from the list.
    void ComboListSel(CComboBox* pCombo, CString content);

    //return the specified string which was the left potion of the inData,
    //and the inData left the right potion.
    CString CutLeft(char symbol, CString* inData);

    //send message and receive data.
    bool QueriesData(int maxLength, CString sMsg, CString* data);

    //get current and voltage settings.
	bool GetRanges(int eNo);

    //get the update rate setting.
	void GetUpdateRate(void);

    //get item settings from instrument.
	void ReadItemSettings(void);

    //send item settings to instrument.
    void SendItemSettings(void);

    //get all datas and display.
    void GetItemData(void);

    //set all buttons disable when getting data or resume them.
    void EnableItems(bool isEnable);

    //when error occured, display it.
	void DispError(int errorID);
    void DispError(CString errorInfo);

    //set the sended message to display.
    void SetSendMonitor(CString msg);

    //set the received data to display.
    void SetReceiveMonitor(CString data);
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_MAINDIALOG_H__45DE7D44_6716_494E_8A58_F6A26855DDBB__INCLUDED_)
