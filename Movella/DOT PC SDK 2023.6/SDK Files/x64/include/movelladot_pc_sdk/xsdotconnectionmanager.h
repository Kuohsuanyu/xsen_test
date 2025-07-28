
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

#ifndef GUARD_XSDOTCONNECTIONMANAGER_H_TMP
#define GUARD_XSDOTCONNECTIONMANAGER_H_TMP
#include <xstypes/xsdeviceidarray.h>
#include <xstypes/xsstringarray.h>
#include <xstypes/xsportinfo.h>
#include <xstypes/xsportinfoarray.h>
#include <xstypes/xsexception.h>
#include <xstypes/xsversion.h>
#include "xsdotcallbackplainc.h"
#ifdef __cplusplus
extern "C" {
#endif
/*! \addtogroup cinterface C Interface
	@{ */
struct XsDotConnectionManager;
#ifndef __cplusplus
typedef struct XsDotConnectionManager XsDotConnectionManager;
#endif
XDDA_DLL_API struct XsDotConnectionManager* XsDotConnectionManager_construct(void);
XDDA_DLL_API void XsDotConnectionManager_destruct(struct XsDotConnectionManager* thisPtr);/*!< \copydoc XsDotConnectionManager::~XsDotConnectionManager()*/
XDDA_DLL_API XsVersion* XsDotConnectionManager_version(XsVersion* returnValue);/*!< \copydoc XsDotConnectionManager::version()*/
XDDA_DLL_API void XsDotConnectionManager_reset(struct XsDotConnectionManager* thisPtr);/*!< \copydoc XsDotConnectionManager::reset()*/
XDDA_DLL_API XsStringArray* XsDotConnectionManager_getAvailableBluetoothAdapters(struct XsDotConnectionManager* thisPtr, XsStringArray* returnValue);/*!< \copydoc XsDotConnectionManager::getAvailableBluetoothAdapters()*/
XDDA_DLL_API int XsDotConnectionManager_setPreferredBluetoothAdapter(struct XsDotConnectionManager* thisPtr, XsString adapterName);/*!< \copydoc XsDotConnectionManager::setPreferredBluetoothAdapter(XsString)*/
XDDA_DLL_API int XsDotConnectionManager_enableDeviceDetection(struct XsDotConnectionManager* thisPtr);/*!< \copydoc XsDotConnectionManager::enableDeviceDetection()*/
XDDA_DLL_API void XsDotConnectionManager_disableDeviceDetection(struct XsDotConnectionManager* thisPtr);/*!< \copydoc XsDotConnectionManager::disableDeviceDetection()*/
XDDA_DLL_API XsPortInfoArray* XsDotConnectionManager_detectUsbDevices(struct XsDotConnectionManager* thisPtr, XsPortInfoArray* returnValue);/*!< \copydoc XsDotConnectionManager::detectUsbDevices()*/
XDDA_DLL_API int XsDotConnectionManager_openPort(struct XsDotConnectionManager* thisPtr, XsPortInfo* portinfo);/*!< \copydoc XsDotConnectionManager::openPort(XsPortInfo&)*/
XDDA_DLL_API void XsDotConnectionManager_closePort(struct XsDotConnectionManager* thisPtr, const XsPortInfo* portinfo);/*!< \copydoc XsDotConnectionManager::closePort(const XsPortInfo&)*/
XDDA_DLL_API XsDotDevice* XsDotConnectionManager_device(const struct XsDotConnectionManager* thisPtr, const XsDeviceId* deviceId);/*!< \copydoc XsDotConnectionManager::device(const XsDeviceId&) const*/
XDDA_DLL_API XsDotUsbDevice* XsDotConnectionManager_usbDevice(const struct XsDotConnectionManager* thisPtr, const XsDeviceId* deviceId);/*!< \copydoc XsDotConnectionManager::usbDevice(const XsDeviceId&) const*/
XDDA_DLL_API void XsDotConnectionManager_close(struct XsDotConnectionManager* thisPtr);/*!< \copydoc XsDotConnectionManager::close()*/
XDDA_DLL_API XsResultValue* XsDotConnectionManager_lastResult(const struct XsDotConnectionManager* thisPtr, XsResultValue* returnValue);/*!< \copydoc XsDotConnectionManager::lastResult() const*/
XDDA_DLL_API XsString* XsDotConnectionManager_lastResultText(const struct XsDotConnectionManager* thisPtr, XsString* returnValue);/*!< \copydoc XsDotConnectionManager::lastResultText() const*/
XDDA_DLL_API int XsDotConnectionManager_startLogging(struct XsDotConnectionManager* thisPtr);/*!< \copydoc XsDotConnectionManager::startLogging()*/
XDDA_DLL_API int XsDotConnectionManager_stopLogging(struct XsDotConnectionManager* thisPtr);/*!< \copydoc XsDotConnectionManager::stopLogging()*/
XDDA_DLL_API int XsDotConnectionManager_closeLogFile(struct XsDotConnectionManager* thisPtr);/*!< \copydoc XsDotConnectionManager::closeLogFile()*/
XDDA_DLL_API int XsDotConnectionManager_startSync(struct XsDotConnectionManager* thisPtr, const XsString* rootNodeMac);/*!< \copydoc XsDotConnectionManager::startSync(const XsString&)*/
XDDA_DLL_API int XsDotConnectionManager_stopSync(struct XsDotConnectionManager* thisPtr);/*!< \copydoc XsDotConnectionManager::stopSync()*/
XDDA_DLL_API void XsDotConnectionManager_clearXsDotCallbackHandlers(struct XsDotConnectionManager* thisPtr, int chain);/*!< \copydoc XsDotConnectionManager::clearXsDotCallbackHandlers(bool)*/
XDDA_DLL_API void XsDotConnectionManager_addXsDotCallbackHandler(struct XsDotConnectionManager* thisPtr, XsDotCallbackPlainC* cb, int chain);/*!< \copydoc XsDotConnectionManager::addXsDotCallbackHandler(XsDotCallbackPlainC*,bool)*/
XDDA_DLL_API void XsDotConnectionManager_removeXsDotCallbackHandler(struct XsDotConnectionManager* thisPtr, XsDotCallbackPlainC* cb, int chain);/*!< \copydoc XsDotConnectionManager::removeXsDotCallbackHandler(XsDotCallbackPlainC*,bool)*/
/*! @} */
#ifdef __cplusplus
} // extern "C"
struct XsDotConnectionManager {
	//! \brief Construct a new XsDotConnectionManager* object. Clean it up with the destruct() function or delete the object
	inline static XsDotConnectionManager* construct(void)
	{
		return XsDotConnectionManager_construct();
	}

	//! \brief Destruct a XsDotConnectionManager object and free all memory allocated for it
	inline void destruct(void)
	{
		XsDotConnectionManager_destruct(this);
	}

	inline static XsVersion version(void)
	{
		XsVersion returnValue;
		return *XsDotConnectionManager_version(&returnValue);
	}

	inline void reset(void)
	{
		XsDotConnectionManager_reset(this);
	}

	inline XsStringArray getAvailableBluetoothAdapters(void)
	{
		XsStringArray returnValue;
		return *XsDotConnectionManager_getAvailableBluetoothAdapters(this, &returnValue);
	}

	inline bool setPreferredBluetoothAdapter(XsString adapterName)
	{
		return 0 != XsDotConnectionManager_setPreferredBluetoothAdapter(this, adapterName);
	}

	inline bool enableDeviceDetection(void)
	{
		return 0 != XsDotConnectionManager_enableDeviceDetection(this);
	}

	inline void disableDeviceDetection(void)
	{
		XsDotConnectionManager_disableDeviceDetection(this);
	}

	inline XsPortInfoArray detectUsbDevices(void)
	{
		XsPortInfoArray returnValue;
		return *XsDotConnectionManager_detectUsbDevices(this, &returnValue);
	}

	inline bool openPort(XsPortInfo& portinfo)
	{
		return 0 != XsDotConnectionManager_openPort(this, &portinfo);
	}

	inline void closePort(const XsPortInfo& portinfo)
	{
		XsDotConnectionManager_closePort(this, &portinfo);
	}

	inline XsDotDevice* device(const XsDeviceId& deviceId) const
	{
		return XsDotConnectionManager_device(this, &deviceId);
	}

	inline XsDotUsbDevice* usbDevice(const XsDeviceId& deviceId) const
	{
		return XsDotConnectionManager_usbDevice(this, &deviceId);
	}

	inline void close(void)
	{
		XsDotConnectionManager_close(this);
	}

	inline XsResultValue lastResult(void) const
	{
		XsResultValue returnValue;
		return *XsDotConnectionManager_lastResult(this, &returnValue);
	}

	inline XsString lastResultText(void) const
	{
		XsString returnValue;
		return *XsDotConnectionManager_lastResultText(this, &returnValue);
	}

	inline bool startLogging(void)
	{
		return 0 != XsDotConnectionManager_startLogging(this);
	}

	inline bool stopLogging(void)
	{
		return 0 != XsDotConnectionManager_stopLogging(this);
	}

	inline bool closeLogFile(void)
	{
		return 0 != XsDotConnectionManager_closeLogFile(this);
	}

	inline bool startSync(const XsString& rootNodeMac)
	{
		return 0 != XsDotConnectionManager_startSync(this, &rootNodeMac);
	}

	inline bool stopSync(void)
	{
		return 0 != XsDotConnectionManager_stopSync(this);
	}

	inline void clearXsDotCallbackHandlers(bool chain = true)
	{
		XsDotConnectionManager_clearXsDotCallbackHandlers(this, chain);
	}

	inline void addXsDotCallbackHandler(XsDotCallbackPlainC* cb, bool chain = true)
	{
		XsDotConnectionManager_addXsDotCallbackHandler(this, cb, chain);
	}

	inline void removeXsDotCallbackHandler(XsDotCallbackPlainC* cb, bool chain = true)
	{
		XsDotConnectionManager_removeXsDotCallbackHandler(this, cb, chain);
	}

	//! \brief Destructor, calls destruct() function to clean up object
	~XsDotConnectionManager()
	{
		XsDotConnectionManager_destruct(this);
	}

	//! \brief overloaded delete operator to allow user to use delete instead of calling destruct() function
	void operator delete (void*)
	{
	}

private:
	XsDotConnectionManager(); // Default constructor not implemented to prevent faulty memory allocation, use construct function instead
#ifndef SWIG
	void* operator new (size_t); //!< \brief new operator not implemented to prevent faulty memory allocation by user, use construct() function instead
	void* operator new[] (size_t); //!< \brief array new operator not implemented to prevent faulty memory allocation by user, use construct() function instead
	void operator delete[] (void*); //!< \brief array delete operator not implemented to prevent faulty memory deallocation by user, use destruct() function instead
#endif
};
#endif // __cplusplus
#endif // GUARD_XSDOTCONNECTIONMANAGER_H_TMP
