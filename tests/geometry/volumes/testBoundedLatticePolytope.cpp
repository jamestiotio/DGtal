/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

/**
 * @file testBoundedLatticePolytope.cpp
 * @ingroup Tests
 * @author Jacques-Olivier Lachaud (\c jacques-olivier.lachaud@univ-savoie.fr )
 * Laboratory of Mathematics (CNRS, UMR 5127), University of Savoie, France
 *
 * @date 2019/01/04
 *
 * Functions for testing class BoundedLatticePolytope.
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <algorithm>
#include "DGtal/base/Common.h"
#include "DGtal/kernel/SpaceND.h"
#include "DGtal/geometry/volumes/BoundedLatticePolytope.h"
#include "DGtalCatch.h"
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;


///////////////////////////////////////////////////////////////////////////////
// Functions for testing class BoundedLatticePolytope.
///////////////////////////////////////////////////////////////////////////////

SCENARIO( "BoundedLatticePolytope< Z2 > unit tests", "[lattice_polytope][2d]" )
{
  typedef SpaceND<2,int>                   Space;
  typedef Space::Point                     Point;
  typedef Space::Vector                    Vector;
  typedef Space::Integer                   Integer;
  typedef BoundedLatticePolytope< Space >  Polytope;

  GIVEN( "A triangle P at (0,0), (5,0), (0,7)" ) {
    Point a( 0, 0 );
    Point b( 5, 0 );
    Point c( 0, 7 );
    Polytope P { a, b, c };
    THEN( "Its domain contains its vertices" ) {
      REQUIRE( P.isDomainPointInside( a ) );
      REQUIRE( P.isDomainPointInside( b ) );
      REQUIRE( P.isDomainPointInside( c ) );
    }
    THEN( "Its vertices lie on its boundary" ) {
      REQUIRE( P.isBoundary( a ) );
      REQUIRE( P.isBoundary( b ) );
      REQUIRE( P.isBoundary( c ) );
      REQUIRE( ! P.isInterior( a ) );
      REQUIRE( ! P.isInterior( b ) );
      REQUIRE( ! P.isInterior( c ) );
    }
    THEN( "It contains more than 3 integer points" ) {
      REQUIRE( P.count() > 3 );
    }
    THEN( "It contains more points than its area" ) {
      REQUIRE( P.count() > (5*7/2) );
    }
    THEN( "It satisfies Pick's formula, ie 2*Area(P) = 2*#Int(P) + #Bd(P) - 2" ) {
      Polytope IntP = P.interiorPolytope();
      auto   nb_int = IntP.count();
      auto    nb_bd = P.count() - IntP.count();
      auto    area2 = 5*7;
      CAPTURE( nb_int );
      CAPTURE( nb_bd );
      CAPTURE( area2 );
      REQUIRE( area2 == 2*nb_int + nb_bd - 2 );
    }
    THEN( "It satisfies #In(P) <= #Int(P) + #Bd(P)" ) {
      auto     nb = P.count();
      auto nb_int = P.countInterior();
      auto  nb_bd = P.countBoundary();
      CAPTURE( nb );
      CAPTURE( nb_int );
      CAPTURE( nb_bd );
      REQUIRE( nb <= nb_int + nb_bd );
    }
    WHEN( "Cut by some half-space" ) {
      Polytope Q = P;
      Q.cut( Vector( -1, 1 ), 3 );
      THEN( "It contains less points" ) {
	REQUIRE( Q.count() < P.count() );
      }
    }
    THEN( "Its boundary points and interior points are its inside points (closed polytope)" ) {
      std::vector<Point> inside;
      std::vector<Point> interior;
      std::vector<Point> boundary;
      std::vector<Point> all;
      P.getPoints( inside );
      P.getInteriorPoints( interior );
      P.getBoundaryPoints( boundary );
      std::sort( inside.begin(), inside.end() );
      std::sort( interior.begin(), interior.end() );
      std::sort( boundary.begin(), boundary.end() );
      std::set_union( interior.cbegin(), interior.cend(), boundary.cbegin(), boundary.cend(),
                      std::back_inserter( all ) );
      REQUIRE( std::equal( inside.cbegin(), inside.cend(), all.cbegin() ) );
    }
  }
}

SCENARIO( "BoundedLatticePolytope< Z3 > unit tests", "[lattice_polytope][3d]" )
{
  typedef SpaceND<3,int>                   Space;
  typedef Space::Point                     Point;
  typedef BoundedLatticePolytope< Space >  Polytope;

  GIVEN( "A twisted simplex P at (0,0,0), (1,0,0), (0,1,0), (1,1,z)" ) {
    Point a( 0, 0, 0 );
    Point b( 1, 0, 0 );
    Point c( 0, 1, 0 );
    Point d( 1, 1, 8 );
    Polytope P { a, b, c, d };
    THEN( "Its domain contains its vertices" ) {
      REQUIRE( P.isDomainPointInside( a ) );
      REQUIRE( P.isDomainPointInside( b ) );
      REQUIRE( P.isDomainPointInside( c ) );
      REQUIRE( P.isDomainPointInside( d ) );
    }
    THEN( "Its vertices lie on its boundary" ) {
      REQUIRE( P.isBoundary( a ) );
      REQUIRE( P.isBoundary( b ) );
      REQUIRE( P.isBoundary( c ) );
      REQUIRE( P.isBoundary( d ) );
      REQUIRE( ! P.isInterior( a ) );
      REQUIRE( ! P.isInterior( b ) );
      REQUIRE( ! P.isInterior( c ) );
      REQUIRE( ! P.isInterior( d ) );
    }
    THEN( "It satisfies #In(P) <= #Int(P) + #Bd(P)" ) {
      auto     nb = P.count();
      auto nb_int = P.countInterior();
      auto  nb_bd = P.countBoundary();
      CAPTURE( nb );
      CAPTURE( nb_int );
      CAPTURE( nb_bd );
      REQUIRE( nb <= nb_int + nb_bd );
    }
    THEN( "It contains only 4 integer points" ) {
      REQUIRE( P.count() == 4 );
    }

  }

  GIVEN( "A closed arbitrary simplex P at (0,0,0), (6,3,0), (0,5,10), (6,4,8)" ) {
    Point a( 0, 0, 0 );
    Point b( 6, 3, 0 );
    Point c( 0, 5, 10 );
    Point d( 6, 4, 8 );
    Polytope P { a, b, c, d };
    
    THEN( "It satisfies #In(P) == #Int(P) + #Bd(P)" ) {
      auto     nb = P.count();
      auto nb_int = P.countInterior();
      auto  nb_bd = P.countBoundary();
      CAPTURE( nb );
      CAPTURE( nb_int );
      CAPTURE( nb_bd );
      REQUIRE( nb == nb_int + nb_bd );
    }
    THEN( "Its boundary points and interior points are its inside points (closed polytope)" ) {
      std::vector<Point> inside;
      std::vector<Point> interior;
      std::vector<Point> boundary;
      std::vector<Point> all;
      P.getPoints( inside );
      P.getInteriorPoints( interior );
      P.getBoundaryPoints( boundary );
      std::sort( inside.begin(), inside.end() );
      std::sort( interior.begin(), interior.end() );
      std::sort( boundary.begin(), boundary.end() );
      std::set_union( interior.cbegin(), interior.cend(), boundary.cbegin(), boundary.cend(),
                      std::back_inserter( all ) );
      REQUIRE( std::equal( inside.cbegin(), inside.cend(), all.cbegin() ) );
    }
    WHEN( "Cut by some axis aligned half-space (1,0,0).x <= 3" ) {
      Polytope Q = P;
      Q.cut( 0, true, 3 );
      THEN( "It contains less points" ) {
        CAPTURE( P );
        CAPTURE( Q );
	REQUIRE( Q.count() < P.count() );
      }
    }
  }
}

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
