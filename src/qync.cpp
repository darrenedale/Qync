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

#include "MainWindow.h"
#include "Manager.h"

#include <QApplication>
#include <QDebug>

/**
 * \mainpage
 *
 * Qync is a front end for the \b rsync utility. rsync is a command-line tool
 * that synchronises parts of filesystems - either an individual file or a
 * directory tree - between two locations (a source and a destination), one of
 * which may be remote (i.e. a networked server). Probably the most common use
 * case for rsync is to synchronise two local directories, the destination
 * being used as a backup of the source. Because rsync only copies files and
 * directories that have meen modified since the last time the backup was
 * updated, and because it can be set to compress data in transit, it is very
 * useful as a fast and resource efficient backup tool, particularly for large
 * quantities of data and particularly when that data is being transferred over
 * a network.
 *
 * <h2>Architecture</h2>
 * The Qync application is built on a modular architecture. At its core is a
 * manager class (\ref QyncManager) that provides access to program features
 * such as presets, and enables the rsync process to be launched. The manager
 * class also ensures that the core functionality of the application is
 * separated from the user interface, so that it is possible to build different
 * interfaces around the core manager class.
 *
 * Any rsync process launched is wrapped in a process class (\ref QyncProcess)
 * that provides the ability to start, interrupt and monitor the progress of the
 * rsync process. The process class emits a number of useful signals that can be
 * used to provide feedback to the end user.
 *
 * Presets are represented as instances of the \ref QyncPreset class. All
 * presets that are saved are completely administered by the manager - the
 * manager ensures that all presets stored are saved to disk and loaded each
 * time the program is started. The preset class can also be used by front ends
 * to build custom settings for the manager to run.
 *
 * Finally, a base class is provided for managing application preferences
 * (\ref QyncPreferences). The base class provides only the core preference
 * settings required for the manager to do its job - currently this is just the
 * path to the rsync executable file. The majority of its features are designed
 * to enable the base class to be extended to provide other settings for the
 * various possible user interfaces. For example, the \ref QyncGuiPreferences
 * class extends the base class to provide preferences relating to the visual
 * appearance of the GUI. The base class is structured such that a minimum of
 * reimplementation and duplication is required when extending the preferences.
 *
 * <h2>Graphical User Interface</h2>
 * An example front end is provided with the Qync application. The Qync GUI
 * makes it easy to use rsync for most operations. As well as simply providing
 * easy access to most of rsync's features, it offers the user the ability to
 * save presets so that s/he can easily recall frequently-used rsync settings
 * and execute them without having to enter the settings every time.
 *
 * The example user interface is provided by the \ref QyncMainWindow,
 * \ref QyncProcessDialogue and \ref QyncPreferencesDialogue classes. The main
 * window class is the main interface through which the user interacts with the
 * program. It provedes the user with facilities to manage his/her presets and
 * with the ability to alter and execute rsync with arbitrary parameters. The
 * main window is responsible for spawning instances of other user interface
 * elements, such as the process dialogue, which provides the user with the
 * ability to monitor and manage a running rsync process, and the preferences
 * dialogue, which allows the user to alter the his/her settings for the user
 * interface.
 *
 * <h2>Linux, Unix and OSX</h2>
 * These versions of Qync depend upon the rsync tool already being installed.
 * For the most part, rsync will be installed by default. If not, use your
 * package manager to install the rsync package.
 *
 * <h2>Windows</h2>
 * The Windows version of Qync uses the cwrsync tool, a port of the rsync source
 * code to Windows, which in turn uses the cygwin linux-like environment for
 * Windows, OpenSSH and PuTTY. The licenses for these software components can
 * be found on the following pages:
 * - \ref cwrsync_license
 * - \ref rsync_license
 * - \ref cygwin_license
 * - \ref openssh_license
 * - \ref putty_license
 *
 * The licenses can also be found in the doc directory of an installation on
 * Windows.
 *
 * The source code for rsync and PuTTY can be found in the src directory. Cygwin
 * and OpenSSH are larger projects, and their source can be found on their
 * websites:
 * - <a href="http://cygwin.com/cvs.html">cygwin</a>
 *   (http://cygwin.com/cvs.html)
 * - <a href="http://www.openssh.com/">OpenSSH</a> (http://www.openssh.com/)
 */

/**
 * \page cwrsync_license cwRsync License
 *
Copyright 2003-2006 Tevfik Karagulle (tevfik@itefix.no).
All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, provided that the above copyright notice(s) and this
permission notice appear in all copies of the Software and that both the
above copyright notice(s) and this permission notice appear in supporting
documentation.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY,FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT OF THIRD PARTY RIGHTS. IN NO EVENT SHALL
THE COPYRIGHT HOLDER OR HOLDERS INCLUDED IN THIS NOTICE BE LIABLE
FOR ANY CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES,
OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * \page rsync_license Rsync License

						GNU GENERAL PUBLIC LICENSE
						   Version 2, June 1991

 Copyright (C) 1989, 1991 Free Software Foundation, Inc.
												  675 Mass Ave, Cambridge, MA
02139, USA Everyone is permitted to copy and distribute verbatim copies of this
license document, but changing it is not allowed.

								Preamble

  The licenses for most software are designed to take away your
freedom to share and change it.  By contrast, the GNU General Public
License is intended to guarantee your freedom to share and change free
software--to make sure the software is free for all its users.  This
General Public License applies to most of the Free Software
Foundation's software and to any other program whose authors commit to
using it.  (Some other Free Software Foundation software is covered by
the GNU Library General Public License instead.)  You can apply it to
your programs, too.

  When we speak of free software, we are referring to freedom, not
price.  Our General Public Licenses are designed to make sure that you
have the freedom to distribute copies of free software (and charge for
this service if you wish), that you receive source code or can get it
if you want it, that you can change the software or use pieces of it
in new free programs; and that you know you can do these things.

  To protect your rights, we need to make restrictions that forbid
anyone to deny you these rights or to ask you to surrender the rights.
These restrictions translate to certain responsibilities for you if you
distribute copies of the software, or if you modify it.

  For example, if you distribute copies of such a program, whether
gratis or for a fee, you must give the recipients all the rights that
you have.  You must make sure that they, too, receive or can get the
source code.  And you must show them these terms so they know their
rights.

  We protect your rights with two steps: (1) copyright the software, and
(2) offer you this license which gives you legal permission to copy,
distribute and/or modify the software.

  Also, for each author's protection and ours, we want to make certain
that everyone understands that there is no warranty for this free
software.  If the software is modified by someone else and passed on, we
want its recipients to know that what they have is not the original, so
that any problems introduced by others will not reflect on the original
authors' reputations.

  Finally, any free program is threatened constantly by software
patents.  We wish to avoid the danger that redistributors of a free
program will individually obtain patent licenses, in effect making the
program proprietary.  To prevent this, we have made it clear that any
patent must be licensed for everyone's free use or not licensed at all.

  The precise terms and conditions for copying, distribution and
modification follow.

						GNU GENERAL PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. This License applies to any program or other work which contains
a notice placed by the copyright holder saying it may be distributed
under the terms of this General Public License.  The "Program", below,
refers to any such program or work, and a "work based on the Program"
means either the Program or any derivative work under copyright law:
that is to say, a work containing the Program or a portion of it,
either verbatim or with modifications and/or translated into another
language.  (Hereinafter, translation is included without limitation in
the term "modification".)  Each licensee is addressed as "you".

Activities other than copying, distribution and modification are not
covered by this License; they are outside its scope.  The act of
running the Program is not restricted, and the output from the Program
is covered only if its contents constitute a work based on the
Program (independent of having been made by running the Program).
Whether that is true depends on what the Program does.

  1. You may copy and distribute verbatim copies of the Program's
source code as you receive it, in any medium, provided that you
conspicuously and appropriately publish on each copy an appropriate
copyright notice and disclaimer of warranty; keep intact all the
notices that refer to this License and to the absence of any warranty;
and give any other recipients of the Program a copy of this License
along with the Program.

You may charge a fee for the physical act of transferring a copy, and
you may at your option offer warranty protection in exchange for a fee.

  2. You may modify your copy or copies of the Program or any portion
of it, thus forming a work based on the Program, and copy and
distribute such modifications or work under the terms of Section 1
above, provided that you also meet all of these conditions:

		a) You must cause the modified files to carry prominent notices
		stating that you changed the files and the date of any change.

		b) You must cause any work that you distribute or publish, that in
		whole or in part contains or is derived from the Program or any
		part thereof, to be licensed as a whole at no charge to all third
		parties under the terms of this License.

		c) If the modified program normally reads commands interactively
		when run, you must cause it, when started running for such
		interactive use in the most ordinary way, to print or display an
		announcement including an appropriate copyright notice and a
		notice that there is no warranty (or else, saying that you provide
		a warranty) and that users may redistribute the program under
		these conditions, and telling the user how to view a copy of this
		License.  (Exception: if the Program itself is interactive but
		does not normally print such an announcement, your work based on
		the Program is not required to print an announcement.)

These requirements apply to the modified work as a whole.  If
identifiable sections of that work are not derived from the Program,
and can be reasonably considered independent and separate works in
themselves, then this License, and its terms, do not apply to those
sections when you distribute them as separate works.  But when you
distribute the same sections as part of a whole which is a work based
on the Program, the distribution of the whole must be on the terms of
this License, whose permissions for other licensees extend to the
entire whole, and thus to each and every part regardless of who wrote it.

Thus, it is not the intent of this section to claim rights or contest
your rights to work written entirely by you; rather, the intent is to
exercise the right to control the distribution of derivative or
collective works based on the Program.

In addition, mere aggregation of another work not based on the Program
with the Program (or with a work based on the Program) on a volume of
a storage or distribution medium does not bring the other work under
the scope of this License.

  3. You may copy and distribute the Program (or a work based on it,
under Section 2) in object code or executable form under the terms of
Sections 1 and 2 above provided that you also do one of the following:

		a) Accompany it with the complete corresponding machine-readable
		source code, which must be distributed under the terms of Sections
		1 and 2 above on a medium customarily used for software interchange; or,

		b) Accompany it with a written offer, valid for at least three
		years, to give any third party, for a charge no more than your
		cost of physically performing source distribution, a complete
		machine-readable copy of the corresponding source code, to be
		distributed under the terms of Sections 1 and 2 above on a medium
		customarily used for software interchange; or,

		c) Accompany it with the information you received as to the offer
		to distribute corresponding source code.  (This alternative is
		allowed only for noncommercial distribution and only if you
		received the program in object code or executable form with such
		an offer, in accord with Subsection b above.)

The source code for a work means the preferred form of the work for
making modifications to it.  For an executable work, complete source
code means all the source code for all modules it contains, plus any
associated interface definition files, plus the scripts used to
control compilation and installation of the executable.  However, as a
special exception, the source code distributed need not include
anything that is normally distributed (in either source or binary
form) with the major components (compiler, kernel, and so on) of the
operating system on which the executable runs, unless that component
itself accompanies the executable.

If distribution of executable or object code is made by offering
access to copy from a designated place, then offering equivalent
access to copy the source code from the same place counts as
distribution of the source code, even though third parties are not
compelled to copy the source along with the object code.

  4. You may not copy, modify, sublicense, or distribute the Program
except as expressly provided under this License.  Any attempt
otherwise to copy, modify, sublicense or distribute the Program is
void, and will automatically terminate your rights under this License.
However, parties who have received copies, or rights, from you under
this License will not have their licenses terminated so long as such
parties remain in full compliance.

  5. You are not required to accept this License, since you have not
signed it.  However, nothing else grants you permission to modify or
distribute the Program or its derivative works.  These actions are
prohibited by law if you do not accept this License.  Therefore, by
modifying or distributing the Program (or any work based on the
Program), you indicate your acceptance of this License to do so, and
all its terms and conditions for copying, distributing or modifying
the Program or works based on it.

  6. Each time you redistribute the Program (or any work based on the
Program), the recipient automatically receives a license from the
original licensor to copy, distribute or modify the Program subject to
these terms and conditions.  You may not impose any further
restrictions on the recipients' exercise of the rights granted herein.
You are not responsible for enforcing compliance by third parties to
this License.

  7. If, as a consequence of a court judgment or allegation of patent
infringement or for any other reason (not limited to patent issues),
conditions are imposed on you (whether by court order, agreement or
otherwise) that contradict the conditions of this License, they do not
excuse you from the conditions of this License.  If you cannot
distribute so as to satisfy simultaneously your obligations under this
License and any other pertinent obligations, then as a consequence you
may not distribute the Program at all.  For example, if a patent
license would not permit royalty-free redistribution of the Program by
all those who receive copies directly or indirectly through you, then
the only way you could satisfy both it and this License would be to
refrain entirely from distribution of the Program.

If any portion of this section is held invalid or unenforceable under
any particular circumstance, the balance of the section is intended to
apply and the section as a whole is intended to apply in other
circumstances.

It is not the purpose of this section to induce you to infringe any
patents or other property right claims or to contest validity of any
such claims; this section has the sole purpose of protecting the
integrity of the free software distribution system, which is
implemented by public license practices.  Many people have made
generous contributions to the wide range of software distributed
through that system in reliance on consistent application of that
system; it is up to the author/donor to decide if he or she is willing
to distribute software through any other system and a licensee cannot
impose that choice.

This section is intended to make thoroughly clear what is believed to
be a consequence of the rest of this License.

  8. If the distribution and/or use of the Program is restricted in
certain countries either by patents or by copyrighted interfaces, the
original copyright holder who places the Program under this License
may add an explicit geographical distribution limitation excluding
those countries, so that distribution is permitted only in or among
countries not thus excluded.  In such case, this License incorporates
the limitation as if written in the body of this License.

  9. The Free Software Foundation may publish revised and/or new versions
of the General Public License from time to time.  Such new versions will
be similar in spirit to the present version, but may differ in detail to
address new problems or concerns.

Each version is given a distinguishing version number.  If the Program
specifies a version number of this License which applies to it and "any
later version", you have the option of following the terms and conditions
either of that version or of any later version published by the Free
Software Foundation.  If the Program does not specify a version number of
this License, you may choose any version ever published by the Free Software
Foundation.

  10. If you wish to incorporate parts of the Program into other free
programs whose distribution conditions are different, write to the author
to ask for permission.  For software which is copyrighted by the Free
Software Foundation, write to the Free Software Foundation; we sometimes
make exceptions for this.  Our decision will be guided by the two goals
of preserving the free status of all derivatives of our free software and
of promoting the sharing and reuse of software generally.

								NO WARRANTY

  11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY
FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
REPAIR OR CORRECTION.

  12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR
REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,
INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING
OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED
TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY
YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER
PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGES.

						 END OF TERMS AND CONDITIONS

		Appendix: How to Apply These Terms to Your New Programs

  If you develop a new program, and you want it to be of the greatest
possible use to the public, the best way to achieve this is to make it
free software which everyone can redistribute and change under these terms.

  To do so, attach the following notices to the program.  It is safest
to attach them to the start of each source file to most effectively
convey the exclusion of warranty; and each file should have at least
the "copyright" line and a pointer to where the full notice is found.


		Copyright (C) 19yy

		This program is free software; you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation; either version 2 of the License, or
		(at your option) any later version.

		This program is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		GNU General Public License for more details.

		You should have received a copy of the GNU General Public License
		along with this program; if not, write to the Free Software
		Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

Also add information on how to contact you by electronic and paper mail.

If the program is interactive, make it output a short notice like this
when it starts in an interactive mode:

		Gnomovision version 69, Copyright (C) 19yy name of author
		Gnomovision comes with ABSOLUTELY NO WARRANTY; for details type `show
w'. This is free software, and you are welcome to redistribute it under certain
conditions; type `show c' for details.

The hypothetical commands `show w' and `show c' should show the appropriate
parts of the General Public License.  Of course, the commands you use may
be called something other than `show w' and `show c'; they could even be
mouse-clicks or menu items--whatever suits your program.

You should also get your employer (if you work as a programmer) or your
school, if any, to sign a "copyright disclaimer" for the program, if
necessary.  Here is a sample; alter the names:

  Yoyodyne, Inc., hereby disclaims all copyright interest in the program
  `Gnomovision' (which makes passes at compilers) written by James Hacker.

  , 1 April 1989
  Ty Coon, President of Vice

This General Public License does not permit incorporating your program into
proprietary programs.  If your program is a subroutine library, you may
consider it more useful to permit linking proprietary applications with the
library.  If this is what you want to do, use the GNU Library General
Public License instead of this License.
*/

/**
 * \page cygwin_license Cygwin License

What are the licensing terms?

Most of the tools are covered by the GNU GPL, some are public domain,
and others have a X11 style copyright. To cover the GNU GPL requirements,
the basic rule is if you give out any binaries, you must also make the
source available. For the full details, be sure to read the text of the
GNU General Public License (GPL).

The Cygwin API library found in the winsup subdirectory of the source code
is also covered by the GNU GPL (with exceptions; see below). By default,
all executables link against this library (and in the process include
GPL'd Cygwin glue code). This means that unless you modify the tools
so that compiled executables do not make use of the Cygwin library, your
compiled programs will also have to be free software distributed under
the GPL with source code available to all.

Cygwin API Licensing Terms

This is a copy of CYGWIN_LICENSE from the cygwin sources

--------------------------------------------------------------------------------

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License (GPL) as published by the Free
Software Foundation; either version 2 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA.
--------------------------------------------------------------------------------

NOTE

In accordance with section 10 of the GPL, Red Hat permits programs whose sources
are distributed under a license that complies with the Open Source definition to
be linked with libcygwin.a without libcygwin.a itself causing the resulting
program to be covered by the GNU GPL.

This means that you can port an Open Source(tm) application to cygwin, and
distribute that executable as if it didn't include a copy of libcygwin.a linked
into it. Note that this does not apply to the cygwin DLL itself. If you
distribute a (possibly modified) version of the DLL you must adhere to the
terms of the GPL, i.e. you must provide sources for the cygwin DLL.

See http://www.opensource.org/docs/definition_plain.html for the precise Open
Source Definition referenced above.

Red Hat sells a special Cygwin License for customers who are unable to provide
their application in open source code form. For more information, please see:
http://www.redhat.com/software/tools/cygwin/, or call 866-2REDHAT ext. 3007
*/

/**
 \page openssh_license OpenSSH License

This file is part of the OpenSSH software.

The licences which components of this software fall under are as
follows.  First, we will summarize and say that all components
are under a BSD licence, or a licence more free than that.

OpenSSH contains no GPL code.

1)
		 * Copyright (c) 1995 Tatu Ylonen <ylo@cs.hut.fi>, Espoo, Finland
		 *                    All rights reserved
		 *
		 * As far as I am concerned, the code I have written for this software
		 * can be used freely for any purpose.  Any derived versions of this
		 * software must be clearly marked as such, and if the derived work is
		 * incompatible with the protocol description in the RFC file, it must
be
		 * called by a name other than "ssh" or "Secure Shell".

		[Tatu continues]
		 *  However, I am not implying to give any licenses to any patents or
		 * copyrights held by third parties, and the software includes parts
that
		 * are not under my direct control.  As far as I know, all included
		 * source code is used in accordance with the relevant license
agreements
		 * and can be used freely for any purpose (the GNU license being the
most
		 * restrictive); see below for details.

		[However, none of that term is relevant at this point in time.  All of
		these restrictively licenced software components which he talks about
		have been removed from OpenSSH, i.e.,

		 - RSA is no longer included, found in the OpenSSL library
		 - IDEA is no longer included, its use is deprecated
		 - DES is now external, in the OpenSSL library
		 - GMP is no longer used, and instead we call BN code from OpenSSL
		 - Zlib is now external, in a library
		 - The make-ssh-known-hosts script is no longer included
		 - TSS has been removed
		 - MD5 is now external, in the OpenSSL library
		 - RC4 support has been replaced with ARC4 support from OpenSSL
		 - Blowfish is now external, in the OpenSSL library

		[The licence continues]

		Note that any information and cryptographic algorithms used in this
		software are publicly available on the Internet and at any major
		bookstore, scientific library, and patent office worldwide.  More
		information can be found e.g. at "http://www.cs.hut.fi/crypto".

		The legal status of this program is some combination of all these
		permissions and restrictions.  Use only at your own responsibility.
		You will be responsible for any legal consequences yourself; I am not
		making any claims whether possessing or using this is legal or not in
		your country, and I am not taking any responsibility on your behalf.


										NO WARRANTY

		BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY
		FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
		OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
		PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER
EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
		MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK
AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
		PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
		REPAIR OR CORRECTION.

		IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
		WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR
		REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR
DAMAGES, INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT
LIMITED TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY
		YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY
OTHER PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE
		POSSIBILITY OF SUCH DAMAGES.

2)
		The 32-bit CRC compensation attack detector in deattack.c was
		contributed by CORE SDI S.A. under a BSD-style license.

		 * Cryptographic attack detector for ssh - source code
		 *
		 * Copyright (c) 1998 CORE SDI S.A., Buenos Aires, Argentina.
		 *
		 * All rights reserved. Redistribution and use in source and binary
		 * forms, with or without modification, are permitted provided that
		 * this copyright notice is retained.
		 *
		 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
		 * WARRANTIES ARE DISCLAIMED. IN NO EVENT SHALL CORE SDI S.A. BE
		 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR
		 * CONSEQUENTIAL DAMAGES RESULTING FROM THE USE OR MISUSE OF THIS
		 * SOFTWARE.
		 *
		 * Ariel Futoransky <futo@core-sdi.com>
		 * <http://www.core-sdi.com>

3)
		ssh-keygen was contributed by David Mazieres under a BSD-style
		license.

		 * Copyright 1995, 1996 by David Mazieres <dm@lcs.mit.edu>.
		 *
		 * Modification and redistribution in source and binary forms is
		 * permitted provided that due credit is given to the author and the
		 * OpenBSD project by leaving this copyright notice intact.

4)
		The Rijndael implementation by Vincent Rijmen, Antoon Bosselaers
		and Paulo Barreto is in the public domain and distributed
		with the following license:

		 * @@version 3.0 (December 2000)
		 *
		 * Optimised ANSI C code for the Rijndael cipher (now AES)
		 *
		 * @@author Vincent Rijmen <vincent.rijmen@esat.kuleuven.ac.be>
		 * @@author Antoon Bosselaers <antoon.bosselaers@esat.kuleuven.ac.be>
		 * @@author Paulo Barreto <paulo.barreto@terra.com.br>
		 *
		 * This code is hereby placed in the public domain.
		 *
		 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
		 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
		 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
		 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
		 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
		 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
		 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
		 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
		 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
		 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
		 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

5)
		One component of the ssh source code is under a 3-clause BSD license,
		held by the University of California, since we pulled these parts from
		original Berkeley code.

		 * Copyright (c) 1983, 1990, 1992, 1993, 1995
		 *      The Regents of the University of California.  All rights
reserved.
		 *
		 * Redistribution and use in source and binary forms, with or without
		 * modification, are permitted provided that the following conditions
		 * are met:
		 * 1. Redistributions of source code must retain the above copyright
		 *    notice, this list of conditions and the following disclaimer.
		 * 2. Redistributions in binary form must reproduce the above copyright
		 *    notice, this list of conditions and the following disclaimer in
the
		 *    documentation and/or other materials provided with the
distribution.
		 * 3. Neither the name of the University nor the names of its
contributors
		 *    may be used to endorse or promote products derived from this
software
		 *    without specific prior written permission.
		 *
		 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS''
AND
		 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
		 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE
		 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE
LIABLE
		 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL
		 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS
		 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
		 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT
		 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
WAY
		 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF
		 * SUCH DAMAGE.

6)
		Remaining components of the software are provided under a standard
		2-term BSD licence with the following names as copyright holders:

		Markus Friedl
		Theo de Raadt
		Niels Provos
		Dug Song
		Aaron Campbell
		Damien Miller
		Kevin Steves
		Daniel Kouril
		Wesley Griffin
		Per Allansson
		Nils Nordman

		 * Redistribution and use in source and binary forms, with or without
		 * modification, are permitted provided that the following conditions
		 * are met:
		 * 1. Redistributions of source code must retain the above copyright
		 *    notice, this list of conditions and the following disclaimer.
		 * 2. Redistributions in binary form must reproduce the above copyright
		 *    notice, this list of conditions and the following disclaimer in
the
		 *    documentation and/or other materials provided with the
distribution.
		 *
		 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
		 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES
		 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.
		 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
		 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT
		 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
USE,
		 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF
		 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

------
$OpenBSD: LICENCE,v 1.16 2003/07/29 18:24:00 markus Exp $
*/

/**
  \page putty_license PuTTY License

PuTTY is copyright 1997-2005 Simon Tatham.

Portions copyright Robert de Bath, Joris van Rantwijk, Delian Delchev, Andreas
Schultz, Jeroen Massar, Wez Furlong, Nicolas Barry, Justin Bradford, Ben Harris,
Malcolm Smith, Ahmad Khalifa, Markus Kuhn, and CORE SDI S.A.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
SIMON TATHAM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
	QApplication app(argc, argv);
	Qync::Manager manager;
	Qync::MainWindow win(&manager);
	win.show();
	return app.exec();
}
