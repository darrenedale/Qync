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
	}


	PreferencesDialogue::~PreferencesDialogue(void) = default;


	/**
	 * \brief Update all the widgets in the preferences window.
	 *
	 * All the widgets in the preferences window are updated to reflect the
	 * state of the preferences in the manager.
	 */
	void PreferencesDialogue::updateWidgets(void) {
		const GuiPreferences * p = dynamic_cast<const GuiPreferences *>(qyncApp->preferences());

		if(!p) {
			QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("There are no preferences to edit."), QMessageBox::Ok);
			close();
			return;
		}

		m_ui->rsyncPath->setText(p->rsyncPath());
		m_ui->simpleUi->setChecked(p->useSimpleUi());
		m_ui->presetsToolbar->setChecked(p->showPresetsToolBar());
		m_ui->synchroniseToolbar->setChecked(p->showSynchroniseToolBar());

		switch(p->toolBarButtonStyle()) {
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
	 * \brief Save the current preferences.
	 *
	 * The manager's preferences object is updated.
	 */
	void PreferencesDialogue::savePreferences(void) {
		GuiPreferences * p = new GuiPreferences;

		p->setRsyncPath(m_ui->rsyncPath->text());
		p->setUseSimpleUi(m_ui->simpleUi->isChecked());
		p->setShowPresetsToolBar(m_ui->presetsToolbar->isChecked());
		p->setShowSynchroniseToolBar(m_ui->synchroniseToolbar->isChecked());

		switch(m_ui->toolbarStyle->currentIndex()) {
			default:
			case 0:
				p->setToolBarButtonStyle(Qt::ToolButtonFollowStyle);
				break;

			case 1:
				p->setToolBarButtonStyle(Qt::ToolButtonIconOnly);
				break;

			case 2:
				p->setToolBarButtonStyle(Qt::ToolButtonTextUnderIcon);
				break;

			case 3:
				p->setToolBarButtonStyle(Qt::ToolButtonTextBesideIcon);
				break;

			case 4:
				p->setToolBarButtonStyle(Qt::ToolButtonTextOnly);
				break;
		}

		/* this should trigger preferencesChanged signal */
		if(qyncApp->setPreferences(p)) {
			if(!p->saveAs(qyncApp->configurationPath() + "/guipreferences")) {
				QMessageBox::warning(this, tr("%1 Warnng").arg(qyncApp->applicationDisplayName()), tr("Your preferences were set but could not be stored. This means that next time you start %1 your preferences will revert to their previous settings.").arg(qyncApp->applicationDisplayName()), QMessageBox::Ok);
			}
		}
		else {
			QMessageBox::warning(this, tr("%1 Warnng").arg(qyncApp->applicationDisplayName()), tr("Your preferences could not be applied."), QMessageBox::Ok);
			delete p;
		}
	}


	/**
	 * \brief Save the current preferences and close the preferences window.
	 *
	 * The manager's preferences object is updated and the preferences
	 * window is closed.
	 */
	void PreferencesDialogue::savePreferencesAndClose(void) {
		savePreferences();
		close();
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
