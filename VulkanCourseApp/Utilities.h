#pragma once

// Indices (locations) of queue families (if they even exist)
struct QueueFamilyIndices {
	int graphicsFamily = -1;

	bool isValid()
	{
		return graphicsFamily >= 0;
	}
};