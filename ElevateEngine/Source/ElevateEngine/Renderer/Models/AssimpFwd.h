#pragma once

// Opaque Assimp declarations for module interfaces.  Including the real Assimp
// headers in a module BMI exposes TU-local inline helpers on GCC.
struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;
