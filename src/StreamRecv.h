// StreamRecv.h : NSPlayer Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������

#define DM_SPEC      WM_APP+3
#define XCM_SPEC     WM_APP+4
#define NS_SPEC      WM_APP+5
#define GETSPEC      0
#define SETSPEC      1������
#define SETNBOD      2
#define SETNSPEC     3     
#define GETNSPEC     4


typedef struct sNSPARAM
{
char  ipAddr[32];
int   input;
float contrast;
float brightness;
float hue;
float saturation;
}NSPARAM;


//startup parameters
typedef struct sAppParam
{

  LONG DmanID;
  RECT geom; //����λ��
  BOOL border;
  char  ipAddr[256];//������IP
  int   input;//ͨ����
}AppParam;

// CNSPlayerApp:
// �йش����ʵ�֣������ StreamRecv.cpp
//

class CNSPlayerApp : public CWinApp
{
public:
	CNSPlayerApp();

	//void  processParams(int argc, char* argv[], AppParam *param);

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CNSPlayerApp theApp;
