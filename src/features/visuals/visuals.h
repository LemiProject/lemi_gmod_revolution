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

	namespace overlay
	{
		void run_overlay();
	}
	
	__forceinline void run_visuals()
	{
		esp::run_esp();
		overlay::run_overlay();
	}
}


