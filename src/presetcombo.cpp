#include "presetcombo.h"

#include "application.h"
#include "preset.h"


namespace Qync {


	namespace PresetComboDetail {
		static constexpr int NewPresetRole = Qt::UserRole + 1;
		static constexpr int NewPresetTag = 99;
	}  // namespace PresetComboDetail


	PresetCombo::PresetCombo(QWidget * parent)
	: QComboBox(parent) {
		refresh();

		connect(this, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int idx) {
			if(-1 == idx) {
				/* reset or no items */
				return;
			}

			if(itemData(idx, PresetComboDetail::NewPresetRole).toInt() == PresetComboDetail::NewPresetTag) {
				/* new selection is the <new preset> item */
				Q_EMIT newPresetSelected();
				return;
			}

			Q_EMIT currentPresetChanged(qyncApp->preset(idx));
		});
	}


	bool PresetCombo::currentItemIsNewPreset(void) const {
		return currentData(PresetComboDetail::NewPresetRole).toInt() == PresetComboDetail::NewPresetTag;
	}


	/* DO NOT call if isNewPreset() is true: assertion in Application::preset() will fail */
	Preset & PresetCombo::currentPreset(void) const {
		return qyncApp->preset(currentIndex());
	}


	void PresetCombo::refresh(void) {
		int idx = currentIndex();
		QString name = currentText();
		QComboBox::clear();
		int i = 0;

		for(const auto & preset : qyncApp->presets()) {
			QComboBox::addItem(preset->name());
			++i;
		}

		if(0 == i) {
			QComboBox::insertItem(0, tr("<New Preset>"));
			setItemData(0, PresetComboDetail::NewPresetTag, PresetComboDetail::NewPresetRole);
		}

		if(-1 == idx) {
			return;
		}

		/* try to preserve the previous selection */
		int nameIdx = findText(name);

		if(-1 != nameIdx) {
			setCurrentIndex(nameIdx);
		}
		else if(count() <= idx) {
			setCurrentIndex(idx);
		}
	}


}  // namespace Qync
