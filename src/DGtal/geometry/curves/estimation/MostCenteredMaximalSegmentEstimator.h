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

#pragma once

/**
 * @file MostCenteredMaximalSegmentEstimator.h
 * @author Tristan Roussillon (\c tristan.roussillon@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2011/01/26
 *
 * Header file for module MostCenteredMaximalSegmentEstimator.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(MostCenteredMaximalSegmentEstimator_RECURSES)
#error Recursive header files inclusion detected in MostCenteredMaximalSegmentEstimator.h
#else // defined(MostCenteredMaximalSegmentEstimator_RECURSES)
/** Prevents recursive inclusion of headers. */
#define MostCenteredMaximalSegmentEstimator_RECURSES

#if !defined MostCenteredMaximalSegmentEstimator_h
/** Prevents repeated inclusion of headers. */
#define MostCenteredMaximalSegmentEstimator_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include <list>

#include "DGtal/base/Common.h"
#include "DGtal/base/Exceptions.h"
#include "DGtal/base/Circulator.h"

#include "DGtal/geometry/curves/estimation/CSegmentComputerFunctor.h"
#include "DGtal/geometry/curves/CForwardSegmentComputer.h"
#include "DGtal/geometry/curves/SaturatedSegmentation.h"

//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

  /////////////////////////////////////////////////////////////////////////////
  // template class MostCenteredMaximalSegmentEstimator
  /**
   * Description of template class 'MostCenteredMaximalSegmentEstimator' <p>
   * \brief Aim: Computes a quantity to each element of a range associated to 
   * the most centered maximal segment  
   *
   * @tparam SegmentComputer at least a model of CForwardSegmentComputer
   * @tparam SCFunctor a model of CSegmentComputerFunctor
   */
  template <typename SegmentComputer, typename SCFunctor>
  class MostCenteredMaximalSegmentEstimator
  {

    BOOST_CONCEPT_ASSERT(( CForwardSegmentComputer<SegmentComputer> )); 
    BOOST_CONCEPT_ASSERT(( CSegmentComputerFunctor<SCFunctor> )); 

    // ----------------------- Types ------------------------------
  public:

    typedef typename SegmentComputer::ConstIterator ConstIterator;
    typedef typename SCFunctor::Quantity Quantity;

    typedef SaturatedSegmentation<SegmentComputer> Segmentation; 
    typedef typename Segmentation::SegmentComputerIterator SegmentIterator; 

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Default constructor. Not valid.
     */
    MostCenteredMaximalSegmentEstimator();

    /**
     * Constructor.
     * @param aSegmentComputer
     * @param aSCFunctor
     */
    MostCenteredMaximalSegmentEstimator(const SegmentComputer& aSegmentComputer, 
                                        const SCFunctor& aSCFunctor);

    /**
     * Destructor.
     */
    ~MostCenteredMaximalSegmentEstimator() {};

    // ----------------------- Interface --------------------------------------
  public:

    /**
     * Initialisation.
     * @param h grid size (must be >0).
     * @param itb, begin iterator
     * @param ite, end iterator
     */
    void init(const double h, const ConstIterator& itb, const ConstIterator& ite);

    /**
     * Unique estimation 
     * @param it any valid iterator
     * @return the estimated quantity at *it
     *
     * NB: the whole range [@e myBegin , @e myEnd)| 
     * is scanned in the worst case
     */
    Quantity eval(const ConstIterator& it);

    /**
     * Estimation for a subrange [@e itb , @e ite )
     *
     * @param itb subrange begin iterator
     * @param ite subrange end iterator     
     * @param result output iterator on the estimated quantity
     *
     * @return the estimated quantity
     * from itb till ite (excluded)
     *
     * NB: the whole range [@e myBegin , @e myEnd)| 
     * is scanned in the worst case
     */
    template <typename OutputIterator>
    OutputIterator eval(const ConstIterator& itb, const ConstIterator& ite, 
                        OutputIterator result); 


    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    bool isValid() const;

    // ------------------------- Protected Datas ------------------------------
  protected:

    // ------------------------- Private Datas --------------------------------
  private:

    /** grid step */
    double myH; 

    /** begin and end iterators */ 
    ConstIterator myBegin,myEnd;

    /** segmentComputer used to segment */ 
    SegmentComputer mySC; 

    /** functor estimating the quantity from a point and a segmentComputer */ 
    SCFunctor mySCFunctor;

    // ------------------------- Internal services ------------------------------

  private:

    /**
     * Specialization of the end of the algorithm
     *
     * @param itb subrange begin iterator
     * @param ite subrange end iterator     
     * @param itCurrent current iterator
     * @param first iterator on the first maximal segment
     * @param last iterator on the last maximal segment
     * @param result output iterator on the estimated quantity
     *
     * @return the estimated quantity
     * from itCurrent till ite (excluded)
     * NB: O(n)
     */
    template <typename OutputIterator>
    OutputIterator endEval(const ConstIterator& itb, const ConstIterator& ite, ConstIterator& itCurrent, 
			   SegmentIterator& first, SegmentIterator& last, 
			   OutputIterator result); 

    template <typename OutputIterator>
    OutputIterator endEval(const ConstIterator& /*itb*/, const ConstIterator& ite, ConstIterator& itCurrent, 
			   SegmentIterator& /*first*/, SegmentIterator& last, 
			   OutputIterator result, IteratorType); 

    template <typename OutputIterator>
    OutputIterator endEval(const ConstIterator& itb, const ConstIterator& ite, ConstIterator& itCurrent, 
			   SegmentIterator& first, SegmentIterator& last, 
			   OutputIterator result, CirculatorType); 

    // ------------------------- Hidden services ------------------------------

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    MostCenteredMaximalSegmentEstimator ( const MostCenteredMaximalSegmentEstimator & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    MostCenteredMaximalSegmentEstimator & operator= ( const MostCenteredMaximalSegmentEstimator & other );


  }; // end of class MostCenteredMaximalSegmentEstimator

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/geometry/curves/estimation/MostCenteredMaximalSegmentEstimator.ih"
#include "DGtal/geometry/curves/estimation/SegmentComputerFunctor.h"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined MostCenteredMaximalSegmentEstimator_h

#undef MostCenteredMaximalSegmentEstimator_RECURSES
#endif // else defined(MostCenteredMaximalSegmentEstimator_RECURSES)
