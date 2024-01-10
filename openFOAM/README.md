# OpenFOAM

This submission uses the tool *OpenFOAM* to generate different simulation scenarios


## Scenario 1a: Lid Driven Cavity

This solves the lid driven cavity problem similar to the first submission in this lecture. This serves as a initial plausibilization with OpenFOAM.

### Geometry

The lid driven cavity consists of a quadratic cavity with dimensions of 2m. The top side of the cavity moves with a fixed speed of 1 $\frac{m}{s}$. 

![Task for Step Description](cavity/media/cavity.png)

### Parameters

|parameter | value |
|----------|-------|
|$\nu$        | 0.002 |
|$R_{e}$        | 1000 |
|$\Delta t$    | 0.005|
|solver    |icoFoam|

### Results

#### Velocity U

##### Start: t = 0.01
![u_0](cavity/media/u_0.png)
![u_0_trace](cavity/media/u_0_trace.png)


##### End: t = 10 
![u_10](cavity/media/u_10.png)
![u_10_trace](cavity/media/u_10_trace.png)



#### Pressure P

##### Start: t = 0.01
![p_0](cavity/media/p_0.png)
![p_0_trace](cavity/media/p_0_trace.png)


##### End: t = 10 
![p_10](cavity/media/p_10.png)
![p_10_trace](cavity/media/p_10_trace.png)


## Scenario 1b: Heated Lid Driven Cavity

This solves the lid driven cavity problem with an additional temperature difference between bottom and lid of the cavity. The Bottom is at 600K, the lid at 300K.

### Geometry

Same as lid driven cavity without heat 

### Parameters

|parameter | value |
|----------|-------|
|$\nu$        | 0.002 |
|$R_{e}$        | 1000 |
|$\beta$        | 3e-03 |
|$T_{Ref}$    | 300|
|$P_{r}$    | 0.7|
|$P_{rt}$    | 0.85|
|$\Delta t$    | 0.005|
|solver    |buoyantBoussinesqPimpleFoam|

### Results

#### Velocity U

##### Start: t = 0.01
![u_0](heatedCavity/media/u_0.png)
![u_0_trace](heatedCavity/media/u_0_trace.png)


##### End: t = 10 
![u_10](heatedCavity/media/u_10.png)
![u_10_trace](heatedCavity/media/u_10_trace.png)



#### Pressure P

##### Start: t = 0.01
![p_0](heatedCavity/media/p_0.png)
![p_0_trace](heatedCavity/media/p_0_trace.png)


##### End: t = 10 
![p_10](heatedCavity/media/p_10.png)
![p_10_trace](heatedCavity/media/p_10_trace.png)

#### Temperature T

##### Start: t = 0.01
![T_0](heatedCavity/media/T_0.png)
![T_0_trace](heatedCavity/media/T_0_trace.png)


##### End: t = 10 
![T_10](heatedCavity/media/T_10.png)
![T_10_trace](heatedCavity/media/T_10_trace.png)

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
|nu        | 0.005 |
|deltaT    | 0.0005|
|write Interval | 20|
|solver    |icoFoam|

as mentioned below, this value for the dynamic viscosity $\nu$ corresponds to $\text{Re} = 1000$ 

### Process

1. `blockMesh`
2. `icoFoam`
3. `paraFoam`

### Resolution

The problem was solved for different resolutions: 
* 1 Unit = 10 Nodes -> 225 Cells 
  * upper left square I is 5x5
  * took 2.3s
* 1 Unit = 20 Nodes -> 900 Cells
  * upper left square I is 10x10
  * took 4.68s
* 1 Unit = 100 Nodes -> 22 500 Cells 
  * upper left square I is 50x50
  * took 337s

### Results

##### Note: 
Unfortunately some of the plots colorbar uses white font, so white background makes it invisble. See accompanying pdf or change background in editor.


#### Flow U

##### Start Value t = 0.01
![t=0.01](cavityChannelWithStep/media/step_uMag_t0_fine.png)


##### End Value t = 2 
![t=2](cavityChannelWithStep/media/step_uMag_t2_fine.png)


#### Pressure p for t = 2
![p](cavityChannelWithStep/media/step_p_tEnd_fine.png)


#### Streamlines for t = 2
![Streamline](cavityChannelWithStep/media/step_uMag_Stream_fine.png)


#### Glyphs (Arrows) for t = 2
![Glyph](cavityChannelWithStep/media/step_uMag_Glyph_fine.png)


#### Low resolution 3D Visualization

![3D end](cavityChannelWithStep/media/step_uMag_tEnd.png)

### Different values for $\nu$

$$ \text{Re} = \frac{v d}{\nu} $$

with $d = 5$ characteristic length, $v = 1$

We can observe how the length of the vortex varies when adapting $\nu$.


#### $\nu = 0.5 \to \text{Re} = 10$
![nu=0.5](cavityChannelWithStep/media/step_5e-1_Stream.png)


#### $\nu = 0.005 \to \text{Re} = 1000$
![nu=0.005](cavityChannelWithStep/media/step_5e-3_Stream.png)



#### $\nu = 0.0001 \to \text{Re} = 50000$

##### $t = 0.6$
![nu=1e-4 time = 0.6](cavityChannelWithStep/media/step_1e-4_Stream_t06.png)

##### $t = 1.0$
![nu=1e-4 time = 1.0](cavityChannelWithStep/media/step_1e-4_Stream_t10.png)

##### $t = 10.0$
![nu=1e-4 time = 10.0](cavityChannelWithStep/media/step_1e-4_Stream_t99.png)


 


