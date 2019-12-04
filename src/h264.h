/*
��H264��H265��RTP����֡����ȡNALU��Ԫ
�󲿷ֺ�����ȡ��FFmpeg��rtpdec_h264.c, rtpdec_hevc.c�ļ�
*/
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <assert.h>
#include <conio.h>


#define  USE_PORT 7000
#define  USE_IP "127.0.0.1"
#define  MAXDATASIZE 1500
#define  H264 96                   //��������

typedef struct
{
	unsigned char v;               //!< �汾��
	unsigned char p;			   //!< Padding bit, Padding MUST NOT be used
	unsigned char x;			   //!< Extension, MUST be zero
	unsigned char cc;       	   //!< CSRC count, normally 0 in the absence of RTP mixers 		
	unsigned char m;			   //!< Marker bit ������� m = 1 ,��Ƭ�������һ���� = 1����Ƭ�������� m = 0 ,��ϰ���׼ȷ 
	unsigned char pt;			   //!< 7 bits, �������� H264 96 
	unsigned int seq;			   //!< ����
	unsigned int timestamp;	       //!< timestamp, 27 MHz for H.264 �¼���
	unsigned int ssrc;			   //!< ���
	unsigned char *  payload;      //!< the payload including payload headers
	unsigned int paylen;		   //!< length of payload in bytes
} RTPpacket_t;

typedef struct 
{
	/*  0                   1                   2                   3
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|V=2|P|X|  CC   |M|     PT      |       sequence number         |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                           timestamp                           |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|           synchronization source (SSRC) identifier            |
	+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	|            contributing source (CSRC) identifiers             |
	|                             ....                              |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    */
	//intel ��cpu ��intelΪС���ֽ��򣨵Ͷ˴浽�׵�ַ�� ��������Ϊ����ֽ��򣨸߶˴浽�͵�ַ��
	/*intel ��cpu �� �߶�->csrc_len:4 -> extension:1-> padding:1 -> version:2 ->�Ͷ�
	 ���ڴ��д洢 ��
	 ��->4001���ڴ��ַ��version:2
	     4002���ڴ��ַ��padding:1
		 4003���ڴ��ַ��extension:1
	 ��->4004���ڴ��ַ��csrc_len:4

     ���紫����� �� �߶�->version:2->padding:1->extension:1->csrc_len:4->�Ͷ�  (Ϊ��ȷ���ĵ�������ʽ)

	 ��������ڴ� ��
	 ��->4001���ڴ��ַ��version:2
	     4002���ڴ��ַ��padding:1
	     4003���ڴ��ַ��extension:1
	 ��->4004���ڴ��ַ��csrc_len:4
	 �����ڴ���� ���߶�->csrc_len:4 -> extension:1-> padding:1 -> version:2 ->�Ͷ� ��
	 ����
	 unsigned char csrc_len:4;        // expect 0 
	 unsigned char extension:1;       // expect 1
	 unsigned char padding:1;         // expect 0 
	 unsigned char version:2;         // expect 2 
	*/
	/* byte 0 */
	 unsigned char csrc_len:4;        /* expect 0 */
	 unsigned char extension:1;       /* expect 1, see RTP_OP below */
	 unsigned char padding:1;         /* expect 0 */
	 unsigned char version:2;         /* expect 2 */
	/* byte 1 */
	 unsigned char payloadtype:7;     /* RTP_PAYLOAD_RTSP */
	 unsigned char marker:1;          /* expect 1 */
	/* bytes 2,3 */
	 unsigned int seq_no;            
	/* bytes 4-7 */
	 unsigned int timestamp;        
	/* bytes 8-11 */
	 unsigned int ssrc;              /* stream number is used here. */
} RTP_HEADER;


typedef struct
{
	unsigned char forbidden_bit;           //! Should always be FALSE
	unsigned char nal_reference_idc;       //! NALU_PRIORITY_xxxx
	unsigned char nal_unit_type;           //! NALU_TYPE_xxxx  
	unsigned int startcodeprefix_len;      //! Ԥ��
	unsigned int len;                      //! Ԥ��
	unsigned int max_size;                 //! Ԥ��
	unsigned char * buf;                   //! Ԥ��
	unsigned int lost_packets;             //! Ԥ��
} NALU_t;

/*
+---------------+
|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+
|F|NRI|  Type   |
+---------------+
*/
typedef struct 
{
	//byte 0
	unsigned char TYPE:5;
	unsigned char NRI:2;
	unsigned char F:1;        
} NALU_HEADER; // 1 BYTE 

/*
+---------------+
|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+
|F|NRI|  Type   |
+---------------+
*/
//��NALU_HEADER�Ľṹ��ͬ
typedef struct 
{
	//byte 0
	unsigned char TYPE:5;
	unsigned char NRI:2; 
	unsigned char F:1;              
} FU_INDICATOR; // 1 BYTE 

/*
+---------------+
|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+
|S|E|R|  Type   |
+---------------+
*/
typedef struct 
{
	//byte 0
	unsigned char TYPE:5;
	unsigned char R:1;
	unsigned char E:1;
	unsigned char S:1;          //��Ƭ����һ���� S = 1 ,����= 0 ��    
} FU_HEADER;   // 1 BYTES 



enum VideoFormat {
  VFMT_NONE,
  VFMT_MPEG2,
  VFMT_MPEG4,
  VFMT_H264,
  VFMT_H263,
  VFMT_AVC1,
  VFMT_H265,
};

enum AudioFormat {
  AFMT_NONE,
  AFMT_PCM_RAW16 = 11,
  AFMT_PCM_ULAW,
  AFMT_PCM_ALAW,
  AFMT_AMR,
  AFMT_AAC,
  AFMT_MP2,
  AFMT_MP3
};

enum PackageFormat {
  PFMT_SEPARATE_STREAMS, // separate streams for audio & video
  PFMT_TRANSPORT_STREAM // combine audio and video into a Transport Stream
};


typedef struct
{
	unsigned int syncword;  //12 bslbf ͬ����The bit string ��1111 1111 1111����˵��һ��ADTS֡�Ŀ�ʼ
	unsigned int id;        //1 bslbf   MPEG ��ʾ��, ����Ϊ1
	unsigned int layer;     //2 uimsbf Indicates which layer is used. Set to ��00��
	unsigned int protection_absent;  //1 bslbf  ��ʾ�Ƿ�����У��
	unsigned int profile;            //2 uimsbf  ��ʾʹ���ĸ������AAC����01 Low Complexity(LC)--- AACLC
	unsigned int sf_index;           //4 uimsbf  ��ʾʹ�õĲ������±�
	unsigned int private_bit;        //1 bslbf 
	unsigned int channel_configuration;  //3 uimsbf  ��ʾ������
	unsigned int original;               //1 bslbf 
	unsigned int home;                   //1 bslbf 
	/*�����Ϊ�ı�Ĳ�����ÿһ֡����ͬ*/
	unsigned int copyright_identification_bit;   //1 bslbf 
	unsigned int copyright_identification_start; //1 bslbf
	unsigned int aac_frame_length;               // 13 bslbf  һ��ADTS֡�ĳ��Ȱ���ADTSͷ��raw data block
	unsigned int adts_buffer_fullness;           //11 bslbf     0x7FF ˵�������ʿɱ������

	/*no_raw_data_blocks_in_frame ��ʾADTS֡����number_of_raw_data_blocks_in_frame + 1��AACԭʼ֡.
	����˵number_of_raw_data_blocks_in_frame == 0 
	��ʾ˵ADTS֡����һ��AAC���ݿ鲢����˵û�С�(һ��AACԭʼ֡����һ��ʱ����1024���������������)
    */
	unsigned int no_raw_data_blocks_in_frame;    //2 uimsfb
} ADTS_HEADER;

 //////////////////////////////////////////////////
//H265/HEVC defines
#define RTP_HEVC_PAYLOAD_HEADER_SIZE       2
#define RTP_HEVC_FU_HEADER_SIZE            1
#define RTP_HEVC_DONL_FIELD_SIZE           2
#define RTP_HEVC_DOND_FIELD_SIZE           1
#define RTP_HEVC_AP_NALU_LENGTH_FIELD_SIZE 2
#define HEVC_SPECIFIED_NAL_UNIT_TYPES      48

enum NALUnitType {
    NAL_TRAIL_N    = 0,
    NAL_TRAIL_R    = 1,
    NAL_TSA_N      = 2,
    NAL_TSA_R      = 3,
    NAL_STSA_N     = 4,
    NAL_STSA_R     = 5,
    NAL_RADL_N     = 6,
    NAL_RADL_R     = 7,
    NAL_RASL_N     = 8,
    NAL_RASL_R     = 9,
    NAL_BLA_W_LP   = 16,
    NAL_BLA_W_RADL = 17,
    NAL_BLA_N_LP   = 18,
    NAL_IDR_W_RADL = 19,
    NAL_IDR_N_LP   = 20,
    NAL_CRA_NUT    = 21,
    NAL_VPS        = 32,
    NAL_SPS        = 33,
    NAL_PPS        = 34,
    NAL_AUD        = 35,
    NAL_EOS_NUT    = 36,
    NAL_EOB_NUT    = 37,
    NAL_FD_NUT     = 38,
    NAL_SEI_PREFIX = 39,
    NAL_SEI_SUFFIX = 40,
};

enum RPSType {
    ST_CURR_BEF = 0,
    ST_CURR_AFT,
    ST_FOLL,
    LT_CURR,
    LT_FOLL,
    NB_RPS_TYPE,
};

enum SliceType {
    B_SLICE = 0,
    P_SLICE = 1,
    I_SLICE = 2,
};

#define IS_IDR(nal_unit_type) (nal_unit_type == NAL_IDR_W_RADL || nal_unit_type == NAL_IDR_N_LP)
#define IS_BLA(nal_unit_type) (nal_unit_type == NAL_BLA_W_RADL || nal_unit_type == NAL_BLA_W_LP || \
                   nal_unit_type == NAL_BLA_N_LP)
#define IS_IRAP(nal_unit_type) (nal_unit_type >= 16 && nal_unit_type <= 23)


struct vc_params_t
{
	LONG width,height;
	DWORD profile, level;
	DWORD nal_length_size;
	void clear()
	{
		memset(this, 0, sizeof(*this));
	}
};

class NALBitstream
{
public:
	NALBitstream() : m_data(NULL), m_len(0), m_idx(0), m_bits(0), m_byte(0), m_zeros(0) {};
	NALBitstream(void * data, int len) { Init(data, len); };
	void Init(void * data, int len) { m_data = (LPBYTE)data; m_len = len; m_idx = 0; m_bits = 0; m_byte = 0; m_zeros = 0; };


	BYTE GetBYTE()
	{
		if ( m_idx >= m_len )
			return 0;
		BYTE b = m_data[m_idx++];

		// to avoid start-code emulation, a byte 0x03 is inserted
		// after any 00 00 pair. Discard that here.
		if ( b == 0 )
		{
			m_zeros++;
			if ( (m_idx < m_len) && (m_zeros == 2) && (m_data[m_idx] == 0x03) )
			{

				m_idx++;
				m_zeros=0;
			}
		} 

		else 
		{
			m_zeros = 0;

		}
		return b;
	};


	UINT32 GetBit() 
	{

		if (m_bits == 0) 
		{
			m_byte = GetBYTE();
			m_bits = 8;

		}
		m_bits--;
		return (m_byte >> m_bits) & 0x1;
	};

	UINT32 GetWord(int bits) 
	{

		UINT32 u = 0;
		while ( bits > 0 )

		{
			u <<= 1;
			u |= GetBit();
			bits--;
		}
		return u;
	};
	UINT32 GetUE() 
	{

		// Exp-Golomb entropy coding: leading zeros, then a one, then
		// the data bits. The number of leading zeros is the number of
		// data bits, counting up from that number of 1s as the base.
		// That is, if you see
		//      0001010
		// You have three leading zeros, so there are three data bits (010)
		// counting up from a base of 111: thus 111 + 010 = 1001 = 9
		int zeros = 0;
		while (m_idx < m_len && GetBit() == 0 ) zeros++;
		return GetWord(zeros) + ((1 << zeros) - 1);
	};


	INT32 GetSE()
	{

		// same as UE but signed.
		// basically the unsigned numbers are used as codes to indicate signed numbers in pairs
		// in increasing value. Thus the encoded values
		//      0, 1, 2, 3, 4
		// mean
		//      0, 1, -1, 2, -2 etc
		UINT32 UE = GetUE();
		bool positive = UE & 1;
		INT32 SE = (UE + 1) >> 1;
		if ( !positive )
		{
			SE = -SE;
		}
		return SE;
	};


private:
	LPBYTE m_data;
	int m_len;
	int m_idx;
	int m_bits;
	BYTE m_byte;
	int m_zeros;
};


/* SDP out-of-band signaling data */
struct PayloadContext {
    int using_donl_field;
    int profile_id;
    uint8_t *sps, *pps, *vps, *sei;
    int sps_size, pps_size, vps_size, sei_size;
};

extern int h264_handle_packet(const uint8_t *buf, int len, int & nIDR, uint8_t * outbuf, int & outlen);

extern  int ff_h264_handle_aggregated_packet( int & nIDR, uint8_t * outbuf, int & outlen,
                                     const uint8_t *buf, int len,
                                     int skip_between, int *nal_counters,
                                     int nal_mask);
extern  int ff_h264_handle_frag_packet(int & nIDR, uint8_t * outbuf, int & outlen,
							   const uint8_t *buf, int len,
                               int start_bit, const uint8_t *nal_header,
                               int nal_header_len);
extern int h264_handle_packet_fu_a(int & nIDR, uint8_t * outbuf, int & outlen,
                                   const uint8_t *buf, int len,
                                   int *nal_counters, int nal_mask);
extern int hevc_handle_packet(const uint8_t *buf, int len, int & nIDR, uint8_t * outbuf, int & outlen);

extern bool  ParseSequenceParameterSet(BYTE* data,int size, vc_params_t& params);