#pragma once


// CConnectServerDlg �Ի���

class CConnectServerDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectServerDlg)

public:
	CConnectServerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConnectServerDlg();

	CString  m_strIP;
	UINT     m_nChan;

// �Ի�������
	enum { IDD = IDD_DIALOG_CONNECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
