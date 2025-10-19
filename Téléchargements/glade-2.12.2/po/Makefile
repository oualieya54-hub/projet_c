# Makefile for program source directory in GNU NLS utilities package.
# Copyright (C) 1995, 1996, 1997 by Ulrich Drepper <drepper@gnu.ai.mit.edu>
#
# This file file be copied and used freely without restrictions.  It can
# be used in projects which are not available under the GNU Public License
# but which still want to provide support for the GNU gettext functionality.
# Please note that the actual code is *not* freely available.
#
# - Modified by Owen Taylor <otaylor@redhat.com> to use GETTEXT_PACKAGE
#   instead of PACKAGE and to look for po2tbl in ./ not in intl/
#
# - Modified by jacob berkman <jacob@ximian.com> to install
#   Makefile.in.in and po2tbl.sed.in for use with glib-gettextize
#
# - Modified by Rodney Dawes <dobey@novell.com> for use with intltool
#
# We have the following line for use by intltoolize:
# INTLTOOL_MAKEFILE

GETTEXT_PACKAGE = glade-2.0
PACKAGE = glade
VERSION = 2.12.2

SHELL = /bin/sh

srcdir = .
top_srcdir = ..
top_builddir = ..


prefix = /usr
exec_prefix = ${prefix}
datadir = ${datarootdir}
datarootdir = ${prefix}/share
libdir = ${exec_prefix}/lib
DATADIRNAME = share
itlocaledir = $(prefix)/$(DATADIRNAME)/locale
subdir = po
install_sh = $(SHELL) /home/eya/Téléchargements/glade-2.12.2/install-sh
# Automake >= 1.8 provides /usr/bin/mkdir -p.
# Until it can be supposed, use the safe fallback:
mkdir_p = $(install_sh) -d

INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644

GMSGFMT = /usr/bin/msgfmt
MSGFMT = /usr/bin/msgfmt
XGETTEXT = /usr/bin/xgettext
INTLTOOL_UPDATE = $(top_builddir)/intltool-update
INTLTOOL_EXTRACT = $(top_builddir)/intltool-extract
MSGMERGE = INTLTOOL_EXTRACT=$(INTLTOOL_EXTRACT) srcdir=$(srcdir) $(INTLTOOL_UPDATE) --gettext-package $(GETTEXT_PACKAGE) --dist
GENPOT   = INTLTOOL_EXTRACT=$(INTLTOOL_EXTRACT) srcdir=$(srcdir) $(INTLTOOL_UPDATE) --gettext-package $(GETTEXT_PACKAGE) --pot

ALL_LINGUAS = 

PO_LINGUAS=$(shell if test -r $(srcdir)/LINGUAS; then grep -v "^\#" $(srcdir)/LINGUAS; fi)

USER_LINGUAS=$(shell if test -n "$(LINGUAS)"; then LLINGUAS="$(LINGUAS)"; ALINGUAS="$(ALL_LINGUAS)"; for lang in $$LLINGUAS; do if test -n "`grep ^$$lang$$ $(srcdir)/LINGUAS`" -o -n "`echo $$ALINGUAS|grep ' ?$$lang ?'`"; then printf "$$lang "; fi; done; fi)

USE_LINGUAS=$(shell if test -n "$(USER_LINGUAS)"; then LLINGUAS="$(USER_LINGUAS)"; else if test -n "$(PO_LINGUAS)"; then LLINGUAS="$(PO_LINGUAS)"; else LLINGUAS="$(ALL_LINGUAS)"; fi; fi; for lang in $$LLINGUAS; do printf "$$lang "; done)

POFILES=$(shell LINGUAS="$(USE_LINGUAS)"; for lang in $$LINGUAS; do printf "$$lang.po "; done)

DISTFILES = ChangeLog Makefile.in.in POTFILES.in $(POFILES)
EXTRA_DISTFILES = POTFILES.skip Makevars LINGUAS

POTFILES = \
	../glade-2.desktop.in \
	../glade/debug.c \
	../glade/editor.c \
	../glade/gb.c \
	../glade/gbsource.c \
	../glade/gbwidget.c \
	../glade/gbwidgets/gbaboutdialog.c \
	../glade/gbwidgets/gbaccellabel.c \
	../glade/gbwidgets/gbalignment.c \
	../glade/gbwidgets/gbarrow.c \
	../glade/gbwidgets/gbaspectframe.c \
	../glade/gbwidgets/gbbutton.c \
	../glade/gbwidgets/gbcalendar.c \
	../glade/gbwidgets/gbcellview.c \
	../glade/gbwidgets/gbcheckbutton.c \
	../glade/gbwidgets/gbcheckmenuitem.c \
	../glade/gbwidgets/gbclist.c \
	../glade/gbwidgets/gbcolorbutton.c \
	../glade/gbwidgets/gbcolorselection.c \
	../glade/gbwidgets/gbcolorselectiondialog.c \
	../glade/gbwidgets/gbcombo.c \
	../glade/gbwidgets/gbcombobox.c \
	../glade/gbwidgets/gbcomboboxentry.c \
	../glade/gbwidgets/gbctree.c \
	../glade/gbwidgets/gbcurve.c \
	../glade/gbwidgets/gbcustom.c \
	../glade/gbwidgets/gbdialog.c \
	../glade/gbwidgets/gbdrawingarea.c \
	../glade/gbwidgets/gbentry.c \
	../glade/gbwidgets/gbeventbox.c \
	../glade/gbwidgets/gbexpander.c \
	../glade/gbwidgets/gbfilechooserbutton.c \
	../glade/gbwidgets/gbfilechooserwidget.c \
	../glade/gbwidgets/gbfilechooserdialog.c \
	../glade/gbwidgets/gbfileselection.c \
	../glade/gbwidgets/gbfixed.c \
	../glade/gbwidgets/gbfontbutton.c \
	../glade/gbwidgets/gbfontselection.c \
	../glade/gbwidgets/gbfontselectiondialog.c \
	../glade/gbwidgets/gbframe.c \
	../glade/gbwidgets/gbgammacurve.c \
	../glade/gbwidgets/gbhandlebox.c \
	../glade/gbwidgets/gbhbox.c \
	../glade/gbwidgets/gbhbuttonbox.c \
	../glade/gbwidgets/gbhpaned.c \
	../glade/gbwidgets/gbhruler.c \
	../glade/gbwidgets/gbhscale.c \
	../glade/gbwidgets/gbhscrollbar.c \
	../glade/gbwidgets/gbhseparator.c \
	../glade/gbwidgets/gbiconview.c \
	../glade/gbwidgets/gbimage.c \
	../glade/gbwidgets/gbimagemenuitem.c \
	../glade/gbwidgets/gbinputdialog.c \
	../glade/gbwidgets/gblabel.c \
	../glade/gbwidgets/gblayout.c \
	../glade/gbwidgets/gblist.c \
	../glade/gbwidgets/gblistitem.c \
	../glade/gbwidgets/gbmenu.c \
	../glade/gbwidgets/gbmenubar.c \
	../glade/gbwidgets/gbmenuitem.c \
	../glade/gbwidgets/gbmenutoolbutton.c \
	../glade/gbwidgets/gbnotebook.c \
	../glade/gbwidgets/gboptionmenu.c \
	../glade/gbwidgets/gbpreview.c \
	../glade/gbwidgets/gbprogressbar.c \
	../glade/gbwidgets/gbradiobutton.c \
	../glade/gbwidgets/gbradiomenuitem.c \
	../glade/gbwidgets/gbradiotoolbutton.c \
	../glade/gbwidgets/gbscrolledwindow.c \
	../glade/gbwidgets/gbseparatormenuitem.c \
	../glade/gbwidgets/gbseparatortoolitem.c \
	../glade/gbwidgets/gbspinbutton.c \
	../glade/gbwidgets/gbstatusbar.c \
	../glade/gbwidgets/gbtable.c \
	../glade/gbwidgets/gbtextview.c \
	../glade/gbwidgets/gbtogglebutton.c \
	../glade/gbwidgets/gbtoggletoolbutton.c \
	../glade/gbwidgets/gbtoolbar.c \
	../glade/gbwidgets/gbtoolbutton.c \
	../glade/gbwidgets/gbtoolitem.c \
	../glade/gbwidgets/gbtreeview.c \
	../glade/gbwidgets/gbvbox.c \
	../glade/gbwidgets/gbvbuttonbox.c \
	../glade/gbwidgets/gbviewport.c \
	../glade/gbwidgets/gbvpaned.c \
	../glade/gbwidgets/gbvruler.c \
	../glade/gbwidgets/gbvscale.c \
	../glade/gbwidgets/gbvscrollbar.c \
	../glade/gbwidgets/gbvseparator.c \
	../glade/gbwidgets/gbwindow.c \
	../glade/glade.c \
	../glade/glade_atk.c \
	../glade/glade_clipboard.c \
	../glade/glade_gnome.c \
	../glade/glade_gnomelib.c \
	../glade/glade_gtk12lib.c \
	../glade/glade_keys_dialog.c \
	../glade/glade_menu_editor.c \
	../glade/glade_palette.c \
	../glade/glade_plugin.c \
	../glade/glade_project.c \
	../glade/glade_project_options.c \
	../glade/glade_project_view.c \
	../glade/glade_project_window.c \
	../glade/glade_widget_data.c \
	../glade/gnome-db/gnomedbcombo.c \
	../glade/gnome-db/gnomedbconnectprop.c \
	../glade/gnome-db/gnomedbdsnconfig.c \
	../glade/gnome-db/gnomedbdsndruid.c \
	../glade/gnome-db/gnomedbeditor.c \
	../glade/gnome-db/gnomedberror.c \
	../glade/gnome-db/gnomedberrordlg.c \
	../glade/gnome-db/gnomedbform.c \
	../glade/gnome-db/gnomedbgraybar.c \
	../glade/gnome-db/gnomedbgrid.c \
	../glade/gnome-db/gnomedblogin.c \
	../glade/gnome-db/gnomedblogindlg.c \
	../glade/gnome-db/gnomedbprovidersel.c \
	../glade/gnome-db/gnomedbsourcesel.c \
	../glade/gnome-db/gnomedbtableeditor.c \
	../glade/gnome/bonobodock.c \
	../glade/gnome/bonobodockitem.c \
	../glade/gnome/gnomeabout.c \
	../glade/gnome/gnomeapp.c \
	../glade/gnome/gnomeappbar.c \
	../glade/gnome/gnomecanvas.c \
	../glade/gnome/gnomecolorpicker.c \
	../glade/gnome/gnomecontrol.c \
	../glade/gnome/gnomedateedit.c \
	../glade/gnome/gnomedialog.c \
	../glade/gnome/gnomedruid.c \
	../glade/gnome/gnomedruidpageedge.c \
	../glade/gnome/gnomedruidpagestandard.c \
	../glade/gnome/gnomeentry.c \
	../glade/gnome/gnomefileentry.c \
	../glade/gnome/gnomefontpicker.c \
	../glade/gnome/gnomehref.c \
	../glade/gnome/gnomeiconentry.c \
	../glade/gnome/gnomeiconlist.c \
	../glade/gnome/gnomeiconselection.c \
	../glade/gnome/gnomemessagebox.c \
	../glade/gnome/gnomepixmap.c \
	../glade/gnome/gnomepixmapentry.c \
	../glade/gnome/gnomepropertybox.c \
	../glade/keys.c \
	../glade/load.c \
	../glade/main.c \
	../glade/palette.c \
	../glade/property.c \
	../glade/save.c \
	../glade/source.c \
	../glade/source_os2.c \
	../glade/styles.c \
	../glade/tree.c \
	../glade/utils.c
# This comment gets stripped out

CATALOGS=$(shell LINGUAS="$(USE_LINGUAS)"; for lang in $$LINGUAS; do printf "$$lang.gmo "; done)

.SUFFIXES:
.SUFFIXES: .po .pox .gmo .mo .msg .cat

.po.pox:
	$(MAKE) $(GETTEXT_PACKAGE).pot
	$(MSGMERGE) $< $(GETTEXT_PACKAGE).pot -o $*.pox

.po.mo:
	$(MSGFMT) -o $@ $<

.po.gmo:
	file=`echo $* | sed 's,.*/,,'`.gmo \
	  && rm -f $$file && $(GMSGFMT) -o $$file $<

.po.cat:
	sed -f ../intl/po2msg.sed < $< > $*.msg \
	  && rm -f $@ && gencat $@ $*.msg


all: all-yes

all-yes: $(CATALOGS)
all-no:

$(GETTEXT_PACKAGE).pot: $(POTFILES)
	$(GENPOT)

install: install-data
install-data: install-data-yes
install-data-no: all
install-data-yes: all
	$(mkdir_p) $(DESTDIR)$(itlocaledir)
	linguas="$(USE_LINGUAS)"; \
	for lang in $$linguas; do \
	  dir=$(DESTDIR)$(itlocaledir)/$$lang/LC_MESSAGES; \
	  $(mkdir_p) $$dir; \
	  if test -r $$lang.gmo; then \
	    $(INSTALL_DATA) $$lang.gmo $$dir/$(GETTEXT_PACKAGE).mo; \
	    echo "installing $$lang.gmo as $$dir/$(GETTEXT_PACKAGE).mo"; \
	  else \
	    $(INSTALL_DATA) $(srcdir)/$$lang.gmo $$dir/$(GETTEXT_PACKAGE).mo; \
	    echo "installing $(srcdir)/$$lang.gmo as" \
		 "$$dir/$(GETTEXT_PACKAGE).mo"; \
	  fi; \
	  if test -r $$lang.gmo.m; then \
	    $(INSTALL_DATA) $$lang.gmo.m $$dir/$(GETTEXT_PACKAGE).mo.m; \
	    echo "installing $$lang.gmo.m as $$dir/$(GETTEXT_PACKAGE).mo.m"; \
	  else \
	    if test -r $(srcdir)/$$lang.gmo.m ; then \
	      $(INSTALL_DATA) $(srcdir)/$$lang.gmo.m \
		$$dir/$(GETTEXT_PACKAGE).mo.m; \
	      echo "installing $(srcdir)/$$lang.gmo.m as" \
		   "$$dir/$(GETTEXT_PACKAGE).mo.m"; \
	    else \
	      true; \
	    fi; \
	  fi; \
	done

# Empty stubs to satisfy archaic automake needs
dvi info tags TAGS ID:

# Define this as empty until I found a useful application.
install-exec installcheck:

uninstall:
	linguas="$(USE_LINGUAS)"; \
	for lang in $$linguas; do \
	  rm -f $(DESTDIR)$(itlocaledir)/$$lang/LC_MESSAGES/$(GETTEXT_PACKAGE).mo; \
	  rm -f $(DESTDIR)$(itlocaledir)/$$lang/LC_MESSAGES/$(GETTEXT_PACKAGE).mo.m; \
	done

check: all $(GETTEXT_PACKAGE).pot
	rm -f missing notexist
	srcdir=$(srcdir) $(INTLTOOL_UPDATE) -m
	if [ -r missing -o -r notexist ]; then \
	  exit 1; \
	fi

mostlyclean:
	rm -f *.pox $(GETTEXT_PACKAGE).pot *.old.po cat-id-tbl.tmp
	rm -f .intltool-merge-cache

clean: mostlyclean

distclean: clean
	rm -f Makefile Makefile.in POTFILES stamp-it
	rm -f *.mo *.msg *.cat *.cat.m *.gmo

maintainer-clean: distclean
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	rm -f Makefile.in.in

distdir = ../$(PACKAGE)-$(VERSION)/$(subdir)
dist distdir: $(DISTFILES)
	dists="$(DISTFILES)"; \
	extra_dists="$(EXTRA_DISTFILES)"; \
	for file in $$extra_dists; do \
	  test -f $(srcdir)/$$file && dists="$$dists $(srcdir)/$$file"; \
	done; \
	for file in $$dists; do \
	  test -f $$file || file="$(srcdir)/$$file"; \
	  ln $$file $(distdir) 2> /dev/null \
	    || cp -p $$file $(distdir); \
	done

update-po: Makefile
	$(MAKE) $(GETTEXT_PACKAGE).pot
	tmpdir=`pwd`; \
	linguas="$(USE_LINGUAS)"; \
	for lang in $$linguas; do \
	  echo "$$lang:"; \
	  result="`$(MSGMERGE) -o $$tmpdir/$$lang.new.po $$lang`"; \
	  if $$result; then \
	    if cmp $(srcdir)/$$lang.po $$tmpdir/$$lang.new.po >/dev/null 2>&1; then \
	      rm -f $$tmpdir/$$lang.new.po; \
            else \
	      if mv -f $$tmpdir/$$lang.new.po $$lang.po; then \
	        :; \
	      else \
	        echo "msgmerge for $$lang.po failed: cannot move $$tmpdir/$$lang.new.po to $$lang.po" 1>&2; \
	        rm -f $$tmpdir/$$lang.new.po; \
	        exit 1; \
	      fi; \
	    fi; \
	  else \
	    echo "msgmerge for $$lang.gmo failed!"; \
	    rm -f $$tmpdir/$$lang.new.po; \
	  fi; \
	done

Makefile POTFILES: stamp-it
	@if test ! -f $@; then \
	  rm -f stamp-it; \
	  $(MAKE) stamp-it; \
	fi

stamp-it: Makefile.in.in $(top_builddir)/config.status POTFILES.in
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/Makefile.in CONFIG_HEADERS= CONFIG_LINKS= \
	       $(SHELL) ./config.status

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
