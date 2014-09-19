/**
 * File: Falls.cc
 *    Author: Eddie Rutledge
 *    February 2008 - ported from PVL by Jim Daly
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods of Falls and functions that act on Falls
 *             objects.  The Falls class is a FALLS representation of a
 *             data distribution in one dimension.
 *
 *  $Id: Falls.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#include <cmath>         // for floor() and ceil()
#include <MathBasics.h>  // for lcm()
#include <Falls.h>
#include <iostream>
#include <algorithm>


namespace ipvtol
{
  using std::istream;
  using std::ostream;
  using std::vector;
  using std::floor;
  using std::ceil;
  using std::min;
  using std::max;

  void Falls::print(ostream& out) const
  {
    out << "(l=" << m_l << ", r=" << m_r
	<< ", s=" << m_s << ", n=" << m_n << ")";
  }

  void Falls::read(istream& in)
  {
    in.ignore(3);
    in >> m_l;
    in.ignore(4);
    in >> m_r;
    in.ignore(4);
    in >> m_s;
    in.ignore(4);
    in >> m_n;
    in.ignore(1);
  }

  // this was adapted from STAPLIB
  //
  // Efficient FALLS intersection algorithm based on periods.
  // See Tech Report CRHC-94-09, Univ of Illinois, p 13.
  bool intersects(const Falls& f1, const Falls& f2)
  {
    // copy FALLS params to local variables
    int r1 = f1.getR();
    int r2 = f2.getR();
    int l1 = f1.getL();
    int l2 = f2.getL();
    int str1 = f1.getS();
    int str2 = f2.getS();
    int num1 = f1.getN();
    int num2 = f2.getN();

    // intersection period
    int per = lcm(static_cast<int>(str1), static_cast<int>(str2));

    // number of members of each FALLS occurring in a period
    int m1 =
      static_cast<int>(ceil(static_cast<float>(per) / str1));
    int m2 =
      static_cast<int>(ceil(static_cast<float>(per) / str2));

    // first segments that overlap
    int I1 = static_cast<int>
       (max(static_cast<float>(0), ceil((static_cast<float>(l2) - r1) / str1)));

    // loop checking for intersections.  If any occur, return true.

    // loop boundaries for first FALLS
    int lb1 = I1;
    int ub1 = min(I1 + m1 - 1, num1 - 1);
    for(int i1 = lb1; i1 <= ub1; i1++)
    {
       // loop boundaries for second FALLS
       int lb2 = static_cast<int>
          (max(static_cast<float>(0),
               ceil((static_cast<float>(str1) * i1 + l1 - r2) / str2)));
       int ub2 = static_cast<int>
          (min(floor((static_cast<float>(str1) * i1 + r1 - l2) / str2),
               (static_cast<float>(m2 - 1))));
       ub2 = min(ub2, num2 - 1);
       if (lb2 <= ub2)
       {
          return true;
       }
    }
    return false;
  }

  // NOTE: this function builds and returns a temporary STL vector of Falls.
  vector<Falls> intersection(const Falls& f1, const Falls& f2)
  {
    vector<Falls> fallsList(intersectionSize(f1, f2));
    intersection(f1, f2, fallsList);
    return fallsList;
  }//end intersection()


  // Adapted from STAPLIB.
  // Efficient FALLS intersection algorithm based on periods.
  // See Tech Report CRHC-94-09, Univ of Illinois, p 13.
  void intersection(const Falls& f1, const Falls& f2, vector<Falls>& result)
  {
    // copy FALLS params to local variables
    int r1 = f1.getR();
    int r2 = f2.getR();
    int l1 = f1.getL();
    int l2 = f2.getL();
    int str1 = f1.getS();
    int str2 = f2.getS();
    int num1 = f1.getN();
    int num2 = f2.getN();

    // intersection period
    int per = lcm(str1, str2);

    // number of members of each FALLS occurring in a period
    int m1 =
      static_cast<int>(ceil(static_cast<float>(per) / str1));
    int m2 =
      static_cast<int>(ceil(static_cast<float>(per) / str2));

    // first segments that intersect
    int I1 = static_cast<int>
       (max(static_cast<float>(0), ceil((static_cast<float>(l2) - r1) / str1)));

    // loop over sub-space of possible intersections m1 x m2
    // where there is actually an intersection, build a new FALLS for each
    // intersection, and add it to the list to be returned
    int listIndex = 0;

    // loop boundaries for the first FALLS
    int lb1 = I1;
    int ub1 = min(I1 + m1 - 1, num1 - 1);
    for(int i1 = lb1; i1 <= ub1; i1++)
    {
      // extract the line segment in question for the first FALLS
      int ls1L = l1 + i1 * str1;
      int ls1R = r1 + i1 * str1;

      // loop boundaries for second FALLS
      int lb2 = static_cast<int>
         (max(static_cast<float>(0),
              ceil((static_cast<float>(str1) * i1 + l1 - r2) / str2)));
      int ub2 = static_cast<int>
         (min(floor((static_cast<float>(str1) * i1 + r1 - l2) / str2),
              static_cast<float>(m2 - 1)));
      ub2 = min(ub2, num2 - 1);
      for (int i2 = lb2; i2 <= ub2; i2++)
      {
         // extract the line segment in question for the second FALLS
         int ls2L = l2 + i2 * str2;
         int ls2R = r2 + i2 * str2;
         
         // find the intersection
         int intL = max(ls1L, ls2L);
         int intR = min(ls1R, ls2R);

         // build the new FALLS, and add it to the list
         int num =
            min((num1 - i1 - 1) / m1, (num2 - i2 - 1) / m2) + 1;
         result[listIndex++] = Falls(intL, intR, per, num);
      }
    }
  }//end intersection()

  // Efficient FALLS intersection algorithm based on periods.
  // See Tech Report CRHC-94-09, Univ of Illinois, p 13.
  unsigned int intersectionSize(const Falls& f1, const Falls& f2)
  {
    // copy FALLS params to local variables
    int r1 = f1.getR();
    int r2 = f2.getR();
    int l1 = f1.getL();
    int l2 = f2.getL();
    int str1 = f1.getS();
    int str2 = f2.getS();
    int num1 = f1.getN();
    int num2 = f2.getN();

    // intersection period
    int per = lcm(str1, str2);

    // number of members of each FALLS occurring in a period
    int m1 =
      static_cast<int>(ceil(static_cast<float>(per) / str1));
    int m2 =
      static_cast<int>(ceil(static_cast<float>(per) / str2));

    // first points that overlap
    int I1 = static_cast<int>
       (max(static_cast<float>(0), ceil((static_cast<float>(l2) - r1) / str1)));

    // Loop over the sub-space of possible intersections m1 x m2
    // where there is actually an intersection, and keep a count
    unsigned int retVal = 0;

    // loop boundaries for first FALLS
    int lb1 = I1;
    int ub1 = min(I1 + m1 - 1, num1 - 1);

    int lb2, ub2, iTmp;
    float fTmp;

    for(int i1 = lb1; i1 <= ub1; i1++)
    {
      // loop boundaries for second FALLS
      fTmp = (static_cast<float>(i1) * str1 + r1 - l2) / str2;
      iTmp = static_cast<int>(fTmp);
      ub2 = min(iTmp, m2 - 1);
      ub2 = min(ub2, num2 - 1);

      lb2 = static_cast<int>(max(static_cast<float>(0), ceil((static_cast<float>(i1) * str1 + l1 - r2) / str2)));
      //ub2 = static_cast<int>(min(floor((static_cast<float>(i1) * str1 + r1 - l2) / str2), m2 - 1));

      if (ub2 >= lb2)
	{
	  retVal += ub2 - lb2 + 1;
	}
    }
    return retVal;
  }//end intersectionSize()
}//end Namspace

