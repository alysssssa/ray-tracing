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
