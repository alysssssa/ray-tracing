## Ray Tracing

Learning from *Ray Tracing in One Weekend* in many many weekends

https://raytracing.github.io/

<img src="glass-in-cornell-box.png" width="500">

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


#### 📕 Bounding Volume Hierarchies
The ray-object intersection is the main time-bottleneck in a ray tracer, and the time is linear with the number of objects. But it’s a repeated search on the same model, so we ought to be able to make it a logarithmic search in the spirit of binary search. Because we are sending millions to billions of rays on the same model, we can do an analog of sorting the model, and then each ray intersection can be a sublinear search. The two most common families of sorting are to 1) divide the space, and 2) divide the objects. The latter is usually much easier to code up and just as fast to run for most models.


#### 📓 Perlin Noise
> A key part of Perlin noise is that it is repeatable: it takes a 3D point as input and always returns the same randomish number. Nearby points return similar numbers. Another important part of Perlin noise is that it be simple and fast, so it’s usually done as a hack.

#### 🌎 Image Texture
Here's the code to read an image from a file and then assign it to a diffuse material:
```
hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    return hittable_list(globe);
}
```
> We start to see some of the power of all colors being textures — we can assign any kind of texture to the lambertian material, and lambertian doesn’t need to be aware of it.

#### 💫 Instances
`Now that we have boxes, we need to rotate them a bit to have them match the real Cornell box. In ray tracing, this is usually done with an instance. An instance is a geometric primitive that has been moved or rotated somehow. This is especially easy in ray tracing because we don’t move anything; instead we move the rays in the opposite direction. For example, consider a translation (often called a move). We could take the pink box at the origin and add 2 to all its x components, or (as we almost always do in ray tracing) leave the box where it is, but in its hit routine subtract 2 off the x-component of the ray origin.`

#### 😶‍🌫️ Volumes
One thing it’s nice to add to a ray tracer is smoke/fog/mist. These are sometimes called volumes or participating media. Another feature that is nice to add is subsurface scattering, which is sort of like dense fog inside an object. This usually adds software architectural mayhem because volumes are a different animal than surfaces, but a cute technique is to make a volume a random surface. A bunch of smoke can be replaced with a surface that probabilistically might or might not be there at every point in the volume.
