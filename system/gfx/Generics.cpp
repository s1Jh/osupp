/*******************************************************************************
 * Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/
#include "Generics.hpp"

#include "Log.hpp"

NS_BEGIN

GenericMeshCollection::GenericMeshCollection()
{
	log::debug("Generating standard meshes");
//	detail::EnsureGraphicalContext();

	bool success = true;

	rectMask.setAttributeDescriptors({
										  AttributeType::VEC2 // position
									  });
	rectMask.insertVertices({{1.f, 1.f},
							  {1.f, -1.f},
							  {-1.f, -1.f},
							  {-1.f, 1.f}});
	rectMask.insertIndices({0, 1, 2, 0, 3, 2});
	success &= rectMask.upload();

	// Create geometry for rectangular meshes
	rect.setAttributeDescriptors({
									 AttributeType::VEC2, // position
									 AttributeType::VEC2  // uv
								 });
	rect.insertVertices({{1.f, 1.f, 1.f, 1.f},
						 {1.f, -1.f, 1.f, 0.f},
						 {-1.f, -1.f, 0.f, 0.f},
						 {-1.f, 1.f, 0.f, 1.f}});
	rect.insertIndices({0, 1, 2, 0, 3, 2});
	success &= rect.upload();

	const unsigned int resolution = 32;

	double circle_rotation = 2.0 * math::PI / (resolution - 1);
	fvec2d circle_vec = {1.f, 0.f};

	circle.setAttributeDescriptors({
									 AttributeType::VEC2, // position
									 AttributeType::VEC2  // uv
								 });
	circle.insertVertex({0.0, 0.0, 0.5f, 0.5f});

	for (unsigned int i = 0; i < resolution; i++) {
		fvec2d vec = {circle_vec.x, circle_vec.y};
		fvec2d uv = (vec + 1.0f) / 2.0f;
		circle.insertVertex({vec.x, vec.y, uv.x, uv.y});
		circle.insertIndice(i);
		circle_vec = math::Rotate(circle_vec, circle_rotation);
	}
	circle.insertIndice(1);
	success &= circle.upload();

	if (!success)
		log::error("Failed to generate static geometry");
}

const GenericMeshCollection& GetGenericMeshes()
{
	static GenericMeshCollection collection;
	return collection;
}

NS_END
