/**
 * \file presetmenu.cpp
 * \author Darren Edale
 * \date September, 2017
 * \version 1.0.0
 *
 * \brief Declaration of the PresetMenu class.
 *
 * \dep
 * - presetmenu.h
 * - application.h
 * - preset.h
 *
 * \todo ensure that all methods to change the contents of the menu have
 * been marked as deleted.
 */

#include "presetmenu.h"

#include "application.h"
#include "preset.h"


namespace Qync {


	/**
	 * \class PresetMenu
	 * \author Darren Edale
	 * \date October 2017
	 * \version 1.1.0
	 *
	 * \brief A combo menu specialised for selecting a sync preset.
	 *
	 * The menu listens for changes in the list of presets stored in the
	 * Application and updates as required. You can manually trigger the
	 * menu to re-read the list of presets in the Application by calling
	 * reset().
	 *
	 * When an item from the menu is selected by the user, the
	 * presetTriggered() and presetIndexTriggered() signals are emitted.
	 * The first of these signals sends a reference to the Preset object
	 * represented by the triggered menu item. The second provides its
	 * index in the Application's preset list. There is no benefit to
	 * using the index-based signal over the Preset-based signal if you
	 * are able to use both - the Preset is passed by const reference
	 * and is therefore just as efficient as using an int.
	 *
	 * It is not possible to add or clear items to/from the menu - the
	 * methods to do so have been explicitly deleted.
	 */


	PresetMenu::PresetMenu(QWidget * parent)
	: QMenu(parent) {
		refresh();
		connect(this, &QMenu::triggered, [this](QAction * action) {
			int idx = action->data().toInt();
			Q_EMIT presetTriggered(qyncApp->preset(idx));
			Q_EMIT presetIndexTriggered(idx);
		});

		connect(qyncApp, &Application::presetsChanged, this, &PresetMenu::refresh);
	}


	void PresetMenu::refresh(void) {
		QMenu::clear();
		int i = 0;

		for(const auto & preset : qyncApp->presets()) {
			QAction * action = QMenu::addAction(preset->name());
			action->setData(i);
			++i;
		}
	}


}  // namespace Qync
