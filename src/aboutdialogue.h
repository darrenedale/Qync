/**
 * @file aboutdialogue.h
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief Declaration of the AboutDialogue class.
 */

#ifndef QYNC_ABOUTDIALOGUE_H
#define QYNC_ABOUTDIALOGUE_H

#include <memory>

#include <QtWidgets/QDialog>

class QString;

namespace Qync {

	namespace Ui {
		class AboutDialogue;
	}

	class AboutDialogue
	: public QDialog {
		Q_OBJECT

	public:
		AboutDialogue();
		~AboutDialogue() override;

	private:
		static QString & processPlaceholders(QString && content);

		std::unique_ptr<Ui::AboutDialogue> m_ui;
	};

}  // namespace Qync

#endif  // QYNC_ABOUTDIALOGUE_H
