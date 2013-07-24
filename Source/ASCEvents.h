/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCEvents.h
 *  Content: ASC(Asphyre Sphinx for C++) Event System implementing observer pattern
 *  Date   : 2013/07/23
 *  Desc   : 1) Event foundations based on observer pattern using subscription mechanism
 *           2) Common ASC events that deal with device creation, resource initialization
 *              and GUI management
 *
 *******************************************************************************************/

#pragma once

#include "ASCTypes.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

/*
 * Event callback function used by the majority of ASC events.
 *
 * Params:
 *   pSender: Sender class used in event invocation, in certain situations
 *            this parameter may be set to nil
 *   pParam: Event-specific parameter that may be optionally passed by
 *           the notifier class. In most cases this parameter has value of nil
 *   bHandled: Determines whether to continue sending event to all other
 *             subscribed classes. If this parameter is set to True, the event handling
 *             will be finished and further subscribers will not be notified
*/
typedef void (*CASCEventCallback)(const void* pSender, const CASCPointer pParam, ASCBoolean* bHandled);

typedef struct CASCEventRec
{
	ASCInt				m_nEventID;
	CASCEventCallback	m_Callback;
	string				m_sClassName;
	ASCInt				m_nPriority1;
	ASCInt				m_nPriority2;
} *PASCEventRec;

class CASCEventProvider;
class CASCEventProviders;

/*
 * Event validation callback function. This function is called to filter
 * subscribers that should or should not receive the current event. 
 * For instance, when some button is clicked, only the class handling the active
 * window should receive and handle the event; such validation should be done
 * inside this event.
 *
 * Params:
 *   pProvider: Reference to the provider that was used in event invocation
 *   pSender: Sender class that called the event. For instance, when a button
 *            is clicked, the sender will be the button class itself. In some
 *            cases, this parameter may be nil
 *   pParam: Application-specific parameter sent by the caller class. This
 *           parameter is typically set to nil
 *   sClassName: The name of the subscriber class that is currently being
 *               validated to receive the event
 *   bAllowed: Determines whether the class that is currently being validated 
 *             should receive the event or not. This parameter is by default 
 *             set to True; setting it to False inside this event will skip the
 *             class identified by ClassName for the current event
*/
typedef void (*CASCEventValidator)(CASCEventProvider* pProvider, void* pSender, ASCPointer pParam, const string& sClassName, ASCBoolean* bAllowed);

/*
 * Event subscription class implementing observer pattern. 
 * Recipient classes are added to the subscriber list to receive notifactions of events
 * sent by other classes
*/
class CASCEventProvider
{
public:
	CASCEventProvider(const CASCEventProviders* pOwner);
	~CASCEventProvider();

	/*
	 * Subscribes the given class and its event handling callback to receive all
	 * events sent from this provider. If the priority is set, the class will
	 * receive events depending on the list of priorities either after or
	 * before other events depending on priority settings. This function returns
	 * the identification number of the subscribed function, which can later be
	 * used to unsubscribe from this provider using UnSubscribe method
	*/
	ASCInt					Subscribe(const string& sClassName, const CASCEventCallback CallBack, const ASCInt nPriority = -1);

	/*
	 * Unsubscribes the event callback function registered using the specified
	 * ID, which is usually returned by Subscribe function
	*/
	void					UnSubscribe(const ASCInt nEventID);

	/*
	 * Unsubscribes all event callbacks registered to the specified class
	*/
	void					UnSubscribeClass(const string& sClassName);

	/*
	 * Sets the secondary priority of all event callback functions registered to
	 * the specified class. This secondary priority is used after the first
	 * primary priority is applied(the one passed to Subscribe function)
	*/
	ASCBoolean				SetClassPriority(const string& sClassName, const ASCInt nPriority);
	
	/*
	 * Marks the priority list of all event callback functions dirty, so it is
	 * refreshed next time an event occurs. This should be called after changing
	 * the priority of some class
	*/
	void					MarkEventListDirty();

	/*
	 * Send event notification to all subscribed classes and their callback
	 * functions, filtered through EventValidator event
	*/
	ASCBoolean				Notify(const void* pSender, const ASCPointer pParam = 0);

	/*
	 * The owner class that contains the list of all existing providers
	*/
	CASCEventProviders*		GetOwner();

	/*
	 * Event validation callback that filters which events should be received
	 * by which classes depending on different circumstances
	*/
	CASCEventValidator		GetEventValidator();
	void					SetEventValidator(const CASCEventValidator Value);
private:
	CASCEventProviders		m_pOwner;
	vector<CASCEventRec>	m_Datas;
	ASCBoolean				m_bEventListDirty;
	CASCEventValidator		m_EventValidator;

	ASCInt					NextEventID();
	ASCInt					IndexOfEventID(const ASCInt nID);

	void					RemoveAll();
	void					Remove(ASCInt nIndex);

	void					EventListSwap(ASCInt nIndex1, ASCInt nIndex2);
	ASCInt					EventListCompare(ASCInt nIndex1, ASCInt nIndex2);
	ASCInt					EventListSplit(ASCInt nStart, ASCInt nStop);
	void					EventListSort(ASCInt nStart, ASCInt nStop);
	void					UpdateEventList();
};

/*
 * List of all available event providers based on observer pattern where
 * subscriber classes are notified of the events sent by other classes
*/
class CASCEventProviders
{
public:
	CASCEventProviders();
	~CASCEventProviders();

	// Adds a new provider to the end of the list and returns the pointer to its class
	CASCEventProvider*		Add();
	// Inserts a new provider to the end of the list and returns its index
	ASCInt					Insert();
	// Removes provider at the specified index from the list
	void					Remove(ASCInt nIndex);
	// Removes all providers from the list
	void					Clear();
	// Returns the index of the specified provider in the list
	ASCInt					IndexOf(const CASCEventProvider* Provider);
	/*
	 * Notifies all providers that their event priority list is dirty and should
	 * be updated the next time an event occurs. This can occur when the ordering
	 * list has been changed
	*/
	void					MarkEventListsDirty();
	/*
	 * Unsubscribes all existing event callback functions for the specified class
	 * from all existing providers
	*/
	void					UnSubscribe(const string& sClassName);

	// Number of registered provider classes in the list
	ASCInt					GetItemCount();
	// Access to individual provider classes in the list
	CASCEventProvider*		GetItem(ASCInt nIndex);
private:
	vector<CASCEventProvider*>	m_Datas;
	ASCBoolean					m_bEventListsDirty;
	ASCBoolean					m_bListSemaphore;
};

extern inline CASCEventProviders* ASCEventProviders();

/*
 * Declarations of important ASC events
 */

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