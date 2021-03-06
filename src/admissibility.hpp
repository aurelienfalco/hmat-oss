/*
  HMat-OSS (HMatrix library, open source software)

  Copyright (C) 2014-2015 Airbus Group SAS

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

  http://github.com/jeromerobert/hmat-oss
*/

/*! \file
  \ingroup HMatrix
  \brief Spatial cluster tree for the Dofs.
*/
#ifndef _ADMISSIBLITY_HPP
#define _ADMISSIBLITY_HPP

#include <cstddef>
#include <string>

namespace hmat {

// Forward declarations
class ClusterTree;

class AdmissibilityCondition
{
public:
  virtual ~AdmissibilityCondition() {}
  /*! \brief Returns true if 2 nodes are admissible together.

    This is used for the tree construction on which we develop the HMatrix.
    Two leaves are admissible if they satisfy the criterion allowing the
    compression of the resulting matrix block.

    \return true  if 2 nodes are admissible.

   */
  virtual bool isAdmissible(const ClusterTree& rows, const ClusterTree& cols) = 0;
  virtual bool isCompressible(const ClusterTree& rows, const ClusterTree& cols);
  virtual std::pair<bool, bool> isRowsColsAdmissible(const ClusterTree& rows, const ClusterTree& cols);
  /*! \brief if the result of this function is true,
  *    the corresponding H-Matrix block will not be created. */
  virtual bool isInert(const ClusterTree& rows, const ClusterTree& cols) = 0;
  /*! \brief Clean up data which may be allocated by isAdmissible  */
  virtual void clean(const ClusterTree&) const {}

  virtual std::string str() const = 0;
};

/**
 * @brief Admissibility criteria that returns two booleans, one for rows and one for cols.
 * For each ClusterTree:
 * true if its size is x times smaller than the other.
 * It allows then to handle Tall & Skinny blocks in H-Matrices
 * @param ratio the ratio between the two sizes
 */
class TallSkinnyAdmissibilityCondition : public AdmissibilityCondition
{
public:
  TallSkinnyAdmissibilityCondition(double ratio_ = 2) : ratio(ratio_) {};
  virtual std::pair<bool, bool> isRowsColsAdmissible(const ClusterTree& rows, const ClusterTree& cols);
private:
  double ratio;
};

/**
 * @brief Hackbusch formula based admissibility
 *  is Hackbusch formula :
 *     min (diameter (), other-> diameter ()) <= eta * distanceTo (other);
 * @param eta    a parameter used in the evaluation of the admissibility.
 * @param maxElementsPerBlock limit memory size of a bloc with AcaFull and Svd compression
 */
class StandardAdmissibilityCondition : public TallSkinnyAdmissibilityCondition
{
public:
  StandardAdmissibilityCondition(double eta, size_t maxElementsPerBlock = 5000000,
                                 size_t maxElementsPerBlockAca = 0);
  bool isAdmissible(const ClusterTree& rows, const ClusterTree& cols);
  virtual std::pair<bool, bool> isRowsColsAdmissible(const ClusterTree& rows, const ClusterTree& cols);
  bool isInert(const ClusterTree& rows, const ClusterTree& cols) {return false;};
  void clean(const ClusterTree& current) const;
  std::string str() const;
  void setEta(double eta);
  /**
   * Force to ignore the eta parameter and concider all small
   * enough blocks as admissible
   */
  void setAlways(bool);
  static StandardAdmissibilityCondition DEFAULT_ADMISSIBLITY;
private:
  double eta_;
  size_t maxElementsPerBlock;
  size_t maxElementsPerBlockAca_;
  bool always_;
};

} //  end namespace hmat
#endif
