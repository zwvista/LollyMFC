// ADOEvents.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ADOEvents.h"
#include <afxctl.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL CADODatabase2::AddEventHandler(CADOConnectionEventHandler *pHandler)
{
	m_pEvents = new CADOConnectionEvents(pHandler);
	return AfxConnectionAdvise(m_pConnection, __uuidof(ConnectionEvents), m_pEvents, TRUE, &m_dwEvents);
}

BOOL CADODatabase2::RemoveEventHandler()
{
	return AfxConnectionUnadvise(m_pConnection, __uuidof(ConnectionEvents), m_pEvents, TRUE, m_dwEvents);
}

CADODatabase2::CADODatabase2(CADOConnectionEventHandler *pHandler)
: CADODatabase()
, m_dwEvents(0)
, m_pEvents(NULL)
{
	if(pHandler) AddEventHandler(pHandler);
}

CADODatabase2::~CADODatabase2()
{
	if(m_dwEvents) RemoveEventHandler();
}

BOOL CADORecordset2::AddEventHandler(CADORecordsetEventHandler *pHandler)
{
	m_pEvents = new CADORecordsetEvents(pHandler);
	return AfxConnectionAdvise(m_pRecordset, __uuidof(RecordsetEvents), m_pEvents, TRUE, &m_dwEvents);
}

BOOL CADORecordset2::RemoveEventHandler()
{
	return AfxConnectionUnadvise(m_pRecordset, __uuidof(RecordsetEvents), m_pEvents, TRUE, m_dwEvents);
}

CADORecordset2::CADORecordset2(CADORecordsetEventHandler *pHandler)
: CADORecordset()
{
	m_dwEvents = 0;
	if(pHandler) AddEventHandler(pHandler);
}

CADORecordset2::CADORecordset2(CADODatabase* pAdoDatabase, CADORecordsetEventHandler *pHandler)
: CADORecordset(pAdoDatabase)
, m_dwEvents(0)
, m_pEvents(NULL)
{
	if(pHandler) AddEventHandler(pHandler);
}

CADORecordset2::~CADORecordset2()
{
	if(m_dwEvents) RemoveEventHandler();
}

BOOL CADORecordset2::Move(int nNumRecords, int nStart /* = CADORecordset2::bmCurrent */)
{
	_variant_t vtStart;
	vtStart.vt = VT_I2;
	vtStart.iVal = nStart;

	try
	{
		m_pRecordset->Move(nNumRecords, vtStart);
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}

BOOL CADORecordset2::GetFieldValue(int nIndex, CADORecordset *pRS)
{
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	try
	{
		pRS->m_pRecordset = m_pRecordset->Fields->GetItem(vtIndex)->Value;
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}

BOOL CADORecordset2::GetFieldValue(LPCTSTR lpFieldName, CADORecordset *pRS)
{
	try
	{
		pRS->m_pRecordset = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}

// Use this function as a replacement for CADORecordset::Execute
// if you want to receive recordset events.
BOOL CADORecordset2::OpenCommand(CADOCommand *pCmd)
{
	Close();

	try
	{
		m_pRecordset->CursorType = adOpenStatic;
		m_pRecordset->CursorLocation = adUseClient;
		m_pRecordset->Open(_variant_t((IDispatch*)pCmd->GetCommand(), TRUE), vtMissing,
			adOpenKeyset, adLockOptimistic, adCmdUnknown);
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}

	return m_pRecordset != NULL;
}

BOOL CADORecordset2::Delete(enum AffectEnum AffectRecords /* = adAffectCurrent */)
{
	try
	{
		if(m_pRecordset->Delete(AffectRecords) != S_OK)
			return FALSE;

		if(m_pRecordset->Update() != S_OK)
			return FALSE;
		
		m_nEditStatus = dbEditNone;
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}

CString CADORecordset2::GetFieldValueAsString( int nIndex )
{
	CString strValue;
	GetFieldValue(nIndex, strValue);
	return strValue;
}

CString CADORecordset2::GetFieldValueAsString( LPCTSTR lpFieldName )
{
	CString strValue;
	GetFieldValue(lpFieldName, strValue);
	return strValue;
}

int CADORecordset2::GetFieldValueAsInt( int nIndex )
{
	int nValue;
	GetFieldValue(nIndex, nValue);
	return nValue;
}

int CADORecordset2::GetFieldValueAsInt( LPCTSTR lpFieldName )
{
	int nValue;
	GetFieldValue(lpFieldName, nValue);
	return nValue;
}

bool CADORecordset2::GetFieldValueAsBool( int nIndex )
{
	bool bValue;
	GetFieldValue(nIndex, bValue);
	return bValue;
}

bool CADORecordset2::GetFieldValueAsBool( LPCTSTR lpFieldName )
{
	bool bValue;
	GetFieldValue(lpFieldName, bValue);
	return bValue;
}
