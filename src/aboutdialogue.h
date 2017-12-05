/**
 * \file aboutdialogue.h
 * \author Darren Edale
 * \date October 2017
 * \version 1.1.0
 *
 * \brief Declaration of the AboutDialogue class.
 *
 * \dep
 * - memory
 * - QDialog
 */

#ifndef QYNC_ABOUTDIALOGUE_H
#define QYNC_ABOUTDIALOGUE_H

#include <memory>

#include <QDialog>

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
		~AboutDialogue();

	private:
		static QString & processPlaceholders(QString && content);

		std::unique_ptr<Ui::AboutDialogue> m_ui;
	};

}  // namespace Qync

#endif  // QYNC_ABOUTDIALOGUE_H
