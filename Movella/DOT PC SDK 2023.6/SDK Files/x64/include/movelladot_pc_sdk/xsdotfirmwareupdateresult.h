
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

#ifndef XSDOTFIRMWAREUPDATERESULT_H
#define XSDOTFIRMWAREUPDATERESULT_H

#include "dotsdkconfig.h"

//////////////////////////////////////////////////////////////////////////////////////////
/*!	\addtogroup enums Global enumerations
	@{
*/

/*!	\brief Movella DOT firmware update result values
	\details These values are used to signal success or specific failures of functions
	\sa XsDotFirmwareUpdateResult_toString
*/
enum class XsDotFirmwareUpdateResult : unsigned char
{
	Success = 0,						 //!< Firmware update was performed successfully
	NoUsb = 1,							 //!< Device must be connected to USB for a firmware update
	NoFwFile = 2,						 //!< Could not open firmware file
	IncorrectFwFile = 3,				 //!< Firmware file does not match device
	CommunicationFailure = 4,			 //!< Failed to communicate with the device
	FailedToStart = 5,					 //!< Device could not start the firmware update
	FailedToEnd = 6,					 //!< Device could not end the firmware update
	BootloaderFail = 7,					 //!< Could not go to bootloader state
	ErrorPreamble = 8,					 //!< Preamble error. Device received incorrect data
	ErrorCmd = 9,						 //!< Device received unknown update command
	ErrorOperating = 10,				 //!< Operating error. Device not expecting this message
	ErrorPacketCrc = 11,				 //!< Data packet CRC error
	ErrorImageCrc = 12,					 //!< Firmware image CRC error
	ErrorIndex = 13,					 //!< Device received unexpected firmware file index
	ErrorFlash = 14,					 //!< Erase/Write flash failed
	ErrorDischarging = 15,				 //!< Device is discharging
	ErrorRecording = 16,				 //!< Device is recording
	ErrorDownloadNoCurl = 17,			 //!< Firmware download could not be started
	ErrorDownloadNoCurlPerform = 18,	 //!< Firmware download could not be performed
	ErrorDownloadNoCurlGetInfo = 19,	 //!< Firmware download could not be finished
	ErrorDownloadCurlEmptyContent = 20,  //!< Firmware download received empty content	
	ErrorUnknown = 21,					 //!< Unknown error received from device
	NoNewFWAvailable = 22,				 //!< Check for updates did not find a newer version to upgrade

#ifdef XSENS_DEBUG
	// Debug/testing values
	SuccessWithoutStartAck = 251,		 //!< Firmware update was performed successfully, without startack
	SuccessWithStartRetry = 252,		 //!< Firmware update was performed successfully, with start retry
	SuccessWithContentRetry = 253,		 //!< Firmware update was performed successfully, with content retry
	SuccessWithEndRetry = 254,			 //!< Firmware update was performed successfully, with end retry
#endif
	InitialValue = 255					 //!< Initialization value
};

//////////////////////////////////////////////////////////////////////////////////////////
/*!  \brief Retrieve a string corresponding to the given result code.

	This function uses the XsResultText list to return a string with the relevant result
	code in a textual format. If the supplied result code is invalid the
	"!!Undefined Result Value!!" string is returned.
*/
XDDA_DLL_API const char* XsDotFirmwareUpdateResult_toString(XsDotFirmwareUpdateResult result);
/*! @} */

typedef enum XsDotFirmwareUpdateResult XsDotFirmwareUpdateResult;

#endif
