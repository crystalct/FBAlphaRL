// ---------------------------------------------------------------------
// FB Neo Retro Loader Plus (CaptainCPS-X, 2013) - (CrystalCT, 2020)
// ---------------------------------------------------------------------
#include "capp.h"
#include <io/pad.h>

void CapApp::InputFrameStart()
{
	npad_read();
}

void CapApp::InputFrameEnd()
{
	for(int x = 0; x < 16; x++)	{
		mIsButtPressed[x] = buttPressedNow[x];
	}
}

int CapApp::InputInit()
{
	ioPadInit(7);

	return 0;
}

#define GET_PAD_D1(BT_X, CELL_PAD_CTRL_X)	\
	if(pPad->button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_X)	\
	{	\
		buttPressedNow[BT_X] = true;	\
	} else {	\
		buttPressedNow[BT_X] = false;	\
	}

#define GET_PAD_D2(BT_X, CELL_PAD_CTRL_X)	\
	if(pPad->button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_X)	\
	{	\
		buttPressedNow[BT_X] = true;	\
	} else {	\
		buttPressedNow[BT_X] = false;	\
	}


int CapApp::npad_read()
{
	//Check the pads / Get gamepads information
	ioPadGetInfo(&padinfo);

	// Use controller 0 for operation (for now)
	if(padinfo.status[0]){
			ioPadGetData(0, &paddata);

			//buttPressedNow[BT_X] = true;
			if(paddata.BTN_START)
				buttPressedNow[BT_START] = true;
			else
				buttPressedNow[BT_START] = false;
			if(paddata.BTN_SELECT)
				buttPressedNow[BT_SELECT] = true;
			else
				buttPressedNow[BT_SELECT] = false;
			if(paddata.BTN_L3)
				buttPressedNow[BT_L3] = true;
			else
				buttPressedNow[BT_L3] = false;
			if(paddata.BTN_R3)
				buttPressedNow[BT_R3] = true;
			else
				buttPressedNow[BT_R3] = false;
			if(paddata.BTN_UP)
				buttPressedNow[BT_UP] = true;
			else
				buttPressedNow[BT_UP] = false;
			if(paddata.BTN_RIGHT)
				buttPressedNow[BT_RIGHT] = true;
			else
				buttPressedNow[BT_RIGHT] = false;
			if(paddata.BTN_DOWN)
				buttPressedNow[BT_DOWN] = true;
			else
				buttPressedNow[BT_DOWN] = false;
			if(paddata.BTN_LEFT)
				buttPressedNow[BT_LEFT] = true;
			else
				buttPressedNow[BT_LEFT] = false;
			if(paddata.BTN_L2)
				buttPressedNow[BT_L2] = true;
			else
				buttPressedNow[BT_L2] = false;
			if(paddata.BTN_R2)
				buttPressedNow[BT_R2] = true;
			else
				buttPressedNow[BT_R2] = false;
			if(paddata.BTN_L1)
				buttPressedNow[BT_L1] = true;
			else
				buttPressedNow[BT_L1] = false;
			if(paddata.BTN_R1)
				buttPressedNow[BT_R1] = true;
			else
				buttPressedNow[BT_R1] = false;
			if(paddata.BTN_TRIANGLE)
				buttPressedNow[BT_TRIANGLE] = true;
			else
				buttPressedNow[BT_TRIANGLE] = false;
			if(paddata.BTN_CIRCLE)
				buttPressedNow[BT_CIRCLE] = true;
			else
				buttPressedNow[BT_CIRCLE] = false;
			if(paddata.BTN_CROSS)
				buttPressedNow[BT_CROSS] = true;
			else
				buttPressedNow[BT_CROSS] = false;
			if(paddata.BTN_SQUARE)
				buttPressedNow[BT_SQUARE] = true;
			else
				buttPressedNow[BT_SQUARE] = false;

			mValRStickX = (uint32_t)paddata.ANA_R_H;
			mValRStickY = (uint32_t)paddata.ANA_R_V;
			mValLStickX = (uint32_t)paddata.ANA_L_H;
			mValLStickY = (uint32_t)paddata.ANA_L_V;
			/*if(paddata.BTN_START){
					goto end;
				}*/
	}
	else {
		printf("Error: No Pads present.\n");
		return 0;
	}

	//}
	/*////
	int32_t nRet = CELL_PAD_OK;

	// Get gamepads information
	CellPadInfo2 gpad_info;
	nRet = cellPadGetInfo2(&gpad_info);

	if(nRet != CELL_PAD_OK) {
		// error
		return 0;
	}

	// Check if we have 1 - 7 controllers connected
	if(gpad_info.now_connect >= 1 && gpad_info.now_connect <= 7)
	{
		// Use controller 1 for operation (for now)
		if(gpad_info.port_status[0] & CELL_PAD_STATUS_CONNECTED)
		{
			// Controller 1 is connected

			// Get Buttons data [pad 1]
			CellPadData* pPad = NULL;
			pPad = (CellPadData*)malloc(sizeof(CellPadData));

			if(cellPadGetData(0, pPad) == CELL_PAD_OK)
			{
				if(pPad->len == 0) return 0;

				GET_PAD_D1(BT_LEFT	, CELL_PAD_CTRL_LEFT);
				GET_PAD_D1(BT_RIGHT	, CELL_PAD_CTRL_RIGHT);
				GET_PAD_D1(BT_DOWN	, CELL_PAD_CTRL_DOWN);
				GET_PAD_D1(BT_UP	, CELL_PAD_CTRL_UP);
				GET_PAD_D1(BT_START	, CELL_PAD_CTRL_START);
				GET_PAD_D1(BT_R3	, CELL_PAD_CTRL_R3);
				GET_PAD_D1(BT_L3	, CELL_PAD_CTRL_L3);
				GET_PAD_D1(BT_SELECT, CELL_PAD_CTRL_SELECT);

				GET_PAD_D2(BT_SQUARE	, CELL_PAD_CTRL_SQUARE);
				GET_PAD_D2(BT_CROSS		, CELL_PAD_CTRL_CROSS);
				GET_PAD_D2(BT_CIRCLE	, CELL_PAD_CTRL_CIRCLE);
				GET_PAD_D2(BT_TRIANGLE	, CELL_PAD_CTRL_TRIANGLE);
				GET_PAD_D2(BT_R1		, CELL_PAD_CTRL_R1);
				GET_PAD_D2(BT_L1		, CELL_PAD_CTRL_L1);
				GET_PAD_D2(BT_R2		, CELL_PAD_CTRL_R2);
				GET_PAD_D2(BT_L2		, CELL_PAD_CTRL_L2);

				// analog controls

				mValRStickX = (uint32_t)pPad->button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X];
				mValRStickY = (uint32_t)pPad->button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_Y];
				mValLStickX = (uint32_t)pPad->button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_X];
				mValLStickY = (uint32_t)pPad->button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_Y];
			}
		}

	} else {
		// todo: report user to connect a controller
	}
*/
	return 1;
}

void CapApp::InputExit()
{
	ioPadEnd();
}
