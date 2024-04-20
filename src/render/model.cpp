#include "model.h"

#include <cstring>

#include <stb/stb_image.h>

void Model::loadModel(const std::string& modelName)
{
	// set the extents for the model
	minX = minY = minZ = 1.1754E+38F;
	maxX = maxY = maxZ = -1.1754E+38F;
	// Load geometry
	// Some obj files contain material information.
	// We'll ignore them for this assignment.
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> objMaterials;
	
    std::string errStr;

	bool rc =
		tinyobj::LoadObj(shapes, objMaterials, errStr, (modelDir + modelName + ".obj").c_str(), (modelDir).c_str());

	if (!rc)
	{
		std::cerr << errStr << std::endl;
	}
	else
	{
		if (shapes.size())
		{
			for (size_t i = 0; i < shapes.size(); i++) {
				Mesh sub_mesh;

				sub_mesh.vertices = shapes[i].mesh.positions;
				sub_mesh.indices = shapes[i].mesh.indices;
				sub_mesh.normals = shapes[i].mesh.normals;
				sub_mesh.texturePos = shapes[i].mesh.texcoords;

				meshes.push_back(sub_mesh);
			}
		}
		if (objMaterials.size())
		{
			for (size_t i = 0; i < objMaterials.size(); i++)
			{
				tinyobj::material_t mat = objMaterials[i];

				if (std::strcmp(mat.diffuse_texname.c_str(), "") != 0)
				{
					Texture diff;
					diff.type = "diff";
					diff.path = textureDir + modelName + "/" + objMaterials[i].diffuse_texname;
					diff.ID = loadTexture(diff.path);

					textures_loaded.push_back(diff);
				}
				if (std::strcmp(mat.specular_texname.c_str(), "") != 0)
				{
					Texture spec;
					spec.type = "spec";
					spec.path = textureDir + modelName + "/" + objMaterials[i].specular_texname;
					spec.ID = loadTexture(spec.path);

					textures_loaded.push_back(spec);
				}
				if (std::strcmp(mat.bump_texname.c_str(), "") != 0)
				{
					Texture normal;

					normal.type = "normal";
					normal.path = textureDir + modelName + "/" + objMaterials[i].bump_texname;
					normal.ID = loadTexture(normal.path);

					textures_loaded.push_back(normal);
				}
			}
		}
		/**
		posBuf = shapes[0].mesh.positions;
		norBuf = shapes[0].mesh.normals;
		texBuf = shapes[0].mesh.texcoords;
		eleBuf = shapes[0].mesh.indices;
		*/
	}
}
