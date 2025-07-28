
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

#ifndef XSRECORDINGINFO_H
#define XSRECORDINGINFO_H

#include "dotsdkconfig.h"

#ifdef __cplusplus
struct XsRecordingInfo;
extern "C" {
#endif

#ifndef __cplusplus
	typedef struct XsRecordingInfo XsRecordingInfo;
#endif

	XDDA_DLL_API int XsRecordingInfo_empty(const XsRecordingInfo* thisPtr);
	XDDA_DLL_API unsigned int XsRecordingInfo_startUTC(XsRecordingInfo const* thisPtr);
	XDDA_DLL_API unsigned short XsRecordingInfo_totalRecordingTime(XsRecordingInfo const* thisPtr);
	XDDA_DLL_API unsigned int XsRecordingInfo_storageSize(XsRecordingInfo const* thisPtr);
#ifdef __cplusplus
}
#endif

struct XsRecordingInfo
{
#ifdef __cplusplus
	/*! \brief Default constructor, creates an empty recording info object */
	XsRecordingInfo()
		: m_startUTC(0)
		, m_totalRecordingTime(0)
		, m_storageSize(0)
	{
	}

	/*! \brief Named constructor, initializes the object to the supplied startTime, total time and storageSize.
		\param startUTC
		\param totalRecordingTime
		\param storageSize
	*/
	explicit XsRecordingInfo(unsigned int startUTC, unsigned short totalRecordingTime, unsigned int storageSize)
		: m_startUTC(startUTC)
		, m_totalRecordingTime(totalRecordingTime)
		, m_storageSize(storageSize)
	{
	}

	//! \brief \copybrief XsRecordingInfo_empty
	inline bool empty() const
	{
		return m_startUTC == 0 && m_totalRecordingTime == 0 && m_storageSize == 0;
	}

	/*! \copydoc XsRecordingInfo_startUTC */
	inline unsigned int startUTC() const
	{
		return m_startUTC;
	}

	/*! \copydoc XsRecordingInfo_totalRecordingTime */
	inline unsigned short totalRecordingTime() const
	{
		return m_totalRecordingTime;
	}

	/*! \copydoc XsRecordingInfo_storageSize */
	inline unsigned int storageSize() const
	{
		return m_storageSize;
	}


private:
#endif
	unsigned int m_startUTC;					//!< Recording start UTC in seconds
	unsigned short m_totalRecordingTime;		//!< Duration of this recording in seconds.
	unsigned int m_storageSize;					//!< Used storage space in bytes.
};

#endif