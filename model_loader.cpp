#include "model_loader.h"

MeshData loadModel(std::string filename) {
	MeshData data;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals); // | aiProcess_FixInfacingNormals
	std::cout << importer.GetErrorString() << std::endl;

	if (!scene->HasMeshes()) return data;
	std::cout << "Loading model \"" << filename << "\"...\n";

	int meshNum = scene->mNumMeshes;
	data.verts.resize(meshNum);
	data.norms.resize(meshNum);
	data.tcoords.resize(meshNum);
	data.indices.resize(meshNum);

	glm::vec3 minVert(FLT_MAX);
	glm::vec3 maxVert(-FLT_MAX);

	for (int i = 0; i < meshNum; ++i) {
		aiMesh* mesh = scene->mMeshes[i];
		std::cout << "Mesh " << i << ": " << mesh->mName.C_Str() << std::endl;

		for (int j = 0; j < mesh->mNumVertices; ++j) {
			aiVector3D vertex = mesh->mVertices[j];
			aiVector3D normal = mesh->mNormals[j];
			aiVector3D texCoord = mesh->mTextureCoords[0][j];
			data.verts[i].push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));
			data.norms[i].push_back(glm::vec4(normal.x, normal.y, normal.z, 0));
			data.tcoords[i].push_back(glm::vec2(texCoord.x, texCoord.y));

			// finding min/max vertexes
			minVert.x = std::min(minVert.x, vertex.x);
			minVert.y = std::min(minVert.y, vertex.y);
			minVert.z = std::min(minVert.z, vertex.z);
			maxVert.x = std::max(maxVert.x, vertex.x);
			maxVert.y = std::max(maxVert.y, vertex.y);
			maxVert.z = std::max(maxVert.z, vertex.z);
		}

		for (int k = 0; k < mesh->mNumFaces; ++k) {
			aiFace& face = mesh->mFaces[k];
			for (int l = 0; l < face.mNumIndices; ++l) { data.indices[i].push_back(face.mIndices[l]); }
		}
	}

	data.center = (minVert + maxVert) * 0.5f;

	return data;
}
