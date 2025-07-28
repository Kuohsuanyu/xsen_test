
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

// include this file in Visual Studio using C/C++->Advanced->Force Includes (the /FI option)
#ifndef DOTSDK_CONFIG_H
#define DOTSDK_CONFIG_H

//////////////////////////////////////////////////
// generic preprocessor defines

// make sure both _WIN32 and WIN32 are defined if either of them is.
#if defined(_WIN32) || defined(_M_IX86)
#	ifndef WIN32
#		define WIN32
#	endif
#	define XSENS_WINDOWS
#endif

#ifdef WIN32
#	ifndef _WIN32
#		define _WIN32
#		define XSENS_WINDOWS
#	endif
#endif

// make things as secure as possible without modifying the code...
#ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef __GNUC__
#include <limits.h>
#if __WORDSIZE == 64
#	define XSENS_64BIT
#else
#	define XSENS_32BIT
#endif
#endif

#if defined(_WIN64) || defined(_M_X64) || defined(_M_IA64)
#	ifndef XSENS_64BIT
#		define XSENS_64BIT
#	endif
#	ifndef XSENS_WINDOWS
#		define XSENS_WINDOWS
#	endif
#	ifndef WIN64
#		define WIN64
#	endif
#else
#	ifndef XSENS_32BIT
#		define XSENS_32BIT
#	endif
#endif

// all xsens libraries should use unicode
#ifndef UNICODE
#define UNICODE
#endif

// use XSENS_32BIT and XSENS_64BIT to check for 32/64 bit builds in your application
// on non-windows systems these should be defined in this file

/*
Configuration | Runtime | DebInfo | Defines
--------------+---------------------------------------
Debug         | MDd     | Yes     | XSENS_DEBUG;_DEBUG
RelWithDeb    | MD      | Yes     | XSENS_DEBUG;XSENS_RELEASE;NDEBUG
Release       | MD      | No      | XSENS_RELEASE;NDEBUG

The common way to setup configuration-dependent defines:
#if defined(XSENS_DEBUG)
	//// Debug or RelWithDeb build
	#if defined(XSENS_RELEASE)
		//// RelWithDeb build
	#else
		//// Debug build
	#endif
#else
	//// Release build
#endif
*/

//////////////////////////////////////////////////
// more generic preprocessor defines

//////////////////////////////////////////////////
// generic preprocessor defines
#include <xscontroller/xscontrollerconfig.h>

#ifndef XDDA_DLL_API
#	ifdef XDDA_DLL_EXPORT
#		ifdef _WIN32
//#			pragma message("XDDA_DLL_API export in dotsdkconfig.h")
#			define XDDA_DLL_API __declspec(dllexport)
#		else
//#			pragma message("XDDA_DLL_API linux export in dotsdkconfig.h")
#			define XDDA_DLL_API __attribute__((visibility("default")))
#		endif
#	else	// ifdef XDA_DLL_EXPORT
#		ifdef XDDA_STATIC_LIB
//#			pragma message("XDDA_DLL_API static in dotsdkconfig.h")
#			define XDDA_DLL_API
#		else
#			ifdef _WIN32
//#				pragma message("XDDA_DLL_API import in dotsdkconfig.h")
#				define XDDA_DLL_API __declspec(dllimport)
#			else
//#				pragma message("XDDA_DLL_API import/static for linux in dotsdkconfig.h")
#				define XDDA_DLL_API
#			endif
#		endif
#	endif	// ifdef XDDA_DLL_EXPORT - else
#endif	// ifndef XDDA_DLL_API

// use XSENS_32BIT and XSENS_64BIT to check for 32/64 bit builds in your application
// on non-windows systems these should be defined

#if !defined(XSENS_RELEASE)
//////////////////////////////////////////////////
// stuff for debug mode

#else
// non-debug stuff
//
#endif

//////////////////////////////////////////////////
// more generic preprocessor defines

// Some macros used by interface generator
#ifndef XSNOEXPORT
#define XSNOEXPORT
#endif
#ifndef XSNOLINUXEXPORT
#define XSNOLINUXEXPORT
#endif
#ifndef XSNOWINDOWSEXPORT
#define XSNOWINDOWSEXPORT
#endif
#ifndef XSNOMACEXPORT
#define XSNOMACEXPORT
#endif
#ifndef XSNOCOMEXPORT
#define XSNOCOMEXPORT
#endif
#ifndef XSINTEXP
#define XSINTEXP
#endif

#endif
