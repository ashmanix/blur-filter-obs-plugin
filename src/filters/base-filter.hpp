#ifndef BASEFILTER_H
#define BASEFILTER_H

#include <obs.h>
#include <string>
#include <memory>

class BaseFilter {

private:
	char *shaderFileName;
	obs_property_t *filterPropertiesGroup = nullptr;

public:
	virtual void SetParameters(gs_effect_t *effect)
	{
		UNUSED_PARAMETER(effect);
	}
	virtual void SetProperties(obs_properties_t *mainProperties,
				   std::string name)
	{
		UNUSED_PARAMETER(mainProperties);
		UNUSED_PARAMETER(name);
	}
	virtual void UpdateFilter(obs_data_t *settings)
	{
		UNUSED_PARAMETER(settings);
	}
	virtual void SetPropertyDefaults(obs_data_t *settings)
	{
		UNUSED_PARAMETER(settings);
	}
	virtual void Render(obs_source_t *context)
	{
		UNUSED_PARAMETER(context);
	}
	virtual void HidePropertiesGroup() {}
	virtual void ShowPropertiesGroup() {}
	virtual const char *GetShaderFilePath() { return nullptr; }
	virtual const char *GetFilterName() { return nullptr; }
};

#endif // BASEFILTER_H
