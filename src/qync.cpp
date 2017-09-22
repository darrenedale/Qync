/**
 * \file qync.cpp
 * \author Darren Edale
 * \date September 2017
 * \version 0.9.6
 *
 * \brief Qync main application file.
 *
 * This file implements the main() function that is the application entry point.
 *
 * \dep
 * - QDebug
 * - application.h
 * - mainwindow.h
 *
 * \todo make a nice application icon (or get someone else to make one).
 * \todo add application icons of all sizes to package
 */

#include <QDebug>

#include "application.h"
#include "mainwindow.h"


/**
 * \brief The main entry point for the Qync application.
 *
 * This function instantiates a Qync::Application and initiates its
 * event loop.
 *
 * \return 0 on clean exit, non-0 otherwise.
 */
int main(int argc, char ** argv) {
	Qync::Application app(argc, argv);
	return app.exec();
}
