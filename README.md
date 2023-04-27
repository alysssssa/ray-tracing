## Ray Tracing

Learning from *Ray Tracing in One Weekend* in many many weekends

https://raytracing.github.io/

> In graphics, you almost always want your formulas to be in terms of vectors so all the x/y/z stuff is under the hood in the vec3 class.


#### ⭐️ Design decision: normal direction
To determine which side of the surface the ray is coming from - important for objects rendered differently on each side
* normal always points out
  - will need to determine which side the ray is on when we colour it
  - by comparing the ray with the normal -> taking the dot product
    - in the same direction -> the ray is inside the object -> dot product is positive
    - in the opposite direction -> the ray is outside the object -> dot product is negative
* normal always points against the ray
  - need to store which side of the surface the ray is on
  
This decision is determined by whether you want to determine the side of the surface at the time of geometry intersection or at the time of coloring. In this book we have more material types than we have geometry types, so we'll go for less work and put the determination at geometry time. This is simply a matter of preference, and you'll see both implementations in the literature.

> One question is whether we mix and match geometry and materials (so we can assign a material to multiple spheres, or vice versa) or if geometry and material are tightly bound (that could be useful for procedural objects where the geometry and material are linked). We’ll go with separate — which is usual in most renderers — but do be aware of the limitation.

Generating a random diffuse bounce ray:
There are two unit radius spheres tangent to the hit point 𝑝 of a surface. These two spheres have a center of (𝐏+𝐧) and (𝐏−𝐧), where 𝐧 is the normal of the surface. The sphere with a center at (𝐏−𝐧) is considered inside the surface, whereas the sphere with center (𝐏+𝐧) is considered outside the surface. Select the tangent unit radius sphere that is on the same side of the surface as the ray origin...


#### ⭐️ Design decision: material 
For different objects to have different materials
* a universal material with lots of parameters and different material types just zero out some of those parameters
* an abstract material class that encapsulates behaviour

Clear materials such as water, glass, and diamonds are dielectrics. When a light ray hits them, it splits into a reflected ray and a refracted (transmitted) ray. We’ll handle that by randomly choosing between reflection or refraction, and only generating one scattered ray per interaction.
