#ifndef FILE_CODE_H
#define FILE_CODE_H
#include <string.h>
/******************************************************************************
*                    MP3 ����
******************************************************************************/
void MP3_Decode(const char* data) {
	int headSize = ((data[6] & 0x7F) << 21)
		+ ((data[7] & 0x7F) << 14)
		+ ((data[8] & 0x7F) << 7)
		+ (data[9] & 0x7F);
	int audioSize = (data[10 + 4] << 24)
		+ (data[10 + 5] << 16)
		+ (data[10 + 6] << 8)
		+ data[10 + 7];
	data += 10 + headSize;
	//����֡
	for (int i = 0; i < audioSize;) {
		unsigned char bitRate = (data[3] & 0xF0) >> 4;
		double sampleRate = (data[3] & 0x0C) >> 2 == 0x00 ? (44.1) : ((data[3] & 0x0C) >> 2 == 0x01 ? 48 : 32);
		unsigned char padding = (data[3] & 0x02) >> 1;
		int size = 144 * bitRate / sampleRate + padding;
		i += size; data += size;
	}
}
/******************************************************************************
*                    JPG ����
******************************************************************************/
void JpgDecode() {

}
/******************************************************************************
*                    IPv4 ����
******************************************************************************/
//IPv4�ײ�(20B+)
struct IPv4Header {
	unsigned char  Ver_HeadLen;	// �汾(4b) + �ײ�����(4b)
	unsigned char  ServiceType;	// ��������(Type of service)(8b)
	unsigned short Length;		// �ܳ�(Total length)(16b)
	unsigned short Identification;//��ʶ(Identification)(16b)
	unsigned short Flag_FragmentOffset;// ��־λ(Flags)(3b) + ��ƫ����(Fragment offset)(13b)
	unsigned char  TTL;			// ���ʱ��(Time to live)(8b)
	unsigned char  Protocol;	// Э��(Protocol)(8b)
	unsigned short CRC;			// �ײ�У���(Header checksum)(16b)
	unsigned int SourAddr;		// Դ��ַ(Source address)(32b)
	unsigned int DestAddr;		// Ŀ�ĵ�ַ(Destination address)(32b)
	unsigned int Option_Padding;// ѡ�������(Option + Padding)(+)
};
IPv4Header IPv4Decode(char* input, char*& data, int& dataLen) {
	IPv4Header head;
	memcpy(&head, input, sizeof(IPv4Header));
	data = input + 20;
	dataLen = head.Length - 20;
}
/******************************************************************************
*                    IPv6 ����
******************************************************************************/
//IPv6�ײ�(40B)
struct IPv6Header {
	unsigned int  Ver_Traffic_Flow;	// �汾(4b) + ͨ������(Traffic Class)(8b) + �����(flow label)(20b)
	unsigned short Length;		// �ܳ�(Total length)(16b)
	unsigned char  NextHead;	// ��һ�ײ�(Next Header)(8b)
	unsigned char  HopLim;		// ��������(Hop Limit)(8b)
	unsigned short SourAddr[8];	// Դ��ַ(Source address)(128b)
	unsigned short DestAddr[8];	// Ŀ�ĵ�ַ(Destination address)(128b)
};
IPv6Header IPv6Decode(char* input, char*& data, int& dataLen) {
	IPv6Header head;
	memcpy(&head, input, sizeof(IPv6Header));
	data = input + 40;
	dataLen = head.Length - 40;
}
/******************************************************************************
*                    UDP ����
******************************************************************************/
//UDP�ײ�(8B)
struct UdpHeader {
	unsigned short SourPort;	// Դ�˿�(Source port)(16b)
	unsigned short DestPort;	// Ŀ�Ķ˿�(Destination port)(16b)
	unsigned short Length;		// UDP���ݰ�����(Datagram length)(16b)
	unsigned short Checksum;	// У���(Checksum)(16b)
};
UdpHeader UdpDecode(char* input, char*& data, int& dataLen) {
	UdpHeader head;
	memcpy(&head, input, sizeof(UdpHeader));
	data = input + 8;
	dataLen = head.Length;
}
/******************************************************************************
*                    TCP ����
******************************************************************************/
//TCP�ײ�(20B+)
struct TCPHeader {
	unsigned short SourPort;	// Դ�˿�(Source port)(16b)
	unsigned short DestPort;	// Ŀ�Ķ˿�(Destination port)(16b)
	unsigned int SeqNum;		// ���(Sequence Number)(32b)
	unsigned int AckNum;		// ȷ�Ϻ�(Acknowlegment Number)(32b)
	unsigned short DataOffset_;	// ����ƫ��_����_URC_ACK_PSH_RST_SYN_FIN(Data offset)(16b)
	unsigned short Window;		// ����(Window)(16b)
	unsigned short CRC;			// У���(Checksum)(16b)
	unsigned short UrgentPtr;	// ����ָ��(Urgent Pointer)(16b)
	unsigned int Option_Padding;// ѡ�������(Option + Padding)(+)
};
TCPHeader TCPDecode(char* input, char*& data, int& dataLen) {
	TCPHeader head;
	memcpy(&head, input, sizeof(TCPHeader));
	data = input + 20;
	dataLen = 1460;
}
#endif