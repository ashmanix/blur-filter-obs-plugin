#include "simple-gaussian.hpp"

SimpleGaussianFilter::SimpleGaussianFilter() {}

SimpleGaussianFilter::~SimpleGaussianFilter() {}

void SimpleGaussianFilter::SetParameters(gs_effect_t *effect)
{
	blurSizeParam = gs_effect_get_param_by_name(effect, "blurSize");
	targetWidthParam = gs_effect_get_param_by_name(effect, "targetWidth");
	targetHeightParam = gs_effect_get_param_by_name(effect, "targetHeight");
}

void SimpleGaussianFilter::UpdateFilter(obs_data_t *settings)
{
	double settingsBlurSize =
		obs_data_get_double(settings, SETTING_SIMPLE_GAUSSIAN_SIZE);
	blurSize = (float)settingsBlurSize;
}

void SimpleGaussianFilter::SetProperties(obs_properties_t *mainProperties,
					 std::string name)
{
	filterProperties = obs_properties_create();

	obs_properties_add_float_slider(filterProperties,
					SETTING_SIMPLE_GAUSSIAN_SIZE,
					obs_module_text("BlurSizeSelectTitle"),
					0.1, 10.0, 0.1);

	filterPropertiesGroup = obs_properties_add_group(
		mainProperties, name.c_str(),
		obs_module_text("FilterOptionsGroupTitle"), OBS_GROUP_NORMAL,
		filterProperties);
}

void SimpleGaussianFilter::SetPropertyDefaults(obs_data_t *settings)
{
	obs_data_set_default_double(settings, SETTING_SIMPLE_GAUSSIAN_SIZE,
				    1.0);
}

void SimpleGaussianFilter::Render(obs_source_t *context)
{
	gs_effect_set_float(blurSizeParam, blurSize);
	gs_effect_set_float(targetWidthParam,
			    (float)obs_source_get_width(context));
	gs_effect_set_float(targetHeightParam,
			    (float)obs_source_get_height(context));
}

void SimpleGaussianFilter::HidePropertiesGroup()
{
	if (filterPropertiesGroup) obs_property_set_visible(filterPropertiesGroup, false);
}

void SimpleGaussianFilter::ShowPropertiesGroup()
{
	if(filterPropertiesGroup) obs_property_set_visible(filterPropertiesGroup, true);
}

const char *SimpleGaussianFilter::GetShaderFilePath()
{
	const char *path = obs_module_file(shaderFileName);
	return path;
}

const char *SimpleGaussianFilter::GetFilterName()
{
	return filterName;
}
