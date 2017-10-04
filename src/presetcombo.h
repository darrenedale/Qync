/**
 * \file presetcombo.h
 * \author Darren Edale
 * \date September, 2017
 * \version 1.0.0
 *
 * \brief Declaration of the PresetCombo class.
 *
 * \dep
 * - QComboBox
 */

#ifndef QYNC_PRESETCOMBO_H
#define QYNC_PRESETCOMBO_H

#include <QComboBox>

namespace Qync {

	class Preset;

	class PresetCombo
	: public QComboBox {
		Q_OBJECT

	public:
		explicit PresetCombo(QWidget * parent = nullptr);

		void addItem(void) = delete;
		void removeItem(void) = delete;
		void insertItem(void) = delete;
		void clear(void) = delete;

		bool currentItemIsNewPreset(void) const;
		Preset & currentPreset(void) const;

	Q_SIGNALS:
		void currentPresetChanged(Preset &);
		void newPresetSelected(void);

	public Q_SLOTS:
		void refresh(void);
	};

}  // namespace Qync

#endif  // QYNC_PRESETCOMBO_H
