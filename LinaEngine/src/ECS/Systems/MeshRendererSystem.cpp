/*
Author: Inan Evin
www.inanevin.com

Copyright 2018 Inan Evin

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions
and limitations under the License.

Class: MeshRendererSystem
Timestamp: 4/27/2019 5:41:27 PM

*/

#include "LinaPch.hpp"
#include "ECS/Systems/MeshRendererSystem.hpp"



namespace LinaEngine::ECS
{
	void MeshRendererSystem::UpdateComponents(float delta, BaseECSComponent ** components)
	{
		TransformComponent* transform = (TransformComponent*)components[0];
		MeshRendererComponent* mesh = (MeshRendererComponent*)components[1];

		context->RenderMesh(*mesh->vertexArray, *mesh->texture, transform->transform.ToMatrix());
	}
}

