/*==========================================================
 Copyright (C) 2005 YOKOGAWA ELECTRIC CORPORATION

    ALL RIGHTS RESERVED BY YOKOGAWA ELECTRIC CORPORATION.
THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT
WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND/OR FITNESS FOR A PARTICULAR PURPOSE.

                            YOKOGAWA ELECTRIC CORPORATION
==========================================================*/
//Connection.h
#if !defined(AFX_DEVICE_H__E150E367_7E9F_48B3_B644_BED4B2D84502__INCLUDED_)
#define AFX_DEVICE_H__E150E367_7E9F_48B3_B644_BED4B2D84502__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CConnection
{
public:
	CString m_sAddress;     //address parameter.
	int m_iPort;            //connection type.
	int m_iTerminitor;      //terminator in message.
	static int m_iID;       //connection ID.
	static CString m_sName; //the model of the instrument.

	CConnection();
	CConnection(int iPort, CString sAddr);
	virtual ~CConnection();

public:
    //Set Connection To The Device
	int Initialize(void);
    //Function: Set Timeout
	int SetTimeout(int timeout);
    //Function: Break the Connection
	int Finish(void);
    //Function: Sending Message
	int Send(CString msg);
    int SendByLength(CString msg, int len);
    //Function: Receive Message
	int ReceiveSetup(void);
	int ReceiveOnly(char* buff, int blen, int* rlen);
	int Receive(char* buff, int blen, int* rlen);
	int ReceiveBlockHeader(int* rlen);
	int ReceiveBlockData(char* buff, int blen, int* rlen, int* end);
    //Function: Get Last Error
    int GetLastError();
    //Function: Set the Remote Status
	int SetRen(int flag);
	int EncodeSerialNumber(char* encode,size_t len, char* src);
};

#endif // !defined(AFX_DEVICE_H__E150E367_7E9F_48B3_B644_BED4B2D84502__INCLUDED_)
