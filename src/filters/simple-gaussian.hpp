#ifndef SIMPLEGUASSIAN_H
#define SIMPLEGUASSIAN_H

#include "base-filter.hpp"
#include <plugin-support.h>
#include <obs-module.h>
#include <string>

#define SETTING_SIMPLE_GAUSSIAN_SIZE "simple_gaussian_blur_size"
#define SETTING_SIMPLE_GAUSSIAN_TYPE "simple_guassian_blur_type"

class SimpleGaussianFilter : public BaseFilter {
private:
	obs_source_t *context;
	// gs_effect_t *effect;

	gs_eparam_t *blurSizeParam;
	gs_eparam_t *targetWidthParam;
	gs_eparam_t *targetHeightParam;

	float blurSize;
	char const *shaderFileName = "simple_guassian_blur.effect";

public:
	SimpleGaussianFilter();
	~SimpleGaussianFilter();
	void SetFilterParameters(gs_effect_t *effect);
	void ConfigureFilter(obs_data_t *settings, obs_source_t *source);
	void AddFilterProperties();
	void Render();
};

#endif // SIMPLEGAUSSIAN_H
