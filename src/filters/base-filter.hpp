#ifndef BASEFILTER_H
#define BASEFILTER_H

#include <obs.h>

class BaseFilter {

private:
	obs_source_t *context;
	// gs_effect_t *effect;
	char *shaderFileName;

public:
	virtual void SetFilterParameters(gs_effect_t *effect){UNUSED_PARAMETER(effect);}
	virtual void ConfigureFilter(obs_data_t *settings, obs_source_t *source)
	{
		UNUSED_PARAMETER(settings);
		UNUSED_PARAMETER(source);
	}
	virtual void AddFilterProperties(){}
	virtual void RemoveFilterProperties(){}
	virtual void SetFilterPropertyDefaults(){}
	virtual void Render(){}
};

#endif // BASEFILTER_H
