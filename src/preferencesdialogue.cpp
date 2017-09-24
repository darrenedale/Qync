/**
 * \file preferencesdialogue.cpp
 * \author Darren Edale
 * \date September 2017
 * \version 1.0.0
 *
 * \brief Implementation of the PreferencesDialogue class.
 *
 * \dep
 * - preferencesdialogue.h
 * - preferencesdialogue.ui
 * - QString
 * - QMessageBox
 * - QFileDialog
 * - application.h
 * - guipreferences.h
 * - common.h
 */

#include "preferencesdialogue.h"
#include "ui_preferencesdialogue.h"

#include <QDebug>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>

#include "application.h"
#include "guipreferences.h"
#include "functions.h"


namespace Qync {


	/**
	 * \class PreferencesDialogue
	 * \author Darren Edale
	 * \date September 2017
	 * \version 1.0.0
	 *
	 * \brief The preferences window of the Qync GUI.
	 *
	 * The PreferencesDialogue class is a QDialog subclass that enables the
	 * user to modify the current application and GUI settings. It is based on the
	 * assumption that the preferences object stored in the current application
	 * manager is a GuiPreferences object, and therefore is suitable only if
	 * used along with the example GUI (or another GUI that uses the
	 * GuiPreferences class for its preferences).
	 *
	 * The dialogue provides access to the rsync path as well as setting the visible
	 * toolbars and the style of the toolbars. The modified settings can be applied
	 * while keeping the dialogue open, applied and the dialogue closed, or
	 * forgotten completely. As the manager saves the preferences whenever they
	 * are set, applied changes are always rememberd between sessions using the
	 * application.
	 */


	/**
	 * \brief Create a new preferences dialogue.
	 *
	 * \param manager is the manager from which the preferences should
	 * be read.
	 * \param parent is the parent widget.
	 */
	PreferencesDialogue::PreferencesDialogue(QWidget * parent)
	: QDialog(parent),
	  m_ui{new Ui::PreferencesDialogue} {
		m_ui->setupUi(this);
		updateWidgets();

		connect(m_ui->simpleUi, &QCheckBox::toggled, m_ui->toolbarGroup, &QGroupBox::setDisabled);
		connect(m_ui->chooseRsyncPath, &QPushButton::clicked, this, &PreferencesDialogue::chooseRsyncPath);

		connect(m_ui->controls->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &PreferencesDialogue::close);
		connect(m_ui->controls->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &PreferencesDialogue::apply);
		connect(m_ui->controls->button(QDialogButtonBox::Save), &QPushButton::clicked, [this](void) {
			save();
			close();
		});
	}


	PreferencesDialogue::~PreferencesDialogue(void) = default;


	/**
	 * \brief Update all the widgets in the preferences window.
	 *
	 * All the widgets in the preferences window are updated to reflect the
	 * state of the preferences in the manager.
	 */
	void PreferencesDialogue::updateWidgets(void) {
		const auto & prefs = qyncApp->preferences();

		m_ui->rsyncPath->setText(prefs.rsyncPath());
		m_ui->simpleUi->setChecked(prefs.useSimpleUi());
		m_ui->presetsToolbar->setChecked(prefs.showPresetsToolBar());
		m_ui->synchroniseToolbar->setChecked(prefs.showSynchroniseToolBar());

		switch(prefs.toolBarButtonStyle()) {
			case Qt::ToolButtonFollowStyle:
				m_ui->toolbarStyle->setCurrentIndex(0);
				break;

			case Qt::ToolButtonIconOnly:
				m_ui->toolbarStyle->setCurrentIndex(1);
				break;

			case Qt::ToolButtonTextUnderIcon:
				m_ui->toolbarStyle->setCurrentIndex(2);
				break;

			case Qt::ToolButtonTextBesideIcon:
				m_ui->toolbarStyle->setCurrentIndex(3);
				break;

			case Qt::ToolButtonTextOnly:
				m_ui->toolbarStyle->setCurrentIndex(4);
				break;
		}
	}


	/**
	 * \brief Apply the current preferences.
	 *
	 * The applications's preferences object is updated.
	 */
	void PreferencesDialogue::apply(void) {
		qDebug() << __PRETTY_FUNCTION__ << "applying preferences";
		auto & prefs = qyncApp->preferences();

		prefs.setRsyncPath(m_ui->rsyncPath->text());
		prefs.setUseSimpleUi(m_ui->simpleUi->isChecked());
		prefs.setShowPresetsToolBar(m_ui->presetsToolbar->isChecked());
		prefs.setShowSynchroniseToolBar(m_ui->synchroniseToolbar->isChecked());

		switch(m_ui->toolbarStyle->currentIndex()) {
			default:
			case 0:
				prefs.setToolBarButtonStyle(Qt::ToolButtonFollowStyle);
				break;

			case 1:
				prefs.setToolBarButtonStyle(Qt::ToolButtonIconOnly);
				break;

			case 2:
				prefs.setToolBarButtonStyle(Qt::ToolButtonTextUnderIcon);
				break;

			case 3:
				prefs.setToolBarButtonStyle(Qt::ToolButtonTextBesideIcon);
				break;

			case 4:
				prefs.setToolBarButtonStyle(Qt::ToolButtonTextOnly);
				break;
		}

		Q_EMIT qyncApp->preferencesChanged();
	}

	/**
	 * \brief Apply the current preferences.
	 *
	 * The applications's preferences object is updated.
	 */
	void PreferencesDialogue::save(void) {
		apply();

		if(!qyncApp->preferences().save()) {
			QMessageBox::warning(this, tr("%1 Warnng").arg(qyncApp->applicationDisplayName()), tr("Your preferences were set but could not be stored. This means that next time you start %1 your preferences will revert to their previous settings.").arg(qyncApp->applicationDisplayName()), QMessageBox::Ok);
		}
	}


	/**
	 * \brief Choose the path to the rsync executable file.
	 *
	 * A local file browser is presented for the user to to choose the rsync
	 * executable. If the user does not cancel the dialogue, the chosen file
	 * is set as the text in the rsync path line edit.
	 */
	void PreferencesDialogue::chooseRsyncPath(void) {
		QString fileName = QFileDialog::getOpenFileName(this, tr("Choose rsync executable"), m_ui->rsyncPath->text());

		if(!fileName.isEmpty()) {
			QFileInfo f(fileName);

			if(f.exists() && f.isFile() && f.isExecutable()) {
				m_ui->rsyncPath->setText(fileName);
			}
			else {
				QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("%1 is not a valid path to an rsync executable.").arg(fileName), QMessageBox::Ok);
			}
		}
	}


}  // namespace Qync
