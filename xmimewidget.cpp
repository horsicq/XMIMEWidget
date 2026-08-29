/* Copyright (c) 2020-2026 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "xmimewidget.h"

#include <QApplication>
#include <QScrollBar>
#include <QTextCursor>

#include "ui_xmimewidget.h"

XMIMEWidget::XMIMEWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::XMIMEWidget), m_pDevice(nullptr)
{
    ui->setupUi(this);

    ui->plainTextEditMIME->setAccessibleName(tr("Detected MIME types"));
    ui->plainTextEditMIME->setAccessibleDescription(tr("Read-only list of MIME types detected for the selected file."));
    ui->checkBoxAll->setAccessibleName(tr("Include generic MIME type"));
    ui->checkBoxAll->setAccessibleDescription(tr("Also include text/plain or application/octet-stream."));
    ui->checkBoxAll->setToolTip(ui->checkBoxAll->accessibleDescription());
    ui->labelStatus->setAccessibleName(tr("MIME detection status"));

    invalidateData(tr("A readable random-access device is required."));
}

XMIMEWidget::~XMIMEWidget()
{
    delete ui;
}

void XMIMEWidget::adjustView()
{
    getGlobalOptions()->adjustWidget(ui->plainTextEditMIME, XOptions::ID_VIEW_FONT_TEXTEDITS);
}

void XMIMEWidget::setData(QIODevice *pDevice)
{
    QObject::disconnect(m_deviceDestroyedConnection);
    m_pDevice = pDevice;

    if (pDevice) {
        m_deviceDestroyedConnection = connect(pDevice, &QObject::destroyed, this, &XMIMEWidget::onDeviceDestroyed);
    } else {
        m_deviceDestroyedConnection = QMetaObject::Connection();
    }

    reloadData(false);
}

void XMIMEWidget::reloadData(bool bSaveSelection)
{
    const QTextCursor previousCursor = ui->plainTextEditMIME->textCursor();
    const qint32 nPreviousScroll = ui->plainTextEditMIME->verticalScrollBar()->value();

    if (!detectTypes()) {
        return;
    }

    process(ui->checkBoxAll->isChecked());

    if (bSaveSelection) {
        const qint32 nMaximumPosition = qMax(0, ui->plainTextEditMIME->document()->characterCount() - 1);
        QTextCursor restoredCursor = ui->plainTextEditMIME->textCursor();
        restoredCursor.setPosition(qBound(0, previousCursor.anchor(), nMaximumPosition));
        restoredCursor.setPosition(qBound(0, previousCursor.position(), nMaximumPosition), QTextCursor::KeepAnchor);
        ui->plainTextEditMIME->setTextCursor(restoredCursor);
        ui->plainTextEditMIME->verticalScrollBar()->setValue(nPreviousScroll);
    }
}

void XMIMEWidget::on_checkBoxAll_toggled(bool bChecked)
{
    process(bChecked);
}

void XMIMEWidget::onDeviceDestroyed()
{
    m_pDevice.clear();
    m_deviceDestroyedConnection = QMetaObject::Connection();
    invalidateData(tr("The source device is no longer available."));
}

bool XMIMEWidget::detectTypes()
{
    if (!isDeviceReady()) {
        invalidateData(tr("A readable random-access device is required."));
        return false;
    }

    setDataControlsEnabled(false);
    setStatus(tr("Detecting MIME types..."));

    QPointer<QIODevice> pDevice = m_pDevice;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    const QStringList listTypes = XMIME::getTypes(pDevice.data(), true);
    QApplication::restoreOverrideCursor();

    if (!pDevice || (pDevice.data() != m_pDevice.data()) || !isDeviceReady()) {
        invalidateData(tr("The source device is no longer available."));
        return false;
    }

    m_listTypes = listTypes;

    if (m_listTypes.isEmpty()) {
        ui->plainTextEditMIME->clear();
        setDataControlsEnabled(false);
        setStatus(tr("No MIME types were detected."));
        return false;
    }

    return true;
}

bool XMIMEWidget::isDeviceReady() const
{
    QIODevice *pDevice = m_pDevice.data();

    return pDevice && pDevice->isOpen() && pDevice->isReadable() && !pDevice->isSequential() && (pDevice->pos() >= 0);
}

void XMIMEWidget::invalidateData(const QString &sStatus)
{
    m_listTypes.clear();
    ui->plainTextEditMIME->clear();
    setDataControlsEnabled(false);
    setStatus(sStatus);
}

void XMIMEWidget::process(bool bAll)
{
    // Filtering is deliberately cache-only. Some embedders close their QFile
    // immediately after setData(); the detected result must remain usable until
    // the device is replaced, destroyed, or an explicit reload is requested.
    if (m_listTypes.isEmpty()) {
        invalidateData(tr("No MIME types were detected."));
        return;
    }

    QStringList listTypes = m_listTypes;

    if (!bAll && (listTypes.count() > 1)) {
        listTypes.removeAll(QStringLiteral("text/plain"));
        listTypes.removeAll(QStringLiteral("application/octet-stream"));
    }

    if (listTypes.isEmpty()) {
        listTypes = m_listTypes;
    }

    const QString sText = listTypes.join(QLatin1Char('\n'));

    if (ui->plainTextEditMIME->toPlainText() != sText) {
        ui->plainTextEditMIME->setPlainText(sText);
        QTextCursor cursor = ui->plainTextEditMIME->textCursor();
        cursor.movePosition(QTextCursor::Start);
        ui->plainTextEditMIME->setTextCursor(cursor);
    }

    setDataControlsEnabled(true);

    if (listTypes.count() == 1) {
        setStatus(tr("1 MIME type detected."));
    } else {
        setStatus(tr("%1 MIME types detected.").arg(listTypes.count()));
    }
}

void XMIMEWidget::setDataControlsEnabled(bool bState)
{
    ui->checkBoxAll->setEnabled(bState);
}

void XMIMEWidget::setStatus(const QString &sStatus)
{
    ui->labelStatus->setText(sStatus);
}

void XMIMEWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
