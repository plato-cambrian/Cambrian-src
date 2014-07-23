#ifndef PRECOMPILEDHEADERS_H
	#include "PreCompiledHeaders.h"
#endif
#include "TApplicationBallotmaster.h"
#include "IEventBallot.h"
#include "WLayoutBrowser.h"

void
DisplayApplicationBallotMaster()
	{
	TProfile * pProfile = NavigationTree_PGetSelectedTreeItemMatchingInterfaceTProfile();	// This is a bit of a hack, however this dialog should appear only when a profile is selected
	if (pProfile == NULL)
		return;
	TApplicationBallotmaster * pApplication = pProfile->PGetApplicationBallotmaster_NZ();
	pApplication->TreeItemW_SelectWithinNavigationTree();
	/*
	CStr strPathApplication = "file:///" + g_oConfiguration.SGetPathOfFileName("Apps/Ballotmaster/default.htm");
	TBrowser * pBrowser = new TBrowser(&g_oConfiguration);
	g_oConfiguration.m_arraypaBrowsers.Add(PA_CHILD pBrowser);
	pBrowser->SetIconNameAndUrl(eMenuAction_DisplayBallotMaster, "Ballotmaster", strPathApplication);
	pBrowser->TreeItemBrowser_DisplayWithinNavigationTree();
	pBrowser->TreeItemW_SelectWithinNavigationTree();
	*/
	}


//	This function must have the same interface as PFn_PaAllocateApplication()
IApplication *
PaAllocateApplicationBallotmaster(TProfile * pProfileParent)
	{
	return new TApplicationBallotmaster(pProfileParent);
	}

TApplicationBallotmaster *
TProfile::PGetApplicationBallotmaster_NZ()
	{
	TApplicationBallotmaster * pApplication = (TApplicationBallotmaster *)m_arraypaApplications.PFindRuntimeObject(RTI(TApplicationBallotmaster));
	if (pApplication == NULL)
		{
		pApplication = (TApplicationBallotmaster *)PaAllocateApplicationBallotmaster(this);
		m_arraypaApplications.Add(PA_CHILD pApplication);
		pApplication->TreeItemApplication_DisplayWithinNavigationTree();
		}
	return pApplication;
	}


CBinXcpStanzaEventCopier::CBinXcpStanzaEventCopier(ITreeItemChatLogEvents * pContactOrGroup) : CBinXcpStanza(eStanzaType_zInformation)
	{
	TAccountXmpp * pAccount = pContactOrGroup->m_pAccount;
	m_pContact = m_paContact = new TContact(pAccount);	// We will serialize using a dummy contact
	m_pContact->m_strNameDisplayTyped = m_pContact->m_pAccount->m_pProfileParent->m_strNameProfile;	// Use the profile name as the contact so the preview looks like someone is receiving the event from the sender
	}

CBinXcpStanzaEventCopier::~CBinXcpStanzaEventCopier()
	{
	delete m_paContact;
	}

void
CBinXcpStanzaEventCopier::EventCopy(IN const IEvent * pEventSource, OUT IEvent * pEventDestination)
	{
	Assert(pEventSource != NULL);
	Assert(pEventSource->m_pVaultParent_NZ != NULL);
	BinXmlSerializeEventForXcpCore(pEventSource, d_ts_zNA);	// TODO: Need to use XmlSerializeCore() instead

	CXmlTree oXmlTree;
	(void)oXmlTree.EParseFileDataToXmlNodesCopy_ML(IN *this);
	pEventDestination->m_pVaultParent_NZ = pEventSource->m_pVaultParent_NZ;	// Use the same vault as the source event
	pEventDestination->XmlUnserializeCore(IN &oXmlTree);
	}

IEvent *
CBinXcpStanzaEventCloner::PaEventClone(IEvent * pEventToClone)
	{
	Assert(pEventToClone != NULL);

	/*
		CVaultEvents * pVault = m_pContact->Vault_PGet_NZ();	// Get an empty vault from the dummy contact
	paEventSent->m_pVaultParent_NZ = pVault;				// We need a valid pointer because the event may need to access the vault, contact or account
	*/
	return 0;
	}

void
TProfile::BallotMaster_EventBallotAddAsTemplate(IEventBallot * pEventBallot)
	{
	PGetApplicationBallotmaster_NZ()->EventBallotAddAsTemplate(pEventBallot);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
TApplicationBallotmaster::TApplicationBallotmaster(TProfile * pProfileParent) : IApplication(pProfileParent, eMenuIconVote)
	{
	m_paVaultBallots = NULL;
	m_paContactDummy = NULL;
	m_pawLayoutBrowser = NULL;
	}

TApplicationBallotmaster::~TApplicationBallotmaster()
	{
	delete m_pawLayoutBrowser;
	delete m_paContactDummy;
	}

void
TApplicationBallotmaster::XmlExchange(INOUT CXmlExchanger * pXmlExchanger)
	{
	IApplication::XmlExchange(INOUT pXmlExchanger);
	pXmlExchanger->XmlExchangeStr("URL", INOUT &m_strUrlAddress);
	CVaultEvents * pVault = PGetVault_NZ();
	pVault->XmlExchange("Ballots", INOUT pXmlExchanger);
	}

const EMenuActionByte c_rgzeActionsMenuApplication[] =
	{
	eMenuAction_ApplicationHide,
	ezMenuActionNone
	};

//	TApplicationBallotmaster::ITreeItem::TreeItem_MenuAppendActions()
void
TApplicationBallotmaster::TreeItem_MenuAppendActions(IOUT WMenu * pMenu)
	{
	pMenu->ActionsAdd(c_rgzeActionsMenuApplication);
	}

//	TApplicationBallotmaster::ITreeItem::TreeItem_EDoMenuAction()
EMenuAction
TApplicationBallotmaster::TreeItem_EDoMenuAction(EMenuAction eMenuAction)
	{
	switch (eMenuAction)
		{
	case eMenuAction_ApplicationHide:
		TreeItemW_Hide();
		return ezMenuActionNone;
	default:
		return IApplication::TreeItem_EDoMenuAction(eMenuAction);
		}
	}

//	TApplicationBallotmaster::ITreeItem::TreeItem_GotFocus()
void
TApplicationBallotmaster::TreeItem_GotFocus()
	{
	if (m_strUrlAddress.FIsEmptyString())
		m_strUrlAddress = "file:///" + m_pProfileParent->m_pConfigurationParent->SGetPathOfFileName("Apps/Ballotmaster/default.htm");
	if (m_pawLayoutBrowser == NULL)
		m_pawLayoutBrowser = new WLayoutBrowser(m_pProfileParent, INOUT_LATER &m_strUrlAddress);
	MainWindow_SetCurrentLayout(IN m_pawLayoutBrowser);
	}


//	This method is somewhat a hack to create a vault to store events. At the moment, Cambrian needs something to store the ballots.
CVaultEvents *
TApplicationBallotmaster::PGetVault_NZ()
	{
	if (m_paVaultBallots == NULL)
		{
		if (m_paContactDummy == NULL)
			{
			TAccountXmpp * pAccount = (TAccountXmpp *)m_pProfileParent->m_arraypaAccountsXmpp.PvGetElementFirst_YZ();
			if (pAccount == NULL)
				pAccount = new TAccountXmpp(m_pProfileParent);	// This will cause a memory leak.  At the moment, I just want the code to work as a proof of concept
			m_paContactDummy = new TContact(pAccount);
			}
		SHashSha1 hashFileName;
		InitToZeroes(OUT &hashFileName, sizeof(hashFileName));	// This also needs to be fixed
		m_paVaultBallots = new CVaultEvents(m_paContactDummy, IN &hashFileName);
		}
	return m_paVaultBallots;

	}
CEventBallotSent *
TApplicationBallotmaster::PAllocateBallot()
	{
	CEventBallotSent * paEventBallot = new CEventBallotSent();
	CVaultEvents * pVault = PGetVault_NZ();
	paEventBallot->m_pVaultParent_NZ = pVault;
	pVault->m_arraypaEvents.Add(PA_CHILD paEventBallot);
	return paEventBallot;
	}

void
TApplicationBallotmaster::EventBallotAddAsTemplate(IEventBallot * pEventBallot)
	{
	CVaultEvents * pVault = PGetVault_NZ();	// Get the vault first because it will initialize m_paContactDummy
	CBinXcpStanzaEventCopier binXcpStanzaCopier(m_paContactDummy);
	CEventBallotSent * paEventBallotTemplate = new CEventBallotSent();	// When adding a template, always use the 'sent' ballot
	binXcpStanzaCopier.EventCopy(IN pEventBallot, OUT paEventBallotTemplate);
	paEventBallotTemplate->m_pVaultParent_NZ = pVault;
	pVault->m_arraypaEvents.Add(PA_CHILD paEventBallotTemplate);
	}

void
TApplicationBallotmaster::ApiBallotSave(IN PSZUC pszXmlBallot)
	{
	CVaultEvents * pVault = PGetVault_NZ();
	pVault->EventsUnserialize(pszXmlBallot);
	}

void
TApplicationBallotmaster::ApiBallotsList(OUT CBin * pbinXmlBallots)
	{
	CBinXcpStanzaTypeInfo binXmlEvents;
	CVaultEvents * pVault = PGetVault_NZ();
	pVault->EventsSerializeForMemory(IOUT &binXmlEvents);
	pbinXmlBallots->BinInitFromCBinStolen(INOUT &binXmlEvents);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
OPoll::OPoll(CEventBallotSent * pBallot)
	{
	Assert(pBallot != NULL);
	Assert(pBallot->EGetEventClass() == CEventBallotSent::c_eEventClass);
	m_pBallot = pBallot;
	//m_sTitle = "?";	// For debugging
	}
OPoll::~OPoll()
	{
	MessageLog_AppendTextFormatSev(eSeverityNoise, "OPoll::~OPoll() 0x$p\n", m_pBallot);
	}

QString
Timestamp_ToStringBase85(TIMESTAMP ts)
	{
	CHU szTimestamp[16];
	Timestamp_CchToString(IN ts, OUT szTimestamp);
	return CString(szTimestamp);
	}

QString
OPoll::id() const
	{
	return Timestamp_ToStringBase85(m_pBallot->m_tsEventID);
	}

QString
OPoll::title() const
	{
	return m_pBallot->m_strTitle;
	}
void
OPoll::title(QString & sTitle)
	{
	MessageLog_AppendTextFormatCo(d_coBlue, "OPoll::title($Q)\n", &sTitle);
	m_pBallot->m_strTitle = sTitle;
	}

QString
OPoll::description() const
	{
	return m_pBallot->m_strDescription;
	}
void
OPoll::description(QString & sDescription)
	{
	m_pBallot->m_strDescription = sDescription;
	}

void
OPoll::save()
	{
	MessageLog_AppendTextFormatCo(d_coBlue, "OPoll::save($Q)\n", &m_sTitle);
	}


///////////////////////////////////////////////////////////////////////////////////////////////////
OPolls::OPolls(OCambrian * poCambrian)
	{
	Assert(poCambrian != NULL);
	m_pBallotmaster = poCambrian->m_pProfile->PGetApplicationBallotmaster_NZ();
	Assert(m_pBallotmaster->EGetRuntimeClass() == RTI(TApplicationBallotmaster));
	}

OPolls::~OPolls()
	{
	}

//	build(), slot
//
//	Ideally the name new() should be used, however since new() is a C++ operator (reserved keyword), we cannot use it.
QVariant
OPolls::build()
	{
	MessageLog_AppendTextFormatCo(d_coBlue, "OPolls::build()\n");
	return QVariant::fromValue(PA_CHILD new OPoll(IN m_pBallotmaster->PAllocateBallot()));
	}



//	list(), slot
//	I think list() should be a property rather than a method.
QVariant
OPolls::list()
	{
	QVariantList oList;
	CVaultEvents * pVaultPolls = m_pBallotmaster->PGetVault_NZ();
	IEvent ** ppEventStop;
	IEvent ** ppEvent = pVaultPolls->m_arraypaEvents.PrgpGetEventsStop(OUT &ppEventStop);
	while (ppEvent != ppEventStop)
		{
		CEventBallotSent * pEvent = (CEventBallotSent *)*ppEvent++;
		oList.append(QVariant::fromValue(PA_CHILD new OPoll(pEvent)));
		}
	//MessageLog_AppendTextFormatCo(d_coBlue, "OPolls::list() - $i elements\n", oList.size());
	return QVariant::fromValue(oList);
	}

/*
//	list(), slot
//
//	Return an XML of all the polls available
QString
OPolls::list() const
	{
	CBin binBallots;
	m_pBallotmaster->ApiBallotsList(OUT &binBallots);
	return binBallots.ToQString();
	}

//	save(), slot
//	Save the polls
void
OPolls::save(QString sXmlPolls)
	{
	CStr strXmlPolls = sXmlPolls;	// Convert to UTF-8
	m_pBallotmaster->ApiBallotSave(strXmlPolls);
	}
*/