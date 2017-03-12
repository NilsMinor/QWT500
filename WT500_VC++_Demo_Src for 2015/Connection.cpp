/*==========================================================
 Copyright (C) 2005 YOKOGAWA ELECTRIC CORPORATION

    ALL RIGHTS RESERVED BY YOKOGAWA ELECTRIC CORPORATION.
THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT
WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND/OR FITNESS FOR A PARTICULAR PURPOSE.

                            YOKOGAWA ELECTRIC CORPORATION
==========================================================*/
//Connection.cpp

#include "stdafx.h"
#include "WT500Demo.h"

#include "tmctl.h"
#include "Connection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//##Static Member Init##
int CConnection::m_iID = -1;
CString CConnection::m_sName = "";

//##Constructor##
CConnection::CConnection()
{
    m_iTerminitor = 2;
}

//##Constructor##
CConnection::CConnection(int iPort, CString sAddr)
{
	m_iPort       = iPort;
	m_sAddress    = sAddr;
//	m_iTerminitor = 1;
	m_iTerminitor = 2;

}

//##Constructor##
CConnection::~CConnection()
{
}

//##Set Connection To The Device##
int CConnection::Initialize()
{
	int ret;//return 0 when successed.
	ret = TmcInitialize(m_iPort, m_sAddress.GetBuffer(0), &m_iID);
	if(ret !=0)
	{
		return ret;
	}
	
    //set terminator of the message.
	ret = TmcSetTerm(m_iID, m_iTerminitor, 1);
	if(ret != 0)
	{
		TmcFinish(m_iID);
		return ret;
	}

    //timeout settings, 20*100ms
	ret = TmcSetTimeout(m_iID, 20);
	if(ret != 0)
	{
		TmcFinish(m_iID);
		return ret;
	}

	//test the device module connected.
	ret = TmcSend(m_iID, "*IDN?");
	int maxLength = 256;
	char* buf ;
	if ((buf = new char[maxLength]) == NULL)
    {
        return 1;
    }
    memset(buf,0,maxLength);
	int realLength;
	ret = TmcReceive(m_iID, buf, maxLength, &realLength);
	m_sName.Format("%s", buf);
	delete[] buf;
	if(ret != 0)
	{
		TmcFinish(m_iID);
	}
	return ret;
}

//##Function: Set Timeout##
int CConnection::SetTimeout(int timeout)
{
	return(TmcSetTimeout(m_iID, timeout));
}

//##Function: Break the Connection##
int CConnection::Finish()
{
	return(TmcFinish(m_iID));
}

//##Function: Sending Message##
int CConnection::Send(CString msg)
{
	return(TmcSend(m_iID, (LPSTR)(LPCTSTR)msg));
}

int CConnection::SendByLength(CString msg, int len)
{
	return(TmcSendByLength(m_iID, (LPSTR)(LPCTSTR)msg, len));
}

//##Function: Receive Message##
int CConnection::ReceiveSetup()
{
	return(TmcReceiveSetup(m_iID));
}

int CConnection::ReceiveOnly(char* buf, int blen, int* rlen)
{
	return(TmcReceiveOnly(m_iID, buf, blen, rlen));
}

int CConnection::Receive(char* buf, int blen, int* rlen)
{
	return(TmcReceive(m_iID, buf, blen, rlen));
}

int CConnection::ReceiveBlockHeader(int* rlen)
{
	return(TmcReceiveBlockHeader(m_iID, rlen));
}

int CConnection::ReceiveBlockData(char* buf, int blen, int* rlen, int* end)
{
	return(TmcReceiveBlockData(m_iID, buf, blen, rlen, end));
}

//##Function: Get Last Error##
int CConnection::GetLastError()
{
	return(TmcGetLastError(m_iID));
}

//##Function: Set the Remote Status##
int CConnection::SetRen(int flag)
{
	return(TmcSetRen(m_iID, flag));
}

//##Function: Encode Serial Number##
int CConnection::EncodeSerialNumber(char* encode, size_t len, char* src)
{
	return(TmcEncodeSerialNumber(encode, len, src));
}

