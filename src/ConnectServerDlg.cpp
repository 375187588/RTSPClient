// ConnectServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StreamRecv.h"
#include "ConnectServerDlg.h"
#include ".\connectserverdlg.h"


// CConnectServerDlg �Ի���

IMPLEMENT_DYNAMIC(CConnectServerDlg, CDialog)
CConnectServerDlg::CConnectServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectServerDlg::IDD, pParent)
{
	m_strIP = "127.0.0.1";
	m_nChan = 0;
}

CConnectServerDlg::~CConnectServerDlg()
{
}

void CConnectServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConnectServerDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CConnectServerDlg ��Ϣ�������

void CConnectServerDlg::OnBnClickedOk()
{
	GetDlgItem(IDC_EDIT_CONNECT_IP)->GetWindowText(m_strIP);
	m_nChan = GetDlgItemInt(IDC_EDIT_CONNECT_PORT);

	if(m_strIP.IsEmpty() )
	{
         MessageBox("IP��ַ��Ч", "��ʾ", MB_OK|MB_ICONWARNING);
		 return;
	}
	
	if( m_nChan < 0 || m_nChan >=4 )
	{
         MessageBox("ͨ������Ч", "��ʾ", MB_OK|MB_ICONWARNING);
		 return;
	}

	OnOK();
}

BOOL CConnectServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_EDIT_CONNECT_IP)->SetWindowText(m_strIP);
    SetDlgItemInt(IDC_EDIT_CONNECT_PORT, m_nChan);

	return TRUE;  

}
