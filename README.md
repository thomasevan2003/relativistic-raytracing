# Relativistic Raytracing
Shader visualization of gravitational lensing around a black hole, written in C++.

## Numerical Integration of a Geodesic

The Schwarzschild metric in coordinates $(t,r,\theta,\phi)$ with $c=1$ is  

![0](./equations/0.png)

which gives the following metric:

![1](./equations/1.png)

The Christoffel symbol is found using

![2](./equations/2.png)

which has the following nonzero elements for the Schwarzschild metric:

![3](./equations/3.png)


![4](./equations/4.png)


![5](./equations/5.png)


![6](./equations/6.png)


![7](./equations/7.png)


![8](./equations/8.png)


![9](./equations/9.png)


![10](./equations/10.png)


![11](./equations/11.png)

Using the Christoffel symbol, the governing differential equation for a geodesic in the Schwarzschild metric is

![12](./equations/12.png)

with the null condition for a photon:

![13](./equations/13.png)

The null condition will be applied at the beginning of a geodesic based on the direction associated with a given pixel: $(d_x, d_y, d_z)$. The camera is in nearly flat space, so the null condition in $(t,x,y,z)$ coordinates simplifies to

![14](./equations/14.png)

Converting from Cartesian $x_\mathrm{c}=(t,x,y,z)$ back to Schwarzschild coordinates $x=(t,r,\theta,\phi)$,

![15](./equations/15.png)


![16](./equations/16.png)


![17](./equations/17.png)


![18](./equations/18.png)


![19](./equations/19.png)


The full solution process is as follows:  
1. Compute initial 4-velocity in Cartesian coordinates using 3-velocity from screenspace position.
2. Convert initial 4-velocity from Cartesian coordinates to Schwarzschild coordinates.
3. Integrate geodesic numerically until exit condition.
4. Project final 3-velocity onto spherical starmap.

## Dependencies
- Dear ImGui
- glfw  
- glad (generated using core gl version 3.3 with loader on)  
- OpenGL

## Resources
The starmap used in this visualization is the Tycho Catalog Skymap Version 2.0, obtained from NASA/Goddard Space Flight Center Scientific Visualization Studio (https://svs.gsfc.nasa.gov/3572).