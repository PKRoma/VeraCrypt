/*
 Derived from source code of TrueCrypt 7.1a, which is
 Copyright (c) 2008-2012 TrueCrypt Developers Association and which is governed
 by the TrueCrypt License 3.0.

 Modifications and additions to the original source code (contained in this file)
 and all other portions of this file are Copyright (c) 2013-2025 AM Crypto
 and are governed by the Apache License 2.0 the full text of which is
 contained in the file License.txt included in VeraCrypt binary and source
 code distribution packages.
*/

#include "Tcdefs.h"
#include "Wipe.h"



// Fill buffer with wipe patterns defined in "National Industrial Security Program Operating Manual", US DoD 5220.22-M.
// Return: FALSE = buffer must be filled with random data

static BOOL Wipe3Dod5220 (int pass, uint8 *buffer, size_t size)
{
	uint8 wipeChar;

	switch (pass)
	{
	case 1:
		wipeChar = 0;
		break;

	case 2:
		wipeChar = 0xff;
		break;

	default:
		return FALSE;
	}

	memset (buffer, wipeChar, size);
	return TRUE;
}


static BOOL Wipe7Dod5220 (int pass, uint8 randChars[TC_WIPE_RAND_CHAR_COUNT], uint8 *buffer, size_t size)
{
	uint8 wipeChar;

	switch (pass)
	{
	case 1:
		wipeChar = randChars[0];
		break;

	case 2:
		wipeChar = ~randChars[0];
		break;

	case 4:
		wipeChar = randChars[1];
		break;

	case 5:
		wipeChar = randChars[2];
		break;

	case 6:
		wipeChar = ~randChars[2];
		break;

	default:
		return FALSE;
	}

	memset (buffer, wipeChar, size);
	return TRUE;
}


// Fill the buffer with wipe patterns defined in the paper "Secure Deletion of Data from Magnetic and Solid-State Memory" by Peter Gutmann.
// Return: FALSE = buffer must be filled with random data

static BOOL Wipe35Gutmann (int pass, uint8 *buffer, size_t size)
{
	uint8 wipePat3[] = { 0x92, 0x49, 0x24 };
	int wipePat3Pos;
	size_t i;

	switch (pass)
	{
	case 5:
		memset (buffer, 0x55, size);
		break;

	case 6:
		memset (buffer, 0xaa, size);
		break;

	case 7:
	case 26:
	case 29:
		wipePat3Pos = 0;
		goto wipe3;

	case 8:
	case 27:
	case 30:
		wipePat3Pos = 1;
		goto wipe3;

	case 9:
	case 28:
	case 31:
		wipePat3Pos = 2;
		goto wipe3;

wipe3:
		if (pass >= 29)
		{
			wipePat3[0] = ~wipePat3[0];
			wipePat3[1] = ~wipePat3[1];
			wipePat3[2] = ~wipePat3[2];
		}

		for (i = 0; i < size; ++i)
		{
			buffer[i] = wipePat3[wipePat3Pos++ % 3];
		}
		break;

	default:
		if (pass >= 10 && pass <= 25)
			memset (buffer, (pass - 10) * 0x11, size);
		else
			return FALSE;
	}

	return TRUE;
}


int GetWipePassCount (WipeAlgorithmId algorithm)
{
	switch (algorithm)
	{
	case TC_WIPE_1_RAND:
		return 1;

	case TC_WIPE_3_DOD_5220:
		return 3;

	case TC_WIPE_7_DOD_5220:
		return 7;

	case TC_WIPE_35_GUTMANN:
		return 35;

	case TC_WIPE_256:
		return 256;
	}

	return -1;	// Prevent compiler warnings
}


BOOL WipeBuffer (WipeAlgorithmId algorithm, uint8 randChars[TC_WIPE_RAND_CHAR_COUNT], int pass, uint8 *buffer, size_t size)
{
	switch (algorithm)
	{
	case TC_WIPE_1_RAND:
	case TC_WIPE_256:
		return FALSE; // Delegate buffer filling to the caller

	case TC_WIPE_3_DOD_5220:
		return Wipe3Dod5220 (pass, buffer, size);

	case TC_WIPE_7_DOD_5220:
		return Wipe7Dod5220 (pass, randChars, buffer, size);

	case TC_WIPE_35_GUTMANN:
		return Wipe35Gutmann (pass, buffer, size);

	/* we will never reach here because all calls to WipeBuffer are preceeded
	 * by a call to GetWipePassCount that already checks the same algorithm
	 * parameters and in case of unsupported value an error is returned before
	 * calling WipeBuffer
	 */
   /*
	default:
		TC_THROW_FATAL_EXCEPTION;*/
	}

	return FALSE;	// Prevent compiler warnings
}
