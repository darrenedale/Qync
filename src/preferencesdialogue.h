/**
 * \file preferencesdialogue.h
 * \author Darren Edale
 * \date October 2017
 * \version 1.1.0
 *
 * \brief Declaration of the PreferencesDialogue class.
 *
 * \dep
 * - memory
 * - QDialog
 */

#ifndef QYNC_PREFERENCESDIALOGUE_H
#define QYNC_PREFERENCESDIALOGUE_H

#include <memory>

#include <QDialog>

namespace Qync {

	namespace Ui {
		class PreferencesDialogue;
	}

	class PreferencesDialogue
	: public QDialog {
		Q_OBJECT

	public:
		explicit PreferencesDialogue(QWidget * = nullptr);
		~PreferencesDialogue();

	private Q_SLOTS:
		void apply();
		void save();
		void chooseRsyncPath();

	protected:
		virtual void updateWidgets();

	private:
		std::unique_ptr<Ui::PreferencesDialogue> m_ui;
	};

}  // namespace Qync

#endif  // QYNC_PREFERENCESDIALOGUE_H
