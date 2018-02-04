import qbs

CppApplication {
	name: "Qync"
    cpp.cxxLanguageVersion: "c++17"

	Depends {
		name: "Qt"
		submodules: [ "core", "widgets", "gui" ]
		required: true
	}

    Qt.core.enableKeywords: false

	Group {
		name: "Tools"
		files: [
			"tools/*",
			".gitignore",
			".clang-format",
		]
	}

	Group {
		name: "Headers"
		files: [
			"src/functions.h",
			"src/application.h",
			"src/guipreferences.h",
			"src/mainwindow.h",
			"src/preferences.h",
			"src/preferencesdialogue.h",
			"src/preset.h",
			"src/process.h",
			"src/processdialogue.h",
			"src/aboutdialogue.h",
			"src/sourcedestinationwidget.h",
			"src/units.h",
			"src/synchronisewhatcombo.h",
			"src/processwidget.h",
			"src/presetcombo.h",
			"src/presetmenu.h",
			"src/notificationwidget.h",
			"src/types.h",
		]
	}

	Group {
		name: "UI"
		files: [
			"src/mainwindow.cpp",
			"src/preferencesdialogue.cpp",
			"src/processdialogue.cpp",
			"src/aboutdialogue.cpp",
			"src/sourcedestinationwidget.cpp",
			"src/synchronisewhatcombo.cpp",
			"src/processwidget.cpp",
			"src/presetcombo.cpp",
			"src/presetmenu.cpp",
			"src/notificationwidget.cpp",
			"ui/processdialogue.ui",
			"ui/mainwindow.ui",
			"ui/preferencesdialogue.ui",
			"ui/aboutdialogue.ui",
			"ui/sourcedestinationwidget.ui",
			"ui/processwidget.ui",
			"ui/notificationwidget.ui",
		]
	}

	Group {
		name: "Documentation"
		files: [
			"docs/license.txt",
			"docs/cwrsync-license.txt",
			"docs/mainpage.dox",
			"docs/license_cwrsync.dox",
			"docs/license_rsync.dox",
			"docs/license_cygwin.dox",
			"docs/license_openssh.dox",
			"docs/license_putty.dox",
			"docs/namespaces.dox",
			"qync.Doxyfile",
		]
	}

	Group {
		name: "Distribution"
		files: [
			"dist/linux/qync.desktop",
			"dist/linux/qync.menu",
			"dist/linux/man/man1/qync.1",
			"dist/linux/doc/qync/copyright",
			"dist/linux/doc/qync/changelog.Debian",
			"dist/linux/doc/qync/changelog",
            "dist/pacman/makepkg.sh",
            "dist/pacman/makepkgs.sh",
		]
	}

	files: [
		"src/qync.cpp",
		"src/functions.cpp",
		"src/application.cpp",
		"src/guipreferences.cpp",
		"src/preferences.cpp",
		"src/preset.cpp",
		"src/process.cpp",
	]
}
