/**
 * \file qync.cpp
 * \author Darren Edale
 * \date October 2017
 * \version 1.1.0
 *
 * \brief Qync main application file.
 *
 * This file implements the main() function that is the application entry point.
 *
 * \dep
 * - QDebug
 * - application.h
 * - mainwindow.h
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
