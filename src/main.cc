/**
 *    @file  main.cc
 *   @brief  gcsa_locate main program.
 *
 *  Uses GCSA2 index to locate k-mers in the underlying graph.
 *
 *  @author  Ali Ghaffaari (\@cartoonist), <ali.ghaffaari@mpi-inf.mpg.de>
 *
 *  @internal
 *       Created:  Thu Aug 03, 2017  04:37
 *  Organization:  Max-Planck-Institut fuer Informatik
 *     Copyright:  Copyright (c) 2017, Ali Ghaffaari
 *
 *  This source code is released under the terms of the MIT License.
 *  See LICENSE file for more information.
 */

#include <cstdlib>
#include <csignal>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <seqan/arg_parse.h>
#include <gcsa/gcsa.h>

#include <config.h>
#include "seed.h"
#include "timer.h"
#include "options.h"
#include "release.h"


  seqan::ArgumentParser::ParseResult
parse_args( Options& options, int argc, char* argv[] );

  void
setup_argparser( seqan::ArgumentParser& parser );

  inline void
get_option_values( Options& options, seqan::ArgumentParser& parser );

  void
locate_seeds( std::string& seq_name, std::string& gcsa_name, unsigned int seed_len,
    unsigned int distance, std::string& output_name );

  void
signal_handler( int signal );

std::size_t done_idx = 0;
std::size_t total_no = 0;
std::size_t total_occs = 0;


  int
main( int argc, char* argv[] )
{
  // Parse the command line.
  Options options;
  auto res = parse_args( options, argc, argv );
  // If parsing was not successful then exit with code 1 if there were errors.
  // Otherwise, exit with code 0 (e.g. help was printed).
  if (res != seqan::ArgumentParser::PARSE_OK)
    return res == seqan::ArgumentParser::PARSE_ERROR;

  /* Install signal handler */
  std::signal( SIGUSR1, signal_handler );

  locate_seeds( options.seq_filename, options.gcsa_filename, options.seed_len,
      options.distance, options.output_filename );

  return EXIT_SUCCESS;
}


  void
signal_handler( int )
{
  std::cout << "Located " << ::done_idx << " out of " << ::total_no
            << " with " << ::total_occs << " occurrences in "
            << Timer<>::get_lap_str( "locate" ) << ": "
            << ::done_idx * 100 / total_no << "% done." << std::endl;
}


  void
locate_seeds( std::string& seq_name, std::string& gcsa_name, unsigned int seed_len,
    unsigned int distance, std::string& /* output_name */ )
{
  //std::ofstream output_file( output_name, std::ofstream::out );
  std::ifstream seq_file( seq_name, std::ifstream::in | std::ifstream::binary );
  if ( !seq_file ) {
    throw std::runtime_error("could not open file '" + seq_name + "'" );
  }
  std::ifstream gcsa_file( gcsa_name, std::ifstream::in | std::ifstream::binary );
  if ( !gcsa_file ) {
    throw std::runtime_error("could not open file '" + gcsa_name + "'" );
  }
  gcsa::GCSA index;
  std::vector< std::string > sequences;
  std::vector< std::string > patterns;
  std::vector< gcsa::node_type > results;

  std::cout << "Loading GCSA index..." << std::endl;
  index.load( gcsa_file );
  std::cout << "Loading sequences..." << std::endl;
  {
    auto timer = Timer<>( "sequences" );
    std::string line;
    while ( std::getline( seq_file, line ) ) {
      sequences.push_back( line );
    }
  }
  std::cout << "Loaded " << sequences.size() << " sequences in "
            << Timer<>::get_duration_str( "sequences" ) << "." << std::endl;
  std::cout << "Generating patterns..." << std::endl;
  {
    auto timer = Timer<>( "patterns" );
    seeding( patterns, sequences, seed_len, distance );
  }
  ::total_no = patterns.size();
  std::cout << "Generated " << patterns.size() << " patterns in "
            << Timer<>::get_duration_str( "patterns" ) << "." << std::endl;
  std::cout << "Locating patterns..." << std::endl;
  std::vector< gcsa::range_type > ranges;
  gcsa::size_type total = 0;
  {
    auto timer = Timer<>( "find" );
    for ( const auto& p : patterns ) {
      gcsa::range_type range = index.find( p );
      if( !gcsa::Range::empty( range ) ) {
        ranges.push_back( range );
        total += index.count( range );
      }
    }
  }
  std::cout << "Found " << ranges.size() << " patterns matching " << total << " paths in "
            << Timer<>::get_duration_str( "find" ) << "." << std::endl;
  total = 0;
  {
    auto timer = Timer<>( "locate" );
    for ( const auto& range : ranges ) {
      index.locate( range, results );
      // TODO: In order to be fair comparison, results should be written to file using async IO.
      ::total_occs += results.size();
      ::done_idx++;
    }
  }
  std::cout << "Located " << ::total_occs << " occurrences in "
            << Timer<>::get_duration_str( "locate" ) << "." << std::endl;
}


  inline seqan::ArgumentParser::ParseResult
parse_args( Options& options, int argc, char* argv[] )
{
  // setup ArgumentParser.
  seqan::ArgumentParser parser( release::name );
  setup_argparser( parser );
  // Embedding program's meta data and build information.
  setShortDescription( parser, release::short_desc );
  setVersion( parser, release::version );
  setDate( parser, LAST_MOD_DATE );
  addDescription( parser, release::desc );
  // parse command line.
  auto res = seqan::parse( parser, argc, argv );
  // only extract options if the program will continue after parse_args()
  if ( res != seqan::ArgumentParser::PARSE_OK ) {
    return res;
  }

  get_option_values( options, parser );

  return seqan::ArgumentParser::PARSE_OK;
}


  inline void
setup_argparser( seqan::ArgumentParser& parser )
{
  // positional arguments.
  std::string POSARG1 = "SEQ_FILE";
  // add usage line.
  addUsageLine(parser, "[\\fIOPTIONS\\fP] \"\\fI" + POSARG1 + "\\fP\"");
  // sequence file -- positional argument.
  seqan::ArgParseArgument seq_arg( seqan::ArgParseArgument::INPUT_FILE, POSARG1 );
  addArgument( parser, seq_arg );
  // GCSA2 index file -- **required** option.
  seqan::ArgParseOption gcsa_arg( "g", "gcsa", "GCSA2 index file.",
      seqan::ArgParseArgument::INPUT_FILE, "GCSA2_FILE" );
  setValidValues( gcsa_arg, gcsa::GCSA::EXTENSION );
  addOption( parser, gcsa_arg );
  setRequired( parser, "g" );
  // Seed length.
  addOption( parser, seqan::ArgParseOption( "l", "seed-len", "Seed length.",
        seqan::ArgParseArgument::INTEGER, "INT" ) );
  setRequired( parser, "l" );
  // Overlapping seeds?
  addOption( parser, seqan::ArgParseOption( "d", "distance",
        "Distance between seeds [default: seed length given by \\fB-l\\fP]",
        seqan::ArgParseArgument::INTEGER, "INT" ) );
  setDefaultValue( parser, "d", 0 );  /* Default value is seed length. */
  // Output file.
  seqan::ArgParseOption output_arg( "o", "output",
      "Write positions where sequences are matched.",
      seqan::ArgParseArgument::OUTPUT_FILE, "OUTPUT" );
  addOption( parser, output_arg );
  setRequired( parser, "o" );
}


  inline void
get_option_values( Options& options, seqan::ArgumentParser& parser )
{
  getArgumentValue( options.seq_filename, parser, 0 );
  getOptionValue( options.gcsa_filename, parser, "gcsa" );
  getOptionValue( options.output_filename, parser, "output" );
  getOptionValue( options.seed_len, parser, "seed-len" );
  getOptionValue( options.distance, parser, "distance" );
  if ( options.distance == 0 ) options.distance = options.seed_len;
}
