/**
 * @file aboutdialogue.cpp
 * @author Darren Edale
 * @date October 2020
 * @version 1.1.1
 *
 * @brief Implementation of the AboutDialogue class.
 */

#include "aboutdialogue.h"
#include "ui_aboutdialogue.h"

#include <QtCore/QString>
#include <QtGui/QIcon>

#include "application.h"

using namespace Qync;

/**
 * @class AboutDialogue
 * @author Darren Edale
 * @date October 2020
 * @version 1.1.1
 *
 * @brief Dialogue showing information about the Qync application.
 */

/**
 * @brief Create a new AboutDialogue instance.
 */
AboutDialogue::AboutDialogue()
:   QDialog(nullptr),
    m_ui(std::make_unique<Ui::AboutDialogue>())
{
    m_ui->setupUi(this);

    m_ui->logo->setPixmap(QIcon(QStringLiteral(":/icons/application")).pixmap(64));
    m_ui->title->setText(processPlaceholders(m_ui->title->text()));
    m_ui->about->setText(processPlaceholders(m_ui->about->text()));
    m_ui->buildDetails->setText(processPlaceholders(m_ui->buildDetails->text()));

    QFont f = m_ui->title->font();
    f.setBold(true);
    f.setPointSizeF(f.pointSizeF() * 1.33);
    m_ui->title->setFont(f);

    f = m_ui->buildDetails->font();
    f.setPointSizeF(f.pointSizeF() * 0.85);
    m_ui->buildDetails->setFont(f);

    connect(m_ui->controls, &QDialogButtonBox::accepted, this, &QDialog::close);
    connect(m_ui->controls, &QDialogButtonBox::rejected, this, &QDialog::close);
}

/**
 * @brief Destroy the AboutDialogue instance.
 */
AboutDialogue::~AboutDialogue() = default;

/**
 * @brief Helper to process placeholders in display strings.
 *
 * @param content The content to process.
 *
 * The content must be provided as an rvalue and is modified directly. The returned value is just a (lvalue) reference
 * to the provided (modified) object, so there should be no copies involved in calling this function.
 *
 * The following placeholders are processed:
 * - @b {ApplicationDisplayName} is replaced with the display name of the application, as provided by
 *      Application::applicationDisplayName()
 * - @b {ApplicationVersion} is replaced with the version string of the application, as provided by
 *      Application::applicationVersion()
 * - @b {ApplicationWebsite} is replaced with the website of the application, as provided by Application::websiteUrl()
 * - @b {ReleaseDate} is replaced with the release date of the application, as provided by Application::releaseDate()
 * - @b {BuildId} is replaced with the build id of the application, as provided by Application::buildId()
 *
 * @return A reference to the provided string with the modifications made.
 */
QString & AboutDialogue::processPlaceholders(QString && content) {
    return content.replace("{ApplicationDisplayName}", qyncApp->applicationDisplayName())
            .replace("{ApplicationVersion}", qyncApp->applicationVersion())
            .replace("{ApplicationWebsite}", qyncApp->websiteUrl())
            .replace("{ReleaseDate}", qyncApp->releaseDate())
            .replace("{BuildId}", qyncApp->buildId());
}
