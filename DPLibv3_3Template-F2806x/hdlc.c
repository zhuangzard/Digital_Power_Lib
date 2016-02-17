/**********************************************************************
 *
 * Filename:    hdmc.c
 *
 * Description: hdmc encoding.
 *
 * Notes:       The parameters for each supported CRC standard are
 *				defined in the header file crc.h.  The implementations
 *				here should stand up to further additions to that list.
 *
 *
 * Copyright (c) 2015 by Yun Lin.
 **********************************************************************/

//#include <stdio.h>
//#include <string.h>

#include "crc.h"
#include "hdlc.h"



int HDLC_encode(const HDLC_pck *decode_pck, HDLC_pck *encode_pck)
{
	int i;
	alt_u8 decode_len = decode_pck->len;
	alt_u8 current_encode_pos = 0;	//store the first data byte to pos 0
	alt_u16 crc;	//store crc code
	if ( decode_len > HDLC_PKT_MAXLEN )
	{
		return -1; //ERROR: packet too large!!!
	}

	encode_pck->pck[current_encode_pos++] = HDLC_START_FLAG;	//assign start flag

	for(i = 0; i < decode_len; i++)
	{
		if(decode_pck->pck[i] == HDLC_ESCAPE_BYTE \
				|| decode_pck->pck[i] ==  HDLC_START_FLAG\
				|| decode_pck->pck[i] ==HDLC_END_FLAG)
		{
			encode_pck->pck[current_encode_pos++] = HDLC_ESCAPE_BYTE;
			encode_pck->pck[current_encode_pos++] = decode_pck->pck[i] ^ HDLC_XOR_BYTE;
		}
		else
		{
			encode_pck->pck[current_encode_pos++] = decode_pck->pck[i];
		}
	}

	//Compute crc code, 2 bytes
	crc = crcFast(decode_pck->pck, decode_len);	//computer
    alt_u8 crc_msb = (alt_u8)((crc & 0xFF00)>>8);
    alt_u8 crc_lsb = (alt_u8)((crc & 0x00FF));

    if(crc_msb == HDLC_ESCAPE_BYTE \
            || crc_msb ==  HDLC_START_FLAG\
            || crc_msb ==HDLC_END_FLAG)
    {
        encode_pck->pck[current_encode_pos++] = HDLC_ESCAPE_BYTE;
        encode_pck->pck[current_encode_pos++] = crc_msb ^ HDLC_XOR_BYTE;
    }
    else
    {
        encode_pck->pck[current_encode_pos++] = crc_msb;
    }

    if(crc_lsb == HDLC_ESCAPE_BYTE \
            || crc_lsb ==  HDLC_START_FLAG\
            || crc_lsb ==HDLC_END_FLAG)
    {
        encode_pck->pck[current_encode_pos++] = HDLC_ESCAPE_BYTE;
        encode_pck->pck[current_encode_pos++] = crc_lsb ^ HDLC_XOR_BYTE;
    }
    else
    {
        encode_pck->pck[current_encode_pos++] = crc_lsb;
    }

	encode_pck->pck[current_encode_pos++] = HDLC_END_FLAG;	//assign end flag

	encode_pck->len = current_encode_pos;


	if ( encode_pck->len > HDLC_PKT_MAXLEN )
	{
		return -1; //ERROR: packet too large!!!
	}

	return 1;
}


void assignSerial2Dsp(const HDLC_pck *serial_data, dsp_data *ToDsp_data)
{
            ToDsp_data->len = serial_data->len;
            ToDsp_data->data0 = serial_data->  pck [ 0 ] ;
            ToDsp_data->data1 = serial_data->  pck [ 1 ] ;
            ToDsp_data->data2 = serial_data->  pck [ 2 ] ;
            ToDsp_data->data3 = serial_data->  pck [ 3 ] ;
            ToDsp_data->data4 = serial_data->  pck [ 4 ] ;
            ToDsp_data->data5 = serial_data->  pck [ 5 ] ;
            ToDsp_data->data6 = serial_data->  pck [ 6 ] ;
            ToDsp_data->data7 = serial_data->  pck [ 7 ] ;
            ToDsp_data->data8 = serial_data->  pck [ 8 ] ;
            ToDsp_data->data9 = serial_data->  pck [ 9 ] ;
            ToDsp_data->data10 = serial_data->  pck [ 10 ] ;
            ToDsp_data->data11 = serial_data->  pck [ 11 ] ;
            ToDsp_data->data12 = serial_data->  pck [ 12 ] ;
            ToDsp_data->data13 = serial_data->  pck [ 13 ] ;
            ToDsp_data->data14 = serial_data->  pck [ 14 ] ;
            ToDsp_data->data15 = serial_data->  pck [ 15 ] ;
            ToDsp_data->data16 = serial_data->  pck [ 16 ] ;
            ToDsp_data->data17 = serial_data->  pck [ 17 ] ;
            ToDsp_data->data18 = serial_data->  pck [ 18 ] ;
            ToDsp_data->data19 = serial_data->  pck [ 19 ] ;
            ToDsp_data->data20 = serial_data->  pck [ 20 ] ;
            ToDsp_data->data21 = serial_data->  pck [ 21 ] ;
            ToDsp_data->data22 = serial_data->  pck [ 22 ] ;
            ToDsp_data->data23 = serial_data->  pck [ 23 ] ;
            ToDsp_data->data24 = serial_data->  pck [ 24 ] ;
            ToDsp_data->data25 = serial_data->  pck [ 25 ] ;
            ToDsp_data->data26 = serial_data->  pck [ 26 ] ;
            ToDsp_data->data27 = serial_data->  pck [ 27 ] ;
            ToDsp_data->data28 = serial_data->  pck [ 28 ] ;
            ToDsp_data->data29 = serial_data->  pck [ 29 ] ;
            ToDsp_data->data30 = serial_data->  pck [ 30 ] ;
            ToDsp_data->data31 = serial_data->  pck [ 31 ] ;
            ToDsp_data->data32 = serial_data->  pck [ 32 ] ;
            ToDsp_data->data33 = serial_data->  pck [ 33 ] ;
            ToDsp_data->data34 = serial_data->  pck [ 34 ] ;
            ToDsp_data->data35 = serial_data->  pck [ 35 ] ;
            ToDsp_data->data36 = serial_data->  pck [ 36 ] ;
            ToDsp_data->data37 = serial_data->  pck [ 37 ] ;
            ToDsp_data->data38 = serial_data->  pck [ 38 ] ;
            ToDsp_data->data39 = serial_data->  pck [ 39 ] ;
            ToDsp_data->data40 = serial_data->  pck [ 40 ] ;
            ToDsp_data->data41 = serial_data->  pck [ 41 ] ;
            ToDsp_data->data42 = serial_data->  pck [ 42 ] ;
            ToDsp_data->data43 = serial_data->  pck [ 43 ] ;
            ToDsp_data->data44 = serial_data->  pck [ 44 ] ;
            ToDsp_data->data45 = serial_data->  pck [ 45 ] ;
            ToDsp_data->data46 = serial_data->  pck [ 46 ] ;
            ToDsp_data->data47 = serial_data->  pck [ 47 ] ;
            ToDsp_data->data48 = serial_data->  pck [ 48 ] ;
            ToDsp_data->data49 = serial_data->  pck [ 49 ] ;
            ToDsp_data->data50 = serial_data->  pck [ 50 ] ;
            ToDsp_data->data51 = serial_data->  pck [ 51 ] ;
            ToDsp_data->data52 = serial_data->  pck [ 52 ] ;
            ToDsp_data->data53 = serial_data->  pck [ 53 ] ;
            ToDsp_data->data54 = serial_data->  pck [ 54 ] ;
            ToDsp_data->data55 = serial_data->  pck [ 55 ] ;
            ToDsp_data->data56 = serial_data->  pck [ 56 ] ;
            ToDsp_data->data57 = serial_data->  pck [ 57 ] ;
            ToDsp_data->data58 = serial_data->  pck [ 58 ] ;
            ToDsp_data->data59 = serial_data->  pck [ 59 ] ;
            ToDsp_data->data60 = serial_data->  pck [ 60 ] ;
            ToDsp_data->data61 = serial_data->  pck [ 61 ] ;
            ToDsp_data->data62 = serial_data->  pck [ 62 ] ;
            ToDsp_data->data63 = serial_data->  pck [ 63 ] ;
            ToDsp_data->data64 = serial_data->  pck [ 64 ] ;
            ToDsp_data->data65 = serial_data->  pck [ 65 ] ;
            ToDsp_data->data66 = serial_data->  pck [ 66 ] ;
            ToDsp_data->data67 = serial_data->  pck [ 67 ] ;
            ToDsp_data->data68 = serial_data->  pck [ 68 ] ;
            ToDsp_data->data69 = serial_data->  pck [ 69 ] ;
            ToDsp_data->data70 = serial_data->  pck [ 70 ] ;
            ToDsp_data->data71 = serial_data->  pck [ 71 ] ;
            ToDsp_data->data72 = serial_data->  pck [ 72 ] ;
            ToDsp_data->data73 = serial_data->  pck [ 73 ] ;
            ToDsp_data->data74 = serial_data->  pck [ 74 ] ;
            ToDsp_data->data75 = serial_data->  pck [ 75 ] ;
            ToDsp_data->data76 = serial_data->  pck [ 76 ] ;
            ToDsp_data->data77 = serial_data->  pck [ 77 ] ;
            ToDsp_data->data78 = serial_data->  pck [ 78 ] ;
            ToDsp_data->data79 = serial_data->  pck [ 79 ] ;
            ToDsp_data->data80 = serial_data->  pck [ 80 ] ;
            ToDsp_data->data81 = serial_data->  pck [ 81 ] ;
            ToDsp_data->data82 = serial_data->  pck [ 82 ] ;
            ToDsp_data->data83 = serial_data->  pck [ 83 ] ;
            ToDsp_data->data84 = serial_data->  pck [ 84 ] ;
            ToDsp_data->data85 = serial_data->  pck [ 85 ] ;
            ToDsp_data->data86 = serial_data->  pck [ 86 ] ;
            ToDsp_data->data87 = serial_data->  pck [ 87 ] ;
            ToDsp_data->data88 = serial_data->  pck [ 88 ] ;
            ToDsp_data->data89 = serial_data->  pck [ 89 ] ;
            ToDsp_data->data90 = serial_data->  pck [ 90 ] ;
            ToDsp_data->data91 = serial_data->  pck [ 91 ] ;
            ToDsp_data->data92 = serial_data->  pck [ 92 ] ;
            ToDsp_data->data93 = serial_data->  pck [ 93 ] ;
            ToDsp_data->data94 = serial_data->  pck [ 94 ] ;
            ToDsp_data->data95 = serial_data->  pck [ 95 ] ;
            ToDsp_data->data96 = serial_data->  pck [ 96 ] ;
            ToDsp_data->data97 = serial_data->  pck [ 97 ] ;
            ToDsp_data->data98 = serial_data->  pck [ 98 ] ;
            ToDsp_data->data99 = serial_data->  pck [ 99 ] ;
            ToDsp_data->data100 = serial_data->  pck [ 100 ] ;
            ToDsp_data->data101 = serial_data->  pck [ 101 ] ;
            ToDsp_data->data102 = serial_data->  pck [ 102 ] ;
            ToDsp_data->data103 = serial_data->  pck [ 103 ] ;
            ToDsp_data->data104 = serial_data->  pck [ 104 ] ;
            ToDsp_data->data105 = serial_data->  pck [ 105 ] ;
            ToDsp_data->data106 = serial_data->  pck [ 106 ] ;
            ToDsp_data->data107 = serial_data->  pck [ 107 ] ;
            ToDsp_data->data108 = serial_data->  pck [ 108 ] ;
            ToDsp_data->data109 = serial_data->  pck [ 109 ] ;
            ToDsp_data->data110 = serial_data->  pck [ 110 ] ;
            ToDsp_data->data111 = serial_data->  pck [ 111 ] ;
            ToDsp_data->data112 = serial_data->  pck [ 112 ] ;
            ToDsp_data->data113 = serial_data->  pck [ 113 ] ;
            ToDsp_data->data114 = serial_data->  pck [ 114 ] ;
            ToDsp_data->data115 = serial_data->  pck [ 115 ] ;
            ToDsp_data->data116 = serial_data->  pck [ 116 ] ;
            ToDsp_data->data117 = serial_data->  pck [ 117 ] ;
            ToDsp_data->data118 = serial_data->  pck [ 118 ] ;
            ToDsp_data->data119 = serial_data->  pck [ 119 ] ;
            ToDsp_data->data120 = serial_data->  pck [ 120 ] ;
            ToDsp_data->data121 = serial_data->  pck [ 121 ] ;
            ToDsp_data->data122 = serial_data->  pck [ 122 ] ;
            ToDsp_data->data123 = serial_data->  pck [ 123 ] ;
            ToDsp_data->data124 = serial_data->  pck [ 124 ] ;
            ToDsp_data->data125 = serial_data->  pck [ 125 ] ;
            ToDsp_data->data126 = serial_data->  pck [ 126 ] ;
            ToDsp_data->data127 = serial_data->  pck [ 127 ] ;
            ToDsp_data->data128 = serial_data->  pck [ 128 ] ;
            ToDsp_data->data129 = serial_data->  pck [ 129 ] ;
            ToDsp_data->data130 = serial_data->  pck [ 130 ] ;
            ToDsp_data->data131 = serial_data->  pck [ 131 ] ;
            ToDsp_data->data132 = serial_data->  pck [ 132 ] ;
            ToDsp_data->data133 = serial_data->  pck [ 133 ] ;
            ToDsp_data->data134 = serial_data->  pck [ 134 ] ;
            ToDsp_data->data135 = serial_data->  pck [ 135 ] ;
            ToDsp_data->data136 = serial_data->  pck [ 136 ] ;
            ToDsp_data->data137 = serial_data->  pck [ 137 ] ;
            ToDsp_data->data138 = serial_data->  pck [ 138 ] ;
            ToDsp_data->data139 = serial_data->  pck [ 139 ] ;
            ToDsp_data->data140 = serial_data->  pck [ 140 ] ;
            ToDsp_data->data141 = serial_data->  pck [ 141 ] ;
            ToDsp_data->data142 = serial_data->  pck [ 142 ] ;
            ToDsp_data->data143 = serial_data->  pck [ 143 ] ;
            ToDsp_data->data144 = serial_data->  pck [ 144 ] ;
            ToDsp_data->data145 = serial_data->  pck [ 145 ] ;
            ToDsp_data->data146 = serial_data->  pck [ 146 ] ;
            ToDsp_data->data147 = serial_data->  pck [ 147 ] ;
            ToDsp_data->data148 = serial_data->  pck [ 148 ] ;
            ToDsp_data->data149 = serial_data->  pck [ 149 ] ;
            ToDsp_data->data150 = serial_data->  pck [ 150 ] ;
            ToDsp_data->data151 = serial_data->  pck [ 151 ] ;
            ToDsp_data->data152 = serial_data->  pck [ 152 ] ;
            ToDsp_data->data153 = serial_data->  pck [ 153 ] ;
            ToDsp_data->data154 = serial_data->  pck [ 154 ] ;
            ToDsp_data->data155 = serial_data->  pck [ 155 ] ;
            ToDsp_data->data156 = serial_data->  pck [ 156 ] ;
            ToDsp_data->data157 = serial_data->  pck [ 157 ] ;
            ToDsp_data->data158 = serial_data->  pck [ 158 ] ;
            ToDsp_data->data159 = serial_data->  pck [ 159 ] ;
            ToDsp_data->data160 = serial_data->  pck [ 160 ] ;
            ToDsp_data->data161 = serial_data->  pck [ 161 ] ;
            ToDsp_data->data162 = serial_data->  pck [ 162 ] ;
            ToDsp_data->data163 = serial_data->  pck [ 163 ] ;
            ToDsp_data->data164 = serial_data->  pck [ 164 ] ;
            ToDsp_data->data165 = serial_data->  pck [ 165 ] ;
            ToDsp_data->data166 = serial_data->  pck [ 166 ] ;
            ToDsp_data->data167 = serial_data->  pck [ 167 ] ;
            ToDsp_data->data168 = serial_data->  pck [ 168 ] ;
            ToDsp_data->data169 = serial_data->  pck [ 169 ] ;
            ToDsp_data->data170 = serial_data->  pck [ 170 ] ;
            ToDsp_data->data171 = serial_data->  pck [ 171 ] ;
            ToDsp_data->data172 = serial_data->  pck [ 172 ] ;
            ToDsp_data->data173 = serial_data->  pck [ 173 ] ;
            ToDsp_data->data174 = serial_data->  pck [ 174 ] ;
            ToDsp_data->data175 = serial_data->  pck [ 175 ] ;
            ToDsp_data->data176 = serial_data->  pck [ 176 ] ;
            ToDsp_data->data177 = serial_data->  pck [ 177 ] ;
            ToDsp_data->data178 = serial_data->  pck [ 178 ] ;
            ToDsp_data->data179 = serial_data->  pck [ 179 ] ;
            ToDsp_data->data180 = serial_data->  pck [ 180 ] ;
            ToDsp_data->data181 = serial_data->  pck [ 181 ] ;
            ToDsp_data->data182 = serial_data->  pck [ 182 ] ;
            ToDsp_data->data183 = serial_data->  pck [ 183 ] ;
            ToDsp_data->data184 = serial_data->  pck [ 184 ] ;
            ToDsp_data->data185 = serial_data->  pck [ 185 ] ;
            ToDsp_data->data186 = serial_data->  pck [ 186 ] ;
            ToDsp_data->data187 = serial_data->  pck [ 187 ] ;
            ToDsp_data->data188 = serial_data->  pck [ 188 ] ;
            ToDsp_data->data189 = serial_data->  pck [ 189 ] ;
            ToDsp_data->data190 = serial_data->  pck [ 190 ] ;
            ToDsp_data->data191 = serial_data->  pck [ 191 ] ;
            ToDsp_data->data192 = serial_data->  pck [ 192 ] ;
            ToDsp_data->data193 = serial_data->  pck [ 193 ] ;
            ToDsp_data->data194 = serial_data->  pck [ 194 ] ;
            ToDsp_data->data195 = serial_data->  pck [ 195 ] ;
            ToDsp_data->data196 = serial_data->  pck [ 196 ] ;
            ToDsp_data->data197 = serial_data->  pck [ 197 ] ;
            ToDsp_data->data198 = serial_data->  pck [ 198 ] ;
            ToDsp_data->data199 = serial_data->  pck [ 199 ] ;
            ToDsp_data->data200 = serial_data->  pck [ 200 ] ;
            ToDsp_data->data201 = serial_data->  pck [ 201 ] ;
            ToDsp_data->data202 = serial_data->  pck [ 202 ] ;
            ToDsp_data->data203 = serial_data->  pck [ 203 ] ;
            ToDsp_data->data204 = serial_data->  pck [ 204 ] ;
            ToDsp_data->data205 = serial_data->  pck [ 205 ] ;
            ToDsp_data->data206 = serial_data->  pck [ 206 ] ;
            ToDsp_data->data207 = serial_data->  pck [ 207 ] ;
            ToDsp_data->data208 = serial_data->  pck [ 208 ] ;
            ToDsp_data->data209 = serial_data->  pck [ 209 ] ;
            ToDsp_data->data210 = serial_data->  pck [ 210 ] ;
            ToDsp_data->data211 = serial_data->  pck [ 211 ] ;
            ToDsp_data->data212 = serial_data->  pck [ 212 ] ;
            ToDsp_data->data213 = serial_data->  pck [ 213 ] ;
            ToDsp_data->data214 = serial_data->  pck [ 214 ] ;
            ToDsp_data->data215 = serial_data->  pck [ 215 ] ;
            ToDsp_data->data216 = serial_data->  pck [ 216 ] ;
            ToDsp_data->data217 = serial_data->  pck [ 217 ] ;
            ToDsp_data->data218 = serial_data->  pck [ 218 ] ;
            ToDsp_data->data219 = serial_data->  pck [ 219 ] ;
            ToDsp_data->data220 = serial_data->  pck [ 220 ] ;
            ToDsp_data->data221 = serial_data->  pck [ 221 ] ;
            ToDsp_data->data222 = serial_data->  pck [ 222 ] ;
            ToDsp_data->data223 = serial_data->  pck [ 223 ] ;
            ToDsp_data->data224 = serial_data->  pck [ 224 ] ;
            ToDsp_data->data225 = serial_data->  pck [ 225 ] ;
            ToDsp_data->data226 = serial_data->  pck [ 226 ] ;
            ToDsp_data->data227 = serial_data->  pck [ 227 ] ;
            ToDsp_data->data228 = serial_data->  pck [ 228 ] ;
            ToDsp_data->data229 = serial_data->  pck [ 229 ] ;
            ToDsp_data->data230 = serial_data->  pck [ 230 ] ;
            ToDsp_data->data231 = serial_data->  pck [ 231 ] ;
            ToDsp_data->data232 = serial_data->  pck [ 232 ] ;
            ToDsp_data->data233 = serial_data->  pck [ 233 ] ;
            ToDsp_data->data234 = serial_data->  pck [ 234 ] ;
            ToDsp_data->data235 = serial_data->  pck [ 235 ] ;
            ToDsp_data->data236 = serial_data->  pck [ 236 ] ;
            ToDsp_data->data237 = serial_data->  pck [ 237 ] ;
            ToDsp_data->data238 = serial_data->  pck [ 238 ] ;
            ToDsp_data->data239 = serial_data->  pck [ 239 ] ;
            ToDsp_data->data240 = serial_data->  pck [ 240 ] ;
            ToDsp_data->data241 = serial_data->  pck [ 241 ] ;
            ToDsp_data->data242 = serial_data->  pck [ 242 ] ;
            ToDsp_data->data243 = serial_data->  pck [ 243 ] ;
            ToDsp_data->data244 = serial_data->  pck [ 244 ] ;
            ToDsp_data->data245 = serial_data->  pck [ 245 ] ;
            ToDsp_data->data246 = serial_data->  pck [ 246 ] ;
            ToDsp_data->data247 = serial_data->  pck [ 247 ] ;
            ToDsp_data->data248 = serial_data->  pck [ 248 ] ;
            ToDsp_data->data249 = serial_data->  pck [ 249 ] ;
            ToDsp_data->data250 = serial_data->  pck [ 250 ] ;
            ToDsp_data->data251 = serial_data->  pck [ 251 ] ;
            ToDsp_data->data252 = serial_data->  pck [ 252 ] ;
            ToDsp_data->data253 = serial_data->  pck [ 253 ] ;
            ToDsp_data->data254 = serial_data->  pck [ 254 ] ;
            ToDsp_data->data255 = serial_data->  pck [ 255 ] ;

}


void assignDsp2Serial(const  dsp_data *FromDsp_data, HDLC_pck *serial_data)
{
    serial_data->len = FromDsp_data->len;
    serial_data->  pck [ 0 ]  =  FromDsp_data->data0  ;
    serial_data->  pck [ 1 ]  =  FromDsp_data->data1  ;
    serial_data->  pck [ 2 ]  =  FromDsp_data->data2  ;
    serial_data->  pck [ 3 ]  =  FromDsp_data->data3  ;
    serial_data->  pck [ 4 ]  =  FromDsp_data->data4  ;
    serial_data->  pck [ 5 ]  =  FromDsp_data->data5  ;
    serial_data->  pck [ 6 ]  =  FromDsp_data->data6  ;
    serial_data->  pck [ 7 ]  =  FromDsp_data->data7  ;
    serial_data->  pck [ 8 ]  =  FromDsp_data->data8  ;
    serial_data->  pck [ 9 ]  =  FromDsp_data->data9  ;
    serial_data->  pck [ 10 ]  =  FromDsp_data->data10  ;
    serial_data->  pck [ 11 ]  =  FromDsp_data->data11  ;
    serial_data->  pck [ 12 ]  =  FromDsp_data->data12  ;
    serial_data->  pck [ 13 ]  =  FromDsp_data->data13  ;
    serial_data->  pck [ 14 ]  =  FromDsp_data->data14  ;
    serial_data->  pck [ 15 ]  =  FromDsp_data->data15  ;
    serial_data->  pck [ 16 ]  =  FromDsp_data->data16  ;
    serial_data->  pck [ 17 ]  =  FromDsp_data->data17  ;
    serial_data->  pck [ 18 ]  =  FromDsp_data->data18  ;
    serial_data->  pck [ 19 ]  =  FromDsp_data->data19  ;
    serial_data->  pck [ 20 ]  =  FromDsp_data->data20  ;
    serial_data->  pck [ 21 ]  =  FromDsp_data->data21  ;
    serial_data->  pck [ 22 ]  =  FromDsp_data->data22  ;
    serial_data->  pck [ 23 ]  =  FromDsp_data->data23  ;
    serial_data->  pck [ 24 ]  =  FromDsp_data->data24  ;
    serial_data->  pck [ 25 ]  =  FromDsp_data->data25  ;
    serial_data->  pck [ 26 ]  =  FromDsp_data->data26  ;
    serial_data->  pck [ 27 ]  =  FromDsp_data->data27  ;
    serial_data->  pck [ 28 ]  =  FromDsp_data->data28  ;
    serial_data->  pck [ 29 ]  =  FromDsp_data->data29  ;
    serial_data->  pck [ 30 ]  =  FromDsp_data->data30  ;
    serial_data->  pck [ 31 ]  =  FromDsp_data->data31  ;
    serial_data->  pck [ 32 ]  =  FromDsp_data->data32  ;
    serial_data->  pck [ 33 ]  =  FromDsp_data->data33  ;
    serial_data->  pck [ 34 ]  =  FromDsp_data->data34  ;
    serial_data->  pck [ 35 ]  =  FromDsp_data->data35  ;
    serial_data->  pck [ 36 ]  =  FromDsp_data->data36  ;
    serial_data->  pck [ 37 ]  =  FromDsp_data->data37  ;
    serial_data->  pck [ 38 ]  =  FromDsp_data->data38  ;
    serial_data->  pck [ 39 ]  =  FromDsp_data->data39  ;
    serial_data->  pck [ 40 ]  =  FromDsp_data->data40  ;
    serial_data->  pck [ 41 ]  =  FromDsp_data->data41  ;
    serial_data->  pck [ 42 ]  =  FromDsp_data->data42  ;
    serial_data->  pck [ 43 ]  =  FromDsp_data->data43  ;
    serial_data->  pck [ 44 ]  =  FromDsp_data->data44  ;
    serial_data->  pck [ 45 ]  =  FromDsp_data->data45  ;
    serial_data->  pck [ 46 ]  =  FromDsp_data->data46  ;
    serial_data->  pck [ 47 ]  =  FromDsp_data->data47  ;
    serial_data->  pck [ 48 ]  =  FromDsp_data->data48  ;
    serial_data->  pck [ 49 ]  =  FromDsp_data->data49  ;
    serial_data->  pck [ 50 ]  =  FromDsp_data->data50  ;
    serial_data->  pck [ 51 ]  =  FromDsp_data->data51  ;
    serial_data->  pck [ 52 ]  =  FromDsp_data->data52  ;
    serial_data->  pck [ 53 ]  =  FromDsp_data->data53  ;
    serial_data->  pck [ 54 ]  =  FromDsp_data->data54  ;
    serial_data->  pck [ 55 ]  =  FromDsp_data->data55  ;
    serial_data->  pck [ 56 ]  =  FromDsp_data->data56  ;
    serial_data->  pck [ 57 ]  =  FromDsp_data->data57  ;
    serial_data->  pck [ 58 ]  =  FromDsp_data->data58  ;
    serial_data->  pck [ 59 ]  =  FromDsp_data->data59  ;
    serial_data->  pck [ 60 ]  =  FromDsp_data->data60  ;
    serial_data->  pck [ 61 ]  =  FromDsp_data->data61  ;
    serial_data->  pck [ 62 ]  =  FromDsp_data->data62  ;
    serial_data->  pck [ 63 ]  =  FromDsp_data->data63  ;
    serial_data->  pck [ 64 ]  =  FromDsp_data->data64  ;
    serial_data->  pck [ 65 ]  =  FromDsp_data->data65  ;
    serial_data->  pck [ 66 ]  =  FromDsp_data->data66  ;
    serial_data->  pck [ 67 ]  =  FromDsp_data->data67  ;
    serial_data->  pck [ 68 ]  =  FromDsp_data->data68  ;
    serial_data->  pck [ 69 ]  =  FromDsp_data->data69  ;
    serial_data->  pck [ 70 ]  =  FromDsp_data->data70  ;
    serial_data->  pck [ 71 ]  =  FromDsp_data->data71  ;
    serial_data->  pck [ 72 ]  =  FromDsp_data->data72  ;
    serial_data->  pck [ 73 ]  =  FromDsp_data->data73  ;
    serial_data->  pck [ 74 ]  =  FromDsp_data->data74  ;
    serial_data->  pck [ 75 ]  =  FromDsp_data->data75  ;
    serial_data->  pck [ 76 ]  =  FromDsp_data->data76  ;
    serial_data->  pck [ 77 ]  =  FromDsp_data->data77  ;
    serial_data->  pck [ 78 ]  =  FromDsp_data->data78  ;
    serial_data->  pck [ 79 ]  =  FromDsp_data->data79  ;
    serial_data->  pck [ 80 ]  =  FromDsp_data->data80  ;
    serial_data->  pck [ 81 ]  =  FromDsp_data->data81  ;
    serial_data->  pck [ 82 ]  =  FromDsp_data->data82  ;
    serial_data->  pck [ 83 ]  =  FromDsp_data->data83  ;
    serial_data->  pck [ 84 ]  =  FromDsp_data->data84  ;
    serial_data->  pck [ 85 ]  =  FromDsp_data->data85  ;
    serial_data->  pck [ 86 ]  =  FromDsp_data->data86  ;
    serial_data->  pck [ 87 ]  =  FromDsp_data->data87  ;
    serial_data->  pck [ 88 ]  =  FromDsp_data->data88  ;
    serial_data->  pck [ 89 ]  =  FromDsp_data->data89  ;
    serial_data->  pck [ 90 ]  =  FromDsp_data->data90  ;
    serial_data->  pck [ 91 ]  =  FromDsp_data->data91  ;
    serial_data->  pck [ 92 ]  =  FromDsp_data->data92  ;
    serial_data->  pck [ 93 ]  =  FromDsp_data->data93  ;
    serial_data->  pck [ 94 ]  =  FromDsp_data->data94  ;
    serial_data->  pck [ 95 ]  =  FromDsp_data->data95  ;
    serial_data->  pck [ 96 ]  =  FromDsp_data->data96  ;
    serial_data->  pck [ 97 ]  =  FromDsp_data->data97  ;
    serial_data->  pck [ 98 ]  =  FromDsp_data->data98  ;
    serial_data->  pck [ 99 ]  =  FromDsp_data->data99  ;
    serial_data->  pck [ 100 ]  =  FromDsp_data->data100  ;
    serial_data->  pck [ 101 ]  =  FromDsp_data->data101  ;
    serial_data->  pck [ 102 ]  =  FromDsp_data->data102  ;
    serial_data->  pck [ 103 ]  =  FromDsp_data->data103  ;
    serial_data->  pck [ 104 ]  =  FromDsp_data->data104  ;
    serial_data->  pck [ 105 ]  =  FromDsp_data->data105  ;
    serial_data->  pck [ 106 ]  =  FromDsp_data->data106  ;
    serial_data->  pck [ 107 ]  =  FromDsp_data->data107  ;
    serial_data->  pck [ 108 ]  =  FromDsp_data->data108  ;
    serial_data->  pck [ 109 ]  =  FromDsp_data->data109  ;
    serial_data->  pck [ 110 ]  =  FromDsp_data->data110  ;
    serial_data->  pck [ 111 ]  =  FromDsp_data->data111  ;
    serial_data->  pck [ 112 ]  =  FromDsp_data->data112  ;
    serial_data->  pck [ 113 ]  =  FromDsp_data->data113  ;
    serial_data->  pck [ 114 ]  =  FromDsp_data->data114  ;
    serial_data->  pck [ 115 ]  =  FromDsp_data->data115  ;
    serial_data->  pck [ 116 ]  =  FromDsp_data->data116  ;
    serial_data->  pck [ 117 ]  =  FromDsp_data->data117  ;
    serial_data->  pck [ 118 ]  =  FromDsp_data->data118  ;
    serial_data->  pck [ 119 ]  =  FromDsp_data->data119  ;
    serial_data->  pck [ 120 ]  =  FromDsp_data->data120  ;
    serial_data->  pck [ 121 ]  =  FromDsp_data->data121  ;
    serial_data->  pck [ 122 ]  =  FromDsp_data->data122  ;
    serial_data->  pck [ 123 ]  =  FromDsp_data->data123  ;
    serial_data->  pck [ 124 ]  =  FromDsp_data->data124  ;
    serial_data->  pck [ 125 ]  =  FromDsp_data->data125  ;
    serial_data->  pck [ 126 ]  =  FromDsp_data->data126  ;
    serial_data->  pck [ 127 ]  =  FromDsp_data->data127  ;
    serial_data->  pck [ 128 ]  =  FromDsp_data->data128  ;
    serial_data->  pck [ 129 ]  =  FromDsp_data->data129  ;
    serial_data->  pck [ 130 ]  =  FromDsp_data->data130  ;
    serial_data->  pck [ 131 ]  =  FromDsp_data->data131  ;
    serial_data->  pck [ 132 ]  =  FromDsp_data->data132  ;
    serial_data->  pck [ 133 ]  =  FromDsp_data->data133  ;
    serial_data->  pck [ 134 ]  =  FromDsp_data->data134  ;
    serial_data->  pck [ 135 ]  =  FromDsp_data->data135  ;
    serial_data->  pck [ 136 ]  =  FromDsp_data->data136  ;
    serial_data->  pck [ 137 ]  =  FromDsp_data->data137  ;
    serial_data->  pck [ 138 ]  =  FromDsp_data->data138  ;
    serial_data->  pck [ 139 ]  =  FromDsp_data->data139  ;
    serial_data->  pck [ 140 ]  =  FromDsp_data->data140  ;
    serial_data->  pck [ 141 ]  =  FromDsp_data->data141  ;
    serial_data->  pck [ 142 ]  =  FromDsp_data->data142  ;
    serial_data->  pck [ 143 ]  =  FromDsp_data->data143  ;
    serial_data->  pck [ 144 ]  =  FromDsp_data->data144  ;
    serial_data->  pck [ 145 ]  =  FromDsp_data->data145  ;
    serial_data->  pck [ 146 ]  =  FromDsp_data->data146  ;
    serial_data->  pck [ 147 ]  =  FromDsp_data->data147  ;
    serial_data->  pck [ 148 ]  =  FromDsp_data->data148  ;
    serial_data->  pck [ 149 ]  =  FromDsp_data->data149  ;
    serial_data->  pck [ 150 ]  =  FromDsp_data->data150  ;
    serial_data->  pck [ 151 ]  =  FromDsp_data->data151  ;
    serial_data->  pck [ 152 ]  =  FromDsp_data->data152  ;
    serial_data->  pck [ 153 ]  =  FromDsp_data->data153  ;
    serial_data->  pck [ 154 ]  =  FromDsp_data->data154  ;
    serial_data->  pck [ 155 ]  =  FromDsp_data->data155  ;
    serial_data->  pck [ 156 ]  =  FromDsp_data->data156  ;
    serial_data->  pck [ 157 ]  =  FromDsp_data->data157  ;
    serial_data->  pck [ 158 ]  =  FromDsp_data->data158  ;
    serial_data->  pck [ 159 ]  =  FromDsp_data->data159  ;
    serial_data->  pck [ 160 ]  =  FromDsp_data->data160  ;
    serial_data->  pck [ 161 ]  =  FromDsp_data->data161  ;
    serial_data->  pck [ 162 ]  =  FromDsp_data->data162  ;
    serial_data->  pck [ 163 ]  =  FromDsp_data->data163  ;
    serial_data->  pck [ 164 ]  =  FromDsp_data->data164  ;
    serial_data->  pck [ 165 ]  =  FromDsp_data->data165  ;
    serial_data->  pck [ 166 ]  =  FromDsp_data->data166  ;
    serial_data->  pck [ 167 ]  =  FromDsp_data->data167  ;
    serial_data->  pck [ 168 ]  =  FromDsp_data->data168  ;
    serial_data->  pck [ 169 ]  =  FromDsp_data->data169  ;
    serial_data->  pck [ 170 ]  =  FromDsp_data->data170  ;
    serial_data->  pck [ 171 ]  =  FromDsp_data->data171  ;
    serial_data->  pck [ 172 ]  =  FromDsp_data->data172  ;
    serial_data->  pck [ 173 ]  =  FromDsp_data->data173  ;
    serial_data->  pck [ 174 ]  =  FromDsp_data->data174  ;
    serial_data->  pck [ 175 ]  =  FromDsp_data->data175  ;
    serial_data->  pck [ 176 ]  =  FromDsp_data->data176  ;
    serial_data->  pck [ 177 ]  =  FromDsp_data->data177  ;
    serial_data->  pck [ 178 ]  =  FromDsp_data->data178  ;
    serial_data->  pck [ 179 ]  =  FromDsp_data->data179  ;
    serial_data->  pck [ 180 ]  =  FromDsp_data->data180  ;
    serial_data->  pck [ 181 ]  =  FromDsp_data->data181  ;
    serial_data->  pck [ 182 ]  =  FromDsp_data->data182  ;
    serial_data->  pck [ 183 ]  =  FromDsp_data->data183  ;
    serial_data->  pck [ 184 ]  =  FromDsp_data->data184  ;
    serial_data->  pck [ 185 ]  =  FromDsp_data->data185  ;
    serial_data->  pck [ 186 ]  =  FromDsp_data->data186  ;
    serial_data->  pck [ 187 ]  =  FromDsp_data->data187  ;
    serial_data->  pck [ 188 ]  =  FromDsp_data->data188  ;
    serial_data->  pck [ 189 ]  =  FromDsp_data->data189  ;
    serial_data->  pck [ 190 ]  =  FromDsp_data->data190  ;
    serial_data->  pck [ 191 ]  =  FromDsp_data->data191  ;
    serial_data->  pck [ 192 ]  =  FromDsp_data->data192  ;
    serial_data->  pck [ 193 ]  =  FromDsp_data->data193  ;
    serial_data->  pck [ 194 ]  =  FromDsp_data->data194  ;
    serial_data->  pck [ 195 ]  =  FromDsp_data->data195  ;
    serial_data->  pck [ 196 ]  =  FromDsp_data->data196  ;
    serial_data->  pck [ 197 ]  =  FromDsp_data->data197  ;
    serial_data->  pck [ 198 ]  =  FromDsp_data->data198  ;
    serial_data->  pck [ 199 ]  =  FromDsp_data->data199  ;
    serial_data->  pck [ 200 ]  =  FromDsp_data->data200  ;
    serial_data->  pck [ 201 ]  =  FromDsp_data->data201  ;
    serial_data->  pck [ 202 ]  =  FromDsp_data->data202  ;
    serial_data->  pck [ 203 ]  =  FromDsp_data->data203  ;
    serial_data->  pck [ 204 ]  =  FromDsp_data->data204  ;
    serial_data->  pck [ 205 ]  =  FromDsp_data->data205  ;
    serial_data->  pck [ 206 ]  =  FromDsp_data->data206  ;
    serial_data->  pck [ 207 ]  =  FromDsp_data->data207  ;
    serial_data->  pck [ 208 ]  =  FromDsp_data->data208  ;
    serial_data->  pck [ 209 ]  =  FromDsp_data->data209  ;
    serial_data->  pck [ 210 ]  =  FromDsp_data->data210  ;
    serial_data->  pck [ 211 ]  =  FromDsp_data->data211  ;
    serial_data->  pck [ 212 ]  =  FromDsp_data->data212  ;
    serial_data->  pck [ 213 ]  =  FromDsp_data->data213  ;
    serial_data->  pck [ 214 ]  =  FromDsp_data->data214  ;
    serial_data->  pck [ 215 ]  =  FromDsp_data->data215  ;
    serial_data->  pck [ 216 ]  =  FromDsp_data->data216  ;
    serial_data->  pck [ 217 ]  =  FromDsp_data->data217  ;
    serial_data->  pck [ 218 ]  =  FromDsp_data->data218  ;
    serial_data->  pck [ 219 ]  =  FromDsp_data->data219  ;
    serial_data->  pck [ 220 ]  =  FromDsp_data->data220  ;
    serial_data->  pck [ 221 ]  =  FromDsp_data->data221  ;
    serial_data->  pck [ 222 ]  =  FromDsp_data->data222  ;
    serial_data->  pck [ 223 ]  =  FromDsp_data->data223  ;
    serial_data->  pck [ 224 ]  =  FromDsp_data->data224  ;
    serial_data->  pck [ 225 ]  =  FromDsp_data->data225  ;
    serial_data->  pck [ 226 ]  =  FromDsp_data->data226  ;
    serial_data->  pck [ 227 ]  =  FromDsp_data->data227  ;
    serial_data->  pck [ 228 ]  =  FromDsp_data->data228  ;
    serial_data->  pck [ 229 ]  =  FromDsp_data->data229  ;
    serial_data->  pck [ 230 ]  =  FromDsp_data->data230  ;
    serial_data->  pck [ 231 ]  =  FromDsp_data->data231  ;
    serial_data->  pck [ 232 ]  =  FromDsp_data->data232  ;
    serial_data->  pck [ 233 ]  =  FromDsp_data->data233  ;
    serial_data->  pck [ 234 ]  =  FromDsp_data->data234  ;
    serial_data->  pck [ 235 ]  =  FromDsp_data->data235  ;
    serial_data->  pck [ 236 ]  =  FromDsp_data->data236  ;
    serial_data->  pck [ 237 ]  =  FromDsp_data->data237  ;
    serial_data->  pck [ 238 ]  =  FromDsp_data->data238  ;
    serial_data->  pck [ 239 ]  =  FromDsp_data->data239  ;
    serial_data->  pck [ 240 ]  =  FromDsp_data->data240  ;
    serial_data->  pck [ 241 ]  =  FromDsp_data->data241  ;
    serial_data->  pck [ 242 ]  =  FromDsp_data->data242  ;
    serial_data->  pck [ 243 ]  =  FromDsp_data->data243  ;
    serial_data->  pck [ 244 ]  =  FromDsp_data->data244  ;
    serial_data->  pck [ 245 ]  =  FromDsp_data->data245  ;
    serial_data->  pck [ 246 ]  =  FromDsp_data->data246  ;
    serial_data->  pck [ 247 ]  =  FromDsp_data->data247  ;
    serial_data->  pck [ 248 ]  =  FromDsp_data->data248  ;
    serial_data->  pck [ 249 ]  =  FromDsp_data->data249  ;
    serial_data->  pck [ 250 ]  =  FromDsp_data->data250  ;
    serial_data->  pck [ 251 ]  =  FromDsp_data->data251  ;
    serial_data->  pck [ 252 ]  =  FromDsp_data->data252  ;
    serial_data->  pck [ 253 ]  =  FromDsp_data->data253  ;
    serial_data->  pck [ 254 ]  =  FromDsp_data->data254  ;
    serial_data->  pck [ 255 ]  =  FromDsp_data->data255  ;

}


