AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src
dist_doc_DATA = README.md
man_MANS = man/gcsa_locate.1
CLEANFILES = man/gcsa_locate.1

man/gcsa_locate.1:
	@mkdir -p `dirname $@`
	$(top_builddir)/src/gcsa_locate --export-help man > $@
