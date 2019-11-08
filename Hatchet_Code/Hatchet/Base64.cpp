// Base64.cpp: ʵ��CBase64��.
//
#include "stdafx.h"
#include "Base64.h"

// ��̬��Ա��ʼ��,7-bit ��ĸ��.
CString CBase64::m_sBase64Alphabet = 
_T( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" );

//---------------------------------------------------------------------------
CBase64::CBase64()
{
}

//---------------------------------------------------------------------------
CBase64::~CBase64()
{
}

//---------------------------------------------------------------------------
CString CBase64::Encode(LPCTSTR szEncoding,int nSize)
{
	CString sOutput = _T( "" );
	int nNumBits;
	UINT nDigit;
	int lp = 0;

	if( szEncoding == NULL )
		return sOutput;

	m_szInput = szEncoding;
	m_nInputSize = nSize;

	m_nBitsRemaining = 0;
	nDigit = ReadBits(&nNumBits, lp );
	while( nNumBits > 0 )
	{
		sOutput += m_sBase64Alphabet[ (int)nDigit ];
		nDigit = ReadBits(&nNumBits, lp );
	}

	while( sOutput.GetLength() % 4 != 0 )
	{
		sOutput += '=';
	}
	return sOutput;
}

//---------------------------------------------------------------------------
// �����������С��С�� 3/4 ���뻺������С. Ϊ�˼��,���ǵĴ�СȡΪһ��.
int CBase64::Decode(LPCTSTR szDecoding, LPTSTR szOutput)
{
    int c, lp =0;
	int nDigit;
    int nDecode[ 256 ];

	ASSERT( szDecoding != NULL );
	ASSERT( szOutput != NULL );
	if( szOutput == NULL )  	
		return 0;
	
	if( szDecoding == NULL )
		return 0;

	CString sInput = szDecoding;
	if( sInput.GetLength() == 0 )
		return 0;

	// ���������.
	int i = 0;
	for( i = 0; i < 256; i++ ) 
		nDecode[i] = -2; // �Ƿ�����.
	for( i=0; i < 64; i++ )
	{
		nDecode[ m_sBase64Alphabet[ i ] ] = i;
		nDecode[ m_sBase64Alphabet[ i ] | 0x80 ] = i; // ����8th bit.
		nDecode[ '=' ] = -1; 
		nDecode[ '=' | 0x80 ] = -1; 
    }

	// ������������.
	memset( szOutput, 0, sInput.GetLength() + 1 );

	// ���������ݽ��н���.
	for( lp = 0, i = 0; lp < sInput.GetLength(); lp++ )
	{
		c = sInput[ lp ];
		nDigit = nDecode[ c & 0x7F ];
		if( nDigit < -1 ) 
		{
			return 0;
		} 
		else if( nDigit >= 0 ) 
			WriteBits( nDigit & 0x3F, szOutput, i );
    }	
	return i;
}

//---------------------------------------------------------------------------
UINT CBase64::ReadBits(int * pBitsRead, int& lp)
{
    ULONG lScratch;
    while( ( m_nBitsRemaining < 6 ) && 
		   ( lp < m_nInputSize ) ) 
	{
		int c = m_szInput[ lp++ ];
        m_lBitStorage <<= 8;
        m_lBitStorage |= (c & 0xff);
		m_nBitsRemaining += 8;
    }

    if( m_nBitsRemaining < 6 ) 
	{
		lScratch = m_lBitStorage << ( 6 - m_nBitsRemaining );
		*pBitsRead = m_nBitsRemaining;
		m_nBitsRemaining = 0;
    } 
	else 
	{
		lScratch = m_lBitStorage >> ( m_nBitsRemaining - 6 );
		*pBitsRead = 6;
		m_nBitsRemaining -= 6;
    }

    return (UINT)lScratch & 0x3F;
}

//---------------------------------------------------------------------------
void CBase64::WriteBits(UINT nBits, LPTSTR szOutput, int& i)
{
	UINT nScratch;

	m_lBitStorage = (m_lBitStorage << 6) | nBits;
	m_nBitsRemaining += 6;
	while( m_nBitsRemaining > 7 ) 
	{
		nScratch = m_lBitStorage >> (m_nBitsRemaining - 8);
		szOutput[ i++ ] = nScratch & 0xFF;
		m_nBitsRemaining -= 8;
	}
}