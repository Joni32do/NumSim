# OpenFOAM

This submission uses the tool *OpenFOAM* to generate different simulation scenarios


## Scenario 1: Lid Driven Cavity

This solves the lid driven cavity problem similar to the first submission in this lecture. This serves as a initial plausibilization with OpenFOAM.


## Scenario 2: Cavity with a Step

We inspect a different domain with an inflow and outflow condition at the left and right boundary, see following image

![Task for Step Description](cavityChannelWithStep/media/step_description.svg)

### Geometry

Therefore we need to define in `blockMeshDict` 8 vertices (with additional 8 vertices since in openFoam everything is 3D). 
Then the three blocks I, II and III are defined with these vertices. Similar the boundary faces are defined. On the left
there is an inflow BC and on the right there is a outflow BC. They are defined in the initial value file `0\p` and `0\U`.


#### Inflow
For `U` is of type `fixedValue` with value `uniform (1 0 0)` 
For `p` of type `zeroGradient`

#### Outflow
For `U` the type is `zeroGradient` 
For `p` of type `fixedValue` with value $0$


### Parameters

|parameter | value |
|----------|-------|
|deltaT    | 0.0005|
|write Interval | 20|
|solver    |icoFoam|

### Process

1. `blockMesh`
2. `icoFoam`
3. `paraFoam`

### Resolution

The problem was solved for different resolutions: 
* 1 Unit = 10 Nodes -> 225 Cells (upper left square I is 5x5)
* 1 Unit = 100 Nodes -> 22 500 Cells (upper left square I is 50x50)

### Results

#### Flow U

##### Start Value t = 0
<img src="cavityChannelWithStep/media/step_uMag_t0_fine.png" alt="t = 0" width="1000"/>
<!-- <img src="cavityChannelWithStep/media/step_uMag_t1_fine.png" alt="t = 1" width="1000"/> -->

##### End Value t = 2 
<img src="cavityChannelWithStep/media/step_uMag_t2_fine.png" alt="t = 2" width="1000"/>


#### Pressure p
<img src="cavityChannelWithStep/media/step_p_tEnd_fine.png" alt="p" width="1000"/>


#### Streamlines
<img src="cavityChannelWithStep/media/step_uMag_Stream_fine.png" alt="Streamline" width="1000"/>


#### Glyphs (Arrows)
<img src="cavityChannelWithStep/media/step_uMag_Glyph_fine.png" alt="Glyph" width="1000"/>



#### Low resolution 3D vis

<img src="cavityChannelWithStep/media/step_3D_init.png" alt="step_3D_init" width="400"/>
<img src="cavityChannelWithStep/media/step_uMag_tEnd.png" alt="step_3D_uMag_tEnd" width="400"/>

 


