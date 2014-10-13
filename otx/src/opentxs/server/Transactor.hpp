/************************************************************
 *
 *  Transactor.hpp
 *
 */

/************************************************************
 -----BEGIN PGP SIGNED MESSAGE-----
 Hash: SHA1

 *                 OPEN TRANSACTIONS
 *
 *       Financial Cryptography and Digital Cash
 *       Library, Protocol, API, Server, CLI, GUI
 *
 *       -- Anonymous Numbered Accounts.
 *       -- Untraceable Digital Cash.
 *       -- Triple-Signed Receipts.
 *       -- Cheques, Vouchers, Transfers, Inboxes.
 *       -- Basket Currencies, Markets, Payment Plans.
 *       -- Signed, XML, Ricardian-style Contracts.
 *       -- Scripted smart contracts.
 *
 *  Copyright (C) 2010-2013 by "Fellow Traveler" (A pseudonym)
 *
 *  EMAIL:
 *  FellowTraveler@rayservers.net
 *
 *  BITCOIN:  1NtTPVVjDsUfDWybS4BwvHpG2pdS9RnYyQ
 *
 *  KEY FINGERPRINT (PGP Key in license file):
 *  9DD5 90EB 9292 4B48 0484  7910 0308 00ED F951 BB8E
 *
 *  OFFICIAL PROJECT WIKI(s):
 *  https://github.com/FellowTraveler/OTX
 *  https://github.com/FellowTraveler/Open-Transactions/wiki
 *
 *  WEBSITE:
 *  http://www.OpenTransactions.org/
 *
 *  Components and licensing:
 *   -- OTX..A Java client GUI.....LICENSE:.....GPLv3
 *   -- otlib.........A class library.......LICENSE:...LAGPLv3
 *   -- otapi.........A client API..........LICENSE:...LAGPLv3
 *   -- opentxs/ot....Command-line client...LICENSE:...LAGPLv3
 *   -- otserver......Server Application....LICENSE:....AGPLv3
 *  Github.com/FellowTraveler/Open-Transactions/wiki/Components
 *
 *  All of the above OT components were designed and written by
 *  Fellow Traveler, with the exception of OTX, which
 *  was contracted out to Vicky C (bitcointrader4@gmail.com).
 *  The open-source community has since actively contributed.
 *
 *  -----------------------------------------------------
 *
 *   LICENSE:
 *   This program is free software: you can redistribute it
 *   and/or modify it under the terms of the GNU Affero
 *   General Public License as published by the Free Software
 *   Foundation, either version 3 of the License, or (at your
 *   option) any later version.
 *
 *   ADDITIONAL PERMISSION under the GNU Affero GPL version 3
 *   section 7: (This paragraph applies only to the LAGPLv3
 *   components listed above.) If you modify this Program, or
 *   any covered work, by linking or combining it with other
 *   code, such other code is not for that reason alone subject
 *   to any of the requirements of the GNU Affero GPL version 3.
 *   (==> This means if you are only using the OT API, then you
 *   don't have to open-source your code--only your changes to
 *   Open-Transactions itself must be open source. Similar to
 *   LGPLv3, except it applies to software-as-a-service, not
 *   just to distributing binaries.)
 *
 *   Extra WAIVER for OpenSSL, Lucre, and all other libraries
 *   used by Open Transactions: This program is released under
 *   the AGPL with the additional exemption that compiling,
 *   linking, and/or using OpenSSL is allowed. The same is true
 *   for any other open source libraries included in this
 *   project: complete waiver from the AGPL is hereby granted to
 *   compile, link, and/or use them with Open-Transactions,
 *   according to their own terms, as long as the rest of the
 *   Open-Transactions terms remain respected, with regard to
 *   the Open-Transactions code itself.
 *
 *   Lucre License:
 *   This code is also "dual-license", meaning that Ben Lau-
 *   rie's license must also be included and respected, since
 *   the code for Lucre is also included with Open Transactions.
 *   See Open-Transactions/src/otlib/lucre/LUCRE_LICENSE.txt
 *   The Laurie requirements are light, but if there is any
 *   problem with his license, simply remove the Lucre code.
 *   Although there are no other blind token algorithms in Open
 *   Transactions (yet. credlib is coming), the other functions
 *   will continue to operate.
 *   See Lucre on Github:  https://github.com/benlaurie/lucre
 *   -----------------------------------------------------
 *   You should have received a copy of the GNU Affero General
 *   Public License along with this program.  If not, see:
 *   http://www.gnu.org/licenses/
 *
 *   If you would like to use this software outside of the free
 *   software license, please contact FellowTraveler.
 *   (Unfortunately many will run anonymously and untraceably,
 *   so who could really stop them?)
 *
 *   DISCLAIMER:
 *   This program is distributed in the hope that it will be
 *   useful, but WITHOUT ANY WARRANTY; without even the implied
 *   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *   PURPOSE.  See the GNU Affero General Public License for
 *   more details.

 -----BEGIN PGP SIGNATURE-----
 Version: GnuPG v1.4.9 (Darwin)

 iQIcBAEBAgAGBQJRSsfJAAoJEAMIAO35UbuOQT8P/RJbka8etf7wbxdHQNAY+2cC
 vDf8J3X8VI+pwMqv6wgTVy17venMZJa4I4ikXD/MRyWV1XbTG0mBXk/7AZk7Rexk
 KTvL/U1kWiez6+8XXLye+k2JNM6v7eej8xMrqEcO0ZArh/DsLoIn1y8p8qjBI7+m
 aE7lhstDiD0z8mwRRLKFLN2IH5rAFaZZUvj5ERJaoYUKdn4c+RcQVei2YOl4T0FU
 LWND3YLoH8naqJXkaOKEN4UfJINCwxhe5Ke9wyfLWLUO7NamRkWD2T7CJ0xocnD1
 sjAzlVGNgaFDRflfIF4QhBx1Ddl6wwhJfw+d08bjqblSq8aXDkmFA7HeunSFKkdn
 oIEOEgyj+veuOMRJC5pnBJ9vV+7qRdDKQWaCKotynt4sWJDGQ9kWGWm74SsNaduN
 TPMyr9kNmGsfR69Q2Zq/FLcLX/j8ESxU+HYUB4vaARw2xEOu2xwDDv6jt0j3Vqsg
 x7rWv4S/Eh18FDNDkVRChiNoOIilLYLL6c38uMf1pnItBuxP3uhgY6COm59kVaRh
 nyGTYCDYD2TK+fI9o89F1297uDCwEJ62U0Q7iTDp5QuXCoxkPfv8/kX6lS6T3y9G
 M9mqIoLbIQ1EDntFv7/t6fUTS2+46uCrdZWbQ5RjYXdrzjij02nDmJAm2BngnZvd
 kamH0Y/n11lCvo1oQxM+
 =uSzz
 -----END PGP SIGNATURE-----
**************************************************************/

#ifndef __OPENTXS_TRANSACTOR_HPP__
#define __OPENTXS_TRANSACTOR_HPP__

#include <opentxs/core/OTAcctList.hpp>
#include <string>
#include <map>
#include <memory>
#include <cstdint>

namespace opentxs
{

class Mint;
class OTServer;
class OTPseudonym;
class OTAssetContract;
class OTIdentifier;
class OTAccount;
class MainFile;

class Transactor
{
    friend class MainFile;

public:
    explicit Transactor(OTServer* server);
    ~Transactor();

    bool issueNextTransactionNumber(OTPseudonym& nym, int64_t& txNumber,
                                    bool storeNumber = true);
    bool verifyTransactionNumber(OTPseudonym& nym,
                                 const int64_t& transactionNumber);
    bool removeTransactionNumber(OTPseudonym& nym,
                                 const int64_t& transactionNumber,
                                 bool save = false);
    bool removeIssuedNumber(OTPseudonym& nym, const int64_t& transactionNumber,
                            bool save = false);

    int64_t transactionNumber() const
    {
        return transactionNumber_;
    }

    void transactionNumber(int64_t value)
    {
        transactionNumber_ = value;
    }

    // When a user uploads an asset contract, the server adds it to the list
    // (and verifies the user's key against the
    // contract.) This way the server has a directory with all the asset
    // contracts that it supports, saved by their ID.
    // As long as the IDs are in the server file, it can look them up.
    // When a new asset type is added, a new Mint is added as well. It goes into
    // the mints folder.
    bool addAssetContract(OTAssetContract& contract);
    OTAssetContract* getAssetContract(const OTIdentifier& id);

    bool addBasketAccountID(const OTIdentifier& basketId,
                            const OTIdentifier& basketAccountId,
                            const OTIdentifier& basketContractId);
    bool lookupBasketAccountID(const OTIdentifier& basketId,
                               OTIdentifier& basketAccountId);

    bool lookupBasketAccountIDByContractID(const OTIdentifier& basketContractId,
                                           OTIdentifier& basketAccountId);
    bool lookupBasketContractIDByAccountID(const OTIdentifier& basketAccountId,
                                           OTIdentifier& basketContractId);

    // Whenever the server issues a voucher (like a cashier's cheque), it puts
    // the funds in one
    // of these voucher accounts (one for each asset type ID). Then it issues
    // the cheque from the
    // same account.
    // TODO: also should save the cheque itself to a folder, where the folder is
    // named based on the date
    // that the cheque will expire.  This way, the server operator can go back
    // later, or have a script,
    // to retrieve the cheques from the expired folders, and total them. The
    // server operator is free to
    // remove that total from the Voucher Account once the cheque has expired:
    // it is his money now.
    std::shared_ptr<OTAccount> getVoucherAccount(
        const OTIdentifier& assetTypeId);

    // Each asset contract has its own series of Mints
    Mint* getMint(const OTIdentifier& assetTypeId, int32_t seriesCount);

private:
    // Why does the map of mints use multimap instead of map?
    // Because there might be multiple valid mints for the same asset type.
    // Perhaps I am redeeming tokens from the previous series, which have not
    // yet expired.
    // Only tokens from the new series are being issued today, but tokens from
    // the previous series are still good until their own expiration date, which
    // is coming up soon.
    // Therefore the server manages different mints for the same asset type, and
    // since the asset type is the key in the multimap, we don't want to
    // accidentally remove one from the list every time another is added. Thus
    // multimap is employed.
    typedef std::multimap<std::string, Mint*> MintsMap;
    typedef std::map<std::string, OTAssetContract*> ContractsMap;
    typedef std::map<std::string, std::string> BasketsMap;

private:
    // This stores the last VALID AND ISSUED transaction number.
    int64_t transactionNumber_;
    // The asset types supported by this server.
    ContractsMap contractsMap_;
    // maps basketId with basketAccountId
    BasketsMap idToBasketMap_;
    // basket issuer account ID, which is *different* on each server, using the
    // Basket Currency's ID, which is the *same* on every server.)
    // Need a way to look up a Basket Account ID using its Contract ID
    BasketsMap contractIdToBasketAccountId_;
    // The list of voucher accounts (see GetVoucherAccount below for details)
    OTAcctList voucherAccounts_;
    // The mints for each asset type.
    MintsMap mintsMap_;

    OTServer* server_; // TODO: remove later when feasible
};

} // namespace opentxs

#endif // __OPENTXS_TRANSACTOR_HPP__
