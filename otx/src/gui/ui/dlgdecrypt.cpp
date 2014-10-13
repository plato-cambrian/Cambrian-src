#include "dlgdecrypt.hpp"
#include "ui_dlgdecrypt.h"

#include <../src/gui/ui/dlgexportedtopass.hpp>

#include <QClipboard>
#include <QMessageBox>

#include <core/OTX.hpp>

#include <opentxs/OTAPI.hpp>
#include <opentxs/OTAPI_Exec.hpp>
#include <opentxs/OT_ME.hpp>
#include <opentxs/OpenTransactions.hpp>

#include <opentxs/OTASCIIArmor.hpp>
#include <opentxs/OTEnvelope.hpp>
#include <opentxs/OTPseudonym.hpp>
#include <opentxs/OTPasswordData.hpp>
#include <opentxs/OTSignedFile.hpp>


DlgDecrypt::DlgDecrypt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDecrypt),
    already_init(false)
{
    ui->setupUi(this);

    this->installEventFilter(this);
}

DlgDecrypt::~DlgDecrypt()
{
    delete ui;
}



bool DlgDecrypt::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if(keyEvent->key() == Qt::Key_Escape)
        {
            close(); // This is caught by this same filter.
            return true;
        }
        return true;
    }
    else
    {
        // standard event processing
        return QDialog::eventFilter(obj, event);
    }
}


void DlgDecrypt::on_pushButtonClipboard_clicked()
{
    // Get text from the clipboard, and add it to the plainTextEdit widget
    //
    QClipboard *clipboard = QApplication::clipboard();

    if (NULL != clipboard)
    {
        QString qstrClipText = clipboard->text();

        if (!qstrClipText.isEmpty())
            ui->plainTextEdit->insertPlainText(qstrClipText);
    }
}

void DlgDecrypt::on_pushButtonDecrypt_clicked()
{
    bool bSuccessDecrypting = false;
    bool bSuccessVerifying  = false;

    QString qstrText = ui->plainTextEdit->toPlainText().trimmed();
    // --------------------------------
    if (qstrText.isEmpty())
    {
        // pop up a message box warning that the input text is empty.
        //
        QMessageBox::warning(this, tr("Ciphertext is Empty"),
                             tr("Please paste something to be decrypted/verified."));
        return;
    }
    // --------------------------------
    else //qstrText not empty.
    {
        std::string str_input(qstrText.toStdString());
        OTString    strInput (str_input.c_str());

        if (strInput.Exists())
        {
            if (strInput.Contains("-----BEGIN OT ARMORED ENVELOPE-----"))
            {
                OTEnvelope theEnvelope;

                if (theEnvelope.SetFromBookendedString(strInput))
                {
                    OTString strOutput;
                    // -------------------------
                    // First we'll try the default nym, if one is available.
                    //
                    QString qstrTempID = OTX::It()->get_default_nym_id();

                    if (!qstrTempID.isEmpty()) // Default Nym IS available.
                    {
                        std::string  str_nym    (qstrTempID.toStdString());
                        OTString     strNym     (str_nym.c_str());
                        OTIdentifier nym_id     (strNym);

                        if (!nym_id.IsEmpty())
                        {
                            OTPasswordData thePWData("Recipient passphrase");

                            OTPseudonym * pNym = OTAPI_Wrap::OTAPI()->GetOrLoadPrivateNym(nym_id,
                                                                                   false, //bChecking=false
                                                                                   "DlgEncrypt::on_pushButtonDecrypt_clicked",
                                                                                   &thePWData);
                            if (NULL != pNym)
                            {
                                if (theEnvelope.Open(*pNym, strOutput) && strOutput.Exists())
                                {
                                    bSuccessDecrypting = true;

                                    strInput  = strOutput;
                                    str_input = strInput.Get();
                                    qstrText  = QString::fromStdString(str_input);
                                }
                            }
                        }
                    }
                    // ------------
                    else // Default nym is NOT available. Okay let's loop through all the Nyms in the wallet then, and try then all...
                    {
                        const int32_t nym_count = OTAPI_Wrap::It()->GetNymCount();
                        // -----------------------------------------------
                        for (int32_t ii = 0; ii < nym_count; ++ii)
                        {
                            //Get OT Nym ID
                            QString OT_nym_id = QString::fromStdString(OTAPI_Wrap::It()->GetNym_ID(ii));

                            if (!OT_nym_id.isEmpty())
                            {
                                std::string  str_nym    (OT_nym_id.toStdString());
                                OTString     strNym     (str_nym.c_str());
                                OTIdentifier nym_id     (strNym);

                                if (!nym_id.IsEmpty())
                                {
                                    OTPasswordData thePWData("Recipient passphrase");

                                    OTPseudonym * pNym = OTAPI_Wrap::OTAPI()->GetOrLoadPrivateNym(nym_id,
                                                                                           false, //bChecking=false
                                                                                           "DlgEncrypt::on_pushButtonDecrypt_clicked",
                                                                                           &thePWData);
                                    if (NULL != pNym)
                                    {
                                        // Okay there is a private key available for this Nym, so let's
                                        // try to open the envelope using it.
                                        //
                                        if (theEnvelope.Open(*pNym, strOutput) && strOutput.Exists())
                                        {
                                            bSuccessDecrypting = true;

                                            strInput  = strOutput;
                                            str_input = strInput.Get();
                                            qstrText  = QString::fromStdString(str_input);

                                            break;
                                        }
                                    }
                                }
                                // ------------
                            }
                        } // for
                    } // else default nym not available.
                    // -----------------------
                    if (!bSuccessDecrypting)
                    {
                        QMessageBox::warning(this, tr("Failed Decrypting"),
                                             tr("None of the identities in your wallet (including your default identity, "
                                                "if applicable) were able to open this message."));
                        return;

                    }
                    // -----------------------
                } // if (theEnvelope.SetFromBookendedString(strInput))

            } // if (strInput.Contains("-----BEGIN OT ARMORED ENVELOPE-----"))
            // --------------------------------------------
            // This call to DecodeIfArmored is what handles the: "-----BEGIN OT ARMORED ... -----"

            if (strInput.DecodeIfArmored(false)) // bEscapedIsAllowed=true by default.
            {
                std::string str_decoded(strInput.Get());
                QString qstrDecoded(QString::fromStdString(str_decoded));

                if (!qstrDecoded.isEmpty())
                    qstrText = qstrDecoded;
                // -----------------------------------
                // At this point, we know it's been decrypted, if applicable, and it's been
                // de-armored, if applicable. So now we check to see if it's a signed file.
                //
                if (strInput.Contains("-----BEGIN SIGNED FILE-----"))
                {
                    OTSignedFile theSignedFile;

                   if (theSignedFile.LoadContractFromString(strInput))
                    {
                        OTString strSignerNymID = "Dummy this function is not implemented. Jorge V";//theSignedFile.GetSignerNymID();
                        std::string str_signer_nym(strSignerNymID.Get());
                        QString qstrSignerNym(QString::fromStdString(str_signer_nym));

                        if (!str_signer_nym.empty())
                        {
                            OTPasswordData thePWData("Sometimes need to load private part of nym in order to use its public key. (Fix that!)");

                            OTPseudonym * pNym = OTAPI_Wrap::OTAPI()->GetOrLoadNym(str_signer_nym,
                                                                                   false, //bChecking=false
                                                                                   "DlgEncrypt::on_pushButtonDecrypt_clicked",
                                                                                   &thePWData);
                            if (NULL != pNym)
                            {
                                if (theSignedFile.VerifySignature(*pNym, &thePWData))
                                {
                                    bSuccessVerifying = true;

                                    OTString strContents = theSignedFile.GetFilePayload();

                                    if (strContents.Exists())
                                    {
                                        strInput  = strContents;
                                        str_input = strInput.Get();
                                        qstrText  = QString::fromStdString(str_input);
                                    }
                                } // signature verified
                            } // pNym exists
                        } // if str_signer_nym exists
                    } // signed file: load contract from string.
                } // "BEGIN SIGNED FILE"
            } // Decode If Armored.
            // -----------------------------------------------
            // if qstrText still contains something, pop up a dialog to display the result to the user.
            //
            if (!qstrText.isEmpty())
            {
                QString qstrType("Output:");

                if (bSuccessVerifying)
                {
                    qstrType = QString(tr("Verified Signature:"));
                }
                // -----------
                if (bSuccessDecrypting)
                {
                    if (bSuccessVerifying)
                        qstrType = QString(tr("Decrypted and Signature Verified:"));
                    else
                        qstrType = QString(tr("Decrypted:"));
                }
                // -----------
                QString qstrSubTitle(tr("Be sure to copy it somewhere before closing this dialog."));
                // -----------
                // Pop up the result dialog.
                //
                DlgExportedToPass dlgExported(this, qstrText,
                                              qstrType,
                                              qstrSubTitle, false);
                dlgExported.exec();
            } // if (!qstrText.isEmpty())
        } // if strInput
    } //qstrText not empty
}


void DlgDecrypt::dialog()
{
    if (!already_init)
    {
        show();

        already_init = true;
    }
}


