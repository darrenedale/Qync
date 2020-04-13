/**
 * @file presetmenu.h
 * @author Darren Edale
 * @date April, 2020
 * @version 1.1.1
 *
 * @brief Declaration of the PresetMenu class.
 *
 * @dep
 * - QMenu
 */

#ifndef QYNC_PRESETMENU_H
#define QYNC_PRESETMENU_H

#include <QtWidgets/QMenu>

namespace Qync {

	class Preset;

	class PresetMenu
	: public QMenu {
		Q_OBJECT

	public:
		explicit PresetMenu(QWidget * = nullptr);

		void addAction() = delete;
		void addActions() = delete;
		void addMenu() = delete;
		void addSection() = delete;
		void insertAction() = delete;
		void insertActions() = delete;
		void insertMenu() = delete;
		void insertSection() = delete;
		void insertSeparator() = delete;
		void removeAction() = delete;
		void clear() = delete;

	public Q_SLOTS:
		void refresh();

	Q_SIGNALS:
		void presetTriggered(Preset &);
		void presetIndexTriggered(int);
	};

}  // namespace Qync

#endif  // QYNC_PRESETMENU_H
