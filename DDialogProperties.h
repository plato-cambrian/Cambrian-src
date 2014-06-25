//	DDialogProperties.h
//
//	Core classes to create "property pages" as well as the implementation of a few of them.

#ifndef DDIALOGPROPERTIES_H
#define DDIALOGPROPERTIES_H
#ifndef PRECOMPILEDHEADERS_H
	#include "PreCompiledHeaders.h"
#endif

class DDialogPropertyPage : public QListWidgetItem, public WWidget
{
public:
	DDialogPropertyPage(PSZAC pszName);
};

//	Base class to display properties of an object
//	This class uses a QListWidget to display the pages (instead of using tabs), thus allowing the possibility to have more pages and have them easily selected (because when there are many pages, the tabs tabs become very small and the text is truncated)
class DDialogProperties : public DDialogOkCancel
{
	Q_OBJECT
protected:
	QListWidget * m_pwPagesList;		// Widget displaying all the property pages
	QStackedWidget * m_pwPagesStacked;
	OLayoutHorizontal * m_poLayoutButtons;	// Layout displaying all the buttons: OK, Cancel and other buttons (if any)

public:
	DDialogProperties();
	void PageAdd(DDialogPropertyPage * pawPage);
	virtual int exec();

public slots:
	void SL_PageChanged(QListWidgetItem * pPageCurrent, QListWidgetItem * pPagePrevious);
	void SL_ButtonOK_clicked();
	void SL_ButtonCancel_clicked();
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class DDialogPropertiyPageAccountGeneral : public DDialogPropertyPage
{
protected:
	TAccountXmpp * m_pAccount;
	WEditPassword * m_pwEditPassword;
public:
	DDialogPropertiyPageAccountGeneral(TAccountXmpp * pAccount);
};

class DDialogPropertiyPageAccountTest : public DDialogPropertyPage
{
public:
	DDialogPropertiyPageAccountTest(PSZAC pszName);
};

class DDialogPropertiesAccount : public DDialogProperties
{
protected:
	TAccountXmpp * m_pAccount;
public:
	DDialogPropertiesAccount(TAccountXmpp * pAccount);
};


///////////////////////////////////////////////////////////////////////////////////////////////////
class DDialogPropertiyPageContactGeneral : public DDialogPropertyPage
{
protected:
	TContact * m_pContact;
public:
	DDialogPropertiyPageContactGeneral(TContact * pContact);
};

class DDialogPropertiesContact : public DDialogProperties
{
protected:
	TContact * m_pContact;
public:
	DDialogPropertiesContact(TContact * pContact);
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class DDialogPropertiyPageGroupGeneral : public DDialogPropertyPage
{
protected:
	TGroup * m_pGroup;
public:
	DDialogPropertiyPageGroupGeneral(TGroup * pGroup);
};

class DDialogPropertiesGroup : public DDialogProperties
{
protected:
	TGroup * m_pGroup;
public:
	DDialogPropertiesGroup(TGroup * pGroup);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//	Widget displaying a list of contacts
class WListContacts : public QListWidget
{
public:
	QIcon m_oIconContact;

public:
	WListContacts();
	void ContactAdd(TContact * pContact);
	void ContactsAdd(IN_MOD_SORT CArrayPtrContacts & arraypContacts);
	void ContactAddTransfer(PA_DELETING QListWidgetItem * paItem);
	void ContactRemove(TContact * pContact);
	void ContactsGetAll(IOUT CArrayPtrContacts * parraypContacts) const;
};
class DDialogGroupAddContacts : public DDialogOkCancelWithLayouts
{
	Q_OBJECT
protected:
	ITreeItemChatLogEvents * m_pContactOrGroup;
	TGroup * m_pGroup;
	TGroup * m_paGroup;
	WListContacts * m_pwListContactsAvailable;
	WListContacts * m_pwListContactsInGroup;

	// Array of contacts when the dialog was initialized
	CArrayPtrContacts m_arraypContactsInGroup;
	CArrayPtrContacts m_arraypContactsAvailable;

public:
	DDialogGroupAddContacts(ITreeItemChatLogEvents * pContactOrGroup);
	~DDialogGroupAddContacts();

protected slots:
	void SL_ContactAvailableDoubleClicked(QListWidgetItem * paItem);
	void SL_ContactInGroupDoubleClicked(QListWidgetItem * paItem);
	void SL_ButtonOK();
};


#endif // DDIALOGPROPERTIES_H
