# Relativistic Raytracing
Shader visualization of gravitational lensing around a black hole, written in C++.

## Numerical Integration of a Geodesic

The Schwarzschild metric in coordinates $(t,r,\theta,\phi)$ with $c=1$ is  
$ds^2=-\left(1-\frac{R_s}{r}\right)dt^2+\left(1-\frac{R_s}{r}\right)^{-1}dr^2+r^2 d\theta^2+r^2\sin^2 \theta d\phi^2$
which gives the following metric:
$$
g_{\mu\nu}=\begin{bmatrix}-\left(1-\frac{R_s}{r}\right)&0&0&0\\0&\left(1-\frac{R_s}{r}\right)^{-1}&0&0\\0&0&r^2&0\\0&0&0&r^2\sin^2\theta\end{bmatrix}
$$
The Christoffel symbol is found using
$$
\Gamma^{\lambda}_{\mu\nu}=\frac{1}{2}g^{\lambda\sigma}\left(\partial_\mu g_{\nu\sigma}+\partial_\nu g_{\sigma\mu}-\partial_\sigma g_{\mu\nu}\right)
$$
which has the following nonzero elements for the Schwarzschild metric:
$$
\Gamma^t_{tr}=\Gamma^t_{rt}=-\frac{R_s}{2r\left(R_s-r\right)}
$$
$$
\Gamma^r_{tt}=-\frac{R_s\left(R_s-r\right)}{2r^3}
$$
$$
\Gamma^r_{rr}=\frac{R_s}{2r\left(R_s-r\right)}
$$
$$
\Gamma^r_{\theta\theta}=R_s-r
$$
$$
\Gamma^r_{\phi\phi}=\sin^2\left(\theta\right)\left(R_s-r\right)
$$
$$
\Gamma^{\theta}_{\theta r}=\Gamma^{\theta}_{r\theta}=\frac{1}{r}
$$
$$
\Gamma^{\theta}_{\phi\phi}=-\frac{\sin(2\theta)}{2}
$$
$$
\Gamma^{\phi}_{\phi r}=\Gamma^{\phi}_{r\phi}=\frac{1}{r}
$$
$$
\Gamma^{\phi}_{\phi\theta}=\Gamma^{\phi}_{\theta\phi}=\frac{\cos\left(\theta\right)}{\sin\left(\theta\right)}
$$
Using the Christoffel symbol, the governing differential equation for a geodesic is
$$
\frac{d^2x^\mu}{d\lambda^2}=-\Gamma^{\mu}_{\rho\sigma}\frac{dx^\rho}{d\lambda}\frac{dx^\sigma}{d\lambda}
$$
with the null condition for a photon:
$$
g_{\mu\nu}\frac{dx^\mu}{d\lambda}\frac{dx^\nu}{d\lambda}=0
$$
The null condition will be applied at the beginning of a geodesic based on the direction associated with a given pixel: $(d_x, d_y, d_z)$. The camera is in nearly flat space, so the null condition in $(t,x,y,z)$ coordinates simplifies to
$$
\frac{dt}{d\lambda}=\sqrt{\left(\frac{dx}{d\lambda}\right)^2+\left(\frac{dy}{d\lambda}\right)^2+\left(\frac{dz}{d\lambda}\right)^2}=\sqrt{d_x^2+d_y^2+d_z^2}
$$
Converting from Cartesian $x_\mathrm{c}=(t,x,y,z)$ back to Schwarzschild coordinates $x=(t,r,\theta,\phi)$,
$$
r=\sqrt(x^2+y^2+z^2)
$$
$$
\theta=\mathrm{acos}\left(\frac{z}{r}\right)
$$
$$
\phi=\mathrm{atan2}\left(y,x\right)
$$
$$
\frac{dx^\nu}{d\lambda}=\frac{dx^\mu}{dx_c^\nu}\frac{dx_c^\nu}{d\lambda}
$$
$$
=\begin{bmatrix}1&0&0&0\\
0&\frac{x}{r}&\frac{y}{r}&\frac{z}{r}\\
0&\frac{xz}{r^2\sqrt{x^2+y^2}}&\frac{yz}{r^2\sqrt{x^2+y^2}}&-\frac{\sqrt{x^2+y^2}}{r^2}\\0&-\frac{y}{x^2+y^2}&\frac{x}{x^2+y^2}&0\end{bmatrix}
\begin{bmatrix}\frac{dt}{d\lambda}\\\frac{dx}{d\lambda}\\\frac{dy}{d\lambda}\\\frac{dz}{d\lambda}\end{bmatrix}
=\begin{bmatrix}t\\\frac{x\frac{dx}{d\lambda}+y\frac{dy}{d\lambda}+z\frac{dz}{d\lambda}}{r}\\
-\frac{x^2\frac{dz}{d\lambda}-xz\frac{dx}{d\lambda}+y^2\frac{dz}{d\lambda}-yz\frac{dy}{d\lambda}}{r^2\sqrt{x^2+y^2}}\\
\frac{x\frac{dy}{d\lambda}-y\frac{dx}{d\lambda}}{x^2+y^2}\end{bmatrix}
$$

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