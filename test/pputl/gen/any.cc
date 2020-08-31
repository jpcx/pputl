/////                                                                      c++20
////////////////////////////////////////////////////////////////////////////////
/// @file
//                          __    ___
//                         /\ \__/\_ \
//   _____   _____   __  __\ \ ,_\//\ \
//  /\ '__`\/\ '__`\/\ \/\ \\ \ \/ \ \ \
//  \ \ \_\ \ \ \_\ \ \ \_\ \\ \ \_ \_\ \_
//   \ \ ,__/\ \ ,__/\ \____/ \ \__\/\____\
//    \ \ \   \ \ \   \/___/   \/__/\/____/
//     \/_/    \/_/
//
//  pputl Preprocessor Utilities
//  Copyright (C) 2020 Justin Collier <m@jpcx.dev>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the internalied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
////////////////////////////////////////////////////////////////////////////////

#include <cctest/cctest.h>

#include <pputl/gen/any.h>
#include <pputl/stringize.h>

#include "streq.h"

#define STR PPUTL_STRINGIZE

#define SIMPLEGEN(i)      v##i
#define CUSTOMGEN(i, ...) PPUTL_CAT(__VA_ARGS__, i)

using namespace testpputl;

TEST(
    pputl.gen.any,
    "generates args by repeating the result of gen(idx, ...?) [0, 256) times")
    <<
    [] {
      STATIC_CHECK(streq(STR(PPUTL_GEN_ANY(0, SIMPLEGEN)), ""));
      STATIC_CHECK(streq(STR(PPUTL_GEN_ANY(1, SIMPLEGEN)), "v0"));
      STATIC_CHECK(streq(STR(PPUTL_GEN_ANY(2, SIMPLEGEN)), "v0, v1"));
      STATIC_CHECK(streq(STR(PPUTL_GEN_ANY(3, SIMPLEGEN)), "v0, v1, v2"));
      STATIC_CHECK(streq(STR(PPUTL_GEN_ANY(4, SIMPLEGEN)), "v0, v1, v2, v3"));
      STATIC_CHECK(streq(
          STR(PPUTL_GEN_ANY(255, SIMPLEGEN)),
          "v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, "
          "v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, "
          "v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, "
          "v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, "
          "v54, v55, v56, v57, v58, v59, v60, v61, v62, v63, v64, v65, v66, "
          "v67, v68, v69, v70, v71, v72, v73, v74, v75, v76, v77, v78, v79, "
          "v80, v81, v82, v83, v84, v85, v86, v87, v88, v89, v90, v91, v92, "
          "v93, v94, v95, v96, v97, v98, v99, v100, v101, v102, v103, v104, "
          "v105, v106, v107, v108, v109, v110, v111, v112, v113, v114, v115, "
          "v116, v117, v118, v119, v120, v121, v122, v123, v124, v125, v126, "
          "v127, v128, v129, v130, v131, v132, v133, v134, v135, v136, v137, "
          "v138, v139, v140, v141, v142, v143, v144, v145, v146, v147, v148, "
          "v149, v150, v151, v152, v153, v154, v155, v156, v157, v158, v159, "
          "v160, v161, v162, v163, v164, v165, v166, v167, v168, v169, v170, "
          "v171, v172, v173, v174, v175, v176, v177, v178, v179, v180, v181, "
          "v182, v183, v184, v185, v186, v187, v188, v189, v190, v191, v192, "
          "v193, v194, v195, v196, v197, v198, v199, v200, v201, v202, v203, "
          "v204, v205, v206, v207, v208, v209, v210, v211, v212, v213, v214, "
          "v215, v216, v217, v218, v219, v220, v221, v222, v223, v224, v225, "
          "v226, v227, v228, v229, v230, v231, v232, v233, v234, v235, v236, "
          "v237, v238, v239, v240, v241, v242, v243, v244, v245, v246, v247, "
          "v248, v249, v250, v251, v252, v253, v254"));

      STATIC_CHECK(streq(STR(PPUTL_GEN_ANY(0, CUSTOMGEN, _)), ""));
      STATIC_CHECK(streq(STR(PPUTL_GEN_ANY(1, CUSTOMGEN, _)), "_0"));
      STATIC_CHECK(streq(STR(PPUTL_GEN_ANY(2, CUSTOMGEN, _)), "_0, _1"));
      STATIC_CHECK(streq(STR(PPUTL_GEN_ANY(3, CUSTOMGEN, _)), "_0, _1, _2"));
      STATIC_CHECK(
          streq(STR(PPUTL_GEN_ANY(4, CUSTOMGEN, _)), "_0, _1, _2, _3"));
      STATIC_CHECK(streq(
          STR(PPUTL_GEN_ANY(255, CUSTOMGEN, _)),
          "_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, "
          "_15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, "
          "_28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, "
          "_41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, "
          "_54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, "
          "_67, _68, _69, _70, _71, _72, _73, _74, _75, _76, _77, _78, _79, "
          "_80, _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, _91, _92, "
          "_93, _94, _95, _96, _97, _98, _99, _100, _101, _102, _103, _104, "
          "_105, _106, _107, _108, _109, _110, _111, _112, _113, _114, _115, "
          "_116, _117, _118, _119, _120, _121, _122, _123, _124, _125, _126, "
          "_127, _128, _129, _130, _131, _132, _133, _134, _135, _136, _137, "
          "_138, _139, _140, _141, _142, _143, _144, _145, _146, _147, _148, "
          "_149, _150, _151, _152, _153, _154, _155, _156, _157, _158, _159, "
          "_160, _161, _162, _163, _164, _165, _166, _167, _168, _169, _170, "
          "_171, _172, _173, _174, _175, _176, _177, _178, _179, _180, _181, "
          "_182, _183, _184, _185, _186, _187, _188, _189, _190, _191, _192, "
          "_193, _194, _195, _196, _197, _198, _199, _200, _201, _202, _203, "
          "_204, _205, _206, _207, _208, _209, _210, _211, _212, _213, _214, "
          "_215, _216, _217, _218, _219, _220, _221, _222, _223, _224, _225, "
          "_226, _227, _228, _229, _230, _231, _232, _233, _234, _235, _236, "
          "_237, _238, _239, _240, _241, _242, _243, _244, _245, _246, _247, "
          "_248, _249, _250, _251, _252, _253, _254"));
    };
