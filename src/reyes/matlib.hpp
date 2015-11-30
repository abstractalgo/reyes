//#pragma once
//
//#include "material.hpp"
//
//namespace reyes
//{
//    namespace matlib
//    {
//        struct Lambert : public Material<PosNormal, void>
//        {
//            position pShdr()
//            {
//                return{ 0, 0, 0 };
//            }
//
//            color cShdr()
//            {
//                return{ 0, 0, 0, 1 };
//            }
//        };
//
//        struct SolidColor : public Material<Pos>
//        {
//            position pShdr()
//            {
//                return{ 0, 0, 0 };
//            }
//
//            color cShdr()
//            {
//                return{ 1, vertex.x, 0, 1 };
//            }
//        };
//    }
//}