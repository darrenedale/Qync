#ifndef PRESETCOMBO_H
#define PRESETCOMBO_H

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
		void insertIntem(void) = delete;
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

#endif  // PRESETCOMBO_H
