#include "box-blur-filter.hpp"

BoxBlurFilter::BoxBlurFilter()
{
	char *effect_path = obs_module_file(shaderFileName);

	bfree(effect_path);
}

BoxBlurFilter::~BoxBlurFilter() {}

void BoxBlurFilter::SetFilterParameters(gs_effect_t *effect)
{
	blurSizeParam = gs_effect_get_param_by_name(effect, "blurSize");
	targetWidthParam = gs_effect_get_param_by_name(effect, "targetWidth");
	targetHeightParam = gs_effect_get_param_by_name(effect, "targetHeight");
}

void BoxBlurFilter::UpdateFilter(obs_data_t *settings)
{
	double settingsBlurSize =
		obs_data_get_double(settings, SETTING_BOX_FILTER_SIZE);
	blurSize = (float)settingsBlurSize;
}

void BoxBlurFilter::SetProperties(obs_properties_t *mainProperties,
					 std::string name)
{
	filterProperties = obs_properties_create();

	obs_properties_add_float_slider(filterProperties,
					SETTING_BOX_FILTER_SIZE,
					obs_module_text("BlurSizeSelectTitle"),
					0, 10.0, 0.1);

	filterPropertiesGroup = obs_properties_add_group(
		mainProperties, name.c_str(),
		obs_module_text("FilterOptionsGroupTitle"), OBS_GROUP_NORMAL, filterProperties);
}

void BoxBlurFilter::SetPropertyDefaults(obs_data_t *settings)
{
	obs_data_set_default_double(settings, SETTING_BOX_FILTER_SIZE,
				    0.1);
	obs_data_set_default_int(settings, SETTING_BOX_FILTER_TYPE, 0);
}

void BoxBlurFilter::Render(obs_source_t *context)
{
	gs_effect_set_float(blurSizeParam,
			    (blurSize == 0.0) ? 0.01f : blurSize);
	gs_effect_set_float(targetWidthParam,
			    (float)obs_source_get_width(context));
	gs_effect_set_float(targetHeightParam,
			    (float)obs_source_get_height(context));
}

void BoxBlurFilter::HidePropertiesGroup()
{
	obs_property_set_visible(filterPropertiesGroup, false);
}

void BoxBlurFilter::ShowPropertiesGroup()
{
	obs_property_set_visible(filterPropertiesGroup, true);
}

const char *BoxBlurFilter::GetShaderFilePath()
{
	const char *path = obs_module_file(shaderFileName);
	return path;
}

const char *BoxBlurFilter::GetFilterName()
{
	return filterName;
}
