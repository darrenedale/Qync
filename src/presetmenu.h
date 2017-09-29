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

		void clear(void) = delete;
		void addAction(void) = delete;

	public Q_SLOTS:
		void refresh(void);

	Q_SIGNALS:
		void presetTriggered(Preset &);
		void presetIndexTriggered(int);
	};

}  // namespace Qync

#endif  // QYNC_PRESETMENU_H
