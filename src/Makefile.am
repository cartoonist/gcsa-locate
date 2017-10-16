WFLAGS = -Wall -Werror -Wno-vla -pedantic
bin_PROGRAMS = gcsa_locate
gcsa_locate_SOURCES = main.cc seed.h timer.h
gcsa_locate_CXXFLAGS = ${WFLAGS}
gcsa_locate_CXXFLAGS += @OPENMP_CXXFLAGS@ @SEQAN2_CFLAGS@ @SDSL_CFLAGS@ @GCSA2_CFLAGS@
gcsa_locate_LDADD = @SEQAN2_LIBS@ @GCSA2_LIBS@ @SDSL_LIBS@
gcsa_locate_LDFLAGS = @OPENMP_CXXFLAGS@
