#include "simple-gaussian.hpp"

SimpleGaussianFilter::SimpleGaussianFilter() {}

SimpleGaussianFilter::~SimpleGaussianFilter() {}

void SimpleGaussianFilter::SetFilterParameters(gs_effect_t *effect)
{
	blurSizeParam = gs_effect_get_param_by_name(effect, "blurSize");
	targetWidthParam = gs_effect_get_param_by_name(effect, "targetWidth");
	targetHeightParam = gs_effect_get_param_by_name(effect, "targetHeight");
}

void SimpleGaussianFilter::ConfigureFilter(obs_data_t *settings,
					   obs_source_t *source)
{
	UNUSED_PARAMETER(settings);
	UNUSED_PARAMETER(source);
	obs_log(LOG_INFO, "something");
}

void SimpleGaussianFilter::AddFilterProperties() {}

void SimpleGaussianFilter::Render()
{
	gs_effect_set_float(blurSizeParam,
			    (blurSize == 0.0) ? 0.01f : blurSize);
	gs_effect_set_float(targetWidthParam,
			    (float)obs_source_get_width(context));
	gs_effect_set_float(targetHeightParam,
			    (float)obs_source_get_height(context));
}
