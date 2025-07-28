
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

#ifndef GUARD_XSDOTUSBDEVICE_H_TMP
#define GUARD_XSDOTUSBDEVICE_H_TMP
#include <xstypes/xsstringarray.h>
#include <xstypes/xsversion.h>
#include <xstypes/xsintarray.h>
#include <xstypes/xsexception.h>
#include <xstypes/xsresetmethod.h>
#include <xstypes/xsresultvalue.h>
#include <xscontroller/xserrormode.h>
#include <movelladot_pc_sdk/xslogoptions.h>
#ifdef __cplusplus
extern "C" {
#endif
/*! \addtogroup cinterface C Interface
	@{ */
struct XsDotUsbDevice;
#ifndef __cplusplus
typedef struct XsDotUsbDevice XsDotUsbDevice;
#endif
XDDA_DLL_API XsString* XsDotUsbDevice_productCode(const struct XsDotUsbDevice* thisPtr, XsString* returnValue);/*!< \copydoc XsDotUsbDevice::productCode() const*/
XDDA_DLL_API XsString* XsDotUsbDevice_bluetoothAddress(struct XsDotUsbDevice* thisPtr, XsString* returnValue);/*!< \copydoc XsDotUsbDevice::bluetoothAddress()*/
XDDA_DLL_API XsString* XsDotUsbDevice_deviceTagName(struct XsDotUsbDevice* thisPtr, XsString* returnValue);/*!< \copydoc XsDotUsbDevice::deviceTagName()*/
XDDA_DLL_API uint32_t XsDotUsbDevice_serialNumber(struct XsDotUsbDevice* thisPtr);/*!< \copydoc XsDotUsbDevice::serialNumber()*/
XDDA_DLL_API int XsDotUsbDevice_eraseFlash(struct XsDotUsbDevice* thisPtr);/*!< \copydoc XsDotUsbDevice::eraseFlash()*/
XDDA_DLL_API int XsDotUsbDevice_recordingCount(struct XsDotUsbDevice* thisPtr);/*!< \copydoc XsDotUsbDevice::recordingCount()*/
XDDA_DLL_API int XsDotUsbDevice_usedFlashSizeTotal(struct XsDotUsbDevice* thisPtr);/*!< \copydoc XsDotUsbDevice::usedFlashSizeTotal()*/
XDDA_DLL_API XsRecordingInfo* XsDotUsbDevice_getRecordingInfo(struct XsDotUsbDevice* thisPtr, XsRecordingInfo* returnValue, int recordingIndex);/*!< \copydoc XsDotUsbDevice::getRecordingInfo(int)*/
XDDA_DLL_API int XsDotUsbDevice_selectExportData(struct XsDotUsbDevice* thisPtr, const XsIntArray* selectedDataFields);/*!< \copydoc XsDotUsbDevice::selectExportData(const XsIntArray&)*/
XDDA_DLL_API int XsDotUsbDevice_startExportRecording(struct XsDotUsbDevice* thisPtr, int recordingIndex);/*!< \copydoc XsDotUsbDevice::startExportRecording(int)*/
XDDA_DLL_API int XsDotUsbDevice_stopExportRecording(struct XsDotUsbDevice* thisPtr);/*!< \copydoc XsDotUsbDevice::stopExportRecording()*/
XDDA_DLL_API int XsDotUsbDevice_enableLogging(struct XsDotUsbDevice* thisPtr, const XsString* filename);/*!< \copydoc XsDotUsbDevice::enableLogging(const XsString&)*/
XDDA_DLL_API int XsDotUsbDevice_disableLogging(struct XsDotUsbDevice* thisPtr);/*!< \copydoc XsDotUsbDevice::disableLogging()*/
XDDA_DLL_API void XsDotUsbDevice_setLogOptions(struct XsDotUsbDevice* thisPtr, XsLogOptions options);/*!< \copydoc XsDotUsbDevice::setLogOptions(XsLogOptions)*/
XDDA_DLL_API XsString* XsDotUsbDevice_portName(const struct XsDotUsbDevice* thisPtr, XsString* returnValue);/*!< \copydoc XsDotUsbDevice::portName() const*/
XDDA_DLL_API XsPortInfo* XsDotUsbDevice_portInfo(const struct XsDotUsbDevice* thisPtr, XsPortInfo* returnValue);/*!< \copydoc XsDotUsbDevice::portInfo() const*/
XDDA_DLL_API XsDeviceId* XsDotUsbDevice_deviceId(const struct XsDotUsbDevice* thisPtr, XsDeviceId* returnValue);/*!< \copydoc XsDotUsbDevice::deviceId() const*/
XDDA_DLL_API XsResultValue XsDotUsbDevice_lastResult(const struct XsDotUsbDevice* thisPtr);/*!< \copydoc XsDotUsbDevice::lastResult() const*/
XDDA_DLL_API XsString* XsDotUsbDevice_lastResultText(const struct XsDotUsbDevice* thisPtr, XsString* returnValue);/*!< \copydoc XsDotUsbDevice::lastResultText() const*/
XDDA_DLL_API XsDeviceState* XsDotUsbDevice_deviceState(const struct XsDotUsbDevice* thisPtr, XsDeviceState* returnValue);/*!< \copydoc XsDotUsbDevice::deviceState() const*/
XDDA_DLL_API XsVersion* XsDotUsbDevice_firmwareVersion(const struct XsDotUsbDevice* thisPtr, XsVersion* returnValue);/*!< \copydoc XsDotUsbDevice::firmwareVersion() const*/
XDDA_DLL_API void XsDotUsbDevice_addRef(struct XsDotUsbDevice* thisPtr);/*!< \copydoc XsDotUsbDevice::addRef()*/
XDDA_DLL_API void XsDotUsbDevice_removeRef(struct XsDotUsbDevice* thisPtr);/*!< \copydoc XsDotUsbDevice::removeRef()*/
XDDA_DLL_API XsSize XsDotUsbDevice_refCounter(const struct XsDotUsbDevice* thisPtr);/*!< \copydoc XsDotUsbDevice::refCounter() const*/
XDDA_DLL_API void XsDotUsbDevice_clearXsDotCallbackHandlers(struct XsDotUsbDevice* thisPtr, int chain);/*!< \copydoc XsDotUsbDevice::clearXsDotCallbackHandlers(bool)*/
XDDA_DLL_API void XsDotUsbDevice_addXsDotCallbackHandler(struct XsDotUsbDevice* thisPtr, XsDotCallbackPlainC* cb, int chain);/*!< \copydoc XsDotUsbDevice::addXsDotCallbackHandler(XsDotCallbackPlainC*,bool)*/
XDDA_DLL_API void XsDotUsbDevice_removeXsDotCallbackHandler(struct XsDotUsbDevice* thisPtr, XsDotCallbackPlainC* cb, int chain);/*!< \copydoc XsDotUsbDevice::removeXsDotCallbackHandler(XsDotCallbackPlainC*,bool)*/
/*! @} */
#ifdef __cplusplus
} // extern "C"
struct XsDotUsbDevice {
	inline XsString productCode(void) const
	{
		XsString returnValue;
		return *XsDotUsbDevice_productCode(this, &returnValue);
	}

	inline XsString bluetoothAddress(void)
	{
		XsString returnValue;
		return *XsDotUsbDevice_bluetoothAddress(this, &returnValue);
	}

	inline XsString deviceTagName(void)
	{
		XsString returnValue;
		return *XsDotUsbDevice_deviceTagName(this, &returnValue);
	}

	inline uint32_t serialNumber(void)
	{
		return XsDotUsbDevice_serialNumber(this);
	}

	inline bool eraseFlash(void)
	{
		return 0 != XsDotUsbDevice_eraseFlash(this);
	}

	inline int recordingCount(void)
	{
		return XsDotUsbDevice_recordingCount(this);
	}

	inline int usedFlashSizeTotal(void)
	{
		return XsDotUsbDevice_usedFlashSizeTotal(this);
	}

	inline XsRecordingInfo getRecordingInfo(int recordingIndex)
	{
		XsRecordingInfo returnValue;
		return *XsDotUsbDevice_getRecordingInfo(this, &returnValue, recordingIndex);
	}

	inline bool selectExportData(const XsIntArray& selectedDataFields)
	{
		return 0 != XsDotUsbDevice_selectExportData(this, &selectedDataFields);
	}

	inline bool startExportRecording(int recordingIndex)
	{
		return 0 != XsDotUsbDevice_startExportRecording(this, recordingIndex);
	}

	inline bool stopExportRecording(void)
	{
		return 0 != XsDotUsbDevice_stopExportRecording(this);
	}

	inline bool enableLogging(const XsString& filename)
	{
		return 0 != XsDotUsbDevice_enableLogging(this, &filename);
	}

	inline bool disableLogging(void)
	{
		return 0 != XsDotUsbDevice_disableLogging(this);
	}

	inline void setLogOptions(XsLogOptions options)
	{
		XsDotUsbDevice_setLogOptions(this, options);
	}

	inline XsString portName(void) const
	{
		XsString returnValue;
		return *XsDotUsbDevice_portName(this, &returnValue);
	}

	inline XsPortInfo portInfo(void) const
	{
		XsPortInfo returnValue;
		return *XsDotUsbDevice_portInfo(this, &returnValue);
	}

	inline XsDeviceId deviceId(void) const
	{
		XsDeviceId returnValue;
		return *XsDotUsbDevice_deviceId(this, &returnValue);
	}

	inline XsResultValue lastResult(void) const
	{
		return XsDotUsbDevice_lastResult(this);
	}

	inline XsString lastResultText(void) const
	{
		XsString returnValue;
		return *XsDotUsbDevice_lastResultText(this, &returnValue);
	}

	inline XsDeviceState deviceState(void) const
	{
		XsDeviceState returnValue;
		return *XsDotUsbDevice_deviceState(this, &returnValue);
	}

	inline XsVersion firmwareVersion(void) const
	{
		XsVersion returnValue;
		return *XsDotUsbDevice_firmwareVersion(this, &returnValue);
	}

	inline void addRef(void)
	{
		XsDotUsbDevice_addRef(this);
	}

	inline void removeRef(void)
	{
		XsDotUsbDevice_removeRef(this);
	}

	inline XsSize refCounter(void) const
	{
		return XsDotUsbDevice_refCounter(this);
	}

	inline void clearXsDotCallbackHandlers(bool chain = true)
	{
		XsDotUsbDevice_clearXsDotCallbackHandlers(this, chain);
	}

	inline void addXsDotCallbackHandler(XsDotCallbackPlainC* cb, bool chain = true)
	{
		XsDotUsbDevice_addXsDotCallbackHandler(this, cb, chain);
	}

	inline void removeXsDotCallbackHandler(XsDotCallbackPlainC* cb, bool chain = true)
	{
		XsDotUsbDevice_removeXsDotCallbackHandler(this, cb, chain);
	}

private:
	XsDotUsbDevice(); // Constructor not implemented to prevent faulty memory allocation, object needs to be created by another object
	~XsDotUsbDevice(); //!< \brief Destructor not implemented, use destruct function instead (if available)
#ifndef SWIG
	void* operator new (size_t); //!< \brief new operator not implemented to prevent faulty memory allocation by user, use construct() function instead
	void* operator new[] (size_t); //!< \brief array new operator not implemented to prevent faulty memory allocation by user, use construct() function instead
	void operator delete (void*); //!< \brief delete operator not implemented to prevent faulty memory deallocation by user, use destruct() function instead
	void operator delete[] (void*); //!< \brief array delete operator not implemented to prevent faulty memory deallocation by user, use destruct() function instead
#endif
};
#endif // __cplusplus
#endif // GUARD_XSDOTUSBDEVICE_H_TMP
