#pragma once


namespace visuals
{
	namespace esp
	{
		void run_esp();
	}

	namespace world
	{
		
	}

	
	__forceinline void run_visuals()
	{
		esp::run_esp();
	}
}


