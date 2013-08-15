/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCEvents.h
 *  Content: ASC(Asphyre Sphinx for C++) important event declarations
 *  Date   : 2013/07/23
 *  Desc   : Declarations of important ASC events
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include "ASCTypes.h"
#include "ASCEventSystem.h"

/*
 * creation event, where all ASC components should be created
*/
extern inline CASCEventProvider* ASCCreateEvent();

/*
 * release event, where all ASC components should be released
 */
extern inline CASCEventProvider* ASCDestroyEvent();

/*
 * Device initialization event, where device and swap chain configuration should
 * be specified. This event occurs right before ASC device is about to be initialized
*/
extern inline CASCEventProvider* ASCDeviceInitEvent();

/*
 * Device creation event, which occurs right after the device has been initialized. 
 * In this event it is possible to load some essential artwork and fonts
*/
extern inline CASCEventProvider* ASCDeviceCreateEvent();

/*
 * Device finalization event, which occurs right before the device is to be
 * finalized to release all other device-dependant resources such as images and fonts
*/
extern inline CASCEventProvider* ASCDeviceDestroyEvent();

/*
 * Device reset event, which occurs either after device has been initialized or
 * recovered from lost scenario. In this event all volatile device-dependant
 * resources should be created
*/
extern inline CASCEventProvider* ASCDeviceResetEvent();

/*
 * Device lost event, which occurs either before the device is to be finalized
 * or when the device has been lost. In this event all volatile device-dependant
 * resources should be released
*/
extern inline CASCEventProvider* ASCDeviceLostEvent();

/*
 * Start of rendering scene event, which occurs when the scene is being
 * prepared to be rendered. In this event the necessary device states can be updated
*/
extern inline CASCEventProvider* ASCBeginSceneEvent();

/*
 * End of rendering scene event, which occurs when the scene has finished
 * rendering and is about to be presented on the screen(or render target).
 * In this event it is necessary to finish all cached rendering processes
*/
extern inline CASCEventProvider* ASCEndSceneEvent();

/*
 * Timer reset event, which occurs when a time-consuming operation has
 * taken place so it is necessary to reset the timer to prevent stalling
*/
extern inline CASCEventProvider* ASCTimerResetEvent();