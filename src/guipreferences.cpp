/**
 * \file guipreferences.cpp
 * \author Darren Edale
 * \date September 2017
 * \version 1.0.0
 *
 * \brief Implementation of the GuiPreferences class.
 *
 * \dep
 * - guipreferences.h
 * - functions.h
 * - QDebug
 */

#include "guipreferences.h"

#include "application.h"
#include "functions.h"

#include <QDebug>


namespace Qync {


	/**
	 * \class GuiPreferences
	 * \author Darren Edale
	 * \date September 2017
	 * \version 1.0.0
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
	GuiPreferences::GuiPreferences(const QString & fileName)
	: Preferences(),
	  m_simpleUi(true),
	  m_presetsToolbar(true),
	  m_syncToolbar(true),
	  m_toolButtonStyle(Qt::ToolButtonFollowStyle) {
		loadFrom(fileName);
	}


	/**
	 * \brief Destroy the QuncGuiPreferences object.
	 */
	GuiPreferences::~GuiPreferences(void) = default;


	/**
	 * \brief Set the default values for all settings.
	 *
	 * By default, both toolbars are shown and the toolbar button style is
	 * set to the default for the current Qt theme. See the documentation
	 * for \ref GuiPreferences::setDefaults() for the defaults for
	 * settings governed by that class.
	 */
	void GuiPreferences::setDefaults(void) {
		Preferences::setDefaults();
		setShowPresetsToolBar(true);
		setShowSynchroniseToolBar(true);
		setToolBarButtonStyle(Qt::ToolButtonFollowStyle);
	}


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
	Qt::ToolButtonStyle GuiPreferences::parseToolButtonStyleText(const QString & style, bool * ok) {
		if("icononly" == style.trimmed().toLower()) {
			if(ok) {
				*ok = true;
			}

			return Qt::ToolButtonIconOnly;
		}
		else if("textonly" == style.trimmed().toLower()) {
			if(ok) {
				*ok = true;
			}

			return Qt::ToolButtonTextOnly;
		}
		else if("textbesideicon" == style.trimmed().toLower()) {
			if(ok) {
				*ok = true;
			}

			return Qt::ToolButtonTextBesideIcon;
		}
		else if("textundericon" == style.trimmed().toLower()) {
			if(ok) {
				*ok = true;
			}

			return Qt::ToolButtonTextUnderIcon;
		}
		else if("styledefault" == style.trimmed().toLower()) {
			if(ok) {
				*ok = true;
			}

			return Qt::ToolButtonFollowStyle;
		}

		if(ok) {
			*ok = false;
		}

		return Qt::ToolButtonFollowStyle;
	}


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
	bool GuiPreferences::emitXmlElements(QXmlStreamWriter & xml) const {
		Preferences::emitXmlElements(xml);
		xml.writeStartElement("guipreferences");
		emitSimpleUiXml(xml);
		emitPresetsToolbarXml(xml);
		emitSynchroniseToolbarXml(xml);
		emitToolBarButtonStyleXml(xml);
		xml.writeEndElement(); /* guipreferences */
		return true;
	}


	/**
	 * \brief Write the "use simple UI" setting to an XML stream.
	 *
	 * \param xml is the stream to which to write.
	 *
	 * \return \b true if the setting was written, \b false otherwise.
	 */
	bool GuiPreferences::emitSimpleUiXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("simpleui");
		xml.writeCharacters(useSimpleUi() ? "true" : "false");
		xml.writeEndElement();
		return true;
	}


	/**
	 * \brief Write the prests toolbar setting to an XML stream.
	 *
	 * \param xml is the stream to which to write.
	 *
	 * \return \b true if the setting was written, \b false otherwise.
	 */
	bool GuiPreferences::emitPresetsToolbarXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("presetstoolbar");
		xml.writeCharacters(showPresetsToolBar() ? "true" : "false");
		xml.writeEndElement();
		return true;
	}


	/**
	 * \brief Write the synchronise toolbar setting to an XML stream.
	 *
	 * \param xml is the stream to which to write.
	 *
	 * \return \b true if the setting was written, \b false otherwise.
	 */
	bool GuiPreferences::emitSynchroniseToolbarXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("synchronisetoolbar");
		xml.writeCharacters(showSynchroniseToolBar() ? "true" : "false");
		xml.writeEndElement();
		return true;
	}


	/**
	 * \brief Write the toolbar button style setting to an XML stream.
	 *
	 * \param xml is the stream to which to write.
	 *
	 * \return \b true if the setting was written, \b false otherwise.
	 */
	bool GuiPreferences::emitToolBarButtonStyleXml(QXmlStreamWriter & xml) const {
		xml.writeStartElement("toolbarbuttonstyle");

		switch(toolBarButtonStyle()) {
			case Qt::ToolButtonIconOnly:
				xml.writeCharacters("IconOnly");
				break;

			case Qt::ToolButtonTextOnly:
				xml.writeCharacters("TextOnly");
				break;

			case Qt::ToolButtonTextBesideIcon:
				xml.writeCharacters("TextBesideIcon");
				break;

			case Qt::ToolButtonTextUnderIcon:
				xml.writeCharacters("TextUnderIcon");
				break;

			case Qt::ToolButtonFollowStyle:
				xml.writeCharacters("StyleDefault");
				break;
		}

		xml.writeEndElement();
		return true;
	}


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
	bool GuiPreferences::parseXmlElement(QXmlStreamReader & xml) {
		Q_ASSERT(xml.isStartElement());

		if("guipreferences" == xml.name()) {
			return parseGuiPreferencesXml(xml);
		}

		return Preferences::parseXmlElement(xml);
	}


	/**
	 * \brief Read all the GUI settings from an XML stream.
	 *
	 * \param xml is the stream from which to read.
	 *
	 * \return \b true if the settings were read, \b false otherwise.
	 */
	bool GuiPreferences::parseGuiPreferencesXml(QXmlStreamReader & xml) {
		Q_ASSERT(xml.isStartElement() && "guipreferences" == xml.name());

		while(!xml.atEnd()) {
			xml.readNext();

			if(xml.isEndElement()) {
				break;
			}

			if(xml.isCharacters()) {
				if(!xml.isWhitespace()) {
					qWarning() << __PRETTY_FUNCTION__ << "GuiPreferences::parseGuiPreferencesXml() - ignoring extraneous non-whitespace content at line" << xml.lineNumber() << "column" << xml.columnNumber();
				}

				/* ignore extraneous characters */
				continue;
			}

			if("simpleui" == xml.name()) {
				QString v = xml.readElementText();
				auto value = parseBooleanText(v);

				if(value.has_value()) {
					setUseSimpleUi(value);
				}
			}
			else if("presetstoolbar" == xml.name()) {
				auto value = parseBooleanText(xml.readElementText());

				if(value.has_value()) {
					setShowPresetsToolBar(value);
				}
			}
			else if("synchronisetoolbar" == xml.name()) {
				auto value = parseBooleanText(xml.readElementText());

				if(value.has_value()) {
					setShowSynchroniseToolBar(value);
				}
			}
			else if("toolbarbuttonstyle" == xml.name()) {
				bool ok;
				Qt::ToolButtonStyle value = GuiPreferences::parseToolButtonStyleText(xml.readElementText(), &ok);

				if(ok) {
					this->setToolBarButtonStyle(value);
				}
			}
			else {
				qWarning() << __PRETTY_FUNCTION__ << "found unexpected XML element" << xml.name() << "at line" << xml.lineNumber();
				Qync::parseUnknownElementXml(xml);
			}
		}

		return true;
	}


	/**
	 * \fn GuiPreferences::simpleUi(void)
	 * \brief Check whether the simple user interface should be used.
	 *
	 * \return \b true if the simple user interface should be used, \b false
	 * if the full user interface should be used.
	 */


	/**
	 * \fn GuiPreferences::setSimpleUi( bool use )
	 * \brief Set whether the simple user interface should be used.
	 *
	 * \param use \b true if the simple user interface should be used, \b false
	 * if the full user interface should be used.
	 */


	/**
	 * \fn GuiPreferences::showPresetsToolBar(void)
	 * \brief Check whether the presets toolbar should be shown.
	 *
	 * \return \b true if the presets toolbar should be shown, \b false
	 * if not.
	 */


	/**
	 * \fn GuiPreferences::showSynchroniseToolBar(void)
	 * \brief Check whether the synchronise toolbar should be shown.
	 *
	 * \return \b true if the synchronise toolbar should be shown, \b false
	 * if not.
	 */


	/**
	 * \fn GuiPreferences::setShowPresetsToolBar(bool)
	 * \brief Set whether the presets toolbar should be shown.
	 *
	 * \param show indicates whether the presets toolbar should be shown.
	 */


	/**
	 * \fn GuiPreferences::setShowSynchroniseToolBar(bool)
	 * \brief Set whether the synchronise toolbar should be shown.
	 *
	 * \param show indicates whether the synchronise toolbar should be
	 * shown.
	 */


	/**
	 * \fn GuiPreferences::toolBarButtonStyle(void)
	 * \brief Get the display style for buttons in the toolbar.
	 *
	 * The toolbar style is one of Qt's ToolButtonStyles. This can be
	 * either icons only, text only, icons with text underneath, icons with
	 * text beside them, or the default setting (one of the previous four)
	 * chosen by the current Qt theme.
	 *
	 * \return The display style for toolbar buttons.
	 */


	/**
	 * \fn GuiPreferences::setToolBarButtonStyle(const Qt::ToolButtonStyle)
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


}  // namespace Qync
