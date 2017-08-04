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
#include <fstream>
#include <vector>

#include <seqan/arg_parse.h>
#include <gcsa/gcsa.h>

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
    bool nonoverlapping, std::string& output_name );

  bool
get_seed( unsigned int& context, std::string& seed, std::string& sequence,
    unsigned int seed_len, bool nonoverlapping );


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

  locate_seeds( options.seq_filename, options.gcsa_filename, options.seed_len,
      options.nonoverlapping, options.output_filename );

  return EXIT_SUCCESS;
}


  void
locate_seeds( std::string& seq_name, std::string& gcsa_name, unsigned int seed_len,
    bool nonoverlapping, std::string& output_name )
{
  std::ifstream seq_file( seq_name, std::ifstream::in | std::ifstream::binary );
  if ( !seq_file ) {
    throw std::runtime_error("could not open file '" + seq_name + "'" );
  }
  std::ifstream gcsa_file( gcsa_name, std::ifstream::in | std::ifstream::binary );
  if ( !gcsa_file ) {
    throw std::runtime_error("could not open file '" + gcsa_name + "'" );
  }
  gcsa::GCSA index;
  index.load( gcsa_file );

  std::string sequence;
  std::string seed;
  std::vector< gcsa::node_type > results;
  while ( std::getline( seq_file, sequence ) ) {
    unsigned int context = 0;
    while ( get_seed ( context, seed, sequence, seed_len, nonoverlapping ) ) {
      gcsa::range_type range = index.find( seed );
      index.locate( range, results, true );
    }
  }

  std::ofstream output_file( output_name, std::ofstream::out );
  for ( auto && r : results ) {
    output_file << gcsa::Node::id( r ) << "\t" << gcsa::Node::offset( r ) << std::endl;
  }
}


  bool
get_seed( unsigned int& context, std::string& seed, std::string& sequence,
    unsigned int seed_len, bool nonoverlapping )
{
  if ( context + seed_len == sequence.length() + 1 || seed_len > sequence.length() ) {
    return false;
  }
  if ( context + seed_len > sequence.length() + 1 ) {
    context = sequence.length() - seed_len;
    nonoverlapping = false;
  }

  seed = sequence.substr( context, seed_len );

  if ( nonoverlapping ) {
    context += seed_len;
  }
  else {
    context += 1;
  }

  return true;
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
  // :TODO:Thu Aug 03 17:05:\@cartoonist: date should be captured from git.
  setDate( parser, __DATE__ );
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
  addOption( parser, seqan::ArgParseOption( "n", "non-overlapping",
        "Use non-overlapping seeds." ) );
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
  options.nonoverlapping = isSet( parser, "non-overlapping" );
}
