/**
 * \file presetmenu.h
 * \author Darren Edale
 * \date September, 2017
 * \version 1.0.0
 *
 * \brief Declaration of the PresetMenu class.
 *
 * \dep
 * - QMenu
 */

#ifndef QYNC_PRESETMENU_H
#define QYNC_PRESETMENU_H

#include <QMenu>

namespace Qync {

	class Preset;

	class PresetMenu
	: public QMenu {
		Q_OBJECT

	public:
		explicit PresetMenu(QWidget * parent = nullptr);

		void addAction(void) = delete;
		void addActions(void) = delete;
		void addMenu(void) = delete;
		void addSection(void) = delete;
		void insertAction(void) = delete;
		void insertActions(void) = delete;
		void insertMenu(void) = delete;
		void insertSection(void) = delete;
		void insertSeparator(void) = delete;
		void removeAction(void) = delete;
		void clear(void) = delete;

	public Q_SLOTS:
		void refresh(void);

	Q_SIGNALS:
		void presetTriggered(Preset &);
		void presetIndexTriggered(int);
	};

}  // namespace Qync

#endif  // QYNC_PRESETMENU_H
