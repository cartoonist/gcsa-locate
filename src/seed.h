/**
 *    @file  seed.h
 *   @brief  Seeding helper functions.
 *
 *  Helper functions for seeding.
 *
 *  @author  Ali Ghaffaari (\@cartoonist), <ali.ghaffaari@mpi-inf.mpg.de>
 *
 *  @internal
 *       Created:  Mon Oct 16, 2017  04:33
 *  Organization:  Max-Planck-Institut fuer Informatik
 *     Copyright:  Copyright (c) 2017, Ali Ghaffaari
 *
 *  This source code is released under the terms of the MIT License.
 *  See LICENSE file for more information.
 */

#ifndef  SEED_H__
#define  SEED_H__

#include <string>


/* Tag template class. */
template< typename TSpec >
  struct Tag {};

/* Seeding strategies */
struct GreedyOverlapStrategy;
struct NonOverlapStrategy;
struct GreedyNonOverlapStrategy;

/* Seeding strategy tags */
typedef Tag< GreedyOverlapStrategy > GreedyOverlapping;
typedef Tag< NonOverlapStrategy > NonOverlapping;
typedef Tag< GreedyNonOverlapStrategy > GreedyNonOverlapping;

/**
 *  @brief  Add any k-mers from the given string set with `step` distance to seed set.
 *
 *  @param  seeds The seed set.
 *  @param  string_set The string set from which seeds are extracted.
 *  @param  k The length of the seeds.
 *  @param  step The step size.
 *
 *  For each string in string set, it add all substring of length `k` starting from 0
 *  to end of string with `step` distance with each other. If `step` is equal to `k`,
 *  it gets non-overlapping substrings of length k.
 */
template< typename TText >
    inline void
  seeding( std::vector< TText >& seeds,
      const std::vector< TText >& string_set,
      unsigned int k,
      unsigned int step )
  {
    for ( unsigned int idx = 0; idx < string_set.size(); ++idx ) {
      for ( unsigned int i = 0; i < string_set[idx].length() - k + 1; i += step ) {
        seeds.push_back( string_set[idx].substr( i, k ) );
      }
    }
  }  /* -----  end of template function seeding  ----- */

/**
 *  @brief  Seeding a set of sequence by reporting overlapping k-mers.
 *
 *  @param  seeds The resulting set of strings containing seeds.
 *  @param  string_set The string set from which seeds are extracted.
 *  @param  k The length of the seeds.
 *  @param  tag Tag for greedy fixed-length overlapping seeding strategy.
 *
 *  Extract a set of overlapping seeds of length k.
 */
template< typename TText >
    inline void
  seeding( std::vector< TText >& seeds,
      const std::vector< TText >& string_set,
      unsigned int k,
      GreedyOverlapping )
  {
    seeds.clear();
    seeding( seeds, string_set, k, 1 );
  }  /* -----  end of template function seeding  ----- */

/**
 *  @brief  Seeding by partitioning each sequence into non-overlapping k-mers.
 *
 *  @param  seeds The resulting set of strings containing seeds.
 *  @param  string_set The string set from which seeds are extracted.
 *  @param  k The length of the seeds.
 *  @param  tag Tag for fixed-length non-overlapping seeding strategy.
 *
 *  Extract a set of non-overlapping seeds of length k.
 */
template< typename TText >
    inline void
  seeding( std::vector< TText >& seeds,
      const std::vector< TText >& string_set,
      unsigned int k,
      NonOverlapping )
  {
    seeds.clear();
    seeding( seeds, string_set, k, k );
  }  /* -----  end of function seeding  ----- */

/**
 *  @brief  Seeding by greedy partitioning each sequence into non-overlapping k-mers.
 *
 *  @param  seeds The resulting set of strings containing seeds.
 *  @param  string_set The string set from which seeds are extracted.
 *  @param  k The length of the seeds.
 *  @param  tag Tag for greedy fixed-length non-overlapping seeding strategy.
 *
 *  Extract a set of non-overlapping seeds of length k.
 *
 *  NOTE: In case that the length of sequence is not dividable by k the last seed
 *        may overlap its previous (greedy).
 */
template< typename TText >
    inline void
  seeding( std::vector< TText >& seeds,
      const std::vector< TText >& string_set,
      unsigned int k,
      GreedyNonOverlapping )
  {
    seeds.clear();

    for ( unsigned int idx = 0; idx < string_set.size(); ++idx ) {
      for ( unsigned int i = 0; i < string_set[idx].length() - k; i += k ) {
        seeds.push_back( string_set[idx].substr( i, k ) );
      }
      unsigned int last = string_set[idx].length() - k;
      seeds.push_back( string_set[idx].substr( last, k ) );
    }
  }  /* -----  end of function seeding  ----- */

#endif  /* ----- #ifndef SEED_H__  ----- */
