// ---------------------------------------------------------------------
//
// Copyright (C) 2010 - 2015 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE at
// the top level of the deal.II distribution.
//
// ---------------------------------------------------------------------


// test for Point::distance_square() for VectorizedArray numbers

#include "../tests.h"
#include <deal.II/base/point.h>
#include <deal.II/base/vectorization.h>


template <int dim, typename number>
void check ()
{
  VectorizedArray<number> distance_vec;
  Point<dim, VectorizedArray<number>> p1_vec, p2_vec;

  for (unsigned int v = 0; v < VectorizedArray<number>::n_array_elements; ++v)
    {
      Point<dim,number> p1, p2;
      for (unsigned int i=0; i<dim; ++i)
        {
          p1[i] = 10.0 + 0.12345 * i + 0.987 * v;
          p1[i] = 0.5 + 0.6789   * i - 0.543 * v;

          p1_vec[i][v] = p1[i];
          p2_vec[i][v] = p2[i];
        }

      distance_vec[v] = p1.distance_square(p2);
    }

  const VectorizedArray<number> distance_vec2 = p1_vec.distance_square(p2_vec);
  distance_vec -= distance_vec2;
  number diff = 0.;

  for (unsigned int v = 0; v < VectorizedArray<number>::n_array_elements; ++v)
    diff += std::abs(distance_vec[v]);

  AssertThrow(diff < 1e-10,
              ExcMessage("diff is " + std::to_string(diff)));

  deallog << "Ok" << std::endl;
}

int main ()
{
  std::ofstream logfile("output");
  deallog << std::setprecision(3);
  deallog.attach(logfile);

  check<1,float>();
  check<2,float>();
  check<3,float>();

  check<1,double>();
  check<2,double>();
  check<3,double>();

}
