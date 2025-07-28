
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

#ifndef XSSYNCACKNOWLEDGERESULT_H
#define XSSYNCACKNOWLEDGERESULT_H

#include "dotsdkconfig.h"
#include <xstypes/xsresultvalue.h>

/*!	\addtogroup enums Global enumerations
	@{
*/

/*! \brief Values for Movella DOT Sync Acknowledge Result
*/
enum class XsSyncAcknowledgeResult : unsigned char
{
	Success = 0x00,						//!< Control message write success
	Failed,								//!< Failed
	InvalidCmd,							//!< Invalid command
	Busy,								//!< Busy
	Started,							//!< Sync started
	NoEnoughSample,						//!< Sync failed for not enough data samples
	Reserved,							//!< Reserved
	SkewTooLarge,						//!< Sync failed for estimated skew too large
	StartingTimingErr,					//!< Sync failed for start time error
	Unstarted,							//!< Sync is not started
	Invalid,							//!< Invalid
	Unknown = 0xFF						//!< Unknown state, no valid recording state has been received
};

//////////////////////////////////////////////////////////////////////////////////////////
/*! \brief Retrieve a character string corresponding to the given Sync Acknowledge result.
	\param res The result code to convert
	\return A static string describing the result code
	\note In the Python interface this function is called XsSyncAcknowledgeResultToString
	\note In C/C++ do NOT delete the returned char* as it points to static memory.
*/
XDDA_DLL_API const char* XsSyncAcknowledgeResult_toString(XsSyncAcknowledgeResult res);

/*! @} */

XDDA_DLL_API bool isValidXsSyncAcknowledgeResult(XsSyncAcknowledgeResult ackRes);

XDDA_DLL_API XsResultValue XsSyncAcknowledgeResult_toResultValue(XsSyncAcknowledgeResult ackRes);

typedef enum XsSyncAcknowledgeResult XsSyncAcknowledgeResult;

#endif
