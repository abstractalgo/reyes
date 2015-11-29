#include <cstdio>
#include "pipeline.hpp"
#include "material.hpp"

using namespace reyes;

void appMain()
{
    // scene setup
    //mem::ObjectStack<Shape> scene(1024);
    //Rectangle* r = new(scene.alloc(sizeof(Rectangle))) Rectangle({ 0, 0, 0 }, { 5, 5 });

    //// camera setup
    //camera camera;
    //camera.orthographic(-10, 10, 10, 10);
    //camera.lookAt(/*eye*/ { 0, 0, -5 }, /*target*/ { 0, 0, 0 } /*up*/);

    //// image setup
    //GBuffer image(10,10);

    /*material* lambertMat;
    material_factory::make<LambertianShader>(lambertMat);
    color a = lambertMat->shade({ 0, 0, 0 }, {1,1,1});*/

    LambertShader& lambshdr = *new LambertShader;
    SolidColorShader& scshdr = *new SolidColorShader;

    GridI<PosNormalMat>* gridA = new TriGrid<PosNormalMat>;
    GridI<position>* gridB; // from gridA
    GridI<PosColor>* gridC = new TriGrid<PosColor>;

    // render
    //reyes::render(scene, camera, image);
}