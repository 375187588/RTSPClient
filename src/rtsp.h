/**
** Blog: https://blog.csdn.net/quickgblink
** Email: quickgblink@126.com
**/
//
// Rtsp.h
//

#ifndef _RTSP_CLIENT_H
#define _RTSP_CLIENT_H

#pragma once

//#include "config_unix.h"
#include "config_win32.h"
#include "H264.h"
#include "SpsDecode.h"
#include <vector>

#define MAX_READBUFSIZE 20000
#define MAX_PACKET_SIZE 10000

typedef		DWORD (WINAPI * RtspDataCallback)(BYTE *pBuf, long Buflen, ULONG lTimeStamp, int nFormat, int nFrameType, LPVOID lpContext);

struct MediaAttribute
{
	unsigned fVideoFrequency; //��Ƶ1���ʱ���ʾ��һ����90000
	unsigned char fVideoPayloadFormat;
	char fConfigAsc[200];
	unsigned char fConfigHex[100];
	int fConfigHexLen;
	unsigned fVideoFrameRate; //��Ƶ֡��
	unsigned fTimeIncBits;
	unsigned fixed_vop_rate;
	unsigned fVideoWidth;
	unsigned fVideoHeight; 
	unsigned fAudioFrequency; //��Ƶ1���ʱ���ʾ��һ�������Ƶ������
	unsigned char fAudioPayloadFormat; 
	unsigned fTrackNum;
	unsigned fAudioChannels; // ������
	VideoFormat fVideoFormat; //��Ƶ�����ʽ
	AudioFormat fAudioFormat; //��Ƶ�����ʽ
};

struct ResultData
{
	int streamChannelId; //0--RTP Stream Data, 1--RTCP
	unsigned char * buffer;
	int  len; 
	int  cache_size; //������һ֡������ǰ֡��PTS����ͬ�������ݳ���
	unsigned char fRTPPayloadFormat;
	unsigned long frtpTimestamp;
	int frtpMarkerBit;
	unsigned char cFrameType; //֡������, 'I'�� 'P'�� 'B'
};

struct MediaSubsession 
{
	char * fMediumName;
	char * fCodecName;
	char * fControlPath;// "a=control:<control-path>" 
	char * fConfig;
	unsigned short fClientPortNum;
	unsigned fNumChannels;
	unsigned frtpTimestampFrequency;
	unsigned char fRTPPayloadFormat;
	char * fSessionId;
	char *fFileName;
	FILE* fFid;
	int fWidth; //��Ƶ���
	int fHeight; //��Ƶ�߶�
	int  rtpChannelID;
	int  rtcpChannelID;
	struct MediaSubsession *fNext;
};

typedef struct
{
	BYTE	RC:5;				// ���ڿ����
	BYTE	P:1;				// ���λ������1��ʾ��β��������ݣ����һ���ֽڱ�ʾ������ݵĳ���
	BYTE	Version:2;			// RTP�汾�ţ�2
	BYTE	PT;					// �����ͣ�200-RTCP-SR��
	USHORT	Len;				// ����
	UINT	SSRC;				// SSRC�α�ʶͬ��Դ

	UINT	TimeNtpH;			// NTPʱ���λ
	UINT	TimeNtpL;			// NTPʱ���λ
	UINT	TimeRtp;			// ��Rtp��Ӧ��ʱ��
	UINT	RtpCounts;			// Rtp���������ݼ���
	UINT	RtpByteCounts;		// Rtp�����͵��ֽ���
}RTCP_HEAD,*PRTCP_HEAD;


enum PACKET_TYPE
{
	RTP_PACKET = 0,
	RTCP_PACKET,
	UNKNOWN_PACKET
};

//һ����Ƶ֡�ڻ�������λ�úʹ�С
struct AudioFrameSlice
{
  int    pos;
  int   framelen;
};


class RtspClient
{

public:
	RtspClient();
	~RtspClient();

	BOOL  OpenStream(const char * szURL);
	void  CloseStream();

	int parseRTSPURL(char* url,char* address,int* portNum); 

    void SetRtspCallback(RtspDataCallback lpFunc, LPVOID lpContext); //���ûص�������ȡý������

	VideoFormat  GetVideoFormat();
	AudioFormat  GetAudioFormat();

	const MediaAttribute * GetMediaAttribute(); 

	BOOL  GetVideoSize(int & nWidth, int & nHeight); //��ȡ��Ƶ�ֱ���

	void  SetAuthInfo(const char * szUserName, const char * szPassword); //�����û���֤��Ϣ

	//int   GetAudioFrameSliceInfo(std::vector<AudioFrameSlice> & vSlices); //��ȡ������Ƶ֡�ڻ�������λ����Ϣ
	const std::vector<AudioFrameSlice> & GetAudioFrameSliceInfo();

	__int64      GetVideoNtpTime(unsigned int rtpTimestamp);  //convert the rtp time into ntp time
	__int64      GetAudioNtpTime(unsigned int rtpTimestamp); //convert the rtp time into ntp time

	unsigned int  GetVideoTimeFrequency() { return VTimestampFrequency; }
	unsigned int  GetAudioTimeFrequency() { return ATimestampFrequency; }

protected:
	void    SetUrl(const char * szURL);

	int     netRTSPConnect(SOCKET fd, uint32_t ip, uint32_t port, uint32_t nTimeOut);
	int     init_rtsp(const char *url, struct MediaAttribute *Attribute);

	int     setupMediaSubsession(struct MediaSubsession* subsession,int subsessionNum);

	int     getSDPDescriptionFromURL(char* url,char* Description);
	int     openConnectionFromURL(char* url);

	struct   MediaSubsession * initializeWithSDP(char* sdpDescription,int *SubsessionNum); 
	struct   MediaAttribute *  SetMediaAttrbute(struct MediaAttribute *Attribute,struct MediaSubsession * subsession,int subsessionNum);

	int     setupStreams(struct MediaSubsession *subsession,int subsessionNum);
	int     startPlayingStreams(struct MediaSubsession *subsession,int subsessionNum); 

	int     teardownMediaSession();
	void    resumeStreams();
	int     pauseMediaSession();
	int     playMediaSession(int start,int end);
	int     SetParametersMediaSession(char *parameters);
	int     GetParametersMediaSession(char *parameters, bool bNotWaitForResponse = false); 

	int     SendOptionsCmd(bool bNotWaitForResponse);

	//����RTP��RTCP���ĺ���
	 int    RTP_OR_RTCP_ReadHandler(PACKET_TYPE * packet_type, struct ResultData** rtp_data, RTCP_HEAD * rtcp_data);

	int     rtpHandler(unsigned char * buffer, unsigned int bytesRead,  struct ResultData** data);
	int     handleRead(unsigned char* buffer,unsigned bufferMaxSize,unsigned* bytesRead,unsigned* NextTCPReadSize);

	//��RTP���в��H264����
	int     rtp_unpackage_H264(unsigned char *inbuf, int len, BOOL & marker, int & nIDR, unsigned char * outbuf, int & total_bytes, int nSeqNo);


	//��RTP���в��AAC����
	int      rtp_unpackage_AAC(unsigned char * bufIn, int recvLen, BOOL  marker, int audioSamprate, unsigned char* pBufOut,  int* pOutLen);

	int      rtp_unpackage_AMR(unsigned char * bufIn, int recvLen,  unsigned char* pBufOut,  int* pOutLen);
	int      rtp_unpackage_MP3(unsigned char * bufIn, int recvLen,  unsigned char* pBufOut,  int* pOutLen);

	int     blockUntilwriteable(int socket);
	int     blockUntilReadable(int socket);

	char*   parseSDPLine(char* inputLine); //����SDP��Ϣ��������ȡ��Ƶ����Ƶ��Ϣ
					   
	int     parseResponseCode(char* line, unsigned int * responseCode);
	int     getResponse(char* responseBuffer,unsigned responseBufferSize) ;

	char*   getLine(char* startOfLine);

	char * parseSDPAttribute_rtpmap(char* sdpLine,unsigned* rtpTimestampFrequency,unsigned *fnumChannels) ;//Check for a "a=rtpmap:<fmt> <codec>/<freq>line
	char * parseSDPAttribute_control(char* sdpLine) ;//Check for a "a=control:<control-path>" line
	int    parseSDPAttribute_range(char* sdpLine);//Check for a "a=range:npt=<startTime>-<endTime>" line
	char * parseSDPAttribute_fmtp(char* sdpLine) ;//Check for a "a=fmtp:" line

	char*   lookupPayloadFormat(unsigned char rtpPayloadType, unsigned& freq, unsigned& nCh);

	unsigned char * AllocNetBuffer(); //��������������ݵ�Buffer
	void           ReleaseBuffer();

	void          FreeSubSessions(struct MediaSubsession *subsession,int subsessionNum);
	void          clearup();

	static DWORD WINAPI RtspThrd(void * pParam); //RTSP���Ӻͽ����߳�
	int         RtspThreadProc(); //����RTP��RTCP�����̺߳���

	static DWORD WINAPI SendThrd(void * pParam); //��������������ݰ����߳�
	int         SendThreadProc(); //����������RTCP�����̺߳���

	int          GetFormatType(unsigned char fRTPPayloadFormat);

	void         SetThreadActive(BOOL bActive);

	void       HostOrderToNetOrder(RTCP_HEAD * pHead);

private:
	unsigned long LVrtpTimestamp;//last video timestamp
	unsigned long LArtpTimestamp;//last audio timestamp

	unsigned long MaxFrameNum;
	unsigned long aloopNum;
	unsigned long vloopNum;
	unsigned long audioFirstTimestamp;
	unsigned long videoFirstTimestamp;
	unsigned subTimeFlag;
     
	unsigned char LVFrameType; //�ϴε���Ƶ֡����('I','P','B')
	BOOL    m_bFrameContainSPS; //��ǰ֡�Ƿ����H264 SPS��IDR = 0x7��

protected:
    int m_socketNum;
	unsigned fCSeq;

	char fBaseURL[128]; //RTSP URL��ַ
	char fLastSessionId[64];
	unsigned long fMaxPlayEndTime;

	unsigned VTimestampFrequency;
	unsigned ATimestampFrequency;

	unsigned char VPayloadType;
	unsigned char APayloadType;
	unsigned long VFrameRate;


	//int  m_AudioSampleRate; //��Ƶ������(44100, 48000, 9600)
    struct MediaAttribute m_Attribute;//��Ƶ����Ƶ����

	int   m_rtpNumber;
	int   m_rtcpNumber;

	int PotPos;

	unsigned char *   m_netbuf;

	struct ResultData  m_VFrame; //��Ƶ����(������һ֡)
	struct ResultData  m_AFrame; //��Ƶ����

	HANDLE      m_RtspThread;
	HANDLE      m_SendThread;
	BOOL        m_bStopThread;
	BOOL        m_bThreadActive;

	RtspDataCallback  m_lpFunc;
	LPVOID            m_lpUserContext;

	unsigned char m_buffer2[1500]; 

	BOOL  m_bUserAuth; //�������Ƿ���Ҫ�û���֤

	char m_username[64];
	char m_password[64];

	char m_realm[64];//��¼�û���֤��Ϣ��Digest��֤ʱ��Ҫʹ�õ�
	char m_nonce[64];//��¼�û���֤��Ϣ��Digest��֤ʱ��Ҫʹ�õ�
	int  m_authType;//�û���֤����,1ΪBASE64��֤��2ΪDigest��֤

	unsigned char   m_sprop_parameter[4096];//H264 SPS������
	int    m_sprop_len;//���������ʵ�����ݵĳ���
 
	int  m_video_rtp_id, m_audio_rtp_id; //��Ƶ����Ƶ����RTPͨ��ID
	int  m_video_rtcp_id, m_audio_rtcp_id; //��Ƶ����Ƶ����RTCPͨ��ID

	std::vector<AudioFrameSlice> m_vAudioFrameSlices;

	RTCP_HEAD  m_video_rtcp_packet;
	RTCP_HEAD  m_audio_rtcp_packet;

	//time_t  m_ntp_msw_video; //��λ���룬�ɻ���ɵ�ǰ������-ʱ��
	//time_t  m_ntp_msw_audio; //��λ����
	//UINT    m_ntp_lsw_video; //��λ��232 picoseconds, 1 second = 1,000,000,000,000 picoseconds
	//UINT    m_ntp_lsw_audio; //��λ��232 picoseconds, 1 second = 1,000,000,000,000 picoseconds

};

#endif
