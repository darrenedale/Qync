#ifndef QYNC_ABOUTDIALOGUE_H
#define QYNC_ABOUTDIALOGUE_H

#include <QDialog>

namespace Qync {

	namespace Ui {
		class AboutDialogue;
	}

	class AboutDialogue : public QDialog
	{
			Q_OBJECT

		public:
			explicit AboutDialogue(QWidget *parent = 0);
			~AboutDialogue();

		private:
			Ui::AboutDialogue *ui;
	};


} // namespace Qync
#endif // QYNC_ABOUTDIALOGUE_H
