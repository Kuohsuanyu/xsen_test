
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

#ifndef XSRECORDINGDATAEXPORTFIELD_H
#define XSRECORDINGDATAEXPORTFIELD_H

#include "dotsdkconfig.h"

/*!	\addtogroup enums Global enumerations
	@{
*/

/*! \brief Values for Movella DOT recording export data fields
*/
enum XsRecordingExportDataField : int
{
	RecordingData_Timestamp       = 0x00,	//!< Timestamp on the sensor in microseconds, 4 bytes, 1x uint32 LE
	RecordingData_Quaternion      = 0x01,	//!< The orientation expressed as a quaternion, 16 bytes, 4x float
	RecordingData_iQ              = 0x02,	//!< Orientation change quaternion (internal format), 9 bytes
	RecordingData_iV              = 0x03,	//!< Velocity change (internal format), 12 bytes
	RecordingData_Euler           = 0x04,	//!< The orientation expressed as Euler angles in degrees, 12 bytes, 3x float
	RecordingData_dQ              = 0x05,	//!< Orientation change quaternion during the previous time interval, 16 bytes, 4x float
	RecordingData_dV              = 0x06,	//!< Velocity change during the previous time interval, 12 bytes, 3x float
	RecordingData_Acceleration    = 0x07,	//!< Acceleration in local earth coordinates and the local gravity removed in m/s2, 12 bytes, 3x float
	RecordingData_AngularVelocity = 0x08,	//!< Rate of turn in sensor coordinates in degrees/s, 12 bytes, 3x float
	RecordingData_MagneticField   = 0x09,	//!< Magnetic field in sensor coordinates in a.u., 6 bytes, 3x int16 LE as fixed point 4.12
	RecordingData_Status          = 0x0a,	//!< Datapacket flags, 2 bytes, 1x uint16 LE
	RecordingData_ClipCountAcc    = 0x0b,	//!< Count of ClipAcc in status, 1 byte, 1x uint8
	RecordingData_ClipCountGyro   = 0x0c,	//!< Count of ClipGyr in status, 1 byte, 1x uint8
};


/*! \brief Retrieve a character string corresponding to the given recording export data field.
	\param dataField The data field to convert
	\return A static string describing the dataField
	\note In the Python interface this function is called XsRecordingExportDataFieldToString
	\note In C/C++ do NOT delete the returned char* as it points to static memory.
*/
XDDA_DLL_API const char* XsRecordingExportDataField_toString(XsRecordingExportDataField dataField);
/*! @} */

typedef enum XsRecordingExportDataField XsRecordingExportDataField;

/*! \brief Return whether the provided dataField is an existing data field */
XDDA_DLL_API bool isValidXsRecordingExportDataField(XsRecordingExportDataField dataField);
#endif
