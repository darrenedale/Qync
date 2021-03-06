/**
 * @file presetcombo.h
 * @author Darren Edale
 * @date April, 2020
 * @version 1.1.1
 *
 * @brief Declaration of the PresetCombo class.
 */

#ifndef QYNC_PRESETCOMBO_H
#define QYNC_PRESETCOMBO_H

#include <QtWidgets/QComboBox>

namespace Qync {

	class Preset;

	class PresetCombo
	: public QComboBox {
		Q_OBJECT

	public:
		explicit PresetCombo(QWidget * parent = nullptr);

		void addItem() = delete;
		void addItems() = delete;
		void insertItem() = delete;
		void insertItems() = delete;
		void removeItem() = delete;
		void clear() = delete;

		[[nodiscard]] bool currentItemIsNewPreset() const;
		[[nodiscard]] Preset & currentPreset() const;

	Q_SIGNALS:
		void currentPresetChanged(Preset &);
		void newPresetSelected();

	public Q_SLOTS:
		void refresh();
	};

}  // namespace Qync

#endif  // QYNC_PRESETCOMBO_H
