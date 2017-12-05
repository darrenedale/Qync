/**
 * \file presetcombo.cpp
 * \author Darren Edale
 * \date September, 2017
 * \version 1.1.0
 *
 * \brief Implementation of the PresetCombo class.
 *
 * \dep
 * - presetcombo.h
 * - application.h
 * - preset.h
 */

#include "presetcombo.h"

#include "application.h"
#include "preset.h"


namespace Qync {


	/**
	 * \namespace Qync::Detail::PresetCombo
	 * \brief Private namespace for implementation details for the PresetCombo
	 * class.
	 */
	namespace Detail {
		namespace PresetCombo {
			static constexpr int NewPresetRole = Qt::UserRole + 1;
			static constexpr int NewPresetTag = 99;
		}  // namespace PresetCombo
	}		// namespace Detail


	/**
	 * \class PresetCombo
	 * \author Darren Edale
	 * \date October 2017
	 * \version 1.1.0
	 *
	 * \brief A combo box specialised for selecting a sync preset.
	 *
	 * The combo box listens for changes in the list of presets stored in the
	 * Application and updates as required. If the list of presets is empty, it
	 * presents a single "&lt;New Preset&gt;" item. The choice of this special
	 * item can be determined by calling currentItemIsNewPreset(). If this
	 * returns \b false, the currently-selected preset can be retrieved by
	 * calling currentPreset().
	 *
	 * You can manually trigger the combo box to re-read the list of presets
	 * in the Application by calling reset().
	 *
	 * It is not possible to add, insert, remove or clear items to/from the
	 * combo box - the methods to do so have been explicitly deleted.
	 */


	/**
	 * \brief Create a PresetCombo.
	 *
	 * \param parent The owning parent widget.
	 */
	PresetCombo::PresetCombo(QWidget * parent)
	: QComboBox(parent) {
		refresh();

		connect(this, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int idx) {
			if(-1 == idx) {
				/* reset or no items */
				return;
			}

			if(itemData(idx, Detail::PresetCombo::NewPresetRole).toInt() == Detail::PresetCombo::NewPresetTag) {
				/* new selection is the <new preset> item */
				Q_EMIT newPresetSelected();
				return;
			}

			Q_EMIT currentPresetChanged(qyncApp->preset(idx));
		});
	}


	/**
	 * \brief Determine whether the current item is the "&lt;New Preset &gt;" item.
	 *
	 * The special "&lt;New Preset &gt;" item is the only item in the list when
	 * there are no presets in the Application's master list. In all other cases,
	 * this special item is absent.
	 *
	 * \return \b true if the special "new preset" item is selected, \b false
	 * otherwise.
	 */
	bool PresetCombo::currentItemIsNewPreset() const {
		return currentData(Detail::PresetCombo::NewPresetRole).toInt() == Detail::PresetCombo::NewPresetTag;
	}


	/**
	 * \brief Fetch the currently-selected preset.
	 *
	 * \warning *DO NOT* call this method if isNewPreset() returns \b true. In
	 * such a case and assertion in Application::preset() will fail.
	 *
	 * \return The selected preset.
	 */
	Preset & PresetCombo::currentPreset() const {
		return qyncApp->preset(currentIndex());
	}


	/**
	 * \brief Refresh the list of presets available in the combo box.
	 *
	 * On a call to this method the Application object is queried for the
	 * presets, and the combo box items are replaced with those
	 * representing the list of available presets.
	 */
	void PresetCombo::refresh() {
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
			setItemData(0, Detail::PresetCombo::NewPresetTag, Detail::PresetCombo::NewPresetRole);
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
