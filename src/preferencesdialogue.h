/**
 * \file PreferencesDialogue.h
 * \author Darren Edale
 * \date September 2017
 * \version 0.9.6
 *
 * \brief Declaration of the PreferencesDialogue class.
 *
 * \dep
 * - QDialog
 */

#ifndef QYNC_PREFERENCESDIALOGUE_H
#define QYNC_PREFERENCESDIALOGUE_H

#include <memory>

#include <QDialog>

//class QLineEdit;
//class QToolButton;
//class QCheckBox;
//class QComboBox;

namespace Qync {

	namespace Ui {
		class PreferencesDialogue;
	}

	class PreferencesDialogue
	  : public QDialog {
		Q_OBJECT

	public:
		explicit PreferencesDialogue(QWidget * parent = nullptr);
		~PreferencesDialogue(void);

	private Q_SLOTS:
		void savePreferences(void);
		void savePreferencesAndClose(void);
		void chooseRsyncPath(void);

	protected:
		virtual void updateWidgets(void);

	private:
		std::unique_ptr<Ui::PreferencesDialogue> m_ui;
		//		QLineEdit * m_rsyncPath;
		//		QToolButton * m_chooseRsyncPath;
		//		QCheckBox * m_presetsToolBar;
		//		QCheckBox * m_synchroniseToolBar;
		//		QComboBox * m_toolBarStyle;
	};

}  // namespace Qync

#endif  // QYNC_PREFERENCESDIALOGUE_H
