#include "Chunk.h"
#include "voxel.h"
#include "../graphics/Mesh.h"
#include "../graphics/ShaderCl.h"
#include "../lighting/Lightmap.h"

#include <math.h>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

Chunk::Chunk(int xpos, int ypos, int zpos):
	x(xpos), y(ypos), z(zpos)
{
	voxels = new voxel[CHUNK_VOL];
	lightmap = new Lightmap();
	for (int z = 0; z < CHUNK_D; z++) {
		for (int x = 0; x < CHUNK_W; x++) {
			for (int y = 0; y < CHUNK_H; y++) {
				int real_x = x + xpos * CHUNK_W;
				int real_z = z + zpos * CHUNK_D;
				int real_y = y + ypos * CHUNK_H;
				float height = glm::perlin(glm::vec3(real_x * 0.07f, real_z * 0.07f, real_y * 0.07f)) * 10 + 5;
				int id = glm::perlin(glm::vec3(real_x * 0.0325f, real_y * 0.0625f, real_z * 0.0325f)) > 0.1f;
				
				if (id)
				{
					voxels[(y * CHUNK_D + z) * CHUNK_W + x].id = 199;
				}
				else if (real_y <= 4)
				{
					voxels[(y * CHUNK_D + z) * CHUNK_W + x].id = 198;
				}
				else
				{
					voxels[(y * CHUNK_D + z) * CHUNK_W + x].id = 0;
					continue;
				}
			}
		}
	}
}

Chunk::~Chunk() {
	delete[] voxels;
	delete lightmap;
}

int Chunk::ChunkBoxDrawer(bool shouldToDraw, Shader& chunkBoxShader)
{
	if (shouldToDraw)
	{
		int arts[] = { 3, 3, 0 };
		Mesh chunkBox(l_cube_vert, 24, arts);
		glm::mat4 model = glm::mat4(1.f);
		model = glm::mat4(1.f);
		model = scale(model, glm::vec3(CHUNK_W, CHUNK_H, CHUNK_D));
		model = translate(model, glm::vec3(x, y, z));
		chunkBoxShader.setMat4("model", model);
		chunkBox.draw(GL_LINES);
	}

	return 0;
}
