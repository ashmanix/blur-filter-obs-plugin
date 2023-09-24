#ifndef BOXBLURFILTER_H
#define BOXBLURFILTER_H

#include "base-filter.hpp"
#include <plugin-support.h>
#include <obs-module.h>
#include <string>
#if defined(__linux__)
  #include <memory>
#elif _WIN32
  #include <memory>
#endif

#define SETTING_BOX_FILTER_SIZE "box_filter_blur_size"

class BoxBlurFilter : public BaseFilter {
private:
	gs_eparam_t *blurSizeParam;
	gs_eparam_t *targetWidthParam;
	gs_eparam_t *targetHeightParam;

	float blurSize;
	obs_properties_t *filterProperties;
	obs_property_t *filterPropertiesGroup;

	char const *shaderFileName = "box_blur.effect";
	char const *filterName = obs_module_text("BlurTypeSelectOptionBox");

public:
	BoxBlurFilter();
	~BoxBlurFilter();
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

#endif // BOXBLURFILTER_H
