// setup scene
// setup camera
// init image

render(shape[] scene, camera camera, backbuffer image)
{
    shape[] unsplit_shapes  = scene
    shape[] split_shapes    = empty
    grid[] uGrids           = empty
    grid[] shadedGrids      = empty

    // BOUND-SPLIT
    // get all shapes under some limit size
    while (unsplit_shapes)                              // if there is any big shape
        shape = unsplit_shapes.pop()                    // get next shape from list
        grid = shape.dice(8,8)                          // coarse uGrid
        grid = grid.project(camera)                     // project uGrid

        if( grid.visible(camera) )                      // try to cull completely

            vec2 rasterSize = grid.rasterEstimate()     // estimate rasterized size
            if ( rasterSize > rasterThreshold )         // 1. too big, needs splitting
                SplitDir dir = split_dir(rasterSize)    // determine bigger dimension
                Shape halves[2]
                shape.split(dir, halves)                // split
                unsplit_shapes.push(halves[0])
                unsplit_shapes.push(halves[1])
            else                                        // 2. small enough
                split_shapes.push(shape)

    // DICE
    // get all small shapes and dice them
    while (split_shapes)
        shape = split_shapes.pop()                      // get shape
        grid = shape.dice(8,8)                          // dice it
        uGrids.push(grid)                               // push the data

    // SHADE
    // run shader on grid points
    while (uGrids)
        grid = grids.pop()                              // get grid
        shadedGrid = grid.shade()                       // shade points in it
        shadedGrids.push(shadedGrid)                    // pust the data

    // SAMPLE
    // sample shaded points and get samples to be injected into a color buffer
    while (uGrids)
        grid = shadedGrids.pop()                        // get the shaded grid
        foreach quadrilateral in grid
            image.rasterize(quadrilateral)              // rasterize and inject into buffer

    // can combine dice and shade and sample
}

// display image