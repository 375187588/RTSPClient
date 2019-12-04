#if !defined(AVDECODER_H_INCLUDED_)
#define AVDECODER_H_INCLUDED_



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AVDecoder.h : header file
#include <windows.h>
#include "WaveSound.h"
#include "H264.h"

#ifdef __cplusplus
extern "C" {
#endif 

#ifdef HAVE_AV_CONFIG_H
#undef HAVE_AV_CONFIG_H
#endif

#include "./include/libavcodec/avcodec.h"
#include "./include/libavutil/mathematics.h"
#include "./include/libswresample/swresample.h"  
#include "libswscale/swscale.h"

#ifdef __cplusplus
}
#endif 

//#pragma comment( lib, "libgcc.a")
//#pragma comment( lib, "libmingwex.a")

//#pragma comment( lib, "libavcodec.a")
//#pragma comment( lib, "libavutil.a")
//#pragma comment(lib,  "libswscale.a")


#pragma comment( lib, "avcodec.lib")
#pragma comment( lib, "avutil.lib")
#pragma comment(lib,  "swscale.lib")
#pragma comment(lib, "swresample.lib")


typedef LRESULT (CALLBACK* OutputVideoCB)(int devNum, PBYTE pRgb, int  dwSize);



 //��AudioFormatת��FFMpeg��AVCodecID
extern AVCodecID  GetAudioCodecID(AudioFormat audio_fmt);

//��VideoFormatת��FFMpeg��AVCodecID
extern AVCodecID  GetVideoCodecID(VideoFormat video_fmt);


enum PLAYSTATE {
	STOPPED, PAUSED, RUNNING, INIT };


/////////////////////////////////////////////////////////////////////////////
// CAVDecoder window

class CAVDecoder : public CWnd
{
// Construction
public:
	CAVDecoder();

	void SetNum(int num) { m_Num = num; }

// Attributes
public:

	HRESULT   StartDecode(); //��ʼ���룬ֻ�ǰ��ڲ�״̬���"Running"����ʼ������������OnDecodeVideo����
	void      StopDecode(); //���ٽ�����

	void      SetVideoSize(int width, int height);
	BOOL      GetVideoSize(CSize & size); //��ȡͼ��ֱ���

	PLAYSTATE GetState() { return m_psCurrent; }

	void      SetupVideoWindow(HWND hVideoWnd); //������ʾͼ��Ĵ��ھ��
	void      ResizeVideoWindow();

	DWORD     GetCurrentBitrate(); 


	//������ʾͼ��ص�����
	void       SetDisplayCallback(OutputVideoCB captureCB) { m_OutputCB = captureCB; }

	//����һ֡ͼ��
	BOOL       OnDecodeVideo(PBYTE pBuffer, long inLen, VideoFormat vFormat, int nFrameType); 

    
	//������Ƶ
	BOOL       OnDecodeAudio(PBYTE inbuf, int inLen, AudioFormat aFormat, AVSampleFormat out_sample_fmt, BYTE * pBufOut, int * pOutLen);

    BOOL       OpenDecoder(AVCodecID vCodec); //�򿪽�����
	void       CloseDecoder(); //�رս�����


	BOOL       IsInited() { return m_bInited; }

	void       SetAudioTimeBase(unsigned int audioTimebase) { m_audio_rtp_timebase = audioTimebase; } //��Ƶ��ʱ����ߣ�һ����ڲ����ʣ�

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAVDecoder)
	//}}AFX_VIRTUAL

// Implementation
public:

	virtual ~CAVDecoder();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAVDecoder)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

protected:

	void     DisplayMesg(TCHAR* szFormat, ...);

	AVFrame * alloc_picture(enum PixelFormat pix_fmt, int width, int height);

private:

    UINT           m_Num;
	HWND           m_hApp;
	OutputVideoCB   m_OutputCB;

	PLAYSTATE m_psCurrent; 

	int   m_nWidth, m_nHeight;

	int      m_nFrameNumber1;  //�����������֡��

	//��Ƶ����������
	AVCodec * m_pVideoCodec;
	AVCodecContext * m_pVideoCodecCtx;
			
	AVFrame *m_picture; 
	SwsContext *m_pImgCtx;

	//AVFrame *m_pFrameRGB;
	//uint8_t *m_PicBuf;
	//int m_PicBytes;

	PBYTE          m_pRgb24;
    UINT           m_nRgbSize;

	AVFrame *out_picture;
	//uint8_t *picture_buf;

	BOOL   m_bInited;


	//��Ƶ����������
	AVCodecContext *m_pAudioCodecCtx;
	AVCodec *m_pAudioCodec;

	int16_t *m_pSamples; //��Ƶ��������ʱ������

	 struct SwrContext *au_convert_ctx; 

	//�ط���Ƶ�Ķ���ָ��
	CWaveSound  *m_pAudioPlay;

	AVFrame *m_pAudioFrame;
	
	unsigned int m_audio_rtp_timebase;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AVDECODER_H_INCLUDED_)
