
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

#ifndef XSPAYLOADMODE_H
#define XSPAYLOADMODE_H

#include "dotsdkconfig.h"

/*!	\addtogroup enums Global enumerations
	@{
*/

/*! \brief Values for Movella DOT payload modes
*/
enum class XsPayloadMode : unsigned char
{
	Undefined				= 0,	//!< Not defined
	HighFidelitywMag		= 1,	//!< High fidelity (with mag)
	ExtendedQuaternion		= 2,	//!< Extended (Quaternion)
	CompleteQuaternion		= 3,	//!< Complete (Quaternion)
	OrientationEuler		= 4,	//!< Orientation (Euler)
	OrientationQuaternion	= 5,	//!< Orientation (Quaternion)
	FreeAcceleration		= 6,	//!< Free Acceleration
	ExtendedEuler			= 7,	//!< Extended (Euler)
	MFM						= 15,	//!< Magnetic Field Mapping
	CompleteEuler			= 16,	//!< Complete (Euler)
	HighFidelity			= 17,	//!< High fidelity
	DeltaQuantitieswMag		= 18,	//!< Delta quantites (with mag)
	DeltaQuantities			= 19,	//!< Delta quantites
	RateQuantitieswMag		= 20,	//!< Rate quantites (with mag)
	RateQuantities			= 21,	//!< Rate quantites
	CustomMode1				= 22,	//!< Custom mode 1
	CustomMode2				= 23,	//!< Custom mode 2
	CustomMode3				= 24,	//!< Custom mode 3
	CustomMode4				= 25,	//!< Custom mode 4
	CustomMode5				= 26,	//!< Custom mode 5
};

//////////////////////////////////////////////////////////////////////////////////////////
/*! \brief Retrieve a character string corresponding to the given payload mode.
	\param mode The result code to convert
	\return A static string describing the result code
	\note In the Python interface this function is called XsPayloadModeToString
	\note In C/C++ do NOT delete the returned char* as it points to static memory.
*/
XDDA_DLL_API const char* XsPayloadMode_toString(XsPayloadMode mode);
/*! @} */

typedef enum XsPayloadMode XsPayloadMode;

#endif
