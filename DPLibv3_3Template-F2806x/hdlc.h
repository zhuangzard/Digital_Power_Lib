/**********************************************************************
 *
 * Filename:    hdlc.h
 *
 * Description: A header file describing hdmc encoding functions and macros.
 *
 * Notes:
 *
 *
 * Copyright (c) 2015 by Yun Lin.
 **********************************************************************/

#ifndef _hdlc_h
#define _hdlc_h

#include "stdint.h"
#include "inttypes.h"

#define alt_u8 uint8_t
#define alt_u16 uint16_t
#define alt_8 int8_t

#define HDLC_PKT_MAXLEN 		256
#define HDLC_START_FLAG			0x7e
#define HDLC_END_FLAG			0x7c
#define HDLC_ESCAPE_BYTE		0x7d
#define HDLC_XOR_BYTE			0x20	//The flag or escape byte in data XOR with 0x20.

typedef struct HDLC{
	alt_u8 pck[HDLC_PKT_MAXLEN];	//packet
	alt_u8 len;			//length of the packet
}HDLC_pck;

int HDLC_encode(const HDLC_pck *decode_pck, HDLC_pck *encode_pck);

//define the structure of dsp-data
typedef struct {
unsigned char 	len;
unsigned char   data0	;
unsigned char	data1	;
unsigned char	data2	;
unsigned char	data3	;
unsigned char	data4	;
unsigned char	data5	;
unsigned char	data6	;
unsigned char	data7	;
unsigned char	data8	;
unsigned char	data9	;
unsigned char	data10	;
unsigned char	data11	;
unsigned char	data12	;
unsigned char	data13	;
unsigned char	data14	;
unsigned char	data15	;
unsigned char	data16	;
unsigned char	data17	;
unsigned char	data18	;
unsigned char	data19	;
unsigned char	data20	;
unsigned char	data21	;
unsigned char	data22	;
unsigned char	data23	;
unsigned char	data24	;
unsigned char	data25	;
unsigned char	data26	;
unsigned char	data27	;
unsigned char	data28	;
unsigned char	data29	;
unsigned char	data30	;
unsigned char	data31	;
unsigned char	data32	;
unsigned char	data33	;
unsigned char	data34	;
unsigned char	data35	;
unsigned char	data36	;
unsigned char	data37	;
unsigned char	data38	;
unsigned char	data39	;
unsigned char	data40	;
unsigned char	data41	;
unsigned char	data42	;
unsigned char	data43	;
unsigned char	data44	;
unsigned char	data45	;
unsigned char	data46	;
unsigned char	data47	;
unsigned char	data48	;
unsigned char	data49	;
unsigned char	data50	;
unsigned char	data51	;
unsigned char	data52	;
unsigned char	data53	;
unsigned char	data54	;
unsigned char	data55	;
unsigned char	data56	;
unsigned char	data57	;
unsigned char	data58	;
unsigned char	data59	;
unsigned char	data60	;
unsigned char	data61	;
unsigned char	data62	;
unsigned char	data63	;
unsigned char	data64	;
unsigned char	data65	;
unsigned char	data66	;
unsigned char	data67	;
unsigned char	data68	;
unsigned char	data69	;
unsigned char	data70	;
unsigned char	data71	;
unsigned char	data72	;
unsigned char	data73	;
unsigned char	data74	;
unsigned char	data75	;
unsigned char	data76	;
unsigned char	data77	;
unsigned char	data78	;
unsigned char	data79	;
unsigned char	data80	;
unsigned char	data81	;
unsigned char	data82	;
unsigned char	data83	;
unsigned char	data84	;
unsigned char	data85	;
unsigned char	data86	;
unsigned char	data87	;
unsigned char	data88	;
unsigned char	data89	;
unsigned char	data90	;
unsigned char	data91	;
unsigned char	data92	;
unsigned char	data93	;
unsigned char	data94	;
unsigned char	data95	;
unsigned char	data96	;
unsigned char	data97	;
unsigned char	data98	;
unsigned char	data99	;
unsigned char	data100	;
unsigned char	data101	;
unsigned char	data102	;
unsigned char	data103	;
unsigned char	data104	;
unsigned char	data105	;
unsigned char	data106	;
unsigned char	data107	;
unsigned char	data108	;
unsigned char	data109	;
unsigned char	data110	;
unsigned char	data111	;
unsigned char	data112	;
unsigned char	data113	;
unsigned char	data114	;
unsigned char	data115	;
unsigned char	data116	;
unsigned char	data117	;
unsigned char	data118	;
unsigned char	data119	;
unsigned char	data120	;
unsigned char	data121	;
unsigned char	data122	;
unsigned char	data123	;
unsigned char	data124	;
unsigned char	data125	;
unsigned char	data126	;
unsigned char	data127	;
unsigned char	data128	;
unsigned char	data129	;
unsigned char	data130	;
unsigned char	data131	;
unsigned char	data132	;
unsigned char	data133	;
unsigned char	data134	;
unsigned char	data135	;
unsigned char	data136	;
unsigned char	data137	;
unsigned char	data138	;
unsigned char	data139	;
unsigned char	data140	;
unsigned char	data141	;
unsigned char	data142	;
unsigned char	data143	;
unsigned char	data144	;
unsigned char	data145	;
unsigned char	data146	;
unsigned char	data147	;
unsigned char	data148	;
unsigned char	data149	;
unsigned char	data150	;
unsigned char	data151	;
unsigned char	data152	;
unsigned char	data153	;
unsigned char	data154	;
unsigned char	data155	;
unsigned char	data156	;
unsigned char	data157	;
unsigned char	data158	;
unsigned char	data159	;
unsigned char	data160	;
unsigned char	data161	;
unsigned char	data162	;
unsigned char	data163	;
unsigned char	data164	;
unsigned char	data165	;
unsigned char	data166	;
unsigned char	data167	;
unsigned char	data168	;
unsigned char	data169	;
unsigned char	data170	;
unsigned char	data171	;
unsigned char	data172	;
unsigned char	data173	;
unsigned char	data174	;
unsigned char	data175	;
unsigned char	data176	;
unsigned char	data177	;
unsigned char	data178	;
unsigned char	data179	;
unsigned char	data180	;
unsigned char	data181	;
unsigned char	data182	;
unsigned char	data183	;
unsigned char	data184	;
unsigned char	data185	;
unsigned char	data186	;
unsigned char	data187	;
unsigned char	data188	;
unsigned char	data189	;
unsigned char	data190	;
unsigned char	data191	;
unsigned char	data192	;
unsigned char	data193	;
unsigned char	data194	;
unsigned char	data195	;
unsigned char	data196	;
unsigned char	data197	;
unsigned char	data198	;
unsigned char	data199	;
unsigned char	data200	;
unsigned char	data201	;
unsigned char	data202	;
unsigned char	data203	;
unsigned char	data204	;
unsigned char	data205	;
unsigned char	data206	;
unsigned char	data207	;
unsigned char	data208	;
unsigned char	data209	;
unsigned char	data210	;
unsigned char	data211	;
unsigned char	data212	;
unsigned char	data213	;
unsigned char	data214	;
unsigned char	data215	;
unsigned char	data216	;
unsigned char	data217	;
unsigned char	data218	;
unsigned char	data219	;
unsigned char	data220	;
unsigned char	data221	;
unsigned char	data222	;
unsigned char	data223	;
unsigned char	data224	;
unsigned char	data225	;
unsigned char	data226	;
unsigned char	data227	;
unsigned char	data228	;
unsigned char	data229	;
unsigned char	data230	;
unsigned char	data231	;
unsigned char	data232	;
unsigned char	data233	;
unsigned char	data234	;
unsigned char	data235	;
unsigned char	data236	;
unsigned char	data237	;
unsigned char	data238	;
unsigned char	data239	;
unsigned char	data240	;
unsigned char	data241	;
unsigned char	data242	;
unsigned char	data243	;
unsigned char	data244	;
unsigned char	data245	;
unsigned char	data246	;
unsigned char	data247	;
unsigned char	data248	;
unsigned char	data249	;
unsigned char	data250	;
unsigned char	data251	;
unsigned char	data252	;
unsigned char	data253	;
unsigned char	data254	;
unsigned char	data255	;
}dsp_data;

void assignDsp2Serial(const  dsp_data *FromDsp_data, HDLC_pck *ToSerial_data);
void assignSerial2Dsp(const HDLC_pck *ToSerial_data, dsp_data *ToDsp_data);

#endif
