/**
 * @file preferencesdialogue.h
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief Declaration of the PreferencesDialogue class.
 */

#ifndef QYNC_PREFERENCESDIALOGUE_H
#define QYNC_PREFERENCESDIALOGUE_H

#include <memory>

#include <QtWidgets/QDialog>

namespace Qync {

	namespace Ui {
		class PreferencesDialogue;
	}

	class PreferencesDialogue
	: public QDialog {
		Q_OBJECT

	public:
		explicit PreferencesDialogue(QWidget * = nullptr);
		~PreferencesDialogue() override;

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
