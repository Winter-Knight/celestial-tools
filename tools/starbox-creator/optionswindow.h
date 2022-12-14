#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

struct Options {
	unsigned int seed;
	unsigned int numStars;
	bool billboards;

	bool  averageStarSizeNormalDistribution;
	float averageStarSize;
	float averageStarSizeDeviation;

	bool averageStarDistanceNormalDistribution;
	float averageStarDistance;
	float averageStarDistanceDeviation;
	
	bool averageColorNormalDistribution;
	glm::vec4 averageColor;
	glm::vec4 averageColorDeviation;
	
	int imageSize;

	// Running options. Don't save.
	bool previewStarbox;
};

struct Actions {
	bool updateStars;
	bool previewStarbox;
	bool saveStarbox;
};


class OptionsWindow {
public:
	~OptionsWindow();
	
	void Init(SDL_Window * w, SDL_GLContext * context);
	void Draw(float delta);

	Options * GetOptions() { return &options; }
	Actions * GetActions() { return &actions; }

	void Load();
	void Save();
	const char * GetSaveText();

private:
	// The options
	Options options;
	Actions actions;
	
	SDL_Window * window; // Local reference to the SDL Window
};

#endif // OPTIONSWINDOW_H
