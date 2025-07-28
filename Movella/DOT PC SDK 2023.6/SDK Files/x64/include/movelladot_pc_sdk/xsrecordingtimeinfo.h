
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

#ifndef XSRECORDINGTIMEINFO_H
#define XSRECORDINGTIMEINFO_H

#include "dotsdkconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
	typedef struct XsRecordingTimeInfo XsRecordingTimeInfo;
#else
	struct XsRecordingTimeInfo;
#endif

	XDDA_DLL_API unsigned int XsRecordingTimeInfo_startUTC(XsRecordingTimeInfo const* thisPtr);
	XDDA_DLL_API unsigned short XsRecordingTimeInfo_totalRecordingTime(XsRecordingTimeInfo const* thisPtr);
	XDDA_DLL_API unsigned short XsRecordingTimeInfo_remainingRecordingTime(XsRecordingTimeInfo const* thisPtr);
#ifdef __cplusplus
}
#endif

struct XsRecordingTimeInfo
{
#ifdef __cplusplus
	/*! \brief Default constructor, creates an empty recording time info object */
	XsRecordingTimeInfo()
		: m_startUTC(0)
		, m_totalRecordingTime(0xFFFF)
		, m_remainingRecordingTime(0xFFFF)
	{
	}

	/*! \brief Named constructor, initializes the object to the supplied startTime, total time and remaining time.
		\param startUTC
		\param totalRecordingTime
		\param remainingRecordingTime
	*/
	explicit XsRecordingTimeInfo(unsigned int startUTC, unsigned short totalRecordingTime = 0xFFFF, unsigned short remainingRecordingTime = 0xFFFF)
		: m_startUTC(startUTC)
		, m_totalRecordingTime(totalRecordingTime)
		, m_remainingRecordingTime(remainingRecordingTime)
	{
	}

	/*! \copydoc XsRecordingTimeInfo_startUTC */
	inline unsigned int startUTC() const
	{
		return m_startUTC;
	}

	/*! \copydoc XsRecordingTimeInfo_totalRecordingTime */
	inline unsigned short totalRecordingTime() const
	{
		return m_totalRecordingTime;
	}

	/*! \copydoc XsRecordingTimeInfo_remainingRecordingTime */
	inline unsigned short remainingRecordingTime() const
	{
		return m_remainingRecordingTime;
	}


private:
#endif
	unsigned int m_startUTC;					//!< Recording start UTC in seconds
	unsigned short m_totalRecordingTime;		//!< Total recording time of a timed recording. Unit is second. 0xFFFF for normal (non-timed) recording.
	unsigned short m_remainingRecordingTime;	//!< Remaining time of a timed recording. Unit is second. 0xFFFF for normal (non-timed) recording.
};

#endif