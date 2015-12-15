//#pragma once
//
//#include "shape.hpp"
//#include "vecmx.hpp"
//
//namespace reyes
//{
//    template<class MaterialTy>
//    struct ParametricSurface : public Shape<MaterialTy>
//    {
//        float start_u, end_u, start_v, end_v;
//        ParametricSurface()
//            : start_u(0)
//            , end_u(1)
//            , start_v(0)
//            , end_v(1)
//        {}
//
//        void splitUV(SplitDir direction, ParametricSurface<MaterialTy>& one, ParametricSurface<MaterialTy>& two)
//        {
//            if (U == direction)
//            {
//                one.start_u = start_u;
//                one.end_u = end_u;
//                one.start_v = start_v;
//                one.end_v = (start_v + end_v)*0.5f;
//
//                two.start_u = start_u;
//                two.end_u = end_u;
//                two.start_v = (start_v + end_v)*0.5f;
//                two.end_v = end_v;
//            }
//            else
//            {
//                one.start_u = start_u;
//                one.end_u = (start_u + end_u)*0.5f;
//                one.start_v = start_v;
//                one.end_v = end_v;
//
//                two.start_u = (start_u + end_u)*0.5f;
//                two.end_u = end_u;
//                two.start_v = start_v;
//                two.end_v = end_v;
//            }
//        }
//
//        virtual void split(SplitDir direction, Scene& scene) = 0;
//
//        mem::blk dice(CameraTransform* camera, mem::AllocatorI* alloc)
//        {
//            // grid (quad 8x8)
//            mem::blk grid_blk = alloc->alloc(sizeof(GQuadGrid<64, 196>));
//            GQuadGrid<64, 196>& grid = *(::new(grid_blk.ptr) GQuadGrid<64, 196>);
//
//            // vertices
//            for (uint16_t u = 0; u < 8; u++)
//            for (uint16_t v = 0; v < 8; v++)
//            {
//                float w = 1.0f / 7.0f;
//                uv _uv = uv(start_u + u*w * (end_u - start_u), start_v + v*w * (end_v - start_v));
//                uint16_t idx = v * 8 + u;
//                grid.data[idx].p = P(_uv);
//                grid.data[idx].n = N(_uv);
//                grid.data[idx].uv = _uv;
//                grid.data[idx].p = material.pShdr(grid.data[idx]);
//                // TODO transformations: model, view, projection
//            }
//
//            // indices
//            for (uint16_t v = 0; v < 7; v++)
//            for (uint16_t u = 0; u < 7; u++)
//            {
//                grid.indices[v * 28 + u * 4 + 0] = v * 8 + u;
//                grid.indices[v * 28 + u * 4 + 1] = v * 8 + u + 1;
//                grid.indices[v * 28 + u * 4 + 2] = v * 8 + 8 + u + 1;
//                grid.indices[v * 28 + u * 4 + 3] = v * 8 + 8 + u;
//            }
//            return grid_blk;
//        }
//        mem::blk shade(MicrogridI<PosNormalUV>* dgrid, mem::AllocatorI* alloc)
//        {
//            GQuadGrid<64, 196>& grid = *(GQuadGrid<64, 196>*)(dgrid);
//
//            // color grid
//            mem::blk colg_blk = alloc->alloc(sizeof(SQuadGrid<64, 196>));
//            SQuadGrid<64, 196>& color_grid = *(::new(colg_blk.ptr) SQuadGrid<64, 196>);
//
//            // data
//            for (uint16_t i = 0; i < 64; i++)
//            {
//                color_grid.data[i].col = material.cShdr(grid.data[i]);
//                color_grid.data[i].p = grid.data[i].p;
//            }
//            // indices
//            for (uint16_t i = 0; i < 196; i++)
//                color_grid.indices[i] = grid.indices[i];
//
//            return colg_blk;
//        }
//        
//        position P(uint16_t idx) { return{ 1, 1, 1 }; }
//        normal N(uint16_t idx) { return{ 1, 1, 1 }; }
//        uv UV(uint16_t idx) { return{ 0, 0 }; }
//
//        virtual normal N(uv uv) = 0;
//        virtual position P(uv uv) = 0;
//
//        void renderOGL(uint16_t ur, uint16_t vr)
//        {
//            float ru = 1.0f / ur;
//            float rv = 1.0f / vr;
//            glBegin(GL_LINES);
//            for (uint16_t i = 0; i < ur + 1; i++)
//            {
//                for (uint16_t j = 0; j < vr + 1; j++)
//                {
//                    uv a, b, c, d;
//                    a = uv(ru*i, rv*j);
//                    b = uv(ru*(i + 1), rv*j);
//                    c = uv(ru*(i + 1), rv*(j + 1));
//                    d = uv(ru*i, rv*(j + 1));
//
//                    vec3 pa, pb, pc, pd;
//                    pa = this->P(a);
//                    pb = this->P(b);
//                    pc = this->P(c);
//                    pd = this->P(d);
//
//                    glVertex3f(pa.x, pa.y, pa.z);
//                    glVertex3f(pd.x, pd.y, pd.z);
//                    glVertex3f(pb.x, pb.y, pb.z);
//
//                    glVertex3f(pb.x, pb.y, pb.z);
//                    glVertex3f(pd.x, pd.y, pd.z);
//                    glVertex3f(pc.x, pc.y, pc.z);
//                }
//            }
//            glEnd();
//            glFlush();
//        }
//    };
//}