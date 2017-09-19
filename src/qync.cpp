/**
 * \file qync.cpp
 * \author Darren Hatherley
 * \date 13th December, 2013
 * \version 0.9.5
 *
 * \brief Qync main application file.
 *
 * This file implements the main() function that is the application entry point.
 *
 * \dep
 * - QyncManager.h
 * - QyncMainWindow.h
 * - QApplication
 *
 * \todo
 * - make a nice application icon (or get someone else to make one).
 * - add application icons of all sizes to package
 */

#include "mainwindow.h"
#include "application.h"

#include <QApplication>
#include <QDebug>


/**
 * \brief The main entry point for the Qync application.
 *
 * This function instantiates a QApplication, creates a main window and a
 * manager (which is given to the main window), shows the main window and
 * executes the application event loop.
 *
 * \return 0 on clean exit, non-0 otherwise.
 */
int main(int argc, char ** argv) {
	Qync::Application app(argc, argv);
	Qync::MainWindow win;
	win.show();
	return app.exec();
}
