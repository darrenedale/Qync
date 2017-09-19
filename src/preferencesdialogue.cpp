/**
 * \file PreferencesDialogue.cpp
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Implementation of the PreferencesDialogue class.
 *
 * \dep
 * - PreferencesDialogue.h
 * - Manager.h
 * - Preferences.h
 * - common.h
 * - QString
 * - QLineEdit
 * - QToolButton
 * - QCheckBox
 * - QComboBox
 * - QPushButton
 * - QGroupBox
 * - QLabel
 * - QDialogButtonBox
 * - QFileDialog
 * - QMessageBox
 * - QVBoxLayout
 * - QHBoxLayout
 * - QGridLayout
 *
 * \todo
 * Nothing.
 */

#include "preferencesdialogue.h"

#include "application.h"
#include "guipreferences.h"
#include "functions.h"

#include <QString>
#include <QLineEdit>
#include <QToolButton>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>


namespace Qync {


	PreferencesDialogue::PreferencesDialogue(QWidget * parent)
	  : QDialog(parent) {
		createWidgets();
		updateWidgets();
	}


	void PreferencesDialogue::createWidgets(void) {
		m_rsyncPath = new QLineEdit();
		m_chooseRsyncPath = new QToolButton();
		m_presetsToolBar = new QCheckBox(tr("Presets"));
		m_synchroniseToolBar = new QCheckBox(tr("Synchronise"));
		m_toolBarStyle = new QComboBox();

		m_chooseRsyncPath->setIcon(QIcon(":/icons/buttons/choosefile"));

		m_toolBarStyle->addItem(tr("Style Default"));
		m_toolBarStyle->addItem(tr("Icons Only"));
		m_toolBarStyle->addItem(tr("Text Under Icons"));
		m_toolBarStyle->addItem(tr("Text Beside Icons"));
		m_toolBarStyle->addItem(tr("Text Only"));

		connect(m_chooseRsyncPath, &QPushButton::clicked, this, &PreferencesDialogue::chooseRsyncPath);

		QVBoxLayout * vLayout = new QVBoxLayout();
		QHBoxLayout * hLayout = new QHBoxLayout();
		hLayout->addWidget(new QLabel(tr("rsync path")));
		hLayout->addWidget(m_rsyncPath);
		hLayout->addWidget(m_chooseRsyncPath);
		vLayout->addLayout(hLayout);

		QGroupBox * group = new QGroupBox(tr("Toolbars"));
		QGridLayout * gLayout = new QGridLayout();
		gLayout->addWidget(m_presetsToolBar, 0, 0);
		gLayout->addWidget(m_synchroniseToolBar, 0, 1);
		hLayout = new QHBoxLayout();
		hLayout->addWidget(new QLabel(tr("Style")));
		hLayout->addWidget(m_toolBarStyle);
		gLayout->addLayout(hLayout, 1, 0, 1, 2, Qt::AlignLeft);
		group->setLayout(gLayout);
		vLayout->addWidget(group);
		vLayout->addStretch(2);

		QDialogButtonBox * buttons = new QDialogButtonBox();

		QPushButton * button = new QPushButton(QIcon(":/icons/buttons/save"), tr("Save"));
		button->setToolTip(tr("Save the preferences and close the preferences window."));
		connect(button, &QPushButton::clicked, this, &PreferencesDialogue::savePreferencesAndClose);
		buttons->addButton(button, QDialogButtonBox::ActionRole);

		button = new QPushButton(QIcon(":/icons/buttons/apply"), tr("Apply"));
		button->setToolTip(tr("Save the preferences."));
		connect(button, &QPushButton::clicked, this, &PreferencesDialogue::savePreferences);
		buttons->addButton(button, QDialogButtonBox::AcceptRole);

		button = new QPushButton(QIcon(":/icons/buttons/close"), tr("Close"));
		button->setToolTip(tr("Close the preferences window without saving the preferences."));
		connect(button, &QPushButton::clicked, this, &PreferencesDialogue::close);
		buttons->addButton(button, QDialogButtonBox::RejectRole);

		vLayout->addWidget(buttons);
		setLayout(vLayout);
	}


	void PreferencesDialogue::updateWidgets(void) {
		const GuiPreferences * p = dynamic_cast<const GuiPreferences *>(qyncApp->preferences());

		if(p) {
			m_rsyncPath->setText(p->rsyncPath());
			m_presetsToolBar->setChecked(p->showPresetsToolBar());
			m_synchroniseToolBar->setChecked(p->showSynchroniseToolBar());

			switch(p->toolBarButtonStyle()) {
				default:
				case Qt::ToolButtonFollowStyle:
					m_toolBarStyle->setCurrentIndex(0);
					break;

				case Qt::ToolButtonIconOnly:
					m_toolBarStyle->setCurrentIndex(1);
					break;

				case Qt::ToolButtonTextUnderIcon:
					m_toolBarStyle->setCurrentIndex(2);
					break;

				case Qt::ToolButtonTextBesideIcon:
					m_toolBarStyle->setCurrentIndex(3);
					break;

				case Qt::ToolButtonTextOnly:
					m_toolBarStyle->setCurrentIndex(4);
					break;
			}
		}
		else {
			QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("There are no preferences to edit."), QMessageBox::Ok);
			close();
		}
	}


	void PreferencesDialogue::savePreferences(void) {
		GuiPreferences * p = new GuiPreferences();

		p->setRsyncPath(m_rsyncPath->text());
		p->setShowPresetsToolBar(m_presetsToolBar->isChecked());
		p->setShowSynchroniseToolBar(m_synchroniseToolBar->isChecked());

		switch(m_toolBarStyle->currentIndex()) {
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

		if(qyncApp->setPreferences(p)) {
			if(!p->saveAs(Application::configurationDirectoryPath() + "guipreferences"))
				QMessageBox::warning(this, tr("%1 Warnng").arg(qyncApp->applicationDisplayName()), tr("Your preferences were set but could not be stored. This means that next time you start %1 your preferences will revert to their previous settings.").arg(qyncApp->applicationDisplayName()), QMessageBox::Ok);
		}
		else {
			QMessageBox::warning(this, tr("%1 Warnng").arg(qyncApp->applicationDisplayName()), tr("Your preferences could not be applied."), QMessageBox::Ok);
			delete p;
		}
	}


	void PreferencesDialogue::savePreferencesAndClose(void) {
		savePreferences();
		close();
	}


	void PreferencesDialogue::chooseRsyncPath(void) {
		QString fileName = QFileDialog::getOpenFileName(this, tr("Choose rsync executable"), m_rsyncPath->text());
		if(!fileName.isEmpty()) {
			QFileInfo f(fileName);
			if(f.exists() && f.isFile() && f.isExecutable())
				m_rsyncPath->setText(fileName);
			else
				QMessageBox::warning(this, tr("%1 Warning").arg(qyncApp->applicationDisplayName()), tr("%1 is not a valid path to an rsync executable.").arg(fileName), QMessageBox::Ok);
		}
	}


}  // namespace Qync
