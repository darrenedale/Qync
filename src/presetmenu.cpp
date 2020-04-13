/**
 * @file presetmenu.cpp
 * @author Darren Edale
 * @date April, 2020
 * @version 1.1.1
 *
 * @brief Declaration of the PresetMenu class.
 */

#include "presetmenu.h"

#include "application.h"
#include "preset.h"

using namespace Qync;

/**
 * @class PresetMenu
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief A combo menu specialised for selecting a sync preset.
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
 * It is not possible to add, insert, remove or clear items to/from
 * the menu - the methods to do so have been explicitly deleted.
 */

/**
 * @brief Create a PresetMenu.
 *
 * @param parent The owning parent widget.
 */
PresetMenu::PresetMenu(QWidget * parent)
:   QMenu(parent)
{
    refresh();

    connect(this, &QMenu::triggered, [this](QAction * action) {
        int idx = action->data().toInt();
        Q_EMIT presetTriggered(qyncApp->preset(idx));
        Q_EMIT presetIndexTriggered(idx);
    });

    connect(qyncApp, &Application::presetsChanged, this, &PresetMenu::refresh);
}

/**
 * @brief Refresh the list of presets available in the menu.
 *
 * On a call to this method the Application object is queried for the
 * presets, and the menu items are replaced with those representing
 * the list of available presets.
 */
void PresetMenu::refresh() {
    QMenu::clear();
    int idx = 0;

    for(const auto & preset : qyncApp->presets()) {
        QAction * action = QMenu::addAction(preset->name());
        action->setData(idx);
        ++idx;
    }
}
