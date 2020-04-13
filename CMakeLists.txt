cmake_minimum_required(VERSION 3.8.0)	# required for c++17

project(Qync LANGUAGES CXX VERSION 1.1.1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 5.7 REQUIRED COMPONENTS Core Widgets)

add_executable(qync
	src/main.cpp
	src/functions.cpp
	src/application.cpp
	src/guipreferences.cpp
	src/mainwindow.cpp
	src/preferences.cpp
	src/preferencesdialogue.cpp
	src/preset.cpp
	src/process.cpp
	src/processdialogue.cpp
	src/aboutdialogue.cpp
	src/sourcedestinationwidget.cpp
	src/synchronisewhatcombo.cpp
	src/processwidget.cpp
	src/presetcombo.cpp
	src/presetmenu.cpp
	src/notificationwidget.cpp
	
	resources/icons.qrc
	resources/toolbaricons.qrc
)

target_compile_features(qync PRIVATE cxx_std_17)
target_link_libraries(qync Qt5::Core Qt5::Widgets)
set_target_properties(qync PROPERTIES
		PROJECT_LABEL Qync
		AUTOUIC_SEARCH_PATHS src/ui
		)

# cpack
include(InstallRequiredSystemLibraries)

set(CPACK_COMPONENTS_IGNORE_GROUPS 1)
set(CPACK_GENERATOR DEB TGZ)

#set(CPACK_DEB_COMPONENT_INSTALL ON)
#set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

#set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")

set(CPACK_PACKAGE_CONTACT "darren -at- equituk -dot- net")

#set(CPACK_COMPONENTS_ALL application libraries)

set(CPACK_DEBIAN_PACKAGE_DEPENDS "rsync (>=3.0.7), libqt5widgets5 (>=5.7), libqt5core5a (>=5.7)")
set(CPACK_DEBIAN_PACKAGE_SECTION "utilities")
set(CPACK_DEBIAN_PACKAGE_REPLACES "qync-qt5")

set(CPACK_PACKAGE_VENDOR "Équit")
set(CPACK_PACKAGE_DESCRIPTION "A file backup/synchronisation utility.")
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/dist/gplv3.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Qync ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")

if(WIN32 AND NOT UNIX)
  set(CPACK_GENERATOR "${CPACK_GENERATOR} NSIS")
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/dist\\\\application_icon_32.png")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\Qonvince.exe")
  set(CPACK_NSIS_DISPLAY_NAME "A file backup/synchronisation utility.")
  set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.my-project-home-page.org")
  set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.my-personal-home-page.com")
  set(CPACK_NSIS_CONTACT "me@my-personal-home-page.com")
  set(CPACK_NSIS_MODIFY_PATH ON)
else(WIN32 AND NOT UNIX)
  set(CPACK_STRIP_FILES "bin/qync")
  set(CPACK_SOURCE_STRIP_FILES "")
endif(WIN32 AND NOT UNIX)

set(CPACK_PACKAGE_EXECUTABLES "qync" "Qync")
include(CPack)

set(CMAKE_INSTALL_PREFIX "/usr")
install(TARGETS qync RUNTIME DESTINATION bin)
install(FILES "dist/linux/qync.desktop"
	DESTINATION "share/applications"
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
	CONFIGURATIONS Release)
install(FILES "dist/linux/qync.menu"
	DESTINATION "share/menu"
	RENAME "qync"
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
	CONFIGURATIONS Release
	OPTIONAL)
install(FILES "dist/linux/doc/qync/copyright" "dist/linux/doc/qync/changelog" "dist/linux/doc/qync/changelog.Debian"
	DESTINATION "share/doc/qync"
	PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
	CONFIGURATIONS Release)
install(FILES "resources/icons/application.svg"
	DESTINATION "share/icons/hicolor/scalable/apps"
	RENAME "qync.svg"
	PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
	CONFIGURATIONS Release)
install(FILES "dist/linux/man/man1/qync.1"
	DESTINATION "share/man/man1"
	PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
	CONFIGURATIONS Release)
