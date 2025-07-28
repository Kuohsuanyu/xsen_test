
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

#ifndef XSLOGOPTIONS_H
#define XSLOGOPTIONS_H

#include <xstypes/pstdint.h>
#include "dotsdkconfig.h"

/*!	\addtogroup enums Global enumerations
	@{
*/

/*! \brief Values for Movella DOT logging options
*/
enum class XsLogOptions : uint16_t
{
	Quaternion				= 0x0000,	//!< Quaternion export (if orientation available)
	Euler					= 0x0001,	//!< Euler export (if orientation available)
	QuaternionAndEuler		= 0x0002,	//!< Export both Quaternion and Euler

	OrientationMask			= 0x0003,	//!< Bitmask for orientation-based log options
};

/*! \brief Operator to support logical AND operator for enum class
*/
inline constexpr XsLogOptions
  operator&(XsLogOptions x, XsLogOptions y)
  {
	return static_cast<XsLogOptions>
	  (static_cast<uint16_t>(x) & static_cast<uint16_t>(y));
  }
/*! @} */

typedef enum XsLogOptions XsLogOptions;

#endif
