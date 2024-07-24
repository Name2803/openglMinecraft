#pragma once

#define CHUNK_W 16
#define CHUNK_H 16
#define CHUNK_D 16
#define CHUNK_VOL (CHUNK_W * CHUNK_H * CHUNK_D)

class voxel;
class Shader;

class Chunk {
public:
	int x, y, z;
	bool modified = true;
	voxel* voxels;
	Chunk(int xposs, int ypos, int zpos);
	~Chunk();

	int ChunkBoxDrawer(bool shouldToDraw, Shader& chunkBoxShader);

private:
    const float l_cube_vert[144] = {
        //vertices      //color
          0.f, 0.f, 0.f,  1.f, 1.f, 1.f,
          0.f, 0.f, 1.f,  0.f, 1.f, 0.f,
          0.f, 0.f, 1.f,  0.f, 1.f, 0.f,
          1.f, 0.f, 1.f,  0.f, 1.f, 1.f,

          1.f, 0.f, 1.f,  0.f, 1.f, 1.f,
          1.f, 0.f, 0.f,  0.f, 0.f, 1.f,
          1.f, 0.f, 0.f,  0.f, 0.f, 1.f,
          0.f, 0.f, 0.f,  1.f, 1.f, 1.f,

          0.f, 0.f, 0.f,  1.f, 1.f, 1.f,
          0.f, 1.f, 0.f,  1.f, 0.f, 0.f,
          0.f, 1.f, 0.f,  1.f, 0.f, 0.f,
          0.f, 1.f, 1.f,  1.f, 1.f, 0.f,

          0.f, 0.f, 1.f,  0.f, 1.f, 0.f,
          0.f, 1.f, 1.f,  1.f, 1.f, 0.f,
          0.f, 1.f, 1.f,  1.f, 1.f, 0.f,
          1.f, 1.f, 1.f,  1.f, 1.f, 1.f,

          1.f, 0.f, 1.f,  0.f, 1.f, 1.f,
          1.f, 1.f, 1.f,  1.f, 1.f, 1.f,
          1.f, 1.f, 1.f,  1.f, 1.f, 1.f,
          1.f, 1.f, 0.f,  1.f, 0.f, 1.f,

          1.f, 0.f, 0.f,  0.f, 0.f, 1.f,
          1.f, 1.f, 0.f,  1.f, 0.f, 1.f,
          1.f, 1.f, 0.f,  1.f, 0.f, 1.f,
          0.f, 1.f, 0.f,  1.f, 0.f, 0.f
    };
};