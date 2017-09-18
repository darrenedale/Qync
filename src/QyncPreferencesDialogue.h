/**
 * \file QyncPreferencesDialogue.h
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Definition of the QyncPreferencesDialogue class.
 *
 * \dep
 * - QDialog
 *
 * \todo
 * Nothing.
 */

#ifndef QYNCPREFERENCESDIALOGUE_H
#define QYNCPREFERENCESDIALOGUE_H

#include <QDialog>

class QyncManager;
class QLineEdit;
class QToolButton;
class QCheckBox;
class QComboBox;


/**
 * \class QyncPreferencesDialogue
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief The preferences window of the Qync GUI.
 *
 * The QyncPreferencesDialogue class is a QDialog subclass that enables the
 * user to modify the current application and GUI settings. It is based on the
 * assumption that the preferences object stored in the current application
 * manager is a QyncGuiPreferences object, and therefore is suitable only if
 * used along with the example GUI (or another GUI that uses the
 * QyncGuiPreferences class for its preferences).
 *
 * The dialogue provides access to the rsync path as well as setting the visible
 * toolbars and the style of the toolbars. The modified settings can be applied
 * while keeping the dialogue open, applied and the dialogue closed, or
 * forgotten completely. As the manager saves the preferences whenever they
 * are set, applied changes are always rememberd between sessions using the
 * application.
 */
class QyncPreferencesDialogue
:	public QDialog {

	Q_OBJECT

	private:
		QyncManager * m_manager;

		QLineEdit * m_rsyncPath;
		QToolButton * m_chooseRsyncPath;
		QCheckBox * m_presetsToolBar;
		QCheckBox * m_synchroniseToolBar;
		QComboBox * m_toolBarStyle;

	private slots:
		/**
		 * \brief Save the current preferences.
		 *
		 * The manager's preferences object is updated.
		 */
		void savePreferences( void );

		/**
		 * \brief Save the current preferences and close the preferences window.
		 *
		 * The manager's preferences object is updated and the preferences
		 * window is closed.
		 */
		void savePreferencesAndClose( void );

		/**
		 * \brief Choose the path to the rsync executable file.
		 *
		 * A local file browser is presented for the user to to choose the rsync
		 * executable. If the user does not cancel the dialogue, the chosen file
		 * is set as the text in the rsync path line edit.
		 */
		void chooseRsyncPath( void );

	protected:
		/**
		 * \brief Create all the widgets for the preferences window.
		 */
		virtual void createWidgets( void );

		/**
		 * \brief Update all the widgets in the preferences window.
		 *
		 * All the widgets in the preferences window are updated to reflect the
		 * state of the preferences in the manager.
		 */
		virtual void updateWidgets( void );

	public:
		/**
		 * \brief Create a new preferences dialogue.
		 *
		 * \param manager is the manager from which the preferences should
		 * be read.
		 * \param parent is the parent widget.
		 */
		explicit QyncPreferencesDialogue( QyncManager * manager = 0, QWidget *parent = 0 );

	signals:

	public slots:

};

#endif // QYNCPREFERENCESDIALOGUE_H
