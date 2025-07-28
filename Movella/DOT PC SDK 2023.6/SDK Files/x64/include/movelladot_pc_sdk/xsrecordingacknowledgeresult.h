
//  WARNING: COPYRIGHT (C) 2023 MOVELLA TECHNOLOGIES OR SUBSIDIARIES WORLDWIDE. ALL RIGHTS RESERVED.
//  THIS FILE AND THE SOURCE CODE IT CONTAINS (AND/OR THE BINARY CODE FILES FOUND IN THE SAME
//  FOLDER THAT CONTAINS THIS FILE) AND ALL RELATED SOFTWARE (COLLECTIVELY, "CODE") ARE SUBJECT
//  TO A RESTRICTED LICENSE AGREEMENT ("AGREEMENT") BETWEEN XSENS AS LICENSOR AND THE AUTHORIZED
//  LICENSEE UNDER THE AGREEMENT. THE CODE MUST BE USED SOLELY WITH XSENS PRODUCTS INCORPORATED
//  INTO LICENSEE PRODUCTS IN ACCORDANCE WITH THE AGREEMENT. ANY USE, MODIFICATION, COPYING OR
//  DISTRIBUTION OF THE CODE IS STRICTLY PROHIBITED UNLESS EXPRESSLY AUTHORIZED BY THE AGREEMENT.
//  IF YOU ARE NOT AN AUTHORIZED USER OF THE CODE IN ACCORDANCE WITH THE AGREEMENT, YOU MUST STOP
//  USING OR VIEWING THE CODE NOW, REMOVE ANY COPIES OF THE CODE FROM YOUR COMPUTER AND NOTIFY
//  XSENS IMMEDIATELY BY EMAIL TO INFO@XSENS.COM. ANY COPIES OR DERIVATIVES OF THE CODE (IN WHOLE
//  OR IN PART) IN SOURCE CODE FORM THAT ARE PERMITTED BY THE AGREEMENT MUST RETAIN THE ABOVE
//  COPYRIGHT NOTICE AND THIS PARAGRAPH IN ITS ENTIRETY, AS REQUIRED BY THE AGREEMENT.
//  

#ifndef XSRECORDINGACKNOWLEDGERESULT_H
#define XSRECORDINGACKNOWLEDGERESULT_H

#include "dotsdkconfig.h"
#include <xstypes/xsresultvalue.h>

/*!	\addtogroup enums Global enumerations
	@{
*/

/*! \brief Values for Movella DOT Recording Acknowledge Result
*/
enum class XsRecordingAcknowledgeResult : unsigned char
{
	Success = 0x00,						//!< Control message write success
	InvalidCmd = 0x02,					//!< Invalid command
	FlashProcessBusy = 0x03,			//!< Flash is occupied by other process
	IdleState = 0x06,					//!< Idle state
	OnErasing = 0x30,					//!< Erasing internal storage
	OnRecording = 0x40,					//!< In recording state
	OnExportFlashInfo = 0x50,			//!< Exporting flash information
	OnExportRecordingFileInfo = 0x60,	//!< Exporting recording file information
	OnExportRecordingFileData = 0x70,	//!< Exporting recording data
	Unknown = 0xFF						//!< Unknown state, no valid recording state has been received
};

//////////////////////////////////////////////////////////////////////////////////////////
/*! \brief Retrieve a character string corresponding to the given Recording Acknowledge result.
	\param res The result code to convert
	\return A static string describing the result code
	\note In the Python interface this function is called XsRecordingAcknowledgeResultToString
	\note In C/C++ do NOT delete the returned char* as it points to static memory.
*/
XDDA_DLL_API const char* XsRecordingAcknowledgeResult_toString(XsRecordingAcknowledgeResult res);

/*! @} */

XDDA_DLL_API bool isValidXsRecordingAcknowledgeResult(XsRecordingAcknowledgeResult ackRes);

XDDA_DLL_API XsResultValue XsRecordingAcknowledgeResult_toResultValue(XsRecordingAcknowledgeResult ackRes);

typedef enum XsRecordingAcknowledgeResult XsRecordingAcknowledgeResult;

#endif
