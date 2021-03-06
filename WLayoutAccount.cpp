#ifndef PRECOMPILEDHEADERS_H
	#include "PreCompiledHeaders.h"
#endif
#include "WLayoutAccount.h"
#include "DialogInvitations.h"

//	TAccountAlias::ITreeItem::TreeItem_GotFocus()
void
TAccountAlias::TreeItem_GotFocus()
	{
	MainWindow_SetCurrentLayout(PA_CHILD new WLayoutAccountAlias(this));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
WLayoutAccount::WLayoutAccount(TAccountXmpp * pAccount)
	{
	Assert(pAccount != NULL);
	//Layout_NoticeAuxiliaryAdd(new WNoticeInviteContactsByEmail(pAccount));
	Splitter_AddWidgetSpacer();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
WLayoutAccountAlias::WLayoutAccountAlias(TAccountAlias * pAlias)
	{
	TAccountXmpp * pAccount = pAlias->m_pAccount;
	WGroupBox * pwGroupBox;
	OLayoutForm * pLayout = Splitter_PoAddGroupBoxAndLayoutForm_VE(OUT &pwGroupBox, "Account: $S", &pAccount->m_strJID);
	if (pAccount->m_uFlagsAccountStatus & TAccountXmpp::FAS_kfAccountAutoGenerated)
		{
		WLabel * pwLabel = new WLabelSelectableWrap;
		pwLabel->Label_SetTextFormat_VE_Gsb(
			"The XMPP account <b>^S</b/> was automatically created by SocietyPro so you may send and receive messages. "
			"If you already have an XMPP account, feel free to add it to SocietyPro.", &pAccount->m_strJID);
		pLayout->addRow(pwLabel);
		}
	pLayout->Layout_PwAddRowLabelEditReadOnly("Username:", pAccount->m_strUsername);
	pLayout->Layout_PwAddRowLabelEditReadOnly("Server:", pAccount->m_strServerName);
	pLayout->Layout_PwAddRowLabelEditReadOnlyInt("Port:", pAccount->m_uServerPort);
	pLayout->Layout_PwAddRowLabelEditReadOnly("Resource:", pAccount->m_strJIDwithResource.PchFindCharacter('/'));
	pLayout->Layout_PwAddRowLabelEditReadOnlyFingerprint("Certificate Fingerprint:", *pAccount->Certificate_PGetBinaryFingerprint());

	WTable * pTableContacts = new WTable(this);
	pTableContacts->SetColumns_VEZ("Display Name", "JID", "Subscription", "Invisible?", "Resource", "Protocol", "Sync", "File Name Chat History", NULL);
	TContact ** ppContactStop;
	TContact ** ppContact = pAccount->m_arraypaContacts.PrgpGetContactsStop(OUT &ppContactStop);
	while (ppContact != ppContactStop)
		{
		TContact * pContact = *ppContact++;
		Assert(pContact != NULL);
		Assert(pContact->EGetRuntimeClass() == RTI(TContact));
		Assert(pContact->m_pAccount == pAccount);
		OTableRow oRow;
		oRow.AddData(pContact->TreeItem_PszGetNameDisplay());
		oRow.AddData(pContact->m_strJidBare);
		oRow.AddData(pContact->XmppRoster_PszGetSubscription());
		oRow.AddData(pContact->TreeItemFlags_FuIsInvisible() ? (PSZUC)"Yes" : NULL);
		oRow.AddData(pContact->m_strRessource);
		oRow.AddData(pContact->Contact_FuCommunicateViaXosp() ? (PSZUC)"XOSP" : pContact->Contact_FuCommunicateViaXmppOnly() ? (PSZUC)"XMPP" : NULL);
		oRow.AddData(pContact->ContactFlag_FuNeedSynchronizeWhenPresenceOnline() ? (PSZUC)"Yes" : NULL);
		oRow.AddData(pContact->Vault_SGetPath());
		pTableContacts->AppendRow(oRow);
		}
	//pTableContacts->AdjustHeightToFitRows();

	#ifdef DEBUG_
	Splitter_PwAddGroupBox_VE("Debug Info: $i roaming notices, $i auxiliary notices.", g_listaNoticesRoaming.UGetLength(), g_listaNoticesAuxiliary.UGetLength());
	#endif
	}
