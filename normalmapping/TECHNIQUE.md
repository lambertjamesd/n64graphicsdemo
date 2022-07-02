# Normal mapping 

Normal mapping should be possible, in theory, on the N64 but the framerate would be garbage and you may need to sacrifice
brightness range

Here are the steps. First render the scene, with no lighting, to a color buffer, next render just the light intensities
to a separate I8 image buffer. The intensity color buffer will have the origin at 128 instead of 0. Then combine the two separate buffers using the following color combiner.

### cycle1
```
PRIMTIVE = (128, 128, 128, 255)
(I8_TEXEL - PRIMTIVE) * PRIMTIVE_ALPHA + 0
```
### cycle 2
```
(COMBINED - 0) * COLOR_TEXEL + 0
```

Once that is setup, the trick to getting normal mapping working is doing 6 different passes when rendering the intensity color buffer, 2 for each axis.

Each pass will use a different normal for the geometry and each axis will use a different texture. The passes are combined using additive blending.

| pass | normal direction | texture      | color combine                 |
|------|------------------|--------------|-------------------------------|
| +x   | tangent0         | normal map r | (TEXEL0 - 42) * SHADE         |
| -x   | -tangent0        | normal map r | (42 - TEXEL0) * SHADE         |
| +y   | tangent1         | normal map g | (TEXEL0 - 42) * SHADE         |
| -y   | -tangent1        | normal map g | (42 - TEXEL0) * SHADE         |
| +z   | normal           | normal map b | (TEXEL0 - 42) * SHADE         |
| -z   | -normal          | normal map b | (42 - TEXEL0) * SHADE         |

Any constant values in the color combiner can be configured using the evironment color.