/*
    ProxyEmy local proxy server
    Copyright (C) 2022 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <QCoreApplication>
#include <QFile>
#include "proxyemybackend.h"
#ifdef Q_OS_WIN
#include <windows.h>
#include <wincrypt.h>
#endif

ProxyEmyBackend::ProxyEmyBackend(QObject *parent) : QObject(parent)
{
    m_rootCertificateInstalled = isRootCertificateInstalled();
}

void ProxyEmyBackend::setWindowTitle(const QString &windowTitle) noexcept
{
    if (m_windowTitle == windowTitle) return;

    m_windowTitle = windowTitle;
    emit windowTitleChanged();
}

void ProxyEmyBackend::setNotificationHub(const NotificationHubViewModel *viewModel) noexcept
{
    if (m_notificationHub == viewModel) return;

    m_notificationHub = const_cast<NotificationHubViewModel *>(viewModel);
    emit notificationHubChanged();
}

void ProxyEmyBackend::installRootCA() noexcept
{
    auto installed = installRootCertificate();
    if (installed) {
        m_notificationHub->pushInfoMessage("Root certificate", "Certificate installed sucessfully!");
        m_rootCertificateInstalled = true;
        emit rootCertificateInstalledChanged();
        return;
    }
    m_notificationHub->pushErrorMessage("Root certificate", "The certificate was not installed.");
}

bool ProxyEmyBackend::installRootCertificate() noexcept
{
#ifdef Q_OS_WIN

    QFile certificateFile(QCoreApplication::applicationDirPath() + "/certificate/RootCABinary.cer");
    if (!certificateFile.open(QIODevice::ReadOnly)) return false;
    auto bytes = certificateFile.readAll();
    certificateFile.close();

    const BYTE* pByte = reinterpret_cast<BYTE*>(bytes.data());
    auto length = bytes.length();

    HCERTSTORE hRootCertStore = CertOpenSystemStoreA(NULL, "ROOT");

    auto result = CertAddEncodedCertificateToStore(hRootCertStore, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, pByte, length, CERT_STORE_ADD_USE_EXISTING, NULL);

    CertCloseStore(hRootCertStore, 0);

    return result;
#else
    return false;
#endif
}

bool ProxyEmyBackend::isRootCertificateInstalled() noexcept
{
#ifdef Q_OS_WIN
    HCERTSTORE hRootCertStore = CertOpenSystemStoreA(NULL, "ROOT");

    auto founded = CertFindCertificateInStore(
          hRootCertStore,
          (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING),
          0,
          CERT_FIND_SUBJECT_STR_A,
          "ProxyEmy-Root-CA",
          NULL);
    CertCloseStore(hRootCertStore, 0);

    return founded != NULL;
#else
    return false;
#endif
}
