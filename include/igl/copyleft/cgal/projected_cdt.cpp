// This file is part of libigl, a simple c++ geometry processing library.
// 
// Copyright (C) 2016 Alec Jacobson
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#include "projected_cdt.h"
#include "insert_into_cdt.h"
#include "assign_scalar.h"
#include "../../list_to_matrix.h"
template <typename Kernel, typename Index>
IGL_INLINE void igl::copyleft::cgal::projected_cdt(
  const std::vector<CGAL::Object> & objects,
  const CGAL::Plane_3<Kernel> & P,
  std::vector<CGAL::Point_3<Kernel> >& vertices,
  std::vector<std::vector<Index> >& faces)
{
  typedef CGAL::Triangulation_vertex_base_2<Kernel>  TVB_2;
  typedef CGAL::Constrained_triangulation_face_base_2<Kernel> CTFB_2;
  typedef CGAL::Triangulation_data_structure_2<TVB_2,CTFB_2> TDS_2;
  typedef CGAL::Exact_intersections_tag Itag;
  typedef CGAL::Constrained_Delaunay_triangulation_2<Kernel,TDS_2,Itag> CDT_2;
  typedef CGAL::Constrained_triangulation_plus_2<CDT_2> CDT_plus_2;
  CDT_plus_2 cdt;
  for(const auto & obj : objects) insert_into_cdt(obj,P,cdt);
  // Read off vertices of the cdt, remembering index
  std::map<typename CDT_plus_2::Vertex_handle,Index> v2i;
  size_t count=0;
  for (
    auto itr = cdt.finite_vertices_begin();
    itr != cdt.finite_vertices_end(); 
    itr++) 
  {
    vertices.push_back(P.to_3d(itr->point()));
    v2i[itr] = count;
    count++;
  }
  // Read off faces and store index triples
  for (
    auto itr = cdt.finite_faces_begin();
    itr != cdt.finite_faces_end(); 
    itr++)
  {
    faces.push_back( 
      { v2i[itr->vertex(0)], v2i[itr->vertex(1)], v2i[itr->vertex(2)] });
  }
}

template < typename Kernel, typename DerivedV, typename DerivedF>
IGL_INLINE void igl::copyleft::cgal::projected_cdt(
  const std::vector<CGAL::Object> & objects,
  const CGAL::Plane_3<Kernel> & P,
  Eigen::PlainObjectBase<DerivedV> & V,
  Eigen::PlainObjectBase<DerivedF> & F)
{
  std::vector<CGAL::Point_3<Kernel> > vertices;
  std::vector<std::vector<typename DerivedF::Scalar> > faces;
  projected_cdt(objects,P,vertices,faces);
  V.resize(vertices.size(),3);
  for(int v = 0;v<vertices.size();v++)
  {
    for(int d = 0;d<3;d++)
    {
      assign_scalar(vertices[v][d], V(v,d));
    }
  }
  list_to_matrix(faces,F);
}

#ifdef IGL_STATIC_LIBRARY
// Explicit template instantiation
// generated by autoexplicit.sh
template void igl::copyleft::cgal::projected_cdt<CGAL::Epick, long>(std::vector<CGAL::Object, std::allocator<CGAL::Object> > const&, CGAL::Plane_3<CGAL::Epick> const&, std::vector<CGAL::Point_3<CGAL::Epick>, std::allocator<CGAL::Point_3<CGAL::Epick> > >&, std::vector<std::vector<long, std::allocator<long> >, std::allocator<std::vector<long, std::allocator<long> > > >&);
// generated by autoexplicit.sh
template void igl::copyleft::cgal::projected_cdt<CGAL::Epeck, long>(std::vector<CGAL::Object, std::allocator<CGAL::Object> > const&, CGAL::Plane_3<CGAL::Epeck> const&, std::vector<CGAL::Point_3<CGAL::Epeck>, std::allocator<CGAL::Point_3<CGAL::Epeck> > >&, std::vector<std::vector<long, std::allocator<long> >, std::allocator<std::vector<long, std::allocator<long> > > >&);
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#endif
