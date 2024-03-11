### 3D Shapes Library

This repository contains a collection of 3D shapes implemented for use in OpenGL or DirectX applications. These shapes are designed to facilitate geometry rendering and manipulation in 3D graphics projects.

#### Shapes Included:

1. *Sphere*: A spherical shape resembling a ball.
2. *HemiSphere*: Half of a sphere, representing a dome-like structure.
3. *Cylinder*: A cylindrical shape with circular bases.
4. *Cone*: A conical shape tapering to a point.
5. *Disk*: A flat, circular shape resembling a disc.
6. *Torus*: A toroidal shape resembling a doughnut.
7. *Cube*: A six-sided shape with square faces.
8. *Pyramid*: A polyhedron with a polygonal base and triangular faces meeting at a common vertex.

#### Data Provided:

For each shape, the following data is available:

1. *Vertices*: Coordinates of the vertices defining the shape's geometry.
2. *Texcoord*: Texture coordinates for mapping textures onto the shape's surface.
3. *Normals*: Normal vectors indicating the direction perpendicular to the surface at each vertex, used for shading and lighting calculations.

#### Usage:

To use these shapes in your OpenGL or DirectX project, simply include the necessary files from this repository and integrate the provided data into your rendering pipeline. You can manipulate and render these shapes according to your application's requirements, leveraging their geometric properties for various visual effects and simulations.

#### Example Code:

cpp
// Example code snippet demonstrating how to render a sphere using OpenGL

void renderSphere() {
    // Load sphere vertices, texcoords, and normals from the provided data
    // Bind and enable vertex and texture coordinate buffers
    // Set up shaders, textures, and other rendering parameters
    // Render the sphere using OpenGL commands
}

int main(int argc, char** argv) {
    return 0;
}


#### Contribution:

Contributions to this repository are welcome! If you have additional shapes, optimizations, or improvements to suggest, feel free to submit a pull request or open an issue to discuss your ideas.

#### License:

This library is provided under the [MIT License](LICENSE), allowing for unrestricted use, modification, and distribution. Refer to the license file for more details.

For any questions, feedback, or inquiries, please contact [Maintainer Name](mailto:email@example.com).

Happy coding! 🌟
