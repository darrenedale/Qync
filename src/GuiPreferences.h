/**
 * \file GuiPreferences.h
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Definition of the GuiPreferences class.
 *
 * \dep
 * - QyncPreferences.h
 * - QString
 * - QXmlStreamWriter
 * - QXmlStreamReader
 *
 * \todo
 * Nothing.
 */

#ifndef QYNC_GUIPREFERENCES_H
#define QYNC_GUIPREFERENCES_H

#include "Preferences.h"

#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace Qync {

	/**
 * \class GuiPreferences
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief A class representing application preferences including GUI
 * preferences.
 *
 * This is an extension of the Preferences class to record preferences for
 * the following GUI-related items:
 * - whether or not the presets toolbar is shown
 * - whether or not the synchronise toolbar is shown
 * - the style of buttons on the toolbar(s):
 *   - Icons only
 *   - Text only
 *   - Icons with text below
 *   - Icons with text beside
 *   - Whatever style the current visual theme suggests
 */
	class GuiPreferences
	  : public Preferences {
	private:
		bool m_presetsToolbar;
		bool m_syncToolbar;
		Qt::ToolButtonStyle m_toolButtonStyle;

	protected:
		/**
		 * \brief Convert the text representation of a toolbar button style to
		 * a Qt:: ToolButtonStyle value.
		 *
		 * \param style is the style text to convert.
		 * \param ok is a pointer to a boolean value that will be set to \b true
		 * if the conversion is successful, \b false if not.
		 *
		 * This is a helper method to parse the content of the toolbar button
		 * style setting from the preferences file. A valid style is always
		 * returned - callers must provide an \b ok parameter and check its
		 * content when the method returns if they need to check whether the
		 * returned value represents the text provided or is just a default
		 * value because the text was not valid.
		 *
		 * \return The toolbar button style.
		 */
		static Qt::ToolButtonStyle parseToolButtonStyleText(const QString & style, bool * ok = nullptr);

		/**
		 * \brief Set the default values for all settings.
		 *
		 * By default, both toolbars are shown and the toolbar button style is
		 * set to the default for the current Qt theme. See the documentation
		 * for \ref QyncPreferences::setDefaults() for the defaults for
		 * settings governed by that class.
		 */
		virtual void setDefaults(void);

		/**
		 * \brief Read an element from an XML stream.
		 *
		 * \param xml is the stream from which to read.
		 *
		 * This is a virtual method. This reimplementation passes all
		 * unrecognised XML elements to the parseXmlElement() method of
		 * QyncPreferences.
		 *
		 * \return \b true if the settings were read, \b false otherwise.
		 */
		virtual bool parseXmlElement(QXmlStreamReader & xml);

		/**
		 * \brief Read all the GUI settings from an XML stream.
		 *
		 * \param xml is the stream from which to read.
		 *
		 * \return \b true if the settings were read, \b false otherwise.
		 */
		bool parseGuiPreferencesXml(QXmlStreamReader & xml);

		/**
		 * \brief Write all settings to an XML stream.
		 *
		 * \param xml is the stream to which to write.
		 *
		 * This is a virtual method. This reimplementation passes the call back
		 * up the class hierarchy by calling QyncPreferences::emitXmlElements()
		 * before it emits its own elements.
		 *
		 * \return \b true if the settings were written, \b false otherwise.
		 */
		virtual bool emitXmlElements(QXmlStreamWriter & xml) const;

		/**
		 * \brief Write all GUI settings to an XML stream.
		 *
		 * \param xml is the stream to which to write.
		 *
		 * \return \b true if the settings were written, \b false otherwise.
		 */
		bool emitGuiPreferencesXml(QXmlStreamWriter & xml) const;

		/**
		 * \brief Write the prests toolbar setting to an XML stream.
		 *
		 * \param xml is the stream to which to write.
		 *
		 * \return \b true if the setting was written, \b false otherwise.
		 */
		bool emitPresetsToolbarXml(QXmlStreamWriter & xml) const;

		/**
		 * \brief Write the synchronise toolbar setting to an XML stream.
		 *
		 * \param xml is the stream to which to write.
		 *
		 * \return \b true if the setting was written, \b false otherwise.
		 */
		bool emitSynchroniseToolbarXml(QXmlStreamWriter & xml) const;

		/**
		 * \brief Write the toolbar button style setting to an XML stream.
		 *
		 * \param xml is the stream to which to write.
		 *
		 * \return \b true if the setting was written, \b false otherwise.
		 */
		bool emitToolBarButtonStyleXml(QXmlStreamWriter & xml) const;

	public:
		/**
		 * \brief Create a new QuncGuiPreferences object.
		 *
		 * \param fileName is the path to the file from which to load the
		 * preferences.
		 *
		 * The file name is optional. If it is not provided, is not a valid
		 * preferences file path or cannot be loaded for some other reason
		 * (e.g. the file is not readable), the preferences will be set to
		 * defaults.
		 */
		explicit GuiPreferences(const QString & fileName = QString());

		/**
		 * \brief Destroy the QuncGuiPreferences object.
		 */
		virtual ~GuiPreferences(void) {}

		/**
		 * \brief Check whether the presets toolbar should be shown.
		 *
		 * \return \b true if the presets toolbar should be shown, \b false
		 * if not.
		 */
		bool showPresetsToolBar(void) const;

		/**
		 * \brief Check whether the synchronise toolbar should be shown.
		 *
		 * \return \b true if the synchronise toolbar should be shown, \b false
		 * if not.
		 */
		bool showSynchroniseToolBar(void) const;

		/**
		 * \brief Set whether the presets toolbar should be shown.
		 *
		 * \param show indicates whether the presets toolbar should be shown.
		 */
		void setShowPresetsToolBar(bool show);

		/**
		 * \brief Set whether the synchronise toolbar should be shown.
		 *
		 * \param show indicates whether the synchronise toolbar should be
		 * shown.
		 */
		void setShowSynchroniseToolBar(bool show);

		/**
		 * \brief Get the display style for buttons in the toolbar.
		 *
		 * The toolbar style is one of Qt's ToolButtonStyles. This can be
		 * either icons only, text only, icons with text underneath, icons with
		 * text beside them, or the default setting (one of the previous four)
		 * chosen by the current Qt theme.
		 *
		 * \return The display style for toolbar buttons.
		 */
		Qt::ToolButtonStyle toolBarButtonStyle(void) const;

		/**
		 * \brief Set the display style for buttons in the toolbar.
		 *
		 * \param style is the style to use.
		 *
		 * The toolbar style is one of Qt's ToolButtonStyles. This can be
		 * either icons only, text only, icons with text underneath, icons with
		 * text beside them, or the default setting (one of the previous four)
		 * chosen by the current Qt theme.
		 *
		 * \return \b true if the style was set, \b false otherwise.
		 */
		bool setToolBarButtonStyle(const Qt::ToolButtonStyle & style);
	};

}  // namespace Qync

#endif  // QYNC_GUIPREFERENCES_H
