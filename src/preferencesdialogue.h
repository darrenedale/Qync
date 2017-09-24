/**
 * \file preferencesdialogue.h
 * \author Darren Edale
 * \date September 2017
 * \version 1.0.0
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
		explicit PreferencesDialogue(QWidget * parent = nullptr);
		~PreferencesDialogue(void);

	private Q_SLOTS:
		void apply(void);
		void save(void);
		void chooseRsyncPath(void);

	protected:
		virtual void updateWidgets(void);

	private:
		std::unique_ptr<Ui::PreferencesDialogue> m_ui;
	};

}  // namespace Qync

#endif  // QYNC_PREFERENCESDIALOGUE_H
