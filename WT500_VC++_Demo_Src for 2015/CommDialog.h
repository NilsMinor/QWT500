/*==========================================================
 Copyright (C) 2005 YOKOGAWA ELECTRIC CORPORATION

    ALL RIGHTS RESERVED BY YOKOGAWA ELECTRIC CORPORATION.
THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT
WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND/OR FITNESS FOR A PARTICULAR PURPOSE.

                            YOKOGAWA ELECTRIC CORPORATION
==========================================================*/
// CommDialog.h
#if !defined(AFX_COMMDIALOG_H__2BB7D549_BBDA_4434_9746_B04AA505304B__INCLUDED_)
#define AFX_COMMDIALOG_H__2BB7D549_BBDA_4434_9746_B04AA505304B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Connection.h"
#include "tmctl.h"
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// CCommDialog

class CCommDialog : public CDialog
{
public:
	CCommDialog(CWnd* pParent = NULL); //constructor
	//{{AFX_DATA(CCommDialog)
	enum { IDD = IDD_COMM_DIALOG };
	CEdit	m_UsbSerialNo;
	CEdit	m_editIp;
	CComboBox	m_comboAddr;
	CString	m_sIp;
	int		m_iWire;
	int		m_nEditOrCombo_Ether;	//0:edit 1:combo
	int		m_nEditOrCombo_USB;		//0:edit 1:combo
	#define Ini_Sec_Connect			"Connection"
	#define Ini_Key_GPIB_Address	"GIPB_Address"
	#define Ini_Key_Ether_IPAddr	"VXI11_IPAddr"
	#define Ini_Key_USB_Serial		"USBTMC_Serial"
	CString Ini_Path;
	char	szFullPath[_MAX_PATH];
	char	szDrive[_MAX_DRIVE];
	char	szDir[_MAX_DIR];
	char	szFname[_MAX_FNAME];
	char	szExt[_MAX_EXT];
	void	GetConnectSettings();
	void	WriteConnectSettings(int);
	int		Check_WTSeries(int wire,char* addr);
	//}}AFX_DATA
	//{{AFX_VIRTUAL(CCommDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV use
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CCommDialog)
	afx_msg void OnRadioGpib();
	afx_msg void OnRadioEther();
	virtual void OnOK();
	afx_msg void OnRadioUsb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    HICON m_hIcon;
	BOOL OnInitDialog();
	CConnection m_connection;
	afx_msg void OnBnClickedBtnSearchSerial();
	afx_msg void OnBnClickedBtnSearchIp();
	CComboBox m_Combo_SerialNo;
	CComboBox m_Combo_IPAddr;
	CButton m_Button_SerialNo;
	CButton m_Button_IPAddr;
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_COMMDIALOG_H__2BB7D549_BBDA_4434_9746_B04AA505304B__INCLUDED_)
