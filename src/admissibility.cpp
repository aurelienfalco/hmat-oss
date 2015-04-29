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

#include "admissibility.hpp"
#include "cluster_tree.hpp"

#include "common/my_assert.h"
#include "hmat_cpp_interface.hpp"

#include <cmath>
#include <sstream>

namespace hmat {

StandardAdmissibilityCondition::StandardAdmissibilityCondition(
    double eta, size_t maxElementsPerBlock):
    eta_(eta), maxElementsPerBlock(maxElementsPerBlock)
{
}

bool
StandardAdmissibilityCondition::isAdmissible(const ClusterTree& rows, const ClusterTree& cols)
{
    CompressionMethod m = HMatSettings::getInstance().compressionMethod;
    bool isFullAlgo = !(m == AcaPartial || m == AcaPlus);
    if (isFullAlgo) {
        size_t elements = ((size_t) rows.data.size()) * cols.data.size();
        if(elements > maxElementsPerBlock)
            return false;
    }
    // TODO may be this test should be done out of the AdmissibilityCondition
    if(rows.data.size() < 2 || cols.data.size() < 2)
        return false;

    AxisAlignedBoundingBox* rows_bbox = static_cast<AxisAlignedBoundingBox*>(rows.admissibilityAlgoData_);
    if (rows_bbox == NULL)
    {
      rows_bbox = new AxisAlignedBoundingBox(rows.data);
      rows.admissibilityAlgoData_ = rows_bbox;
    }
    AxisAlignedBoundingBox* cols_bbox = static_cast<AxisAlignedBoundingBox*>(cols.admissibilityAlgoData_);
    if (cols_bbox == NULL)
    {
      cols_bbox = new AxisAlignedBoundingBox(cols.data);
      cols.admissibilityAlgoData_ = cols_bbox;
    }
    return std::min(rows_bbox->diameter(), cols_bbox->diameter()) <= eta_ * rows_bbox->distanceTo(*cols_bbox);
}

std::string
StandardAdmissibilityCondition::str() const
{
  std::ostringstream oss;
  oss << "Hackbusch formula, with eta = " << eta_;
  return oss.str();
}

void StandardAdmissibilityCondition::setEta(double eta) {
    eta_ = eta;
}

StandardAdmissibilityCondition StandardAdmissibilityCondition::DEFAULT_ADMISSIBLITY = StandardAdmissibilityCondition(2.0);

}  // end namespace hmat