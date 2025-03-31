#ifndef FASTGAUSSIAN_H
#define FASTGAUSSIAN_H

#include "base-filter.hpp"
#include <plugin-support.h>
#include <obs-module.h>
#include <string>
#include <memory>

#define SETTING_FAST_GAUSSIAN_DIRECTIONS "fast_gaussian_blur_directions"
#define SETTING_FAST_GAUSSIAN_QUALITY "fast_gaussian_blur_quality"
#define SETTING_FAST_GAUSSIAN_SIZE "fast_gaussian_blur_size"

class FastGaussianFilter : public BaseFilter {
private:
	gs_eparam_t *blurDirectionsParam = nullptr;
	gs_eparam_t *blurQualityParam = nullptr;
	gs_eparam_t *blurSizeParam = nullptr;
	gs_eparam_t *targetWidthParam = nullptr;
	gs_eparam_t *targetHeightParam = nullptr;

	float blurDirections;
	float blurQuality;
	float blurSize;
	obs_properties_t *filterProperties = nullptr;
	obs_property_t *filterPropertiesGroup = nullptr;

	char const *shaderFileName = "fast_gaussian_blur.effect";
	char const *filterName =
		obs_module_text("BlurTypeSelectOptionFastGaussian");

public:
	FastGaussianFilter();
	~FastGaussianFilter();
	void SetParameters(gs_effect_t *effect);
	void UpdateFilter(obs_data_t *settings);
	void SetProperties(obs_properties_t *mainProperties, std::string name);
	void SetPropertyDefaults(obs_data_t *settings);
	void Render(obs_source_t *context);
	void HidePropertiesGroup();
	void ShowPropertiesGroup();
	const char *GetShaderFilePath();
	const char *GetFilterName();
};

#endif // FASTGAUSSIAN_H
