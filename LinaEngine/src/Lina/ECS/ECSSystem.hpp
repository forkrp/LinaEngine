/*
Author: Inan Evin
www.inanevin.com

Copyright 2018 Inan Evin

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, 
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions 
and limitations under the License.

Class: ECSSystem
Timestamp: 4/8/2019 5:28:34 PM

*/

#pragma once

#ifndef ECSSystem_HPP
#define ECSSystem_HPP


#include "ECSComponent.hpp"


namespace LinaEngine
{
	class BaseECSSystem
	{

	public:

		BaseECSSystem(const std::vector<uint32>& componentTypesIn) : componentTypes(componentTypesIn) {};
		virtual void updateComponents(float delta, BaseECSComponent** components) {};
		const std::vector<uint32> GetComponentTypes() { return componentTypes; }

	private:

		std::vector<uint32> componentTypes;
	
		
	};
}


#endif