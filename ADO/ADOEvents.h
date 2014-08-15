// Module:   ADOEvents.h: 
// Date:     Jan 25, 2004
// Author:   Zhao Wei
// Mail to:  zwvista@sina.com
//
// To turn your Dialog/View class into an ADO Event handler class, follow these steps.
// 1. Add CADOConnectionEventHandler and/or CADORecordsetEventHandler to its deriving list.
//    eg.
//         class CMyDialog : public CDialog, protected CADOConnectionEventHandler
// 2. Override any/all of the virtual functions of class CADOConnectionEventHandler 
//    and/or class CADORecordsetEventHandler to handle some of the events you 
//    are interested in. All these virtual event handlers just default to doing 
//    nothing and cancelling further notifications.
// 3. Pass the address of your Dialog/View object as an argument to the 
//    constructor of CADODatabase2 objects and/or CADORecordset2 objects,
//    serving as the event source.
//    eg.  
//         CMyDialog dlg; // CMyDialog must derive from 
//                        // both CADOConnectionEventHandler and CADORecordsetEventHandler
//         CADODatabase2 db(&dlg);
//         CADORecordset2 rs(&db, &dlg);
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADOEVENTS_H__D222B712_242F_4D23_994B_B68A5A18F274__INCLUDED_)
#define AFX_ADOEVENTS_H__D222B712_242F_4D23_994B_B68A5A18F274__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ado.h"

class CADOConnectionEventHandler
{
public:
	virtual void InfoMessage( 
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Connection *pConnection)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void BeginTransComplete( 
		LONG TransactionLevel,
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Connection *pConnection)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void CommitTransComplete( 
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Connection *pConnection)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void RollbackTransComplete( 
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Connection *pConnection)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void WillExecute( 
		BSTR *Source,
		CursorTypeEnum *CursorType,
		LockTypeEnum *LockType,
		long *Options,
		EventStatusEnum *adStatus,
		struct _Command *pCommand,
		struct _Recordset *pRecordset,
		struct _Connection *pConnection)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void ExecuteComplete( 
		LONG RecordsAffected,
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Command *pCommand,
		struct _Recordset *pRecordset,
		struct _Connection *pConnection)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void WillConnect( 
		BSTR *ConnectionString,
		BSTR *UserID,
		BSTR *Password,
		long *Options,
		EventStatusEnum *adStatus,
		struct _Connection *pConnection)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void ConnectComplete( 
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Connection *pConnection)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void Disconnect( 
		EventStatusEnum *adStatus,
		struct _Connection *pConnection)
	{
		*adStatus = adStatusUnwantedEvent;
	}
};

//----The Connection events----------------------------------------------

class CADOConnectionEvents : public ConnectionEventsVt
{
private:
	ULONG   m_cRef;
protected:
	CADOConnectionEventHandler *m_pHandler;
public:
	CADOConnectionEvents(CADOConnectionEventHandler *pHandler) { m_cRef = 0; m_pHandler = pHandler; }
	~CADOConnectionEvents() {}

	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
	{
		*ppv = NULL;
		if (riid == __uuidof(IUnknown) || 
			riid == __uuidof(ConnectionEventsVt)) *ppv = this;
		if (*ppv == NULL)
			return ResultFromScode(E_NOINTERFACE);
		AddRef();
		return NOERROR;
	}
	STDMETHODIMP_(ULONG) AddRef(void) { return ++m_cRef; }
	STDMETHODIMP_(ULONG) Release(void)
	{ 
		if (0 != --m_cRef) return m_cRef;
		delete this;
		return 0;
	}

	STDMETHODIMP raw_InfoMessage( 
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Connection *pConnection)
	{
		m_pHandler->InfoMessage(pError, adStatus, pConnection);
		return S_OK;
	}

	STDMETHODIMP raw_BeginTransComplete( 
		LONG TransactionLevel,
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Connection *pConnection)
	{
		m_pHandler->BeginTransComplete(TransactionLevel, pError, adStatus, pConnection);
		return S_OK;
	}

	STDMETHODIMP raw_CommitTransComplete( 
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Connection *pConnection)
	{
		m_pHandler->CommitTransComplete(pError, adStatus, pConnection);
		return S_OK;
	}

	STDMETHODIMP raw_RollbackTransComplete( 
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Connection *pConnection)
	{
		m_pHandler->RollbackTransComplete(pError, adStatus, pConnection);
		return S_OK;
	}

	STDMETHODIMP raw_WillExecute( 
		BSTR *Source,
		CursorTypeEnum *CursorType,
		LockTypeEnum *LockType,
		long *Options,
		EventStatusEnum *adStatus,
		struct _Command *pCommand,
		struct _Recordset *pRecordset,
		struct _Connection *pConnection)
	{
		m_pHandler->WillExecute(Source, CursorType, LockType, Options, adStatus, pCommand, pRecordset, pConnection);
		return S_OK;
	}

	STDMETHODIMP raw_ExecuteComplete( 
		LONG RecordsAffected,
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Command *pCommand,
		struct _Recordset *pRecordset,
		struct _Connection *pConnection)
	{
		m_pHandler->ExecuteComplete(RecordsAffected, pError, adStatus, pCommand, pRecordset, pConnection);
		return S_OK;
	}

	STDMETHODIMP raw_WillConnect( 
		BSTR *ConnectionString,
		BSTR *UserID,
		BSTR *Password,
		long *Options,
		EventStatusEnum *adStatus,
		struct _Connection *pConnection)
	{
		m_pHandler->WillConnect(ConnectionString, UserID, Password, Options, adStatus, pConnection);
		return S_OK;
	}

	STDMETHODIMP raw_ConnectComplete( 
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Connection *pConnection)
	{
		m_pHandler->ConnectComplete(pError, adStatus, pConnection);
		return S_OK;
	}

	STDMETHODIMP raw_Disconnect( 
		EventStatusEnum *adStatus,
		struct _Connection *pConnection)
	{
		m_pHandler->Disconnect(adStatus, pConnection);
		return S_OK;
	}
};

class CADORecordsetEventHandler
{
public:
	virtual void WillChangeField( 
		LONG cFields,
		VARIANT Fields,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void FieldChangeComplete( 
		LONG cFields,
		VARIANT Fields,
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void WillChangeRecord( 
		EventReasonEnum adReason,
		LONG cRecords,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void RecordChangeComplete( 
		EventReasonEnum adReason,
		LONG cRecords,
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void WillChangeRecordset( 
		EventReasonEnum adReason,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void RecordsetChangeComplete( 
		EventReasonEnum adReason,
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void WillMove( 
		EventReasonEnum adReason,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void MoveComplete( 
		EventReasonEnum adReason,
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void EndOfRecordset( 
		VARIANT_BOOL *fMoreData,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void FetchProgress( 
		long Progress,
		long MaxProgress,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		*adStatus = adStatusUnwantedEvent;
	}

	virtual void FetchComplete( 
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		*adStatus = adStatusUnwantedEvent;
	}
};

//-----The Recordset events----------------------------------------------

class CADORecordsetEvents : public RecordsetEventsVt
{
private:
	ULONG m_cRef;   
protected:
	CADORecordsetEventHandler *m_pHandler;
public:
	CADORecordsetEvents(CADORecordsetEventHandler *pHandler) { m_cRef = 0; m_pHandler = pHandler; }
	~CADORecordsetEvents() {}

	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv) 
	{
		*ppv = NULL;
		if (riid == __uuidof(IUnknown) || 
			riid == __uuidof(RecordsetEventsVt)) *ppv = this;
		if (*ppv == NULL)
			return ResultFromScode(E_NOINTERFACE);
		AddRef();
		return NOERROR;
	}
	STDMETHODIMP_(ULONG) AddRef(void) { return ++m_cRef; }
	STDMETHODIMP_(ULONG) Release(void)   
	{ 
		if (0 != --m_cRef) return m_cRef;
		delete this;
		return 0;
	}

	STDMETHODIMP raw_WillChangeField( 
		LONG cFields,
		VARIANT Fields,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		m_pHandler->WillChangeField(cFields, Fields, adStatus, pRecordset);
		return S_OK;
	}

	STDMETHODIMP raw_FieldChangeComplete( 
		LONG cFields,
		VARIANT Fields,
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		m_pHandler->FieldChangeComplete(cFields, Fields, pError, adStatus, pRecordset);
		return S_OK;
	}

	STDMETHODIMP raw_WillChangeRecord( 
		EventReasonEnum adReason,
		LONG cRecords,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		m_pHandler->WillChangeRecord(adReason, cRecords, adStatus, pRecordset);
		return S_OK;
	}

	STDMETHODIMP raw_RecordChangeComplete( 
		EventReasonEnum adReason,
		LONG cRecords,
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		m_pHandler->RecordChangeComplete(adReason, cRecords, pError, adStatus, pRecordset);
		return S_OK;
	}

	STDMETHODIMP raw_WillChangeRecordset( 
		EventReasonEnum adReason,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		m_pHandler->WillChangeRecordset(adReason, adStatus, pRecordset);
		return S_OK;
	}

	STDMETHODIMP raw_RecordsetChangeComplete( 
		EventReasonEnum adReason,
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		m_pHandler->RecordsetChangeComplete(adReason, pError, adStatus, pRecordset);
		return S_OK;
	}

	STDMETHODIMP raw_WillMove( 
		EventReasonEnum adReason,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		m_pHandler->WillMove(adReason, adStatus, pRecordset);
		return S_OK;
	}

	STDMETHODIMP raw_MoveComplete( 
		EventReasonEnum adReason,
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		m_pHandler->MoveComplete(adReason, pError, adStatus, pRecordset);
		return S_OK;
	}

	STDMETHODIMP raw_EndOfRecordset( 
		VARIANT_BOOL *fMoreData,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		m_pHandler->EndOfRecordset(fMoreData, adStatus, pRecordset);
		return S_OK;
	}

	STDMETHODIMP raw_FetchProgress( 
		long Progress,
		long MaxProgress,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		m_pHandler->FetchProgress(Progress, MaxProgress, adStatus, pRecordset);
		return S_OK;
	}

	STDMETHODIMP raw_FetchComplete( 
		struct Error *pError,
		EventStatusEnum *adStatus,
		struct _Recordset *pRecordset)
	{
		m_pHandler->FetchComplete(pError, adStatus, pRecordset);
		return S_OK;
	}
};

class CADODatabase2 : public CADODatabase
{
public:
	CADODatabase2(CADOConnectionEventHandler *pHandler = NULL);
	virtual ~CADODatabase2(void);
protected:
	BOOL AddEventHandler(CADOConnectionEventHandler *pHandler);
	BOOL RemoveEventHandler();
	CADOConnectionEvents *m_pEvents;
	DWORD m_dwEvents;
};

class CADORecordset2 : public CADORecordset
{
public:
	enum cadoBookmarkEnum
	{
		bmCurrent = adBookmarkCurrent,
		bmFirst = adBookmarkFirst,
		bmLast = adBookmarkLast
	};
	CADORecordset2(CADORecordsetEventHandler *pHandler = NULL);
	CADORecordset2(CADODatabase* pAdoDatabase, CADORecordsetEventHandler *pHandler = NULL);
	virtual ~CADORecordset2(void);
	using CADORecordset::GetFieldValue;
	BOOL Move(int nNumRecords, int nStart = CADORecordset2::bmCurrent);
	BOOL GetFieldValue(int nIndex, CADORecordset *pRS);
	BOOL GetFieldValue(LPCTSTR lpFieldName, CADORecordset *pRS);
	BOOL OpenCommand(CADOCommand *pCmd);
	void CancelEdit(){m_nEditStatus = dbEditNone;}
	BOOL Delete(enum AffectEnum AffectRecords = adAffectCurrent);
	void Disconnect(){GetRecordset()->putref_ActiveConnection(NULL);}
	enum EditModeEnum GetEditMode(){return GetRecordset()->GetEditMode();}
	CString GetFieldValueAsString(int nIndex);
	CString GetFieldValueAsString(LPCTSTR lpFieldName);
	int GetFieldValueAsInt(int nIndex);
	int GetFieldValueAsInt(LPCTSTR lpFieldName);
	bool GetFieldValueAsBool(int nIndex);
	bool GetFieldValueAsBool(LPCTSTR lpFieldName);
protected:
	BOOL AddEventHandler(CADORecordsetEventHandler *pHandler);
	BOOL RemoveEventHandler();
	CADORecordsetEvents *m_pEvents;
	DWORD m_dwEvents;
};

#endif // !defined(AFX_ADOEVENTS_H__D222B712_242F_4D23_994B_B68A5A18F274__INCLUDED_)
